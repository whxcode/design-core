# JS 与 C++ 传递 Uint8 大数据指南

本文档基于以下代码现状整理：

- `design-web/src/Image/index.tsx`
- `design-core/z-webapi/src/ZBinding.cpp`

目标：在 Web 前端（`Uint8Array`）与 wasm/C++ 之间高效传递大块二进制数据（如图片、模型、音视频片段）。

## 1. 背景

在 Emscripten + Embind 场景下，JS 与 C++ 间传 `Uint8` 常见有三种模式：

1. Embind 自动拷贝（最简单）
2. 手动写入 wasm 堆 + 指针传递（高性能）
3. 分块传输（大文件更稳）

你当前代码里已经有前两种雏形：

- `putImage1(val buffer)`：JS 传 `Uint8Array`，C++ 用 `vecFromJSArray<uint8_t>` 接收
- `calloc(size)` + `putImage2(ptr, len)`：JS 先拿指针，再把数据写入 `HEAPU8`

## 2. 方案一：Embind 自动拷贝

### 2.1 调用方式（前端）

```ts
// design-web/src/Image/index.tsx
const bytes = new Uint8Array(data);
core.putImage1(bytes);
```

### 2.2 绑定方式（C++）

```cpp
// design-core/z-webapi/src/ZBinding.cpp
.function("putImage1", optional_override([](ZApp& self, emscripten::val buffer) {
    auto bytes = emscripten::vecFromJSArray<uint8_t>(buffer);
    // bytes 是 C++ 侧独立 vector，已发生拷贝
    self.putImageByCopy(bytes.data(), bytes.size());
}))
```

### 2.3 优缺点

优点：
- 接口最简单，最不容易用错。
- 生命周期安全，C++ 拿到的是自己的 `vector`。
- 对业务代码侵入最小，调试成本低。

缺点：
- 会发生至少一次完整内存拷贝。
- 对 10MB+ 甚至 100MB+ 数据，CPU 和峰值内存开销明显。
- 高频调用（如连续帧）下容易造成卡顿和 GC 压力。

适用场景：
- 低频、数据量中小（例如一次性配置、缩略图）。

## 3. 方案二：手动写 wasm 堆（指针 + 长度）

这是大数据主推方案。

### 3.1 推荐接口设计（C++）

建议将 `calloc` 改为明确的 wasm 内存分配/释放接口，避免混淆：

```cpp
// design-core/z-webapi/src/ZBinding.cpp
class_<ZApp>("App")
  .function("mallocBytes", optional_override([](ZApp& self, size_t size) -> uintptr_t {
      void* p = std::malloc(size);
      return reinterpret_cast<uintptr_t>(p);
  }))
  .function("freeBytes", optional_override([](ZApp& self, uintptr_t ptr) {
      std::free(reinterpret_cast<void*>(ptr));
  }))
  .function("putImage2", optional_override([](ZApp& self, uintptr_t ptr, size_t len) {
      auto* p = reinterpret_cast<const uint8_t*>(ptr);
      self.putImageNoCopy(p, len); // 如需异步持有，必须在这里再拷贝一份
  }));
```

### 3.2 推荐调用方式（前端）

```ts
// design-web/src/Image/index.tsx
const bytes = new Uint8Array(data);
const len = bytes.byteLength;

const ptr = core.mallocBytes(len);
try {
  // 在 wasm 堆上创建视图
  const heapView = new Uint8Array(app.HEAPU8.buffer, ptr, len);
  heapView.set(bytes); // 单次拷贝到 wasm 堆

  // 传递地址和长度给 C++
  core.putImage2(ptr, len);
} finally {
  core.freeBytes(ptr);
}
```

### 3.3 优缺点

优点：
- 拷贝次数最少（通常仅 JS -> wasm 堆一次）。
- 吞吐高，适合大图、大模型、高频数据流。
- 可精细控制内存生命周期。

缺点：
- 需要手动管理内存，容易泄漏或重复释放。
- 指针失效风险更高（特别是异步处理时）。
- 代码复杂度高于自动拷贝方案。

适用场景：
- 大数据（10MB+）或频繁传输。
- 对性能和延迟敏感的模块。

## 4. 方案三：分块传输（Chunk）

当单次数据过大（比如超大图片、二进制包）时，建议分块写入，减少主线程长任务。

### 4.1 接口示例（C++）

```cpp
.function("beginUpload", &ZApp::beginUpload)
.function("appendChunk", optional_override([](ZApp& self, uintptr_t ptr, size_t len) {
    auto* p = reinterpret_cast<const uint8_t*>(ptr);
    self.appendChunk(p, len);
}))
.function("endUpload", &ZApp::endUpload);
```

### 4.2 前端示例（TS）

```ts
async function uploadInChunks(bytes: Uint8Array, chunkSize = 4 * 1024 * 1024) {
  core.beginUpload();
  for (let offset = 0; offset < bytes.length; offset += chunkSize) {
    const chunk = bytes.subarray(offset, Math.min(offset + chunkSize, bytes.length));
    const ptr = core.mallocBytes(chunk.byteLength);
    try {
      new Uint8Array(app.HEAPU8.buffer, ptr, chunk.byteLength).set(chunk);
      core.appendChunk(ptr, chunk.byteLength);
    } finally {
      core.freeBytes(ptr);
    }

    // 给浏览器一点调度机会，避免长任务
    await Promise.resolve();
  }
  core.endUpload();
}
```

### 4.3 优缺点

优点：
- 降低单次长任务导致的 UI 卡顿。
- 可做进度显示、失败重试、断点续传。
- 对超大文件更稳。

缺点：
- 协议复杂（begin/append/end 状态管理）。
- 总体代码量和测试成本更高。

适用场景：
- 超大文件或需要进度反馈的上传/导入流程。

## 5. 方案对比

| 方案 | 拷贝次数 | 性能 | 实现复杂度 | 安全性 | 推荐场景 |
|---|---:|---|---|---|---|
| Embind 自动拷贝 | 高 | 中-低 | 低 | 高 | 小中数据、低频调用 |
| 指针 + HEAPU8 | 低 | 高 | 中 | 中 | 大数据、高频调用 |
| 分块传输 | 低（分块） | 高 | 高 | 中 | 超大数据、需进度控制 |

## 6. 当前项目建议

1. 保留 `putImage1` 作为简单接口（便于验证和小数据调用）。
2. 将大数据主路径切换到 `mallocBytes/freeBytes + putImage2`。
3. 如果图片/资源会超过几十 MB，增加 chunk 流程。
4. 明确生命周期约定：
   - `putImage2` 内如果要异步处理，必须先在 C++ 侧复制一份；
   - JS 只有在 C++ 不再使用该内存后才 `freeBytes`。

## 7. 常见问题

- 为什么 `putImage1` 看起来更“省事”？
  - 因为 Embind 帮你做了转换和拷贝，但大数据时性能成本会集中暴露。

- `HEAPU8` 视图会失效吗？
  - 会。若 wasm 内存增长，旧 `buffer` 可能变化。每次写入前都应基于当前 `app.HEAPU8.buffer` 重新创建视图。

- 可以完全零拷贝吗？
  - JS 与 wasm 内存空间天然隔离，通常至少要拷贝一次到 wasm 堆。工程上追求的是“最少必要拷贝”。

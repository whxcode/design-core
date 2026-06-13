# wasm 与 N-API 对比（基于 Design-Core 项目总结）

更新时间：2026-06

---

# 一、先说结论

对于我的 Design-Core 项目：

```text
选择 N-API 的核心原因：

1. 原生线程能力
2. 无 Wasm Memory 限制
3. 原生文件系统访问
4. 原生第三方库集成
5. 更适合长期后台任务
```

而不是：

```text
N-API 比 Wasm 快很多
```

对于大量小对象接口：

```cpp
viewport()
selection()
transform()
setViewport()
setSelection()
```

两者性能差异通常非常有限。

---

# 二、运行模型

## Wasm

```text
Electron
 └── JS
      └── Wasm Runtime
             └── C++
```

C++ 运行在 Wasm 沙箱中。

所有 C++ 数据：

```cpp
ViewportData
Node
Document
Image
```

本质都存在：

```text
wasm_memory
```

中。

---

## N-API

```text
Electron
 ├── V8
 ├── JS
 └── addon.node
         └── C++
```

C++ 直接运行在宿主进程中。

属于真正的本地代码。

---

# 三、小对象传输

例如：

```cpp
struct ViewportData {
    float x;
    float y;
    float scale;
};

ViewportData viewport();
```

JS：

```js
const vp = viewport();
console.log(vp.x);
```

---

## Wasm

```text
C++
 ↓
embind读取字段
 ↓
创建JS对象
 ↓
返回JS
```

---

## N-API

```text
C++
 ↓
napi创建JS对象
 ↓
返回JS
```

---

结论：

```text
对于小结构体：

ViewportData
Rect
Transform
Selection

性能差异通常很小。
```

---

# 四、大 Buffer 传输

例如：

```cpp
putImage(uint8_t* data, size_t len);
```

图片：

```text
100MB
```

---

## Wasm

常见流程：

```js
const ptr = _malloc(len);

HEAPU8.set(data, ptr);

_putImage(ptr, len);
```

流程：

```text
JS Uint8Array
      ↓
copy
      ↓
wasm_memory
      ↓
C++
```

通常至少一次 memcpy。

---

## N-API

```js
addon.putImage(buffer);
```

C++：

```cpp
auto* ptr = buffer.Data();
```

流程：

```text
JS Buffer
      ↓
直接拿地址
      ↓
C++
```

可能做到零拷贝。

---

结论：

```text
大Buffer场景：

N-API 更有优势。
```

---

# 五、线程能力（最重要）

## N-API

直接使用：

```cpp
std::thread
std::jthread
mutex
condition_variable
future
promise
thread pool
```

例如：

```text
自动保存线程
历史记录合并线程
图片解码线程
资源加载线程
导出线程
```

全部原生支持。

---

## Wasm

理论支持：

```text
WebWorker
SharedArrayBuffer
PThread
```

但：

```text
配置复杂
调试复杂
兼容复杂
```

通常需要：

```text
JS Worker + Wasm
```

共同工作。

---

结论：

```text
对于编辑器内核：

线程能力
>>>>>>>>>>>>>>>
数据传输性能
```

这是 N-API 最大优势。

---

# 六、内存限制

## Wasm

线性内存：

```text
理论最大约 4GB
```

实际项目：

```text
2GB~4GB
```

附近会开始遇到各种问题。

---

## N-API

直接使用：

```text
64位进程内存
```

理论：

```text
几十GB
上百GB
```

仅受机器内存限制。

---

结论：

```text
大型PSD
大型画布
AI缓存
撤销栈

N-API 更从容。
```

---

# 七、文件系统访问

## N-API

直接：

```cpp
std::ifstream
std::ofstream
filesystem
mmap
sqlite
```

即可。

---

## Wasm

通常：

```text
Wasm
 ↓
JS
 ↓
Electron
 ↓
文件系统
```

需要经过宿主代理。

---

结论：

```text
N-API 更接近传统桌面程序。
```

---

# 八、第三方库集成

## N-API

直接使用：

```text
FFmpeg
OpenCV
SQLite
Skia
Harfbuzz
libpng
libjpeg
```

即可。

---

## Wasm

很多库需要：

```text
重新编译
裁剪
适配
```

工作量更大。

---

# 九、后台任务场景

编辑器常见需求：

```text
自动保存
历史记录压缩
资源预加载
图片解码
文件导出
缩略图生成
AI缓存整理
```

---

N-API：

```cpp
后台线程
一直运行
```

即可。

---

Wasm：

通常：

```text
Worker
+
消息通信
+
Wasm
```

实现复杂度更高。

---

# 十、最终结论

对于 Design-Core：

## 不应该作为选择 N-API 的理由

```text
viewport() 更快
selection() 更快
setViewport() 更快
```

这些差异很小。

---

## 应该作为选择 N-API 的理由

```text
1. 原生线程能力
2. 长期后台任务能力
3. 无 Wasm 4GB 限制
4. 原生文件系统访问
5. 原生第三方库集成
6. 大 Buffer 更容易零拷贝
7. 更符合桌面编辑器架构
```

一句话总结：

```text
Wasm 更像运行在沙箱中的 C++。

N-API 更像真正的本地程序。
```

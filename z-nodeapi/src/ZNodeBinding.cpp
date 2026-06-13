#include <napi.h>

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "z-app/include/ZApp.h"
#include "z-app/include/ZAppEvent.h"
#include "z-document/include/commit/z-commit.h"
#include "z-document/include/layers/z-document.h"
#include "z-document/include/viewport/z-viewport.h"
#include "z-editor/include/command/z-command.h"
#include "z-editor/include/ui-event/z-ui-event.h"
#include "z-tools/include/z-editor-theme.h"
#include "z-window/include/ZWindow.h"
#include "z-nodeapi/include/z-node-document-adapter.h"

// ============================================================
// 辅助函数
// ============================================================

namespace {

// JS 对象 → ZUIEvent（等价于 wasm 的 z-js-value.cpp）
ZUIEvent ToUIEvent(Napi::Object source) {
    ZUIEvent target;

    auto readInt = [&](const char* key, int fallback) {
        auto v = source.Get(key);
        return v.IsUndefined() || v.IsNull() ? fallback : v.As<Napi::Number>().Int32Value();
    };
    auto readFloat = [&](const char* key, float fallback) {
        auto v = source.Get(key);
        return v.IsUndefined() || v.IsNull() ? fallback : v.As<Napi::Number>().FloatValue();
    };
    auto readBool = [&](const char* key, bool fallback) {
        auto v = source.Get(key);
        return v.IsUndefined() || v.IsNull() ? fallback : v.As<Napi::Boolean>().Value();
    };

    target.type = static_cast<ZUIEventType>(readInt("type", 0));
    target.x = readFloat("x", 0.0f);
    target.y = readFloat("y", 0.0f);
    target.deltaX = readFloat("deltaX", 0.0f);
    target.deltaY = readFloat("deltaY", 0.0f);
    target.keyCode = static_cast<KeyCode>(readInt("keyCode", 0));
    target.button = static_cast<MouseButton>(readInt("button", 0));
    target.metaKey = readBool("metaKey", false);
    target.ctrlKey = readBool("ctrlKey", false);
    target.altKey = readBool("altKey", false);
    target.shiftKey = readBool("shiftKey", false);

    return target;
}

// JS 对象 → WindowContext
WindowContext ToWindowContext(Napi::Object source) {
    WindowContext ctx;

    auto readUint = [&](const char* key, size_t fallback) {
        auto v = source.Get(key);
        return v.IsUndefined() || v.IsNull() ? fallback : static_cast<size_t>(v.As<Napi::Number>().Int64Value());
    };

    ctx.zWidth = readUint("width", 1);
    ctx.zHeight = readUint("height", 1);
    ctx.zPixelWidth = readUint("pixelWidth", 1);
    ctx.zPixelHeight = readUint("pixelHeight", 1);
    ctx.zDpr = source.Get("dpr").IsUndefined() ? 1.0f : source.Get("dpr").As<Napi::Number>().FloatValue();

    return ctx;
}

// ViewportData → JS 对象
Napi::Object ViewportToJS(Napi::Env env, const ViewportData& vp) {
    auto obj = Napi::Object::New(env);
    obj["offsetX"] = Napi::Number::New(env, vp.offsetX);
    obj["offsetY"] = Napi::Number::New(env, vp.offsetY);
    obj["scale"] = Napi::Number::New(env, vp.scale);
    return obj;
}

}  // namespace

// ============================================================
// App 类 — 主入口，包装 ZApp 单例
// ============================================================

class AppHandle : public Napi::ObjectWrap<AppHandle> {
public:
    static Napi::Function GetClass(Napi::Env env) {
        return DefineClass(env, "App", {
            InstanceMethod("putImage", &AppHandle::PutImage),
            InstanceMethod("draw", &AppHandle::Draw),
            InstanceMethod("viewport", &AppHandle::Viewport),
            InstanceMethod("handler", &AppHandle::Handler),
            InstanceMethod("editorMode", &AppHandle::EditorMode),
            InstanceMethod("onUIEvent", &AppHandle::OnUIEvent),
            InstanceMethod("setTheme", &AppHandle::SetTheme),
            InstanceMethod("appEvent", &AppHandle::GetAppEvent),
            InstanceMethod("command", &AppHandle::GetCommand),
            InstanceMethod("window", &AppHandle::GetWindow),
            InstanceMethod("document", &AppHandle::GetDocument),
            InstanceMethod("exportDocument", &AppHandle::ExportDocument),
            InstanceMethod("loadDocument", &AppHandle::LoadDocument),
        });
    }

    AppHandle(const Napi::CallbackInfo& info)
        : Napi::ObjectWrap<AppHandle>(info) {
        zApp = &ZApp::Get();
    }

private:
    ZApp* zApp;

    Napi::Value PutImage(const Napi::CallbackInfo& info) {
        auto size = info[0].As<Napi::Number>().Int64Value();
        auto w = info[1].As<Napi::Number>().Int64Value();
        auto h = info[2].As<Napi::Number>().Int64Value();

        auto ptr = reinterpret_cast<uintptr_t>(std::malloc(size));
        std::memset(reinterpret_cast<void*>(ptr), 0, size);

        zApp->addImage(ptr, size, 0, 0, w, h);
        return Napi::Number::New(info.Env(), static_cast<uint32_t>(ptr));
    }

    Napi::Value Draw(const Napi::CallbackInfo& info) {
        zApp->requestRedraw();
        return info.Env().Undefined();
    }

    Napi::Value Viewport(const Napi::CallbackInfo& info) {
        return ViewportToJS(info.Env(), zApp->getViewportData());
    }

    Napi::Value Handler(const Napi::CallbackInfo& info) {
        return Napi::Number::New(info.Env(), static_cast<int>(zApp->getHandlerType()));
    }

    Napi::Value EditorMode(const Napi::CallbackInfo& info) {
        return Napi::Number::New(info.Env(), static_cast<int>(zApp->getEditorModeType()));
    }

    Napi::Value OnUIEvent(const Napi::CallbackInfo& info) {
        auto event = ToUIEvent(info[0].As<Napi::Object>());
        zApp->onUIEvent(event);
        return info.Env().Undefined();
    }

    Napi::Value SetTheme(const Napi::CallbackInfo& info) {
        auto type = static_cast<ZEditorThemeType>(info[0].As<Napi::Number>().Int32Value());
        zApp->setTheme(type);
        return info.Env().Undefined();
    }

    Napi::Value GetAppEvent(const Napi::CallbackInfo& info);
    Napi::Value GetCommand(const Napi::CallbackInfo& info);
    Napi::Value GetWindow(const Napi::CallbackInfo& info);
    Napi::Value GetDocument(const Napi::CallbackInfo& info);
    Napi::Value ExportDocument(const Napi::CallbackInfo& info);
    Napi::Value LoadDocument(const Napi::CallbackInfo& info);
};

// ============================================================
// AppEvent 类 — 事件监听
// ============================================================

class AppEventHandle : public Napi::ObjectWrap<AppEventHandle> {
public:
    static Napi::Function GetClass(Napi::Env env) {
        return DefineClass(env, "AppEvent", {
            InstanceMethod("on", &AppEventHandle::On),
            InstanceMethod("off", &AppEventHandle::Off),
        });
    }

    AppEventHandle(const Napi::CallbackInfo& info)
        : Napi::ObjectWrap<AppEventHandle>(info) {
    }

    void Bind(ZAppEvent* event) { zEvent = event; }

private:
    ZAppEvent* zEvent{nullptr};

    Napi::Value On(const Napi::CallbackInfo& info) {
        auto env = info.Env();
        auto type = static_cast<ZAppEventType>(info[0].As<Napi::Number>().Int32Value());
        auto cb = info[1].As<Napi::Function>();

        // napi_ref 是 C 类型，可被共享
        auto ref = std::make_shared<napi_ref>();
        napi_create_reference(env, cb, 1, ref.get());

        auto id = zEvent->on(type, [ref, env](ZAppEventType eventType) {
            napi_value fn;
            if (napi_get_reference_value(env, *ref, &fn) != napi_ok) return;
            napi_value typeVal;
            napi_create_uint32(env, static_cast<uint32_t>(eventType), &typeVal);
            napi_value result;
            napi_call_function(env, nullptr, fn, 1, &typeVal, &result);
        });

        return Napi::Number::New(info.Env(), static_cast<uint32_t>(id));
    }

    Napi::Value Off(const Napi::CallbackInfo& info) {
        auto type = static_cast<ZAppEventType>(info[0].As<Napi::Number>().Int32Value());
        auto id = static_cast<size_t>(info[1].As<Napi::Number>().Int64Value());
        zEvent->off(type, id);
        return info.Env().Undefined();
    }
};

// ============================================================
// Command 类
// ============================================================

class CommandHandle : public Napi::ObjectWrap<CommandHandle> {
public:
    static Napi::Function GetClass(Napi::Env env) {
        return DefineClass(env, "Command", {
            InstanceMethod("canExecute", &CommandHandle::CanExecute),
            InstanceMethod("execute", &CommandHandle::Execute),
        });
    }

    CommandHandle(const Napi::CallbackInfo& info)
        : Napi::ObjectWrap<CommandHandle>(info) {
    }

    void Bind(ZCommand* cmd) { zCmd = cmd; }

private:
    ZCommand* zCmd{nullptr};

    Napi::Value CanExecute(const Napi::CallbackInfo& info) {
        auto type = static_cast<ZCommandType>(info[0].As<Napi::Number>().Int32Value());
        return Napi::Boolean::New(info.Env(), zCmd->canExecute(type));
    }

    Napi::Value Execute(const Napi::CallbackInfo& info) {
        auto type = static_cast<ZCommandType>(info[0].As<Napi::Number>().Int32Value());
        zCmd->execute(type);
        return info.Env().Undefined();
    }
};

// ============================================================
// Window 类
// ============================================================

class WindowHandle : public Napi::ObjectWrap<WindowHandle> {
public:
    static Napi::Function GetClass(Napi::Env env) {
        return DefineClass(env, "Window", {
            InstanceMethod("dump", &WindowHandle::Dump),
            InstanceMethod("setContext", &WindowHandle::SetContext),
        });
    }

    WindowHandle(const Napi::CallbackInfo& info)
        : Napi::ObjectWrap<WindowHandle>(info) {
    }

    void Bind(ZWindow* wnd) { zWnd = wnd; }

private:
    ZWindow* zWnd{nullptr};

    Napi::Value Dump(const Napi::CallbackInfo& info) {
        zWnd->dump();
        return info.Env().Undefined();
    }

    Napi::Value SetContext(const Napi::CallbackInfo& info) {
        auto ctx = ToWindowContext(info[0].As<Napi::Object>());
        zWnd->setContext(ctx);
        return info.Env().Undefined();
    }
};

// ============================================================
// Document 类
// ============================================================

class DocumentHandle : public Napi::ObjectWrap<DocumentHandle> {
public:
    static Napi::Function GetClass(Napi::Env env) {
        return DefineClass(env, "Document", {
            InstanceMethod("setName", &DocumentHandle::SetName),
        });
    }

    DocumentHandle(const Napi::CallbackInfo& info)
        : Napi::ObjectWrap<DocumentHandle>(info) {
    }

    void Bind(ZDocument* doc) { zDoc = doc; }

private:
    ZDocument* zDoc{nullptr};

    Napi::Value SetName(const Napi::CallbackInfo& info) {
        zDoc->setName();
        return info.Env().Undefined();
    }
};

// ============================================================
// AppHandle 的延迟绑定方法（需要子类已经定义）
// ============================================================

Napi::Value AppHandle::GetAppEvent(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    auto constructor = AppEventHandle::GetClass(env);
    auto obj = constructor.New({});
    auto handle = Napi::ObjectWrap<AppEventHandle>::Unwrap(obj);
    handle->Bind(&zApp->getAppEvent());
    return obj;
}

Napi::Value AppHandle::GetCommand(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    auto constructor = CommandHandle::GetClass(env);
    auto obj = constructor.New({});
    auto handle = Napi::ObjectWrap<CommandHandle>::Unwrap(obj);
    handle->Bind(&zApp->getCommand());
    return obj;
}

Napi::Value AppHandle::GetWindow(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    auto constructor = WindowHandle::GetClass(env);
    auto obj = constructor.New({});
    auto handle = Napi::ObjectWrap<WindowHandle>::Unwrap(obj);
    handle->Bind(&zApp->getWindow());
    return obj;
}

Napi::Value AppHandle::GetDocument(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    auto constructor = DocumentHandle::GetClass(env);
    auto obj = constructor.New({});
    auto handle = Napi::ObjectWrap<DocumentHandle>::Unwrap(obj);
    handle->Bind(&zApp->getDocument());
    return obj;
}

Napi::Value AppHandle::ExportDocument(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    auto pkg = zApp->exportDocumentPackage();
    auto result = ZNodeDocumentAdapter::ToValue(env, pkg);
    return Napi::Value(env, result);
}

Napi::Value AppHandle::LoadDocument(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    auto payload = info[0];

    // 检测是否为 legacy 格式（直接传 Uint8Array/Buffer）
    if (payload.IsBuffer()) {
        auto buf = payload.As<Napi::Buffer<uint8_t>>();
        std::vector<uint8_t> legacyDoc(buf.Data(), buf.Data() + buf.Length());
        auto result = zApp->openLegacyDocument(legacyDoc);
        return Napi::Value(env, ZNodeDocumentAdapter::ToValue(env, result));
    }

    // 标准包格式 {document, pages, ...}
    auto obj = payload.As<Napi::Object>();
    ZDocumentPackage pkg;
    std::string message;

    if (!ZNodeDocumentAdapter::ToDocumentPackage(env, obj, pkg, message)) {
        // 尝试作为 legacy 处理（原始 buffer 作为 document 主体）
        auto maybeDoc = obj.Get("document");
        if (maybeDoc.IsBuffer()) {
            auto buf = maybeDoc.As<Napi::Buffer<uint8_t>>();
            std::vector<uint8_t> legacyDoc(buf.Data(), buf.Data() + buf.Length());
            auto result = zApp->openLegacyDocument(legacyDoc);
            return Napi::Value(env, ZNodeDocumentAdapter::ToValue(env, result));
        }
    }

    auto result = zApp->openDocumentPackage(pkg);
    return Napi::Value(env, ZNodeDocumentAdapter::ToValue(env, result));
}

// ============================================================
// 入口 — getApp 和 version
// ============================================================

Napi::Value GetApp(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    auto constructor = AppHandle::GetClass(env);
    return constructor.New({});
}

Napi::Value Version(const Napi::CallbackInfo& info) {
    return Napi::String::New(info.Env(), "design-core-addon/0.0.0");
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports["version"] = Napi::Function::New(env, Version);
    exports["getApp"] = Napi::Function::New(env, GetApp);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)

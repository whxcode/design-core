#include <node_api.h>

#include <cstring>

namespace {

napi_value Version(napi_env env, napi_callback_info) {
    napi_value result;
    napi_create_string_utf8(env, "design-core-addon/0.0.0", NAPI_AUTO_LENGTH, &result);
    return result;
}

void SetFunction(napi_env env, napi_value exports, const char* name, napi_callback callback) {
    napi_value fn;
    napi_create_function(env, name, std::strlen(name), callback, nullptr, &fn);
    napi_set_named_property(env, exports, name, fn);
}

napi_value Init(napi_env env, napi_value exports) {
    SetFunction(env, exports, "version", Version);
    return exports;
}

}  // namespace

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)

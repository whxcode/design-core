#pragma once

#include <node_api.h>

#include <string>

#include "z-app-service/include/z-document-io.h"

class ZNodeDocumentAdapter {
public:
    static napi_value ToValue(napi_env env, const ZDocumentIOResult& result);
    static napi_value ToValue(napi_env env, const ZDocumentPackage& package);
    static bool ToDocumentPackage(napi_env env, napi_value value, ZDocumentPackage& package,
                                  std::string& message);
};

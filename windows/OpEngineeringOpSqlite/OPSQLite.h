#pragma once

#include "JSValue.h"
#include <jsi/jsi.h>
#include "NativeModules.h"
#include <JSI/JsiApiContext.h>
#include <winrt/Microsoft.ReactNative.h>
#include <TurboModuleProvider.h>
#include "bindings.h"

using namespace winrt::Microsoft::ReactNative;
using namespace facebook;

namespace winrt::OpEngineeringOpSqlite
{

    REACT_MODULE(OPSQLite, L"OPSQLite")
    struct OPSQLite {
        // See https://microsoft.github.io/react-native-windows/docs/native-modules for details on writing native modules

        REACT_INIT(Initialize)
        void Initialize(ReactContext const& reactContext) noexcept
        {
            m_reactContext = reactContext;
        }

        REACT_SYNC_METHOD(install)
        bool install() noexcept
        {
            if(m_reactContext == nullptr) {
                return false;
            }
            jsi::Runtime* jsiRuntime = TryGetOrCreateContextRuntime(m_reactContext);
            
            if (jsiRuntime == nullptr) {
                return false;
            }

            auto& runtime = *jsiRuntime;
            auto callInvoker = MakeAbiCallInvoker(m_reactContext.Handle().JSDispatcher());

            

         //   opsqlite::install(runtime, callInvoker, nullptr, nullptr);
            
            return true;
        }

    private:
        ReactContext m_reactContext{ nullptr };
    };

} // namespace winrt::OpEngineeringOpSqlite

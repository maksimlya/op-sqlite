#pragma once

#include "JSValue.h"
#include <jsi/jsi.h>
#include "NativeModules.h"
#include <JSI/JsiApiContext.h>
#include <winrt/Windows.Storage.h>
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

            // Retrieve the local folder path for the database storage
            winrt::hstring localFolderPath = winrt::Windows::Storage::ApplicationData::Current().LocalFolder().Path();
            std::string documentPath = winrt::to_string(localFolderPath);

            // Define the path for the SQLite library if needed, similar to how it's done on iOS.
            // This example assumes you have a path for the SQLite library
            std::string crsqlitePath = ""; // Set this to the actual path if needed

            opsqlite::install(runtime, callInvoker, documentPath.c_str(), crsqlitePath.c_str());
            
            return true;
        }

    private:
        ReactContext m_reactContext{ nullptr };
    };

} // namespace winrt::OpEngineeringOpSqlite

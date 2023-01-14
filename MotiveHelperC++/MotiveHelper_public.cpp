#include "pch.h"
#include "MotiveHelper.h"
#include <string.h>
#include <stdio.h.>
#include <inttypes.h>

namespace MotiveHelperLib
{
    extern "C"
    {
        // called by Init
        HRESULT __declspec(dllexport) CreateMotiveHelper(void** ppHelper)
        {
            if (ppHelper == NULL)
            {
                return E_POINTER;
            }
            *ppHelper = NULL;

            MotiveHelper* pHelper = new MotiveHelper();
            *ppHelper = pHelper;
            return S_OK;
        }

        void __declspec(dllexport) DestroyMotiveHelper(void* pHelper)
        {
            delete (MotiveHelper*)pHelper;
        }

        bool __declspec(dllexport) ConnectMotive(void* pv)
        {
            if (!pv) return false;
            MotiveHelper* pHelper = static_cast<MotiveHelper*>(pv);
            return pHelper->ConnectMotive() == 0;
        }

        bool __declspec(dllexport) IsCalibrated(void* pv)
        {
            if (!pv) return false;
            MotiveHelper* pHelper = static_cast<MotiveHelper*>(pv);
            return pHelper->IsCalibrated();
        }

        void __declspec(dllexport) DisconnectMotive(void* pv)
        {
            if (!pv) return;
            MotiveHelper* pHelper = static_cast<MotiveHelper*>(pv);
            pHelper->DisconnectMotive();
        }

        BSTR __declspec(dllexport) GetCalibrationFileName(void* pv)
        {
            printf("GetCalibrationFileNmeC++\r\n");
            if (!pv)
            {
               printf("pointer is null\r\n");
                return ::SysAllocString(L"Bad pointer");
            }
            else
            {
                printf("pointer is non null, casting to motive helper\r\n");
                try
                {
                    MotiveHelper* pHelper = static_cast<MotiveHelper*>(pv);
                    return pHelper->GetCalibrationFileName();
                }
                catch( ... )
                {
                    return ::SysAllocString(L"Something blew chunks");
                }
            }
        }

    }
};

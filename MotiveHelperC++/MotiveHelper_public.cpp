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

        int  __declspec(dllexport) GetFrameMarkerCount(void* pv)
        {
            if (!pv) return 0;
            MotiveHelper* pHelper = static_cast<MotiveHelper*>(pv);
            return pHelper->GetFrameMarkerCount();
        }

        bool __declspec(dllexport) GetFrameMarkerInfo(void* pv, int MarkerID, MarkerInfo* pInfo)
        {
            if (!pv) return false;
            MotiveHelper* pHelper = static_cast<MotiveHelper*>(pv);
            return pHelper->GetFrameMarkerInfo(MarkerID, pInfo);
        }

        bool __declspec(dllexport) GetFrameMarkerCentroid(void* pv, int MarkerID, int CameraID, float* pX, float* pY)
        {
            if (!pv) return false;
            MotiveHelper* pHelper = static_cast<MotiveHelper*>(pv);
            return pHelper->GetFrameMarkerCentroid(MarkerID, CameraID, pX, pY);
        }

        void __declspec(dllexport) GetCameraFilterSettings(void* pv, CameraFilterSettings* pCameraSettings)
        {
            if (!pv) return;
            MotiveHelper* pHelper = static_cast<MotiveHelper*>(pv);
            pHelper->GetCameraFilterSettings(pCameraSettings);
        }

        void __declspec(dllexport) SetCameraFilterSettings(void* pv, CameraFilterSettings* pCameraSettings)
        {
            if (!pv) return;
            MotiveHelper* pHelper = static_cast<MotiveHelper*>(pv);
            pHelper->SetCameraFilterSettings(pCameraSettings);
        }

        void __declspec(dllexport) SetShutterDelay(void* pv, int microSec)
        {
            if (!pv) return;
            MotiveHelper* pHelper = static_cast<MotiveHelper*>(pv);
            pHelper->SetShutterDelay(microSec);
        }

        void __declspec(dllexport) GetMarkerSizeSettings(void* pv, MarkerSizeSettings* pSettings)
        {
            if (!pv) return;
            MotiveHelper* pHelper = static_cast<MotiveHelper*>(pv);
            pHelper->GetMarkerSizeSettings(pSettings);
        }

        void __declspec(dllexport) SetMarkerSizeSettings(void* pv, MarkerSizeSettings* pSettings)
        {
            if (!pv) return;
            MotiveHelper* pHelper = static_cast<MotiveHelper*>(pv);
            pHelper->SetMarkerSizeSettings(pSettings);
        }

        bool __declspec(dllexport) GetCameraModel(void* pv, int CameraID, CameraModel* pCamModel)
        {
            if (!pv) return false;
            MotiveHelper* pHelper = static_cast<MotiveHelper*>(pv);
            return pHelper->GetCameraModel(CameraID, pCamModel);
        }

        bool __declspec(dllexport) GetCameraInfo(void* pv, int CameraID, CameraInfo* pCamInfo)
        {
            if (!pv) return false;
            MotiveHelper* pHelper = static_cast<MotiveHelper*>(pv);
            return pHelper->GetCameraInfo(CameraID, pCamInfo);
        }

        bool __declspec(dllexport) SetCameraInfo(void* pv, int CameraID, CameraInfo* pCamInfo)
        {
            if (!pv) return false;
            MotiveHelper* pHelper = static_cast<MotiveHelper*>(pv);
            return pHelper->SetCameraInfo(CameraID, pCamInfo);
        }

        int  __declspec(dllexport) GetCameraCount(void* pv)
        {
            if (!pv) return 0;
            MotiveHelper* pHelper = static_cast<MotiveHelper*>(pv);
            return pHelper->GetCameraCount();
        }
    }
};

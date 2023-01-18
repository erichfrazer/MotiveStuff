#pragma once

#include <map>
#include <fstream>
#include <assert.h>
#include <conio.h>
#include <thread>
#include <mutex>

#include <MotiveAPI.h>
#include "Matrix.h"
#include "MotiveHelper_public.h"

#define use_motive

using namespace std;
using namespace CameraLibrary;
using namespace MotiveHelperLib;

class APIListener : public MotiveAPIListener
{
public:
    // Called when a camera connects to the system.
    void CameraConnected(int serialNumber) override
    {
        CameraConnectedOrDisconnected(serialNumber, true);
    }

    // Called when a camera disconnects from the system.
    void CameraDisconnected(int serialNumber) override
    {
        CameraConnectedOrDisconnected(serialNumber, false);
    }

    // Called by the API when a new frame is available
    void FrameAvailable() override
    {
        std::lock_guard<std::mutex> lock(mFrameAvailableMutex);

        mFrameAvailable = true;
        mFrameAvailableCond.notify_one();
    }

    // Blocks the main thread until a new frame is available from the API
    bool WaitForFrame(std::chrono::milliseconds timeout = 20ms)
    {
        bool frameAvailable = false;
        std::unique_lock<std::mutex> lock(mFrameAvailableMutex);

        frameAvailable = mFrameAvailableCond.wait_for(lock, timeout, [this] { return mFrameAvailable; });

        mFrameAvailable = false;
        return frameAvailable;
    }

private:
    std::mutex mFrameAvailableMutex;
    std::condition_variable mFrameAvailableCond;
    bool mFrameAvailable = false;
    bool mVerbose = false;

    void CameraConnectedOrDisconnected(int serialNumber, bool connected)
    {
#ifdef use_motive
        if (mVerbose)
        {
            int cameraIndex = TT_CameraIndexFromSerial(serialNumber);
            if (cameraIndex >= 0)
            {
                wchar_t cameraName[256];

                TT_CameraName(cameraIndex, cameraName, (int)sizeof(cameraName));
                if (connected)
                {
                    wprintf(L"APIListener - Camera Connected: %s\n", cameraName);
                }
                else
                {
                    wprintf(L"APIListener - Camera Disconnected: %s\n", cameraName);
                }
            }
        }
#endif

    }
};

class MotiveHelper
{
public:

	MotiveHelper();
	~MotiveHelper();

	HRESULT ConnectCameras();
	void DisconnectCameras();
	HRESULT ConnectMotive();
	void DisconnectMotive();
	bool IsCalibrated();
    BSTR GetCalibrationFileName();
    int  GetFrameMarkerCount();
    bool GetFrameMarkerInfo(int MarkerID, MarkerInfo* pInfo);
    bool GetFrameMarkerCentroid(int MarkerID, int CameraID, float* pX, float* pY);
    void GetCameraFilterSettings(CameraFilterSettings* pCameraSettings);
    void SetCameraFilterSettings(CameraFilterSettings* pCameraSettings);
    void SetShutterDelay(int microSec);
    void GetMarkerSizeSettings(MarkerSizeSettings* pSettings);
    void SetMarkerSizeSettings(MarkerSizeSettings* pSettings);
    bool GetCameraModel(int CameraID, CameraModel* pCamModel);
    bool GetCameraInfo(int CameraID, CameraInfo* pCamInfo);
    bool SetCameraInfo(int CameraID, CameraInfo* pCamInfo);
    int  GetCameraCount();

private:

    long m_nFrameCounter;
	APIListener m_frameListener;
    static DWORD StaticFrameProcessorProc(void* pThis);
    DWORD FrameProcessorProc();
    std::thread m_pFrameProcessorThread;
    bool m_bDieThread;
    void WaitForThreadToDie();
    int m_nBodyCount;
    bool m_bIsTracking;
    wchar_t m_szBodyName[256];

    void ProcessFrame();

	int ConnectClient();
	void StopAllCameras();
	void printf(const char* pformat, ...);

	Camera* m_pCamera;

	int g_AnalogSamplesPerMocapFrame;


};


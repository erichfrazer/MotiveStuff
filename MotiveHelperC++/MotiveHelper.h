#pragma once

#include <map>
#include <fstream>
#include <assert.h>
#include <conio.h>
#include <thread>
#include <mutex>

#include <MotiveAPI.h>
#include "Matrix.h"

#define use_motive

using namespace std;
using namespace CameraLibrary;

struct CameraInfo
{
	int CameraId;
	double LensCenterX;                 //== Primary Point X (in pixels)
	double LensCenterY;                 //== Primary Point Y (in pixels)
	double HorizontalFocalLength;       //== Horizontal Focal Length (in pixels)
	double VerticalFocalLength;         //== Vertical Focal Length {in pixels)
	double KC1;                         //== Distortion Parameter 1
	double KC2;                         //== Distortion Parameter 2
	double KC3;                         //== Distortion Parameter 3
	double Tangential0;                 //== Tangential 0
	double Tangential1;                 //== Tangential 1
};

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

	map<int, CameraInfo> m_CamInfo;

	Camera* m_pCamera;

	int g_AnalogSamplesPerMocapFrame;


};


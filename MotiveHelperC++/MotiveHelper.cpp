#include "pch.h"
#include "MotiveHelper.h"
#include <string.h>
#include <stdio.h.>
#include <inttypes.h>


#ifdef use_motive
    #pragma comment( lib, "MotiveAPI.lib")
#endif

MotiveHelper::MotiveHelper()
{
    m_pCamera = NULL;
    g_AnalogSamplesPerMocapFrame = 0;
    m_bDieThread = false;
}

MotiveHelper::~MotiveHelper()
{
    DisconnectCameras();
}

void MotiveHelper::WaitForThreadToDie()
{
    m_bDieThread = true;
    m_pFrameProcessorThread.join();
}

HRESULT MotiveHelper::ConnectCameras()
{
    // CameraLibrary_EnableDevelopment();
    // CameraManager::X().WaitForInitialization();

    // EnumerateCameras();
    return 0;
}

void MotiveHelper::DisconnectCameras()
{
    WaitForThreadToDie();
    StopAllCameras();
}

void MotiveHelper::StopAllCameras()
{
}

void MotiveHelper::printf(const char* pFormat, ...)
{
}

HRESULT MotiveHelper::ConnectMotive()
{
#ifdef use_motive
    eMotiveAPIResult res = TT_Initialize();
    if (res != kApiResult_Success)
    {
        return (HRESULT)res;
    }

    const wchar_t* calibrationFile = L"C:\\ProgramData\\OptiTrack\\Motive\\System Calibration.cal";
    const wchar_t* profileFile = L"C:\\ProgramData\\OptiTrack\\MotiveProfile.motive";

    FILE* pFile = NULL;
    _wfopen_s(&pFile, calibrationFile, L"rb");
    fseek(pFile, 0, SEEK_END);
    long size = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);
    BYTE* pBuffer = new BYTE[size];
    fread(pBuffer, 1, size, pFile);
    fclose(pFile);

    MotiveAPICameraList camList = TT_CameraExtrinsicsCalibrationFromMemory(pBuffer, size, res);


    TT_AttachListener(&m_frameListener);

    // Load a camera calibration. For this example, we'll load the calibration that is automatically
    // saved by Motive when the system is calibrated.
    int cameraCount = 0;
    wprintf(L"Loading Calibration: \"%s\"\n\n", calibrationFile);
    res = TT_LoadCalibration(calibrationFile, &cameraCount);
    if (res != kApiResult_Success)
    {
        return (HRESULT)res;
    }

    // Load a profile. For this example, we'll load the profile that is automatically
    // saved by Motive.
    wprintf(L"Loading Profile: \"%s\"\n\n", profileFile);
    res = TT_LoadProfile(profileFile);
    if (res != kApiResult_Success)
    {
        return (HRESULT)res;
    }

    // Wait until all the cameras from the calibration are available
    do
    {
        TT_Update();
        std::this_thread::sleep_for(20ms);

    } while (TT_CameraCount() < cameraCount);

    // List all connected cameras
    printf("Cameras:\n");
    cameraCount = TT_CameraCount();
    for (int i = 0; i < cameraCount; i++)
    {
        wchar_t name[256];

        TT_CameraName(i, name, 256);
        wprintf(L"\t%s\n", name);
    }
    printf("\n");

    for (int i = 0; i < cameraCount; i++)
    {
        eMotiveAPICameraStates camState;
        TT_CameraState(i, camState);
        if (camState == eMotiveAPICameraStates::Camera_Enabled)
        {
            printf("Camera %d is enabled\n", i);
        }
    }

    // alright, now start a thread to start processing frames..
    m_pFrameProcessorThread = std::thread(StaticFrameProcessorProc, this);

    return 0;

#if false
	unsigned char version[4] = { 0 };
	NatNet_GetVersion(version);
	NatNet_SetLogCallback(this->NatLog);

	m_pClient = new NatNetClient();
	m_pClient->SetFrameReceivedCallback(FrameCallback, this);

	const unsigned int kDiscoveryWaitTimeMillisec = 1* 1000; // Wait 1 seconds for responses.
	const int kMaxDescriptions = 10; // Get info for, at most, the first 10 servers to respond.
	sNatNetDiscoveredServer servers[kMaxDescriptions];
	int actualNumDescriptions = kMaxDescriptions;
	NatNet_BroadcastServerDiscovery(servers, &actualNumDescriptions);

	m_ConnectParams.connectionType = ConnectionType_Multicast;

#ifdef _WIN32
    _snprintf_s(
#else
    snprintf(
#endif
        m_pMulticastAddress, sizeof m_pMulticastAddress,
        "%" PRIu8 ".%" PRIu8".%" PRIu8".%" PRIu8"",
        servers[0].serverDescription.ConnectionMulticastAddress[0],
        servers[0].serverDescription.ConnectionMulticastAddress[1],
        servers[0].serverDescription.ConnectionMulticastAddress[2],
        servers[0].serverDescription.ConnectionMulticastAddress[3]
    );

    memcpy(&m_ServerDescription, &servers[0].serverDescription, sizeof(m_ServerDescription));

	m_ConnectParams.multicastAddress = m_pMulticastAddress;
	m_ConnectParams.serverCommandPort = servers[0].serverCommandPort;
	m_ConnectParams.serverDataPort = servers[0].serverDescription.ConnectionDataPort;
	m_ConnectParams.serverAddress = servers[0].serverAddress;
	m_ConnectParams.localAddress = servers[0].localAddress;

    int nRet = ConnectClient();

#endif
#endif

	return S_OK;
}

DWORD MotiveHelper::StaticFrameProcessorProc(void* pv)
{
    MotiveHelper* pThis = (MotiveHelper*)pv;
    return pThis->FrameProcessorProc();
}

DWORD MotiveHelper::FrameProcessorProc()
{
#ifdef use_motive
    while (!m_bDieThread)
    {
        // Blocks and waits for the next available frame.
        // TT_Update or TT_UpdateSingleFrame must be called in order to access the frame.
        // TT_Update will clear the frame queue, leaving only the most recent frame available.
        // TT_UpdateSingleFrame will only remove one frame from the queue with each call.
        bool frameAvailable = m_frameListener.WaitForFrame();

        while (TT_UpdateSingleFrame() == kApiResult_Success && frameAvailable)
        {
            ++m_nFrameCounter;
        }
    }
#endif

    return 0;
}

void MotiveHelper::DisconnectMotive()
{
#ifdef use_motive
    TT_Shutdown();
#endif
}

bool MotiveHelper::IsCalibrated()
{
    printf("IsCalibrated\r\n");
    return true;

#ifdef use_motive
    eMotiveAPICalibrationState state = TT_CalibrationState();
    return state == eMotiveAPICalibrationState::Complete;
#endif
}

int MotiveHelper::ConnectClient()
{
    return 0;
}

BSTR MotiveHelper::GetCalibrationFileName()
{
    printf("MotiveHelper::GetCalibrationFileName\r\n");
    const wchar_t* calibrationFile = L"C:\\ProgramData\\OptiTrack\\Motive\\System Calibration.cal";
    WCHAR* wszString = ::SysAllocString(calibrationFile);
    return wszString;
}

void MotiveHelper::ProcessFrame()
{
#ifdef use_motive
    m_nBodyCount = TT_RigidBodyCount(); // should be only 1
    if (m_nBodyCount > 1) return;

    float   yaw, pitch, roll;
    float   x, y, z;
    float   qx, qy, qz, qw;
    bool    tracked;

    m_bIsTracking = TT_RigidBodyLocation(0, &x, &y, &z, &qx, &qy, &qz, &qw, &yaw, &pitch, &roll);
    if (!m_bIsTracking)
    {
        wcscpy_s(m_szBodyName, L"?");
        return;
    }

    TT_RigidBodyName(0, m_szBodyName, 256);

    TransformMatrix xRot(TransformMatrix::RotateX(pitch * kRadToDeg));
    TransformMatrix yRot(TransformMatrix::RotateY(yaw * kRadToDeg));
    TransformMatrix zRot(TransformMatrix::RotateZ(roll * kRadToDeg));

    // Compose the local-to-world rotation matrix in XYZ (pitch, yaw, roll) order.

    TransformMatrix worldTransform = xRot * yRot * zRot;

    // Inject world-space coordinates of the origin.

    worldTransform.SetTranslation(x, y, z);

    // Invert the transform matrix to convert from a local-to-world to a world-to-local.

    worldTransform.Invert();

    float   mx, my, mz;
    float   tx, ty, tz;

    int markerCount = TT_RigidBodyMarkerCount(0);
    for (int j = 0; j < markerCount; ++j)
    {
        // Get the world-space coordinates of each rigid body marker.
        TT_RigidBodyPointCloudMarker(0, j, tracked, mx, my, mz);

        // Get the rigid body's local coordinate for each marker.
        TT_RigidBodyMarker(0, j, &tx, &ty, &tz);

        // Transform the rigid body point from world coordinates to local rigid body coordinates.
        // Any world-space point can be substituted here to transform it into the local space of
        // the rigid body.

        Point4  worldPnt(mx, my, mz, 1.0f);
        Point4  localPnt = worldTransform * worldPnt;

        printf("  >> %d: Local: (%.3f, %.3f, %.3f) World-To-Local: (%.3f, %.3f, %.3f)\n", j + 1,
            tx, ty, tz, localPnt[0], localPnt[1], localPnt[2]);
    }
#endif

}

int MotiveHelper::GetFrameMarkerCount()
{
    return TT_FrameMarkerCount();
}

bool MotiveHelper::GetFrameMarkerInfo(int MarkerID, MarkerInfo* pInfo)
{
    pInfo->X = TT_FrameMarkerX(MarkerID);
    pInfo->Y = TT_FrameMarkerY(MarkerID);
    pInfo->Z = TT_FrameMarkerZ(MarkerID);
    return true;
}

bool MotiveHelper::GetFrameMarkerCentroid(int MarkerID, int CameraID, float* pX, float* pY)
{
    return TT_FrameCameraCentroid(MarkerID, CameraID, *pX, *pY);
}

void MotiveHelper::GetCameraFilterSettings(CameraFilterSettings* pCameraSettings)
{
    MotiveAPIFilterSettings settings;
    TT_CameraFilterSettings(settings);
    pCameraSettings->MaxMarkerSize = settings.MaxMarkerSize;
    pCameraSettings->MinMarkerSize = settings.MinMarkerSize;
    pCameraSettings->MinRoundness = settings.MinRoundness;
    pCameraSettings->Type = (int)settings.FilterType;
}

void MotiveHelper::SetCameraFilterSettings(CameraFilterSettings* pCameraSettings)
{
    MotiveAPIFilterSettings settings;
    settings.MaxMarkerSize = pCameraSettings->MaxMarkerSize;
    settings.MinMarkerSize = pCameraSettings->MinMarkerSize;
    settings.MinRoundness = pCameraSettings->MinRoundness;
    settings.FilterType = (MotiveAPIFilterSettings::eFilterType) pCameraSettings->Type;
    TT_SetCameraFilterSettings(settings);
}

void MotiveHelper::SetShutterDelay(int microSec)
{
    TT_SetShutterDelay(microSec);
}

void MotiveHelper::GetMarkerSizeSettings(MarkerSizeSettings* pSettings)
{
    MotiveAPIMarkerSizeSettings settings;
    TT_CameraMarkerSize(settings);
    pSettings->MarkerSize = settings.MarkerSize;
    pSettings->Type = settings.MarkerSizeType;
}

void MotiveHelper::SetMarkerSizeSettings(MarkerSizeSettings* pSettings)
{
    MotiveAPIMarkerSizeSettings settings;
    settings.MarkerSize = pSettings->MarkerSize;
    settings.MarkerSizeType = (MotiveAPIMarkerSizeSettings::eMarkerSizeType) pSettings->Type;
    TT_SetCameraMarkerSize(settings);
}

bool MotiveHelper::GetCameraModel(int CameraID, CameraModel* pCamModel)
{
    return false;
}

bool MotiveHelper::GetCameraInfo(int CameraID, CameraInfo* pCamInfo)
{
    pCamInfo->ImagerGain = TT_CameraImagerGain(CameraID);
    pCamInfo->ImagerGainLevels = TT_CameraImagerGainLevels(CameraID);
    pCamInfo->AutoGainControl = false; // can't set it
    pCamInfo->AutomaticExposure = false; // can't set it
    pCamInfo->ExposureMicroSec = TT_CameraExposure(CameraID);
    pCamInfo->FilterSwitch = TT_CameraFilterSwitch(CameraID);
    pCamInfo->FrameRate = TT_CameraFrameRate(CameraID);
    pCamInfo->MeasuredFrameRate = TT_MeasuredIncomingFrameRate();
    pCamInfo->MeasuredDataRate = TT_MeasuredIncomingDataRate();
    pCamInfo->IRLightsOn = TT_CameraIRLedsOn(CameraID);
    pCamInfo->Temperature = TT_CameraTemperature(CameraID);
    pCamInfo->Threshold = TT_CameraThreshold(CameraID);
    pCamInfo->VideoType = TT_CameraVideoType(CameraID);
    return true;
}

bool MotiveHelper::SetCameraInfo(int CameraID, CameraInfo* pCamInfo)
{
    TT_SetCameraAEC(CameraID, pCamInfo->AutomaticExposure);
    TT_SetCameraAGC(CameraID, pCamInfo->AutoGainControl);
    TT_SetCameraExposure(CameraID, pCamInfo->ExposureMicroSec);
    TT_SetCameraFilterSwitch(CameraID, pCamInfo->FilterSwitch);
    TT_SetCameraImagerGain(CameraID, pCamInfo->ImagerGain);
    TT_SetCameraIRLedsOn(CameraID, pCamInfo->IRLightsOn);
    TT_SetCameraThreshold(CameraID, pCamInfo->Threshold);
    TT_SetCameraVideoType(CameraID, pCamInfo->VideoType);
    return true;
}

int MotiveHelper::GetCameraCount()
{
    return TT_CameraCount();
}

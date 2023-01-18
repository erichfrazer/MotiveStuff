#pragma once

#include <Windows.h>

namespace MotiveHelperLib
{
	extern "C"
	{
		typedef struct _MarkerInfo
		{
			float X;
			float Y;
			float Z;
			float Diameter;
			float Residual;
			char Name[256];
			bool Selected;
			unsigned int Flags;
			unsigned long long lowBitsID;
			unsigned long long highBitsID;
		} MarkerInfo;

		typedef struct _CameraFilterSettings
		{
			int Type;
			int MinMarkerSize;
			int MaxMarkerSize;
			float MinRoundness;
		} CameraFilterSettings;

		typedef struct _MarkerSizeSettings
		{
			int Type;
			float MarkerSize;
		} MarkerSizeSettings;

		typedef struct _CameraModel
		{
			float X;
			float Y;
			float Z;
			int Number;
			float R00;
			float R10;
			float R20;
			float R01;
			float R11;
			float R21;
			float R02;
			float R12;
			float R22;
			float PrincipleX;
			float PrincipleY;
			float FocalX;
			float FocalY;
			float kc1;
			float kc2;
			float kc3;
			float tangential0;
			float tangential1;
		} CameraModel;

		typedef struct _CameraInfo
		{
			int FrameRate;
			int Threshold;
			bool IRLightsOn;
			bool FilterSwitch;
			float Temperature;
			int VideoType;
			int ExposureMicroSec;
			bool AutoGainControl;
			bool AutomaticExposure;
			double MeasuredFrameRate;
			double MeasuredDataRate;
			int ImagerGain;
			int ImagerGainLevels;
		} CameraInfo;

		HRESULT __declspec(dllexport) CreateMotiveHelper(void** ppHelper);
		void __declspec(dllexport) DestroyMotiveHelper(void* ppHelper);

		bool __declspec(dllexport) ConnectMotive(void* pHelper);
		bool __declspec(dllexport) IsCalibrated(void* pHelper);
		void __declspec(dllexport) DisconnectMotive(void* pHelper);
		BSTR __declspec(dllexport) GetCalibrationFileName(void* pHelper);
		int  __declspec(dllexport) GetFrameMarkerCount(void* pHelper);
		bool __declspec(dllexport) GetFrameMarkerInfo(void* pHelper, int MarkerID, MarkerInfo* pInfo);
		bool __declspec(dllexport) GetFrameMarkerCentroid(void* pHelper, int MarkerID, int CameraID, float* pX, float* pY);
		void __declspec(dllexport) GetCameraFilterSettings(void* pHelper, CameraFilterSettings* pCameraSettings);
		void __declspec(dllexport) SetCameraFilterSettings(void* pHelper, CameraFilterSettings* pCameraSettings);

		void __declspec(dllexport) GetMarkerSizeSettings(void* pHelper, MarkerSizeSettings* pSettings);
		void __declspec(dllexport) SetMarkerSizeSettings(void* pHelper, MarkerSizeSettings* pSettings);

		void __declspec(dllexport) SetShutterDelay(void* pHelper, int microSec);
		bool __declspec(dllexport) GetCameraModel(void* pHelper, int CameraID, CameraModel* pCamModel);
		bool __declspec(dllexport) GetCameraInfo(void* pHelper, int CameraID, CameraInfo* pCamInfo);
		bool __declspec(dllexport) SetCameraInfo(void* pHelper, int CameraID, CameraInfo* pCamInfo);
		int  __declspec(dllexport) GetCameraCount(void* pHelper);
	};
}

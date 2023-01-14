#pragma once

#include <Windows.h>

namespace MotiveHelperLib
{
	extern "C"
	{
		HRESULT __declspec(dllexport) CreateMotiveHelper(void** ppHelper);
		void __declspec(dllexport) DestroyMotiveHelper(void* ppHelper);

		bool __declspec(dllexport) ConnectMotive(void* pHelper);
		bool __declspec(dllexport) IsCalibrated(void* pHelper);
		void __declspec(dllexport) DisconnectMotive(void* pHelper);
		BSTR __declspec(dllexport) GetCalibrationFileName(void* pHelper);
	};
}

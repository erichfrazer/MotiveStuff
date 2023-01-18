using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace MotiveHelperCS
{
    unsafe public struct FrameMarkerInfo
    {
        public float X;
        public float Y;
        public float Z;
        public float Diameter;
        public float Residual;
        public fixed char Name[256];
        public bool Selected;
        public uint Flags;
        public UInt64 lowBitsID;
        public UInt64 highBitsID;
    }

    public struct CameraFilterSettings
    {
        public int Type;
        public int MinMarkerSize;
        public int MaxMarkerSize;
        public float MinRoundness;
    }

    public struct MarkerSizeSettings
    {
        public int Type;
        public float MarkerSize;
    }


    public struct CameraModel
    {
	    public float X;
        public float Y;
        public float Z;
        public int Number;
        public float R00;
        public float R10;
        public float R20;
        public float R01;
        public float R11;
        public float R21;
        public float R02;
        public float R12;
        public float R22;
        public float PrincipleX;
        public float PrincipleY;
        public float FocalX;
        public float FocalY;
        public float kc1;
        public float kc2;
        public float kc3;
        public float tangential0;
        public float tangential1;
    }

    public struct CameraInfo
    {
        public int FrameRate;
        public int Threshold;
        public bool IRLightsOn;
        public bool FilterSwitch;
        public float Temperature;
        public int VideoType;
        public int ExposureMicroSec;
        public bool AutoGainControl;
        public bool AutomaticExposure;
        public double MeasuredFrameRate;
        public double MeasuredDataRate;
        public int ImagerGain;
        public int ImagerGainLevels;
    }

    public class MotiveHelper
    {
        private const string DllName = "D:\\mr2\\MotiveStuff\\MotiveProject\\bin\\Release\\net6.0\\MotiveHelper.dll";

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int CreateMotiveHelper(ref IntPtr ppHelper);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int DestroyMotiveHelper(IntPtr pHelper);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        [ return: MarshalAs(UnmanagedType.BStr)]
        public static extern string GetCalibrationFileName(IntPtr pHelper);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool IsCalibrated(IntPtr pHelper);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool ConnectMotive(IntPtr pHelper);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "GetFrameMarkerCount")]
        public static extern int GetFrameMarkerCount_Native(IntPtr pHelper);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "GetFrameMarker")]
        public static extern int GetFrameMarker_Native(IntPtr pHelper, int id, ref FrameMarkerInfo fmi);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "GetFrameMarkerCentroid")]
        public static extern bool GetFrameMarkerCentroid_Native(IntPtr pHelper, int MarkerID, int CameraID, ref float x, ref float y);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "GetCameraFilterSettings")]
        public static extern bool GetCameraFilterSettings_Native(IntPtr pHelper, ref CameraFilterSettings pSettings);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "SetCameraFilterSettings")]
        public static extern bool SetCameraFilterSettings_Native(IntPtr pHelper, ref CameraFilterSettings pSettings);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "SetShutterDelay")]
        public static extern bool SetShutterDelay_Native(IntPtr pHelper, int microSec);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "GetRigidBodyCount")]
        public static extern int GetRigidBodyCount_Native(IntPtr pHelper);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "GetMarkerSizeSettings")]
        public static extern bool GetMarkerSizeSettings_Native(IntPtr pHelper, ref MarkerSizeSettings pSettings);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "SetMarkerSizeSettings")]
        public static extern bool SetMarkerSizeSettings_Native(IntPtr pHelper, ref MarkerSizeSettings pSettings);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "GetCameraModel")]
        public static extern bool GetCameraModel_Native(IntPtr pHelper, int CameraID, ref CameraModel pCamModel);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "GetCameraInfo")]
        public static extern bool GetCameraInfo_Native(IntPtr pHelper, int CameraID, ref CameraInfo pCamInfo);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "SetCameraInfo")]
        public static extern bool SetCameraInfo_Native(IntPtr pHelper, int CameraID, ref CameraInfo pCamInfo);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, EntryPoint = "GetCameraCount")]
        public static extern int GetCameraCount_Native(IntPtr pHelper);




        IntPtr m_pNativeMotive;

        public static MotiveHelper Singleton = new MotiveHelper();

        MotiveHelper()
        {
        }

        ~MotiveHelper()
        {
            Free();
        }

        public int Init( )
        {
            Console.WriteLine("Init called");

            if (m_pNativeMotive == IntPtr.Zero)
            {
                int hresult = CreateMotiveHelper(ref m_pNativeMotive);
                if (hresult != 0)
                {
                    Console.WriteLine("create motive helper failed.");
                    m_pNativeMotive = IntPtr.Zero;
                    return -1;
                }
            }

            Console.WriteLine("Init called, now calling ConnectMotive");

            bool bWorked = ConnectMotive(m_pNativeMotive);
            if( !bWorked )
            {
                Console.WriteLine("calling connectMotive failed.");
                return -1;
            }

            Console.WriteLine("creating and connecting motive worked.");
            return 0;
        }

        public void Free()
        {
            if (m_pNativeMotive != IntPtr.Zero)
            {
                DestroyMotiveHelper(m_pNativeMotive);
                m_pNativeMotive = IntPtr.Zero;
            }
        }

        public void GetCalibrationFileNameCS( out string sz )
        {
            if( m_pNativeMotive == IntPtr.Zero )
            {
                sz = "Motive isn't inited.";
            }
            else
            {
                sz = GetCalibrationFileName(m_pNativeMotive);
            }
        }

        public bool IsCalibrated2( )
        {
            if (m_pNativeMotive == IntPtr.Zero)
            {
                return false;
            }
            else
            {
                return IsCalibrated(m_pNativeMotive);
            }
        }

        public int GetFrameMarkerCount( )
        {
            if (m_pNativeMotive == IntPtr.Zero)
            {
                return 0;
            }
            else
            {
                return GetFrameMarkerCount_Native(m_pNativeMotive);
            }
        }

        public void GetFrameMarker( int id, ref FrameMarkerInfo pMarker )
        {
            if (m_pNativeMotive == IntPtr.Zero)
            {
                return;
            }
            else
            {
                GetFrameMarker_Native(m_pNativeMotive, id, ref pMarker);
            }
        }

        public bool GetFrameMarkerCentroid( int FrameID, int CameraID, ref float x, ref float y )
        {
            if (m_pNativeMotive == IntPtr.Zero)
            {
                return false;
            }
            else
            {
                return GetFrameMarkerCentroid_Native(m_pNativeMotive, FrameID, CameraID, ref x, ref y);
            }
        }

        public void GetCameraFilterSettings(ref CameraFilterSettings pSettings)
        {
            if (m_pNativeMotive == IntPtr.Zero)
            {
                return;
            }
            else
            {
                GetCameraFilterSettings_Native(m_pNativeMotive, ref pSettings);
            }
        }

        public void SetCameraFilterSettings(ref CameraFilterSettings pSettings)
        {
            if (m_pNativeMotive == IntPtr.Zero)
            {
                return;
            }
            else
            {
                SetCameraFilterSettings_Native(m_pNativeMotive, ref pSettings);
            }
        }

        public void GetMarkerSizeSettings(ref MarkerSizeSettings pSettings)
        {
            if (m_pNativeMotive == IntPtr.Zero)
            {
                return;
            }
            else
            {
                GetMarkerSizeSettings_Native(m_pNativeMotive, ref pSettings);
            }
        }

        public void SetMarkerSizeSettings(ref MarkerSizeSettings pSettings)
        {
            if (m_pNativeMotive == IntPtr.Zero)
            {
                return;
            }
            else
            {
                SetMarkerSizeSettings_Native(m_pNativeMotive, ref pSettings);
            }
        }

        public void SetShutterDelay(int microSec)
        {
            if (m_pNativeMotive == IntPtr.Zero)
            {
                return;
            }
            else
            {
                SetShutterDelay_Native(m_pNativeMotive, microSec);
            }
        }

        public int GetRigidBodyCount( )
        {
            if (m_pNativeMotive == IntPtr.Zero)
            {
                return 0;
            }
            else
            {
                return GetRigidBodyCount_Native(m_pNativeMotive);
            }
        }

        public int GetCameraCount()
        {
            if (m_pNativeMotive == IntPtr.Zero)
            {
                return 0;
            }
            else
            {
                return GetCameraCount_Native(m_pNativeMotive);
            }
        }

        public bool GetCameraModel( int CameraID, ref CameraModel pCameraModel )
        {
            if (m_pNativeMotive == IntPtr.Zero)
            {
                return false;
            }
            else
            {
                return GetCameraModel_Native(m_pNativeMotive, CameraID, ref pCameraModel);
            }
        }

        public bool GetCameraInfo(int CameraID, ref CameraInfo pCameraInfo)
        {
            if (m_pNativeMotive == IntPtr.Zero)
            {
                return false;
            }
            else
            {
                return GetCameraInfo_Native(m_pNativeMotive, CameraID, ref pCameraInfo);
            }
        }

        public bool SetCameraInfo(int CameraID, ref CameraInfo pCameraInfo)
        {
            if (m_pNativeMotive == IntPtr.Zero)
            {
                return false;
            }
            else
            {
                return SetCameraInfo_Native(m_pNativeMotive, CameraID, ref pCameraInfo);
            }
        }
    }
}   
using System.Runtime.InteropServices;

namespace MotiveHelperCS
{
    public class MotiveHelper
    {
        private const string DllName = "D:\\mr2\\MotiveStuff\\MotiveHelperC++\\x64\\Debug\\MotiveHelper.dll";

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int CreateMotiveHelper(ref IntPtr ppHelper);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int DestroyMotiveHelper(IntPtr pHelper);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        [ return: MarshalAs(UnmanagedType.BStr)]
        public static extern string GetCalibrationFileName(IntPtr pHelper);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool IsCalibrated(IntPtr pHelper);

        IntPtr m_pNativeMotive;

        public static MotiveHelper Singleton = new MotiveHelper();

        MotiveHelper()
        {
        }

        ~MotiveHelper()
        {
            Free();
        }

        public void Init( )
        {
            if (m_pNativeMotive == IntPtr.Zero)
            {
                CreateMotiveHelper(ref m_pNativeMotive);
            }
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
    }
}   
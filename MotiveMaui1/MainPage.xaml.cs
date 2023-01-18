using System.Numerics;
using System.Threading.Tasks;
using Grpc.Net.Client;
using MotiveHelperCS;
using MotiveHelperGRPCSharedns;

namespace MotiveMaui1;

public partial class MainPage : ContentPage
{
	MotiveRequest.MotiveRequestClient m_pClient;
	List<CameraInfo> m_CameraInfo;

	bool m_bConnected;
	int m_nCameraCount;
	int m_nCurrentCamera;

	public MainPage(MotiveRequest.MotiveRequestClient pClient)
	{
		m_pClient = pClient;

		InitializeComponent();
    }

	void OnTimer()
	{
		if( !m_bConnected )
		{
			return;
		}

		Google.Protobuf.WellKnownTypes.Empty empty = new Google.Protobuf.WellKnownTypes.Empty();
        int camCount = m_pClient.GetCameraCount(new EmptyMessage()).Message;
		if (camCount != m_nCameraCount)
		{
			m_CameraInfo = new List<CameraInfo>();
			m_CameraInfo.Clear();
			for( int i = 0; i < camCount;i++ )
			{
				m_CameraInfo.Add(new CameraInfo());
			}
			m_nCameraCount = camCount;
		}
		TotalCameras.Text = "Total Cameras = " + m_nCameraCount;

		SnapCameraInfo();
        UpdateCurrentCameraInfo();
    }

    CameraInfo CurrentCamInfo
	{
		get
		{
			return m_CameraInfo[m_nCurrentCamera];
		}
	}

	void UpdateCurrentCameraInfo()
	{
		LblSelectedCamera.Text = "Selected Camera = " + m_nCurrentCamera;
		LblAutoExposure.Text = "Auto exposure = " + CurrentCamInfo.AutomaticExposure;
        LblAutoGain.Text = "Auto gain = " + CurrentCamInfo.AutoGainControl;
		LblExposure.Text = "Exposure = " + CurrentCamInfo.ExposureMicroSec + " uSec";
		LblGain.Text = "Gain = " + CurrentCamInfo.ImagerGain;
    }

    int ClosestCamera( )
	{
		return -1;
	}

	void SnapCameraInfo( )
	{
		for( int i = 0; i < m_nCameraCount; i++ )
		{
			CameraInfoMsg camInfoMsg =  m_pClient.GetCameraInfo(new BasicReply() { Message = i });
			CameraInfo camInfo = m_CameraInfo[i];
			camInfo.ImagerGain = camInfoMsg.ImagerGain;
			camInfo.ImagerGainLevels = camInfoMsg.ImagerGainLevels;
			camInfo.FrameRate = camInfoMsg.FrameRate;
			camInfo.ExposureMicroSec = camInfoMsg.ExposureMicroSec;
			camInfo.AutomaticExposure = camInfoMsg.AutomaticExposure;
			camInfo.AutoGainControl = camInfoMsg.AutoGainControl;
			camInfo.FilterSwitch = camInfoMsg.FilterSwitch;
			camInfo.IRLightsOn = camInfoMsg.IRLightsOn;
			camInfo.MeasuredDataRate = camInfoMsg.MeasuredDataRate;
			camInfo.MeasuredFrameRate= camInfoMsg.MeasuredFrameRate;
			camInfo.Temperature = camInfoMsg.Temperature;
			camInfo.Threshold = camInfoMsg.Threshold;
			camInfo.VideoType = camInfoMsg.VideoType;
		}
	}

	private void ListCameras_ItemSelected( object sender, EventArgs e )
	{
		ListView lv = sender as ListView;
		SelectedItemChangedEventArgs e2 = e as SelectedItemChangedEventArgs;
		int index = e2.SelectedItemIndex;
		m_nCurrentCamera = index;
    }

    private void BtnConnectClicked(object sender, EventArgs e)
	{
        BasicReply reply = m_pClient.Init(new InitRequest(), null);
		if( reply.Message != 0 )
		{
            CalibrationName.Text = "Could not initialize Motive. Are you missing the license or the hardware dongle?";
			return;
		}

		Console.WriteLine("Got back from m_pClient.Init");
		Google.Protobuf.WellKnownTypes.BoolValue bIsCalibrated = m_pClient.IsCalibrated( new EmptyMessage( ));
        CalibrationName.Text = m_pClient.GetCalibrationFileName(new EmptyMessage()).Message;
		m_bConnected = true;

        var timer = Application.Current.Dispatcher.CreateTimer();
        timer.Interval = TimeSpan.FromSeconds(1);
        timer.Tick += (s, e) => OnTimer();
        timer.Start();
    }
}


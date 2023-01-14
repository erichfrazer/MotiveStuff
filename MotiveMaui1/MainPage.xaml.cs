using System.Threading.Tasks;
using Grpc.Net.Client;
using MotiveHelperGRPCSharedns;

namespace MotiveMaui1;

public partial class MainPage : ContentPage
{
	int count = 0;
	MotiveRequest.MotiveRequestClient m_pClient;

	public MainPage(MotiveRequest.MotiveRequestClient pClient)
	{
		m_pClient = pClient;

		InitializeComponent();
	}

	private void OnCounterClicked(object sender, EventArgs e)
	{
        m_pClient.Init(new InitRequest(), null);
		Console.WriteLine("Got back from m_pClient.Init");
		Google.Protobuf.WellKnownTypes.BoolValue bIsCalibrated = m_pClient.IsCalibrated( new EmptyMessage( ));
		Label1.Text = m_pClient.GetCalibrationFileName(new EmptyMessage()).Message;

		count++;

		if (count == 1)
			CounterBtn.Text = $"Clicked {count} time";
		else
			CounterBtn.Text = $"Clicked {count} times";

		SemanticScreenReader.Announce(CounterBtn.Text);
    }
}


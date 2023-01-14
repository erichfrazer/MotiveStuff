using System.Threading.Tasks;
using Grpc.Net.Client;
using MotiveMaui1;

namespace MotiveMaui1;

public partial class App : Application
{
	public App( MotiveRequest.MotiveRequestClient pMotiveClient)
	{
		InitializeComponent();

		MainPage = new MainPage(pMotiveClient);
	}
}

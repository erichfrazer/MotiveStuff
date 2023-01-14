global using Grpc.Core;
global using MotiveHelperGRPCSharedns;

using System.Threading.Tasks;
using Grpc.Net.Client;
using MotiveMaui1;

namespace MotiveMaui1;

public static class MauiProgram
{
	public static MauiApp CreateMauiApp()
	{
        string BaseAddress = "https://localhost:7201";
        var baseUri = new Uri(BaseAddress);
        var channel = GrpcChannel.ForAddress(baseUri);
		MotiveRequest.MotiveRequestClient pClient = new MotiveRequest.MotiveRequestClient(channel);
        
		var builder = MauiApp.CreateBuilder();
		builder
			.UseMauiApp<App>()
			.ConfigureFonts(fonts =>
			{
				fonts.AddFont("OpenSans-Regular.ttf", "OpenSansRegular");
				fonts.AddFont("OpenSans-Semibold.ttf", "OpenSansSemibold");
			});

		builder.Services.AddScoped(services =>
		{
			return pClient;
		});

		return builder.Build();
	}
}

using Microsoft.AspNetCore.Builder;
using Microsoft.Extensions.DependencyInjection;
using MotiveHelperGPRC.Services;
using MotiveHelperGRPCSharedns;
using System;
using static MotiveHelperGRPCSharedns.MotiveRequest;

System.Threading.Thread t = new System.Threading.Thread(ThreadProc);
t.Start();
t.Join();

[STAThread]
void ThreadProc( )
{
    var builder = WebApplication.CreateBuilder(args);

    // Additional configuration is required to successfully run gRPC on macOS.
    // For instructions on how to configure Kestrel and gRPC clients on macOS, visit https://go.microsoft.com/fwlink/?linkid=2099682

    // Add services to the container.
    builder.Services.AddGrpc();

    var app = builder.Build();

    // Configure the HTTP request pipeline.
    app.MapGrpcService<MotiveHelperService>();
    app.MapGet("/", () => "Communication with gRPC endpoints must be made through a gRPC client. To learn how to create a client, visit: https://go.microsoft.com/fwlink/?linkid=2086909");

    app.Run();
}

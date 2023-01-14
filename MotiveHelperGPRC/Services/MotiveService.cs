using Grpc.Core;
using MotiveHelperGRPCSharedns;
using static MotiveHelperGRPCSharedns.MotiveRequest;
using MotiveHelperCS;
using Google.Protobuf.WellKnownTypes;

namespace MotiveHelperGPRC.Services
{
    public class MotiveHelperService : MotiveRequestBase
    {
        private readonly ILogger<MotiveRequestBase> ? _logger;
        public MotiveHelperService(ILogger<MotiveRequestBase> logger)
        {
            _logger = logger;
        }

        public override Task<BasicReply> Init(InitRequest request, ServerCallContext context)
        {
            Console.WriteLine("Init called");

            MotiveHelper.Singleton.Init();

            Console.WriteLine("Init returned");

            return Task.FromResult(
                new BasicReply() { Message = 1 });
        }

        public override Task<BasicReply> Free(InitRequest request, ServerCallContext context)
        {
            return Task.FromResult(
                new BasicReply() { Message = 1 });
        }

        public override Task<StringReply> GetCalibrationFileName(EmptyMessage request, ServerCallContext context)
        {
            Console.WriteLine("GetCalibrationFileName called");
            Task<StringReply> t = Task<StringReply>.Run(() =>
            {
                string sz;
                MotiveHelper.Singleton.GetCalibrationFileNameCS( out sz );
                Console.WriteLine("GetCalibrationFileName = " + sz);
                StringReply sr2 = new StringReply() {  Message = sz };
                return sr2;
            });
            return t;
         }

        public override Task<BoolValue> IsCalibrated(EmptyMessage request, ServerCallContext context)
        {
            Task<BoolValue> t = Task<BoolValue>.Run(() =>
            {
                bool bIsCalibrated = MotiveHelper.Singleton.IsCalibrated2();
                BoolValue bv = new BoolValue();
                bv.Value = bIsCalibrated;
                return bv;
            });
            return t;
        }
    }


}
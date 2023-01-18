using Grpc.Core;
using MotiveHelperGRPCSharedns;
using static MotiveHelperGRPCSharedns.MotiveRequest;
using MotiveHelperCS;
using Google.Protobuf.WellKnownTypes;
using System.Threading.Tasks;
using System;
using Microsoft.Extensions.Logging;

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
            Console.WriteLine("Service: Init called");
            
            int result = MotiveHelper.Singleton.Init();
            if( result != 0 )
            {
                Console.WriteLine("Service: Motive init failed");
            }

            return Task.FromResult(
                new BasicReply() { Message = result });
        }

        public override Task<BasicReply> Free(InitRequest request, ServerCallContext context)
        {
            MotiveHelper.Singleton.Free();

            return Task.FromResult(
                new BasicReply() { Message = 0 });
        }

        public override Task<StringReply> GetCalibrationFileName(EmptyMessage request, ServerCallContext context)
        {
            Console.WriteLine("Service: GetCalibrationFileName called");
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

        public override Task<FrameMarkerInfoMsg> GetFrameMarker(BasicReply request, ServerCallContext context)
        {
            Task<FrameMarkerInfoMsg> t = Task<FrameMarkerInfoMsg>.Run(() =>
            {
                FrameMarkerInfo marker = new FrameMarkerInfo();
                MotiveHelper.Singleton.GetFrameMarker(request.Message, ref marker);
                FrameMarkerInfoMsg res = new FrameMarkerInfoMsg();
                res.X = marker.X;
                res.Y = marker.Y;
                res.Z = marker.Z;
                

                return res;
            });
            return t;
        }

        public override Task<BasicReply> GetFrameMarkerCount(EmptyMessage request, ServerCallContext context)
        {
            Task<BasicReply> t = Task<BasicReply>.Run(() =>
            {
                BasicReply res = new BasicReply();
                res.Message = MotiveHelper.Singleton.GetFrameMarkerCount();
                return res;
            });
            return t;
        }

        public override Task<XYReply> GetFrameMarkerCentroid(
            MarkerAndCameraIDReply request, 
            ServerCallContext context)
        {
            Task<XYReply> t = Task<XYReply>.Run(() =>
            {
                XYReply res = new XYReply();

                float x = 0;
                float y = 0;
                res.Contributing = MotiveHelper.Singleton.GetFrameMarkerCentroid(request.MarkerID, request.CameraID, ref x, ref y);
                res.X = x;
                res.Y = y;
                return res;
            });
            return t;
        }

        public override Task<EmptyMessage> SetCameraFilterSettings(FilterSettingsMsg request, ServerCallContext context)
        {
            Task<EmptyMessage> t = Task<EmptyMessage>.Run(() =>
            {
                EmptyMessage res = new EmptyMessage();

                CameraFilterSettings settings = new CameraFilterSettings();
                MotiveHelper.Singleton.SetCameraFilterSettings(ref settings);
                settings.MaxMarkerSize = request.MaxMarkerSize;
                settings.MinMarkerSize = request.MinMarkerSize;
                settings.MinRoundness = request.MinRoundness;
                settings.Type = request.Type;
                MotiveHelper.Singleton.SetCameraFilterSettings(ref settings);
                return res;
            });
            return t;
        }

        public override Task<FilterSettingsMsg> GetCameraFilterSettings(EmptyMessage request, ServerCallContext context)
        {
            Task<FilterSettingsMsg> t = Task<FilterSettingsMsg>.Run(() =>
            {
                FilterSettingsMsg res = new FilterSettingsMsg();

                CameraFilterSettings settings = new CameraFilterSettings();
                MotiveHelper.Singleton.GetCameraFilterSettings(ref settings);
                res.MaxMarkerSize = settings.MaxMarkerSize;
                res.MinMarkerSize = settings.MinMarkerSize;
                res.MinRoundness = settings.MinRoundness;
                res.Type = settings.Type;
                return res;
            });
            return t;
        }

        public override Task<BasicReply> GetRigidBodyCount(EmptyMessage request, ServerCallContext context)
        {
            Task<BasicReply> t = Task<BasicReply>.Run(() =>
            {
                BasicReply res = new BasicReply();
                int count = MotiveHelper.Singleton.GetRigidBodyCount();
                res.Message = count;
                return res;
            });
            return t;
        }

        public override Task<MarkerSizeSettingsMsg> GetCameraMarkerSize(EmptyMessage request, ServerCallContext context)
        {
            Task<MarkerSizeSettingsMsg> t = Task<MarkerSizeSettingsMsg>.Run(() =>
            {
                MarkerSizeSettingsMsg res = new MarkerSizeSettingsMsg();

                MarkerSizeSettings settings = new MarkerSizeSettings();
                MotiveHelper.Singleton.GetMarkerSizeSettings(ref settings);
                res.MarkerSize = settings.MarkerSize;
                res.Type = settings.Type;
                return res;
            });
            return t;
        }

        public override Task<EmptyMessage> SetCameraMarkerSize(MarkerSizeSettingsMsg request, ServerCallContext context)
        {
            Task<EmptyMessage> t = Task<EmptyMessage>.Run(() =>
            {
                EmptyMessage res = new EmptyMessage();
                MarkerSizeSettings settings = new MarkerSizeSettings();
                settings.MarkerSize = request.MarkerSize;
                settings.Type = request.Type;
                MotiveHelper.Singleton.SetMarkerSizeSettings(ref settings);
                return res;
            });
            return t;
        }

        public override Task<CameraModelMsg> GetCameraModel(BasicReply request, ServerCallContext context)
        {
            return base.GetCameraModel(request, context);
        }

        public override Task<BasicReply> IsFrameAvailable(EmptyMessage request, ServerCallContext context)
        {
            return base.IsFrameAvailable(request, context);
        }

        public override Task<CameraInfoMsg> GetCameraInfo(BasicReply request, ServerCallContext context)
        {
            Task<CameraInfoMsg> t = Task<CameraInfoMsg>.Run(() =>
            {
                CameraInfoMsg res = new CameraInfoMsg();
                CameraInfo settings = new CameraInfo();
                MotiveHelper.Singleton.GetCameraInfo(request.Message, ref settings);
                res.AutoGainControl = settings.AutoGainControl;
                res.AutomaticExposure = settings.AutomaticExposure;
                res.FilterSwitch = settings.FilterSwitch;
                res.FrameRate= settings.FrameRate;
                res.ImagerGain = settings.ImagerGain;
                res.ImagerGainLevels = settings.ImagerGainLevels;
                res.IRLightsOn = settings.IRLightsOn;
                res.MeasuredDataRate = settings.MeasuredDataRate;
                res.MeasuredFrameRate = settings.MeasuredFrameRate;
                res.Temperature = settings.Temperature;
                res.Threshold = settings.Threshold;
                res.VideoType = settings.VideoType;
                res.ExposureMicroSec = settings.ExposureMicroSec;
                return res;
            });
            return t;
        }

        public override Task<BasicReply> SetCameraInfo(CameraInfoMsg request, ServerCallContext context)
        {
            Task<BasicReply> t = Task<BasicReply>.Run(() =>
            {
                BasicReply res = new BasicReply();
                CameraInfo settings = new CameraInfo();
                settings.AutoGainControl = request.AutoGainControl;
                settings.AutomaticExposure = request.AutomaticExposure;
                settings.FilterSwitch = request.FilterSwitch;
                settings.FrameRate = request.FrameRate;
                settings.ImagerGain = request.ImagerGain;
                settings.ImagerGainLevels = request.ImagerGainLevels;
                settings.IRLightsOn = request.IRLightsOn;
                settings.MeasuredDataRate = request.MeasuredDataRate;
                settings.MeasuredFrameRate = request.MeasuredFrameRate;
                settings.Temperature = request.Temperature;
                settings.Threshold = request.Threshold;
                settings.VideoType = request.VideoType;
                settings.ExposureMicroSec = request.ExposureMicroSec;
                MotiveHelper.Singleton.SetCameraInfo(request.CameraIndex, ref settings);
                return res;
            });
            return t;
        }

        public override Task<BasicReply> GetCameraCount(EmptyMessage request, ServerCallContext context)
        {
            Task<BasicReply> t = Task<BasicReply>.Run(() =>
            {
                BasicReply res = new BasicReply();
                int count = MotiveHelper.Singleton.GetCameraCount();
                res.Message = count;
                return res;
            });
            return t;
        }
    }


}
using System.Runtime.CompilerServices;

namespace Spike
{
    public class Console
    {
        public static void LogInfo(string message)
        {
            LogInfo_Native(message);
        }

        public static void LogWarn(string message)
        {
            LogWarn_Native(message);
        }

        public static void LogDebug(string message)
        {
            LogDebug_Native(message);
        }

        public static void LogError(string message)
        {
            LogError_Native(message);
        }

        public static void LogFatal(string message)
        {
            LogFatal_Native(message);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void LogInfo_Native(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void LogWarn_Native(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void LogDebug_Native(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void LogError_Native(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void LogFatal_Native(string message);
    }
}

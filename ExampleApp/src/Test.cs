using System;
using Spike;

namespace Example
{
    class Test : Entity
    {
        public int Integer = 50;
        public void OnCreate()
        {
            Spike.Console.LogInfo("Scripting Test");
            Spike.Console.LogDebug("Scripting Test");
            Spike.Console.LogWarn("Scripting Test");
            Spike.Console.LogError("Scripting Test");
            Spike.Console.LogFatal("Scripting Test");
        }

        public void OnUpdate(float ts)
        {
            //Spike.Console.LogInfo("helo from Update");
        }
    }
}

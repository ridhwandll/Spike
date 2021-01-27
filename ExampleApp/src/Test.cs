using System;
using Spike;

namespace Example
{
    class Test : Entity
    {
        public int Integer = 80;
        public void OnCreate()
        {
            Spike.Console.LogInfo("Script Logging Test");
            Spike.Console.LogDebug("Script Logging Test");
            Spike.Console.LogWarn("Script Logging Test");
            Spike.Console.LogError("Script Logging Test");
            Spike.Console.LogFatal("Script Logging Test");
        }

        public void OnUpdate(float ts)
        {
            if (Input.IsKeyPressed(KeyCode.Space))
            {
                Spike.Console.LogInfo("Space key is pressed!!");
            }

            Spike.Console.LogInfo(Integer.ToString());
        }
    }
}

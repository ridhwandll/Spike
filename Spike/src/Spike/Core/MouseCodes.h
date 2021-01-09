/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : MouseCodes
File Type      : h
File created on: 2021/01/09
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once

namespace Spike
{
    using MouseCode = uint16_t;

    namespace Mouse
    {
        enum : MouseCode
        {
            // From glfw3.h
            Button0 =           0,
            Button1 =           1,
            Button2 =           2,
            Button3 =           3,
            Button4 =           4,
            Button5 =           5,
            Button6 =           6,
            Button7 =           7,

            ButtonLast =        Button7,
            ButtonLeft =        Button0,
            ButtonRight =       Button1,
            ButtonMiddle =      Button2
        };
    }
    

}

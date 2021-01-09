/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Random
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
#include <random>

namespace Spike
{
    class Random
    {
    public:
        template<typename T>
        static T SPKRandom(T min = 0, T max = 100)
        {
            if constexpr (std::is_same_v<T, int>)
            {
                if (min > max)
                {
                    std::swap(min, max);
                    SPK_CORE_LOG_WARN("Minimum is greater than maximum in {0}", __FUNCTION__);
                }

                std::uniform_int_distribution<> distribution(min, max);
                auto& mt = Engine();
                return distribution(mt);
            }

            if constexpr (std::is_same_v<T, float>)
            {
                if (min > max)
                {
                    std::swap(min, max);
                    SPK_CORE_LOG_WARN("Minimum is greater than maximum in {0}", __FUNCTION__);
                }

                std::uniform_real_distribution<T> distribution(min, max);
                auto& mt = Engine();
                return distribution(mt);
            }

            if constexpr (std::is_same_v<T, double>)
            {
                if (min > max)
                {
                    std::swap(min, max);
                    SPK_CORE_LOG_WARN("Minimum is greater than maximum in {0}", __FUNCTION__);
                }

                std::uniform_real_distribution<T> distribution(min, max);
                auto& mt = Engine();
                return distribution(mt);
            }

            if constexpr (std::is_same_v<T, bool>)
            {
                std::bernoulli_distribution distribution(0.5);
                auto& mt = Engine();
                return distribution(mt);
            }

            if constexpr (!std::is_same_v<T, bool> || !std::is_same_v<T, int> || !std::is_same_v<T, float> || !std::is_same_v<T, double>)
            {
                SPK_INTERNAL_ASSERT("Given data type in LERandom is not supported!");
                return -1;
            }
        }
    private:
        static std::mt19937_64& Random::Engine()
        {
            static std::random_device rd;
            static std::mt19937_64 engine(rd());
            return engine;
        }
    };

}

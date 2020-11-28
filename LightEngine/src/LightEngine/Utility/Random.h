#pragma once
#include <random>

namespace LightEngine
{
    class Random
    {
    public:
        template<typename T>
        static T LERandom(T min = 0, T max = 500)
        {
            if constexpr (std::is_same_v<T, int>)
            {
                if (min > max)
                {
                    std::swap(min, max);
                    LE_CORE_LOG_WARN("Minimum is greater than maximum in {0}", __FUNCTION__);
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
                    LE_CORE_LOG_WARN("Minimum is greater than maximum in {0}", __FUNCTION__);
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
                    LE_CORE_LOG_WARN("Minimum is greater than maximum in {0}", __FUNCTION__);
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
                LE_CORE_ASSERT(false, "Given data type not supported!")
                return 1;
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
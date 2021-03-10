//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "UUID.h"
#include <random>

namespace Spike
{
    static std::random_device s_RandomDevice;
    static std::mt19937_64 engine(s_RandomDevice());
    static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

    UUID::UUID()
        : m_UUID(s_UniformDistribution(engine)){}

    UUID::UUID(uint64_t uuid)
        : m_UUID(uuid) {}

    UUID::UUID(const UUID& other)
        : m_UUID(other.m_UUID) {}

}
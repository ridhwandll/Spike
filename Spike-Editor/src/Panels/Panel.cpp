#include "Panel.h"

namespace Spike
{
    Panel* Panel::m_Instance = new Panel();

    Panel* Panel::Get()
    {
        return m_Instance;
    }
}
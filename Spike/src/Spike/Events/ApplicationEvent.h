//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved

#pragma once
#include "Spike/Core/Base.h"
#include "Event.h"

namespace Spike
{
//WINDOW RESIZE EVENT
    class  WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(Uint width, Uint height)
            : m_Width(width), m_Height(height) {}

        Uint GetWidth() const { return m_Width; }
        Uint GetHeight() const { return m_Height; }

        String ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    private:
        Uint m_Width, m_Height;
    };
//WINDOW CLOSE EVENT
    class  WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() {}

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
//APP TICK
    class  AppTickEvent : public Event
    {
    public:
        AppTickEvent() {}

        EVENT_CLASS_TYPE(AppTick)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
//APP UPDATE 
    class  AppUpdateEvent : public Event
    {
    public:
        AppUpdateEvent() {}

        EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
//APP RENDER EVENT
    class  AppRenderEvent : public Event
    {
    public:
        AppRenderEvent() {}

        EVENT_CLASS_TYPE(AppRender)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
}

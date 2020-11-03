#pragma once

#include "LightEngine/Window.h"

#include <GLFW/glfw3.h>

namespace LightEngine
{

    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps& props); //Constructor declaration for this class
        virtual ~WindowsWindow(); //Destructor declaration for this class

        void OnUpdate() override; //Override the OnUpdate function of the Window class

        //Override the GetWidth() and GetHeight() function of the Window class in Window.h
        inline unsigned int GetWidth() const override { return m_Data.Width; } 
        inline unsigned int GetHeight() const override { return m_Data.Height; }

        // Window attributes
        inline void SetEventCallback(const EventCallbackFn& callback) override 
        {
            m_Data.EventCallback = callback;
        }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        inline virtual void* GetNativeWindow() const { return m_Window; }
    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();
    private:
        GLFWwindow* m_Window;

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };

}
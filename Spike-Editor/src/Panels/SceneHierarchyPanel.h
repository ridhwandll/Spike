/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : SceneHierarchyPanel
File Type      : h
File created on: 2021/01/09
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim/
  that you wrote the original software.
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Panel.h"
#include "Spike/Core/Log.h"
#include "Spike/Core/Base.h"
#include "Spike/Scene/Entity.h"
#include "Spike/Events/Event.h"
#include "Spike/Events/KeyEvent.h"

namespace Spike
{
    class SceneHierarchyPanel : public Panel
    {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& context);
        void SetContext(const Ref<Scene>& context);
        void SetSelectedEntity(Entity entity) { m_SelectionContext = entity; }

        void OnImGuiRender();
        void OnEvent(Event& e);
    private:
        bool OnKeyPressed(KeyPressedEvent& e);
        void DrawEntityNode(Entity entity);
    private:
        bool m_IsHierarchyFocused = false;
        bool m_IsHierarchyHovered = false;
    };
}
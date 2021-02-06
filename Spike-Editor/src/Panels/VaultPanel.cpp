/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : VaultPanel
File Type      : cpp
File created on: 2021/02/06
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

3.THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "VaultPanel.h"
#include "Spike/Core/Vault.h"
#include <filesystem>
#include <imgui/imgui.h>
#include <FontAwesome.h>

namespace Spike
{
    static bool s_Loaded = false; //TEMP (TODO)
    VaultPanel::VaultPanel()
    {
        m_ProjectPath.clear();
        s_Loaded = false;
    }

    Vector<DirectoryEntry> VaultPanel::GetFiles(const String& directory)
    {
        Vector<DirectoryEntry> result;
        for (const auto& entry : std::filesystem::directory_iterator(directory))
        {
            String path = entry.path().string();
            DirectoryEntry e = { entry.path().stem().string(), entry.path().extension().string(), path, entry.is_directory() };

            if (entry.is_directory())
                e.SubEntries = GetFiles(entry.path().string());
            result.push_back(e);
        }
        return result;
    }

    void VaultPanel::OnImGuiRender()
    {
        if (m_ProjectPath.empty())
            m_ProjectPath = Vault::GetProjectPath();

        ImGui::Begin("Spike Vault");
        if (ImGui::Button(ICON_FK_REFRESH))
        {
            if (Vault::Reload())
                SPK_CORE_LOG_INFO("Vault reloaded successfully.");
            m_Files = GetFiles(m_ProjectPath);
        }

        if (!s_Loaded && !m_ProjectPath.empty())
        {
            m_Files = GetFiles(m_ProjectPath);
            s_Loaded = true;
        }

        if (Vault::IsVaultInitialized())
        {
            for (auto& file : m_Files)
                this->DrawPath(file);
        }
        ImGui::End();
    }

    void VaultPanel::DrawPath(DirectoryEntry& entry)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

        if (!entry.IsDirectory)
            flags |= ImGuiTreeNodeFlags_Leaf;

        String nodeString;

        if (entry.Extension == ".cs" || entry.Extension == ".glsl")
            nodeString = ICON_FK_FILE_CODE_O + String(" ") + entry.Name + entry.Extension;

        else if (entry.Extension == ".spike" || entry.Extension == ".txt")
            nodeString = ICON_FK_FILE_TEXT + String(" ") + entry.Name + entry.Extension;

        else if (entry.Extension == ".png" || entry.Extension == ".jpg" || entry.Extension == ".gif" || entry.Extension == ".bmp" || entry.Extension == ".psd")
            nodeString = ICON_FK_FILE_IMAGE_O + String(" ") + entry.Name + entry.Extension;

        else if (entry.IsDirectory)
            nodeString = ICON_FK_FOLDER + String(" ") + entry.Name;

        else
            nodeString = entry.Name + entry.Extension;

        if (ImGui::TreeNodeEx(nodeString.c_str(), flags))
        {
            if (entry.IsDirectory)
                for (auto& subDirectory : entry.SubEntries)
                    DrawPath(subDirectory);

            ImGui::TreePop();
        }
    }
}

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
#include "Spike/Scene/SceneSerializer.h"
#include "EditorLayer.h"
#include <filesystem>
#include <imgui/imgui.h>
#include <FontAwesome.h>

namespace Spike
{
    static void* s_EditorLayerStorage;
    static Ref<Texture> s_TexturePreviewStorage;
    static bool s_Loaded = false; //TEMP (TODO)
    VaultPanel::VaultPanel(const void* editorLayerPtr)
    {
        s_EditorLayerStorage = (EditorLayer*)editorLayerPtr;
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
            if (Vault::IsVaultInitialized())
            {
                Vault::Reload();
                m_ProjectPath = Vault::GetProjectPath();
                m_Files = GetFiles(m_ProjectPath);
            }
            else
                SPK_CORE_LOG_WARN("Open A working directory first! Go to 'Files->Open Folder' to open a working directory.");
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

        bool show = true;
        ImGui::Begin("Texture Preview", &show, ImGuiWindowFlags_NoResize);
        if (s_TexturePreviewStorage)
        {
            uint64_t rendererID = s_TexturePreviewStorage->GetRendererID();
            glm::vec2 imageRes = s_TexturePreviewStorage->GetResolution();
            ImVec2 windowRes = ImGui::GetWindowSize();

            glm::vec2 imageMiddle = { imageRes.x * 0.5f, imageRes.y * 0.5f };
            glm::vec2 windowMiddle = { windowRes.x * 0.5f, windowRes.y * 0.5f };

            glm::vec2 result = { windowMiddle - imageMiddle };
            ImGui::SetCursorPos({ result.x, result.y });
            ImGui::Image((ImTextureID)rendererID, { imageRes.x, imageRes.y }, { 0, 1 }, { 1, 0 });
        }
        else
            ImGui::TextUnformatted("No Texture is selected. Select an image file\nin the Spike Vault to show it up here!");
        ImGui::End();
    }

    void VaultPanel::DrawPath(DirectoryEntry& entry)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

        if (!entry.IsDirectory)
            flags |= ImGuiTreeNodeFlags_Leaf;

        String nodeString;

        bool codeExtBools = entry.Extension == ".cs" || entry.Extension == ".glsl" || entry.Extension == ".cpp" || entry.Extension == ".lua"
            || entry.Extension == ".py" || entry.Extension == ".hlsl" || entry.Extension == ".js" || entry.Extension == ".c" || entry.Extension == ".h";

        bool imageExtBools = entry.Extension == ".png" || entry.Extension == ".jpg" || entry.Extension == ".gif"
            || entry.Extension == ".bmp" || entry.Extension == ".psd";

        if (codeExtBools) nodeString = ICON_FK_CODE + String(" ") + entry.Name + entry.Extension;
        else if (entry.Extension == ".spike" || entry.Extension == ".txt") nodeString = ICON_FK_FILE_TEXT_O + String(" ") + entry.Name + entry.Extension;
        else if (imageExtBools) nodeString = ICON_FK_FILE_IMAGE_O + String(" ") + entry.Name + entry.Extension;
        else if (entry.IsDirectory) nodeString = ICON_FK_FOLDER + String(" ") + entry.Name;
        else nodeString = entry.Name + entry.Extension;

        if (ImGui::TreeNodeEx(nodeString.c_str(), flags))
        {
            if (entry.IsDirectory)
                for (auto& subDirectory : entry.SubEntries)
                    DrawPath(subDirectory);

            if (entry.Extension == ".spike" && ImGui::IsItemClicked(0))
            {
                ((EditorLayer*)s_EditorLayerStorage)->m_ActiveFilepath = entry.AbsolutePath;
                ((EditorLayer*)s_EditorLayerStorage)->m_FirstTimeSave = false;
                ((EditorLayer*)s_EditorLayerStorage)->m_EditorScene = Ref<Scene>::Create();
                ((EditorLayer*)s_EditorLayerStorage)->m_EditorScene->OnViewportResize((uint32_t)((EditorLayer*)s_EditorLayerStorage)->m_ViewportSize.x, (uint32_t)((EditorLayer*)s_EditorLayerStorage)->m_ViewportSize.y);
                ((EditorLayer*)s_EditorLayerStorage)->m_SceneHierarchyPanel.SetContext(((EditorLayer*)s_EditorLayerStorage)->m_EditorScene);

                SceneSerializer serializer(((EditorLayer*)s_EditorLayerStorage)->m_EditorScene);
                serializer.Deserialize(entry.AbsolutePath);
            }

            if (imageExtBools && ImGui::IsItemClicked(0))
            {
                if (s_TexturePreviewStorage)
                    s_TexturePreviewStorage = nullptr;
                s_TexturePreviewStorage = Texture2D::Create(entry.AbsolutePath);
            }

            ImGui::TreePop();
        }
    }
}

/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : CodeEditorPanel
File Type      : cpp
File created on: 2021/02/08
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
#include "Spike/Core/Base.h"
#include "Spike/Core/Input.h"
#include "Spike/Core/Vault.h"
#include "CodeEditorPanel.h"
#include "Spike/Utility/FileDialogs.h"
#include <string>
#include <fstream>
#include <imgui/imgui.h>

namespace Spike
{
    static char s_Star = ' ';

    CodeEditorPanel::CodeEditorPanel()
    {
        ImGuiIO& io = ImGui::GetIO();
        m_Font = io.Fonts->Fonts[1]; //Grab the jetbrains mono font, Loaded at ImGuiLayer.cpp at line 61 (approximately)

        auto lang = TextEditor::LanguageDefinition::CPlusPlus();
        m_TextEditor.SetLanguageDefinition(lang);
        m_TextEditor.SetShowWhitespaces(false);
    }

    void CodeEditorPanel::OnImGuiRender()
    {
        if (m_TextStorage == m_TextEditor.GetText()) s_Star = ' ';
        else                                         s_Star = '*';

        String titleText = "Spike CodeEditor";
        if (!m_CurrentFilePath.empty())
            titleText = ("Spike CodeEditor | " + m_CurrentFileName) + s_Star;

        ImGui::Begin(String(titleText + "###Spike CodeEditor").c_str());

        if (ImGui::Button("Save"))
            this->Save();

        ImGui::PushFont(m_Font);
        m_TextEditor.Render("CodeEditor");
        ImGui::PopFont();

        ImGui::End();
    }

    void CodeEditorPanel::OpenFile(const String& path, const String& extension)
    {
        m_FileType = extension;

        TextEditor::LanguageDefinition lang;
        m_CurrentFileName = Vault::GetNameWithExtension(path);

        if (extension == ".c")
            lang = TextEditor::LanguageDefinition::C();

        if (extension == ".cpp")
            lang = TextEditor::LanguageDefinition::CPlusPlus();

        if (extension == ".cs")
            lang = TextEditor::LanguageDefinition::CPlusPlus();

        else if (extension == ".lua")
            lang = TextEditor::LanguageDefinition::Lua();

        else if (extension == ".glsl")
            lang = TextEditor::LanguageDefinition::GLSL();

        else if (extension == ".hlsl")
            lang = TextEditor::LanguageDefinition::HLSL();

        else if (extension == ".sql")
            lang = TextEditor::LanguageDefinition::SQL();

        else if (extension == ".as")
            lang = TextEditor::LanguageDefinition::AngelScript();

        m_TextEditor.SetLanguageDefinition(lang);
        m_CurrentFilePath = path;

        std::ifstream in(m_CurrentFilePath);
        if (in.good())
        {
            String str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            m_TextEditor.SetText(str);
            m_TextStorage = str;
        }
        else
            SPK_CORE_LOG_ERROR("Error in opening filepath '%s'", path.c_str());

        ImGui::SetWindowFocus(String("Spike CodeEditor - " + m_CurrentFileName + "###Spike CodeEditor").c_str());
    }

    void CodeEditorPanel::Save()
    {
        s_Star = ' ';
        m_CurrentFileName = Vault::GetNameWithExtension(m_CurrentFilePath);
        const char* filepath;
        if (m_CurrentFilePath.empty())
        {
            filepath = FileDialogs::SaveFile("Choose location to save the script", "Script.txt", 0, 0, "Script");
            if (filepath)
                m_CurrentFilePath = filepath;
            else return;
        }

        std::ofstream out(m_CurrentFilePath);
        String text = m_TextEditor.GetText();
        m_TextStorage = text;

        if (out.good())
            out << m_TextEditor.GetText();

    }
}
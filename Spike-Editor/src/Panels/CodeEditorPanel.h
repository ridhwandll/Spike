/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : CodeEditorPanel
File Type      : h
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
#pragma once
#include "TextEditor.h"

namespace Spike
{
    class CodeEditorPanel
    {
    public:
        CodeEditorPanel();
        ~CodeEditorPanel() = default;

        void OnImGuiRender();
        void OpenFile(const String& path, const String& extension = ".cs");
        void Save();
    private:
        TextEditor m_TextEditor;
        ImFont* m_Font;

        String m_TextStorage;
        String m_CurrentFilePath;
        String m_CurrentFileName;
        String m_FileType;
    };
}
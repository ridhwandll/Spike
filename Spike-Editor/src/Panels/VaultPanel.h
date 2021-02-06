/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : VaultPanel
File Type      : h
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
#pragma once
#include "Spike/Core/Base.h"

namespace Spike
{
    struct DirectoryEntry
    {
        String Name;
        String Extension;
        String AbsolutePath;

        bool IsDirectory;
        Vector<DirectoryEntry> SubEntries;
    };

    class VaultPanel
    {
    public:
        VaultPanel();
        ~VaultPanel() = default;

        void OnImGuiRender();
    private:
        void DrawPath(DirectoryEntry& entry);
        Vector<DirectoryEntry> GetFiles(const String& directory);
    private:
        Vector<DirectoryEntry> m_Files;
        String m_ProjectPath;
    };
}

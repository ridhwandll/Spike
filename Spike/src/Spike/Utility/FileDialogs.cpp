/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : FileDialogs
File Type      : cpp
File created on: 2021/02/05
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
#include "spkpch.h"
#include "FileDialogs.h"
#include <tinyfiledialogs.h>

namespace Spike
{
    int FileDialogs::AMessageBox(const String& title, const String& message, DialogType dialogType, IconType iconType, DefaultButton defaultButton)
    {
        int result;
        result = tinyfd_messageBox(title.c_str(), message.c_str(), DialogTypeToString(dialogType), IconTypeToString(iconType), (int)defaultButton);
        return result;
    }

    char const* FileDialogs::InputBox(const String& title, const String& message)
    {
        char const* result;
        result = tinyfd_inputBox(title.c_str(), message.c_str(), "");
        return result;
    }

    char const* FileDialogs::PasswordBox(const String& title, const String& message)
    {
        char const* result;
        result = tinyfd_inputBox(title.c_str(), message.c_str(), NULL);
        return result;
    }

    char const* FileDialogs::OpenFile(const String& title, const int numberOfFilters, char const* const* const filterPatterns, const String& filterDesc, bool allowMultipleSelects)
    {
        char const* result;
        result = tinyfd_openFileDialog(title.c_str(), "", numberOfFilters, filterPatterns, filterDesc.c_str(), (int)allowMultipleSelects);
        return result;
    }

    char const* FileDialogs::SaveFile(const String& title, const int numberOfFilters, char const* const* const filterPatterns, const String& filterDesc)
    {
        char const* result;
        result = tinyfd_saveFileDialog(title.c_str(), "", numberOfFilters, filterPatterns, filterDesc.c_str());
        return result;
    }

    char const* FileDialogs::SelectFolder(const String& title)
    {
        char const* result = tinyfd_selectFolderDialog(title.c_str(), "");
        return result;
    }

    const char* FileDialogs::DialogTypeToString(DialogType type)
    {
        switch (type)
        {
            case DialogType::Ok:              return "ok";
            case DialogType::Ok__Cancel:      return "okcancel";
            case DialogType::Yes__No:         return "yesno";
            case DialogType::Yes__No__Cancel: return "yesnocancel";
        }
        SPK_CORE_LOG_WARN("Invalid DialogType!"); /* [Spike] Unreachable [Spike] */
        return "ok";
    }

    const char* FileDialogs::IconTypeToString(IconType type)
    {
        switch (type)
        {
            case IconType::Info:     return "info";
            case IconType::Warning:  return "warning";
            case IconType::Error:    return "error";
            case IconType::Question: return "question";
        }
        SPK_CORE_LOG_WARN("Invalid IconType!"); /* [Spike] Unreachable [Spike] */
        return "error";
    }
}
/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : FileDialogs
File Type      : h
File created on: 2021/02/05
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once

namespace Spike
{
    enum class DialogType
    {
        Ok = 0,
        Ok__Cancel,
        Yes__No,
        Yes__No__Cancel
    };

    enum class IconType
    {
        Info = 0,
        Warning,
        Error,
        Question
    };

    enum class DefaultButton
    {
        No = 0,
        Yes = 1,
        NO_in_YesNoCancel = 2
    };

    /* [Spike] Wrapper around the tinyfiledialogs [Spike] */
    class FileDialogs
    {
    public:
        FileDialogs() = default;
        ~FileDialogs() = default;

        static int AMessageBox(const String& title, const String& message, DialogType dialogType, IconType iconType, DefaultButton defaultButton);
        static char const* InputBox(const String& title, const String& message);
        static char const* PasswordBox(const String& title, const String& message);
        static char const* OpenFile(const String& title, const String& defaultName, const int numberOfFilters, char const* const* const filterPatterns, const String& filterDesc, bool allowMultipleSelects);
        static char const* SaveFile(const String& title, const String& defaultName, const int numberOfFilters, char const* const* const filterPatterns, const String& filterDesc);
        static char const* SelectFolder(const String& title);
    private:
        static const char* DialogTypeToString(DialogType type);
        static const char* IconTypeToString(IconType type);
    };
}

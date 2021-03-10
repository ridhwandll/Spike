//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Core/Base.h"
#include <glm/glm.hpp>

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
        VaultPanel(const void* editorLayerPtr);
        ~VaultPanel() = default;

        void OnImGuiRender();
    private:
        void DrawPath(DirectoryEntry& entry);
        Vector<DirectoryEntry> GetFiles(const String& directory);
        void DrawImageAtMiddle(const glm::vec2& imageRes, const glm::vec2& windowRes);
    private:
        Vector<DirectoryEntry> m_Files;
        String m_ProjectPath;
    };
}

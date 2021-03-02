/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Vault
File Type      : h
File created on: 2021/02/04
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
#include "Spike/Renderer/Shader.h"
#include "Spike/Renderer/Texture.h"
#include <unordered_map>

namespace Spike
{
    enum class ResourceType
    {
        SHADER = 0, TEXTURE, SCRIPT
    };

    class Vault
    {
    public:
        Vault() = default;
        ~Vault() = default;

        static void Init(const String& projectPath);
        static void Shutdown();
        static bool Reload();

        template <typename T>
        static void Submit(Ref<T>& resource)
        {
            if constexpr (std::is_same_v<T, Shader>)
            {
                auto filepath = resource->GetFilepath();
                for (auto& cacheShader : s_Shaders)
                    if (cacheShader.first == filepath)
                        return;
                s_Shaders[filepath] = resource;
            }
            if constexpr (std::is_same_v<T, Texture2D>)
            {
                auto filepath = resource->GetFilepath();
                for (auto& cacheTexture : s_Textures)
                    if (cacheTexture.first == filepath)
                        return;
                s_Textures[filepath] = resource;
            }
        }

        template <typename T>
        static Ref<T> Get(const String& nameWithExtension)
        {
            if constexpr (std::is_same_v<T, Shader>)
            {
                for (auto& shader : s_Shaders)
                    if (GetNameWithExtension(shader.first) == nameWithExtension)
                        return shader.second;
            }
            if constexpr (std::is_same_v<T, Texture2D>)
            {
                for (auto& texture : s_Textures)
                    if (GetNameWithExtension(texture.first) == nameWithExtension)
                        return texture.second;
            }
            return nullptr;
        }

        /* [Spike] Filepath utilities [Spike] */
        static String GetNameWithoutExtension(const String& assetFilepath);
        static String GetNameWithExtension(const String& assetFilepath);
        static String GetExtension(const String& assetFilepath);
        static String GetProjectPath() { return s_ProjectPath; }

        static bool Exists(const String& nameWithExtension, ResourceType type);
        static bool Exists(const char* path, ResourceType type);
        static bool IsVaultInitialized();

        static Vector<Ref<Shader>> GetAllShaders();
        static Vector<Ref<Texture>> GetAllTextures();
        static Vector<String> GetAllDirsInProjectPath();
        static Vector<String> GetAllFolderNamesProjectPath();

        /* [Spike] Mapped as { filepath : Resource } [Spike] */
        static std::unordered_map<String, String> GetAllScripts();

        static bool CreateFolder(const char* parentDirectory, const char* name);
        static void ClearAllCache();

        /* [Spike] File Readers [Spike] */
        static String ReadFile(const String& filepath);
        static Vector<char> ReadBinaryFile(const String& filepath);
    private:
        static String s_ProjectPath; /* [Spike] Base Path, such as: "C:/Users/Dummy/Desktop/SpikeProject" [Spike] */
        static bool s_VaultInitialized;

        /* [Spike] Mapped as { filepath : Resource  } [Spike] */
        static std::unordered_map<String, Ref<Shader>> s_Shaders;
        static std::unordered_map<String, Ref<Texture2D>> s_Textures;
        static std::unordered_map<String, String> s_Scripts;
    };
}

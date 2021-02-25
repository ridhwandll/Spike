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
        _Shader, _Texture, _Script
    };

    class Vault
    {
    public:
        Vault() = default;
        ~Vault() = default;

        /* [Spike] Returns the path, in which the scene was saved [Spike] */
        static String Init(const String& projectPath);
        static void Shutdown();
        static bool Reload();

        static Ref<Shader> SubmitShader(Ref<Shader>& shader);
        static Ref<Shader> SubmitBuiltInShader(Ref<Shader>& shader);
        static Ref<Texture2D> SubmitTexture2D(Ref<Texture2D>& texture);

        static Ref<Shader> GetBuiltInShaderFromCache(const String& nameWithoutExtension);
        static Ref<Shader> GetShaderFromCache(const String& nameWithExtension);
        static Ref<Texture> GetTexture2DFromCache(const String& nameWithExtension);

        /* [Spike] Filepath utilities [Spike] */
        static String GetNameWithoutExtension(const String& assetFilepath);
        static String GetNameWithExtension(const String& assetFilepath);
        static String GetExtension(const String& assetFilepath);
        static String GetProjectPath() { return s_ProjectPath; }

        static bool Exists(const String& nameWithExtension, ResourceType type);
        static bool Exists(const char* path, ResourceType type);
        static bool IsVaultInitialized();
        static Vector<Ref<Shader>> GetAllShaders();
        static Vector<Ref<Shader>> GetAllBuiltInShaders();
        static Vector<Ref<Texture>> GetAllTextures();
        static Vector<String> GetAllDirsInProjectPath();
        static Vector<String> GetAllFolderNamesProjectPath();

        /* [Spike] Mapped as { filepath : Resource } [Spike] */
        static std::unordered_map<String, String> GetAllScripts();

        static bool CreateFolder(const char* parentDirectory, const char* name);
        static void ClearAllCache();
        static String ReadFile(const String& filepath);
    private:
        static String s_ProjectPath; /* [Spike] Base Path, such as: "C:/Users/Dummy/Desktop/SpikeProject" [Spike] */
        static bool s_VaultInitialized;
        static Vector<Ref<Shader>> s_BuiltInShaders;

        /* [Spike] Mapped as { filepath : Resource  } [Spike] */
        static std::unordered_map<String, Ref<Shader>> s_Shaders;
        static std::unordered_map<String, Ref<Texture2D>> s_Textures;
        static std::unordered_map<String, String> s_Scripts;
    };
}

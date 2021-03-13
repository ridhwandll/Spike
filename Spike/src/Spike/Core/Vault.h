//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
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

        template<typename T>
        static std::unordered_map<String, Ref<T>>& GetMap()
        {
            if constexpr (std::is_same_v<T, Shader>)
                return s_Shaders;
            if constexpr (std::is_same_v<T, Texture2D>)
                return s_Textures;
            SPK_INTERNAL_ASSERT("Unknown Resource type");
        }

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
            const std::unordered_map<String, Ref<T>>& resources = GetMap<T>();
            for (auto& res : resources)
                if (GetNameWithExtension(res.first) == nameWithExtension)
                    return res.second;
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
        static Vector<String> GetAllFilePathsFromParentPath(const String& path);

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

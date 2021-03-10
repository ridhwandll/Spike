//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "Vault.h"
#include <filesystem>

namespace Spike
{
    String Vault::s_ProjectPath = "";
    bool Vault::s_VaultInitialized = false;
    std::unordered_map<String, Ref<Shader>>             Vault::s_Shaders;
    std::unordered_map<String, Ref<Texture2D>>          Vault::s_Textures;
    std::unordered_map<String, String>                  Vault::s_Scripts;

    void Vault::Init(const String& projectPath)
    {
        if (s_VaultInitialized)
            Vault::Shutdown();

        s_ProjectPath = projectPath;
        s_VaultInitialized = true;
        Reload();
    }

    void Vault::Shutdown()
    {
        s_ProjectPath.clear();
        ClearAllCache();
    }

    bool Vault::Reload()
    {
        Vault::ClearAllCache();
        for (const auto& entry : std::filesystem::recursive_directory_iterator(s_ProjectPath))
        {
            if (GetExtension(entry.path().string()) == ".glsl")
            {
                auto shader = Shader::Create(entry.path().string().c_str());
                s_Shaders[entry.path().string()] = shader.Raw();
            }
            if (GetExtension(entry.path().string()) == ".png")
            {
                auto texture = Texture2D::Create(entry.path().string().c_str());
                s_Textures[entry.path().string()] = texture.Raw();
            }
            if (GetExtension(entry.path().string()) == ".jpg")
            {
                auto texture = Texture2D::Create(entry.path().string().c_str());
                s_Textures[entry.path().string()] = texture.Raw();
            }
            if (GetExtension(entry.path().string()) == ".cs")
            {
                s_Scripts[entry.path().string()] = ReadFile(entry.path().string());
            }
        }
        return true;
    }

    String Vault::GetNameWithoutExtension(const String& assetFilepath)
    {
        String name;
        auto lastSlash = assetFilepath.find_last_of("/\\");
        lastSlash = lastSlash == String::npos ? 0 : lastSlash + 1;
        auto lastDot = assetFilepath.rfind('.');
        auto count = lastDot == String::npos ? assetFilepath.size() - lastSlash : lastDot - lastSlash;
        name = assetFilepath.substr(lastSlash, count);
        return name;

    }

    bool Vault::Exists(const String& nameWithExtension, ResourceType type)
    {
        switch (type)
        {
        case ResourceType::SHADER:
            for (auto& shader : s_Shaders)
                if (GetNameWithExtension(shader.first) == nameWithExtension)
                    return true;

        case ResourceType::TEXTURE:
            for (auto& texture : s_Textures)
                if (GetNameWithExtension(texture.first) == nameWithExtension)
                    return true;

        case ResourceType::SCRIPT:
            for (auto& script : s_Scripts)
                if (GetNameWithExtension(script.first) == nameWithExtension)
                    return true;
        }
        return false;
    }

    bool Vault::Exists(const char* path, ResourceType type)
    {
        switch (type)
        {
        case Spike::ResourceType::SHADER:
            for (auto& shader : s_Shaders)
                if (shader.first == path)
                    return true;

        case Spike::ResourceType::TEXTURE:
            for (auto& texture : s_Textures)
                if (texture.first == path)
                    return true;

        case Spike::ResourceType::SCRIPT:
            for (auto& script : s_Scripts)
                if (script.first == path)
                    return true;
        }
        return false;
    }

    Vector<Ref<Shader>> Vault::GetAllShaders()
    {
        Vector<Ref<Shader>> shaders;
        shaders.resize(s_Shaders.size());
        for (auto& shader : s_Shaders)
        {
            shaders.emplace_back(shader.second);
        }
        return shaders;
    }

    Vector<Ref<Texture>> Vault::GetAllTextures()
    {
        Vector<Ref<Texture>> textures;
        textures.resize(s_Textures.size());
        for (auto& texture : s_Textures)
        {
            textures.emplace_back(texture.second);
        }
        return textures;
    }

    Vector<String> Vault::GetAllDirsInProjectPath()
    {
        Vector<String> paths;
        for (const auto& entry : std::filesystem::directory_iterator(s_ProjectPath))
        {
            paths.push_back(entry.path().string());
        }
        return paths;
    }

    Vector<String> Vault::GetAllFolderNamesProjectPath()
    {
        Vector<String> names;
        for (const auto& entry : std::filesystem::directory_iterator(s_ProjectPath))
        {
            names.push_back(GetNameWithoutExtension(entry.path().string()));
        }
        return names;
    }

    std::unordered_map<String, String> Vault::GetAllScripts()
    {
        return s_Scripts;
    }

    bool Vault::CreateFolder(const char* parentDirectory, const char* name)
    {
        String path = String(parentDirectory) + "/" + String(name);
        if (std::filesystem::create_directory(path) || std::filesystem::exists(path))
            return true;
        return false;
    }

    void Vault::ClearAllCache()
    {
        s_Textures.clear();
        s_Shaders.clear();
        s_Scripts.clear();
    }

    String Vault::ReadFile(const String& filepath)
    {
        String result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
        }
        else
        {
            SPK_CORE_LOG_CRITICAL("Could not open file path \"%s\"", filepath.c_str());
        }
        return result;
    }

    Vector<char> Vault::ReadBinaryFile(const String& filepath)
    {
        std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

        if (!stream)
            SPK_CORE_LOG_ERROR("Cannot open filepath: %s!", filepath.c_str());

        auto end = stream.tellg();
        stream.seekg(0, std::ios::beg);
        auto size = std::size_t(end - stream.tellg());
        if (size == 0) return {};

        Vector<char> buffer(size);
        if (!stream.read((char*)buffer.data(), buffer.size()))
            SPK_CORE_LOG_ERROR("Cannot read file: %s", filepath.c_str());
        return buffer;
    }

    String Vault::GetExtension(const String& assetFilepath)
    {
        return std::filesystem::path(assetFilepath).extension().string();
    }

    String Vault::GetNameWithExtension(const String& assetFilepath)
    {
        return std::filesystem::path(assetFilepath).filename().string();
    }

    bool Vault::IsVaultInitialized()
    {
        return s_VaultInitialized;
    }
}
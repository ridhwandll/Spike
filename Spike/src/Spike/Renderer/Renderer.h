/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Renderer
File Type      : h
File created on: 2021/01/09
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

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "RenderCommand.h"
#include "EditorCamera.h"
#include "Mesh.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Spike
{
    class Mesh;
    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();
        static void OnWindowResize(uint32_t width, uint32_t height);
        static void BeginScene(EditorCamera& camera);
        static void EndScene();

        static void SubmitMesh(Ref<Mesh> mesh, const glm::mat4& transform);
        static void SubmitMesh(Ref<Mesh> mesh, uint32_t entityID, const glm::mat4& transform); //BIG TODO: mousepicking on 3D
        static void Submit(Ref<Shader> shader, Ref<VertexArray> vertexArray, const glm::mat4& transform);

        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };
        static Scope<SceneData> s_SceneData;
    };
}
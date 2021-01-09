#type vertex
#version 430 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 4) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;

out vec3 v_Normal;

void main()
{
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
    v_Normal = a_Normal;
}

#type fragment
#version 430 core

out vec4 finalColor;
in vec3 v_Normal;

void main()
{
    finalColor = vec4((v_Normal * 0.5 + 0.5), 1.0);
}
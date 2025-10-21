[vertex]
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 4) in vec4 a_Color;

out vec4 v_Color;

uniform mat4 u_ViewProjection;

void main()
{
    v_Color = a_Color;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

[fragment]
#version 460 core
in vec4 v_Color;

out vec4 FragColor;

void main()
{
    FragColor = v_Color;
}

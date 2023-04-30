#version 410 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;

out float Height;
out vec3 Position;

out vec3 FragPos;
out vec3 Normal;
//out vec3 outColor
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //outColor = aColor;
    FragPos = vec3(model * vec4(aPos, 1.0f));
    Normal = mat3(transpose(inverse(model))) *  vec3(0.0, 1.0, 0.0);
    Height = aPos.y;
    Position = (view * model * vec4(aPos, 1.0)).xyz;
    gl_Position = projection * view * model * vec4(aPos, 1.0);

}
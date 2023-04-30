#version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out float Height;
void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0f));
    Height = aPos.y;
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
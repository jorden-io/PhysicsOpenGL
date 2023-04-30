#version 410 core
out vec4 FragColor;

in float Height;

void main()
{
    float h = (Height + 8)/16.0f;
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
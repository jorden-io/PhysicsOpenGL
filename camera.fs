#version 330 core
out vec4 FragColor;

in vec3 ourColor;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

    FragColor = vec4(0.0f, 0.5f, 0.0f, 1.0f);
    //FragColor = vec4(ourColor, 1.0);
}
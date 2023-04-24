#version 330 core
out vec4 FragColor;

in vec3 ourColor;
uniform vec3 outColor;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
    FragColor = vec4(0.0f, 0.2f, 0.0f, 0.5f);
    //FragColor = vec4(ourColor, 1.0f);
    //FragColor = vec4(0.0 + outColor.x, 0.0 + outColor.y, 0.0 + outColor.z, 1.0);
}
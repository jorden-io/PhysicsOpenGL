#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

//in vec3 ourColor;
uniform float height;
uniform vec3 viewPos;
//uniform vec3 outColor;
uniform vec3 lightPos;

void main()
{
    vec3 ourColor;
    if(height > 42){
        ourColor = vec3(0.5f, 0.5f, 0.5f);
    }
    else if(height > 34){
        ourColor = vec3(0.1f, 0.1f, 0.1f);
    }
    else if(height > 25)
    {
        ourColor = vec3(0.0f, 0.30f, 0.0f);
    }
    else if(height > 18){
        ourColor = vec3(0.0f, 0.45f, 0.0f);
    }
    else if(height < 16){
        //ourColor = vec3(0.588f, 0.294f, 0.0f);
        ourColor = vec3(0.0f, 0.0f, 0.3f);
    }
    else
    {
        ourColor = vec3(0.89f, 0.737f, 0.604f);
    };
    vec3 lightColor = vec3(0.0f, 0.0f, 0.3f);
    //vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

    //FragColor = vec4(0.0f, 0.3f, 0.0f, 1.0f);
    float ambientStrength = 0.3f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir  = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 256);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * ourColor;
    FragColor = vec4(result, 1.0f);
    //FragColor = vec4(ourColor, 1.0f);
    //FragColor = vec4(0.0 + outColor.x, 0.0 + outColor.y, 0.0 + outColor.z, 1.0);
}
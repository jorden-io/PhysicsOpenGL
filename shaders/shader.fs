#version 410 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

in vec3 outColor;
uniform vec3 viewPos;
uniform vec3 lightPos;

void main()
{
    vec3 ourColor = vec3(0.0f, 0.0f, 1.0f);
    //vec3 ourColor = vec3(0.0f, 0.0f, 0.3f);
    vec3 lightColor = vec3(outColor * outColor);

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
#version 410 core
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
    float h = (FragPos.y + 16)/32.0f;
   //vec3 ourColor = vec3(h * 1.5, h * 1.5, h * 1.5);
    vec3 ourColor = vec3(0.2f, 0.8f, 0.7f);
    vec3 lightColor = vec3(1.0f);

    float ambientStrength = 0.3f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir  = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.8f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 256);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse) * ourColor;
    FragColor = vec4((result), 1.0f);
}
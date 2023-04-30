#version 410 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in float Height;
//in vec3 outColor;

uniform float fheight;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform float random;

void main()
{
    vec3 ourColor;

    float h = (Height + 8)/16.0f;
    ourColor = vec3(h + 0.0, h + 0.0, h + 0.5);
    // if(h >= 1.0){
    //     ourColor = vec3(h + 1.0, h, h);
    // }
    // else if(h > 0.9){
    //     ourColor = vec3(h + 0.9, h, h);
    // }
    // else if(h > 0.8){
    //     ourColor = vec3(h + 0.8, h, h);
    // }
    // else if(h > 0.7){
    //     ourColor = vec3(h + 0.7, h, h);
    // }
    // else if(h > 0.6){
    //     ourColor = vec3(h + 0.6, h, h);
    // }
    // else if(h > 0.5){
    //     ourColor = vec3(h + 0.5, h, h);
    // }
    // else if(h > 0.3){
    //     ourColor = vec3(h + 0.4, h, h);
    // }
    // else if(h > 0.3){
    //     ourColor = vec3(h + 0.3, h, h);
    // }
    // else if(h > 0.2){
    //     ourColor = vec3(h + 0.2, h, h);
    // }
    // else{
    //     ourColor = vec3(h + 0.1, h, h);
    // }
    //vec3 ourColor = vec3(h, h, h + 0.3);
    vec3 lightColor = vec3(h + 0.8, h + 0.8, h + 0.8);
    //vec3 lightColor = vec3(1.0, 1.0, 1.0);

    float ambientStrength = 0.05;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir  = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.8f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 256);
    vec3 specular = specularStrength * spec * lightColor * h;

    vec3 result = (ambient + diffuse + specular) * ourColor;
    FragColor = vec4(result, 0.35f);
}
#version 410 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in float Height;
//in vec3 outColor;

uniform float fheight;
uniform vec3 viewPos;
uniform vec3 lightPos;

void main()
{
    vec3 ourColor;

    float h = (Height + 128)/256.0f;
    if(h > 1.25){
        ourColor = vec3(0.85, 0.85, 0.85);
    }
    if(h > 1.20){
        ourColor = vec3(0.80, 0.80, 0.80);
    }
    else if(h > 1.00){
        ourColor = vec3(0.70, 0.70, 0.70);
    }
    else if(h > 0.80){
        ourColor = vec3(0.60, 0.60, 0.60);
    }
    else if(h > 0.75){
        ourColor = vec3(0.50, 0.50, 0.50);
    }
    else if(h > 0.65){
        ourColor = vec3(0.30, 0.30, 0.30);
    }
    // else if(h > 0.495){
    //     ourColor = vec3(0.75, 0.75, 0.75);
    // }
    // else if(h > 0.35){
    //     ourColor = vec3(0.70, 0.70, 0.70);
    // }
    else if(h > 0.55){
        ourColor = vec3(0.35, 0.35, 0.35);
    }
    else if(h > 0.45){
        ourColor = vec3(0.38, 0.38, 0.38);
    }
    else if(h > 0.35){
        ourColor = vec3(0.40, 0.40, 0.40);
    }
    else if(h > 0.30){
        ourColor = vec3(0.0, 0.28, 0.0);
    }
    else if(h > 0.25){
        ourColor = vec3(0.0, 0.30, 0.0);
    }
    // else if(h > 0.20){
    //     ourColor = vec3(0.55, 0.55, 0.55);
    // }
    else if(h > 0.15){
        ourColor = vec3(0.0, 0.36, 0.0);
    }
    else if(h > 0.0010){
        ourColor = vec3(0.0, 0.32, 0.0);
    }
    else{
        ourColor = vec3(h + 0.89,h + 0.737,h + 0.604);
    }
    //vec3 ourColor = vec3(outColor);
    //ourColor = vec3(h, h, h + 0.3);
    vec3 lightColor = vec3(h + 0.8, h + 0.8, h + 0.8);
    //vec3 lightColor = vec3(0.6, 0.6, 0.6);

    float ambientStrength = 0.05f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir  = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.1f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 2);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * ourColor;
    FragColor = vec4(result, 1.0f);
}
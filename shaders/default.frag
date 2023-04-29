#version 330 core

out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

vec4 point_light()
{
    vec3 lightVector = lightPos - FragPos;
    float distance = length(lightVector);
    float a = 3.0;
    float b = 0.7;
    float intensity = 1.0 / (a * distance * distance + b * distance + 1.0);

    float ambient = 0.2;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightVector);
    float diffuse = max(dot(normal, lightDir), 0.0);

    float specularLight = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    float specular = specularLight * spec;

    return texture(texture1, TexCoord) * lightColor * (diffuse * intensity + ambient) + texture(texture2, TexCoord).r * specular * intensity;
}

void main()
{
    FragColor = point_light();
}
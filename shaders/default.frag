#version 330 core

out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    float ambient = 0.2;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diffuse = max(dot(normal, lightDir), 0.0);

    float specularLight = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float specular = specularLight * spec;

    FragColor = texture(texture1, TexCoord) * lightColor * (diffuse + ambient + specular);
}
define_asset PerVertex GLSL
define_matrices ViewProjMatrix ModelMatrix NormalMatrix
define_textures texSampler
define_material material.Ambient material.Diffuse material.Specular material.Shininess material.SpecularMul
define_camera CameraPos
define_lights LightCount

[begin] VertexShader
#version 330 core

layout(location = 0) in vec3 v_Pos;
layout(location = 1) in vec3 v_Normal;
layout(location = 2) in vec2 v_TexCoord;

struct Light {
    
    vec3 Position;
    vec3 Direction;
    
    vec3 Diffuse;
    vec3 Specular;
    
    vec4 Attenuation;
    
    float Focus;
    
};

struct Material {
    
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    
    float Shininess;
    float SpecularMul;
    
};


uniform Light lights[50];
uniform int LightCount;
uniform Material material;

uniform vec3 CameraPos;

uniform mat4 ViewProjMatrix;
uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;

out vec2 TexCoord;

varying vec4 v_Color;

void main() {
    
    float Distance;
    float Attenuation;
    
    vec3  FragPos = vec3(ModelMatrix * vec4(v_Pos, 1.0));
    vec3  viewDir = normalize(CameraPos - FragPos);
    
    vec3  Norm    = vec3(0.0, 0.0, 0.0);
    vec3  Reflect = vec3(0.0, 0.0, 0.0);
    float Spec    = 0.0;
    
    vec3  lightDir;
    
    vec3  lightDiffuse  = vec3(0.0, 0.0, 0.0);
    vec3  lightSpecular = vec3(0.0, 0.0, 0.0);
    
    for(int i=0; i <= LightCount; i++) {
        
        Distance = length(lights[i].Position - FragPos);
        if (Distance > lights[i].Attenuation.r) {continue;}
        
        Attenuation = 1.0 / (lights[i].Attenuation.g + lights[i].Attenuation.b * Distance + lights[i].Attenuation.a * pow(Distance * Distance, lights[i].Focus));
        
        lightDir = normalize(lights[i].Position - FragPos);
        
        Norm    = normalize(v_Normal);
        Reflect = reflect(-lightDir, Norm);
        Spec    = pow( max( dot(viewDir, Reflect ), 0.0 ), material.Shininess) * material.SpecularMul;
        
        lightDiffuse  += lights[i].Diffuse  * Attenuation;
        lightSpecular += lights[i].Specular * Spec;
        
    }
    
    v_Color = vec4(material.Ambient + (lightDiffuse * material.Diffuse) + (lightSpecular * material.Specular), 1.0);
    
    TexCoord = v_TexCoord;
    
    gl_Position = (ViewProjMatrix * ModelMatrix) * vec4(v_Pos, 1.0);
}
[end]

[begin] FragmentShader
#version 330 core

uniform sampler2D texSampler;

in vec2 TexCoord;

in vec3 matAmbient;
in vec3 matDiffuse;
in vec3 matSpecular;
in float matShininess;

varying vec4 v_Color;

out vec4 FragColor;

void main() {
    
    float Gamma = 2.2;
    
    FragColor = vec4( pow(v_Color.rgb, vec3(1.0/Gamma) ), v_Color.a ) * texture(texSampler, TexCoord);
    
}
[end]

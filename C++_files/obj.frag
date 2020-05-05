#version 150

//DEssa måste vara fler eller lika med så många ljus man skickar in
#define NUMBER_OF_POINT_LIGHTS 10
#define NUMBER_OF_DIR_LIGHTS 10
#define NUMBER_OF_SPOT_LIGHTS 10
struct PointLight
{
   vec3 position;
   float constant;
   float linear;
   float quadratic;
   float amp;
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   vec3 colour;

};

struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 colour;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    float amp;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 colour;
};

uniform PointLight pointLightz[NUMBER_OF_POINT_LIGHTS];
uniform DirLight dirLightz[NUMBER_OF_DIR_LIGHTS];
uniform SpotLight spotLightz[NUMBER_OF_SPOT_LIGHTS];

in vec3 normal;
in vec2 exTexCoord;
in vec3 pos;
in vec3 fragPos;

uniform vec3 viewPos;
uniform int number_of_point_lights;
uniform int number_of_dir_lights;
uniform int number_of_spot_lights;

out vec4 out_Color;
uniform sampler2D Tex;
uniform float timer;
uniform vec4 opac;

//Function declarations
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDirection);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);


/////////////////////////////////PointLight

vec3 calcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
    vec3 lightDir = normalize( light.position - fragPos );

    // Diffuse shading
    float diff = max( dot( normal, lightDir ), 0.0 );

    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), 200 );

    // Attenuation
    float distance = length( light.position - fragPos );
    float attenuation = light.amp / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );

    // Combine results
    vec3 ambient = light.ambient * light.colour;//vec3( texture( material.diffuse, TexCoords ) );
    vec3 diffuse = light.diffuse * diff *light.colour;// vec3( texture( material.diffuse, TexCoords ) );
    vec3 specular = light.specular * spec *light.colour; //vec3( texture( material.specular, TexCoords ) );

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ( ambient + diffuse + specular );
}

/////////////////////////69///////Directional Lighting

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDir = normalize( -light.direction );

    // Diffuse shading
    float diff = max( dot( normal, lightDir ), 0.0 );

    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDirection, reflectDir ), 0.0 ), 0.4 );

    // Combine results
    vec3 ambient = light.ambient * light.colour; //vec3( texture( material.diffuse, TexCoords ) );
    vec3 diffuse = light.diffuse * diff * light.colour; //vec3( texture( material.diffuse, TexCoords ) );
    vec3 specular = light.specular * spec * light.colour;//vec3( texture( material.specular, TexCoords ) );

    return ( ambient + diffuse + specular );

}
////////////////////////spotlight
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
    vec3 lightDir = normalize( light.position - fragPos );

    // Diffuse shading
    float diff = max( dot( normal, lightDir ), 0.0 );

    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDirection, reflectDir ), 0.0 ), 256 );

    // Attenuation
    float distance = length( light.position - fragPos );
    float attenuation = light.amp / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );

    // Spotlight intensity
    float theta = dot( lightDir, normalize( -light.direction ) );
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp( ( theta - light.outerCutOff ) / epsilon, 0.0, 1.0 );

    // Combine results
    vec3 ambient = light.ambient * light.colour; //vec3( texture( material.diffuse, TexCoords ) );
    vec3 diffuse = light.diffuse * diff * light.colour; //vec3( texture( material.diffuse, TexCoords ) );
    vec3 specular = light.specular * spec * light.colour;// vec3( texture( material.specular, TexCoords ) );

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ( ambient + diffuse + specular );
}



void main(void)
{
vec4 textures;
float drawoverheight;
if(pos.x > 0.0){
  drawoverheight = opac.z * abs(pos.x  - 30.0)/30.0 + opac.w*pos.x/30.0;
}
else{
  drawoverheight = opac.y * abs(pos.x + 30.0)/30.0 + opac.x * abs(pos.x)/30.0;
  }

if(drawoverheight > pos.y){
  textures = texture(Tex, exTexCoord);
  textures.w = 0.0;
}
else {
  textures = texture(Tex, exTexCoord);
  textures.w = timer;
}
vec3 norm = normalize(normal);
vec3 viewDirection = normalize(viewPos - fragPos);
vec3 pointResult = vec3(0.0f,0.0f,0.0f);
vec3 dirResult = vec3(0.0f,0.0f,0.0f);
vec3 spotResult = vec3(0.0f,0.0f,0.0f);

for ( int i= 0; i < number_of_point_lights; i++){
  // pointResult += calcPointLight(pointLightz[i], norm, fragPos, viewDirection);
}

for ( int i= 0; i < number_of_dir_lights; i++){
   dirResult += calcDirLight(dirLightz[i], norm, viewDirection);
}

for ( int i= 0; i < number_of_spot_lights; i++){
   spotResult += calcSpotLight(spotLightz[i], norm, fragPos, viewDirection);
}


vec3 result = (  spotResult + dirResult + pointResult );
out_Color = textures*vec4(result, 1.0f);

}

#version 150

#define NUMBER_OF_POINT_LIGHTS 1
struct PointLight
{
   vec3 position;

   float constant;
   float linear;
   float quadratic;

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

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 colour;
};

// In variables
in vec3 normal;
in vec2 exTexCoord;
in vec3 fragPos;

// Uniform
uniform vec3 viewPos;
uniform int number_of_point_lights;
//uniform PointLight pointLight;
uniform PointLight pointLightz[1];
//uniform DirLight dirLight;
//uniform SpotLight spotLight;
uniform sampler2D Tex;

//uniform vec3 fragColour;

// Out variables
out vec4 colour;

//Function declarations
//vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);
//vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDirection);
//vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);


/////////////////////////////////PointLight

vec3 calcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize( light.position - fragPos );

    // Diffuse shading
    float diff = max( dot( normal, lightDir ), 0.0 );

    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), 32 );

    // Attenuation
    float distance = length( light.position - fragPos );
    float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic*(distance * distance));

    // Combine results
    vec3 ambient = light.ambient * light.colour;
    vec3 diffuse = light.diffuse * light.colour;
    vec3 specular = light.specular * light.colour;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ( ambient + diffuse + specular );

}

/////////////////////////69///////Directional Lighting

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDirection)
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
    vec3 specular = light.specular * spec * light.colour; //vec3( texture( material.specular, TexCoords ) );

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
    float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );

    // Spotlight intensity
    float theta = dot( lightDir, normalize( -light.direction ) );
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp( ( theta - light.outerCutOff ) / epsilon, 0.0, 1.0 );

    // Combine results
    vec3 ambient = light.ambient * light.colour; //vec3( texture( material.diffuse, TexCoords ) );
    vec3 diffuse = light.diffuse * diff * light.colour; //vec3( texture( material.diffuse, TexCoords ) );
    vec3 specular = light.specular * spec * light.colour * 0.5;// vec3( texture( material.specular, TexCoords ) );

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ( ambient + diffuse + specular );
}


////////////////////Main////////////////////
void main(void)
{
  vec3 norm = normalize(normal);
  vec3 viewDirection = normalize(viewPos - fragPos);
  //vec3 res = vec3(0.0f,0.0f,0.0f);
  //for ( int i= 0; i < 1; i++)
  //{
  vec3 res = calcPointLight(pointLightz[0], norm, fragPos, viewDirection);
  //}

  //vec3 pointLights = calcPointLight(pointLight, norm, fragPos, viewDirection);//*lightColour;
  //vec3 dirLights = CalcDirLight(dirLight, norm, viewDirection);
  //vec3 spotLights = calcSpotLight(spotLight, norm, fragPos, viewDirection);

  vec3 result = ( res ) * vec3( texture(Tex, exTexCoord ) );
  colour = vec4(result, 1.0f);
}

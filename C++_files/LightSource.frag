#version 150

struct PointLight
{
   vec3 position;

   float constant;
   float linear;
   float quadratic;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

};


struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// In variables
in vec3 normal;
in vec2 exTexCoord;
in vec3 fragPos;

// Uniform
uniform vec3 viewPos;
uniform PointLight pointLight;
uniform DirLight dirLight;
uniform sampler2D boxTex;
uniform vec3 fragColour; //Ta bort sen när textures kommer in
uniform vec3 lightColour;

// Out variables
out vec4 colour;

//Function declarations
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDirection);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDirection);


/////////////////////////////////PointLight

vec3 calcPointLight( PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize( pointLight.position - fragPos );

    // Diffuse shading
    float diff = max( dot( normal, lightDir ), 0.0 );

    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), 36);//lägg till material.shinieness

    // Attenuation
    float distance = length( pointLight.position - fragPos );
    float attenuation = 1.0f / ( pointLight.constant + pointLight.linear * distance + pointLight.quadratic * ( distance * distance ) );

    // Combine results Lägg till texture koordinater här sen
    vec3 ambient = pointLight.ambient * lightColour;
    vec3 diffuse = pointLight.diffuse * diff * lightColour; //vec3( texture( material.diffuse, TexCoords ) );
    vec3 specular = pointLight.specular * spec * lightColour; //vec3( texture( material.specular, TexCoords ) );

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ( ambient + diffuse + specular );

}


////////////////////Main////////////////////
void main(void)
{
  vec3 norm = normalize(normal);
  vec3 viewDirection = normalize(viewPos - fragPos);

  vec3 result = calcPointLight(pointLight, norm, fragPos, viewDirection);//*lightColour;

  result = result  * vec3( texture( boxTex, exTexCoord ) );
  colour = vec4(result, 1.0f);//*vec3( texture( boxTex, exTexCoord ) );
}

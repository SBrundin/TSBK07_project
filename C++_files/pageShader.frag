#version 150

//DEssa måste vara fler eller lika med så många ljus man skickar in
#define NUMBER_OF_POINT_LIGHTS 2
#define NUMBER_OF_DIR_LIGHTS 1
#define NUMBER_OF_SPOT_LIGHTS 1

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

in vec3 ex_Normal;
in vec2 ex_TexCoord;
in vec3 in_Normal;
in vec3 position;
in vec3 normal; // samma som in_Normal hehe ändra sen
in vec3 fragPos;

out vec4 out_Color;
uniform sampler2D Tex, TexUp0, TexUp1, TexUp2, sideTex;
uniform int ID;
uniform vec3 viewPos;
uniform int number_of_point_lights;
uniform int number_of_dir_lights;
uniform int number_of_spot_lights;

uniform PointLight pointLightz[NUMBER_OF_POINT_LIGHTS];
uniform DirLight dirLightz[NUMBER_OF_DIR_LIGHTS];
uniform SpotLight spotLightz[NUMBER_OF_SPOT_LIGHTS];


////////////////////////7LIGHTS//////////////////////////////
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
    float attenuation = 10.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );

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
    float attenuation = 10.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );

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


/////////////////////////////////////////////////////Main
void main(void)
{

vec4 color0, color1, color2;
vec4 color = vec4(1.0, 0.0, 0.0, 1.0);

vec3 norm = normalize(normal);
vec3 viewDirection = normalize(viewPos - fragPos);
vec3 pointResult = vec3(0.0f,0.0f,0.0f);
vec3 dirResult = vec3(0.0f,0.0f,0.0f);
vec3 spotResult = vec3(0.0f,0.0f,0.0f);


if (ID == 0){
  if(in_Normal.y == 0){
    color = texture(sideTex, ex_TexCoord);
  }
  else{
  color0 = min(abs(position.x/30), 1.0) * texture(Tex, 10*ex_TexCoord);
  color1 = min(abs(1 - position.x/30), 1.0) * texture(Tex, 15*ex_TexCoord);
  color2 = abs(position.z/80) * texture(Tex, 5.0*ex_TexCoord);
  color = color0 + color1 + color2;
  }
  }
else if (ID == 1){    //Draws on top
  if(in_Normal.y > 0){
    color0 = min(abs(position.x/30), 1.0) * texture(TexUp0, 10*ex_TexCoord);
    color1 = min(abs(1 - position.x/30), 1.0) * texture(TexUp1, 15*ex_TexCoord);
    color2 = abs(position.z/80) * texture(TexUp2, 5.0*ex_TexCoord);
    color = color0 + color1 + color2;
  }
  else if(in_Normal.y < 0){
    color0 = min(abs(position.x/30), 1.0) * texture(Tex, 10*ex_TexCoord);
    color1 = min(abs(1 - position.x/30), 1.0) * texture(Tex, 15*ex_TexCoord);
    color2 = abs(position.z/80) * texture(Tex, 5.0*ex_TexCoord);
    color = color0 + color1 + color2;
  }
  else{
    color = texture(sideTex, ex_TexCoord);
  }
  }
else if (ID == 2){ //Draws on bottom
  if(in_Normal.y < 0){
    color0 = min(abs(position.x/30), 1.0) * texture(TexUp0, 10*ex_TexCoord);
    color1 = min(abs(1 - position.x/30), 1.0) * texture(TexUp1, 15*ex_TexCoord);
    color2 = abs(position.z/80) * texture(TexUp2, 5.0*ex_TexCoord);
    color = color0 + color1 + color2;
  }
  else if(in_Normal.y > 0){
    color0 = min(abs(position.x/30), 1.0) * texture(Tex, 10*ex_TexCoord);
    color1 = min(abs(1 - position.x/30), 1.0) * texture(Tex, 15*ex_TexCoord);
    color2 = abs(position.z/80) * texture(Tex, 5.0*ex_TexCoord);
    color = color0 + color1 + color2;
  }
  else{
    color = texture(sideTex, ex_TexCoord);
  }
  }
else if (ID == 3){ //Draws special lava top
  if(in_Normal.y < 0){
    color0 = min(abs(position.x/30), 1.0) * texture(Tex, 10*ex_TexCoord);
    color1 = min(abs(1 - position.x/30), 1.0) * texture(Tex, 15*ex_TexCoord);
    color2 = abs(position.z/80) * texture(Tex, 5.0*ex_TexCoord);
    color = color0 + color1 + color2;
  }
  else if(in_Normal.y > 0){
    if (position.x > -3 && position.x < 5 && position.z > 5 && position.z < 15){
      float dist = min(sqrt(pow((position.x - 1.0)/4.0, 2) + pow((position.z - 10.0)/5.0, 2)), 1.0);
      color0 =  dist * min(abs(position.x/30), 1.0) * texture(TexUp0, 10*ex_TexCoord);
      color0 = color0 + abs(position.z/80) * texture(TexUp0, 5.0*ex_TexCoord);
      color1 = dist * min(abs(1 - position.x/30), 1.0) * texture(TexUp1, 15*ex_TexCoord);
      color2 = 5 * (1 - dist) * texture(TexUp2, 5.0*ex_TexCoord);
      color = color0 + color1 + color2;
    }
    else if (position.x > -9.0 && position.x < 1.0 && position.z > -20.0 && position.z < -15.0){
      float dist = min(sqrt(pow((position.x + 4.0)/5.0, 2) + pow((position.z + 17.5)/2.5, 2)), 1.0);
      color0 =  dist * min(abs(position.x/30), 1.0) * texture(TexUp0, 10*ex_TexCoord);
      color0 = color0 + abs(position.z/80) * texture(TexUp0, 5.0*ex_TexCoord);
      color1 = dist * min(abs(1 - position.x/30), 1.0) * texture(TexUp1, 15*ex_TexCoord);
      color2 = 5 * (1 - dist) * texture(TexUp2, 5.0*ex_TexCoord);
      color = color0 + color1 + color2;
    }
    else{
      color0 = min(abs(position.x/30), 1.0) * texture(TexUp0, 10*ex_TexCoord);
      color1 = min(abs(1 - position.x/30), 1.0) * texture(TexUp1, 15*ex_TexCoord);
      color2 = abs(position.z/80) * texture(TexUp0, 5.0*ex_TexCoord);
      color = color0 + color1 + color2;
    }
  }
  else{
    color = texture(sideTex, ex_TexCoord);
  }
  }
else{
  color = vec4(1.0, 0.0, 0.0, 1.0);
  }
  ///////////////////7Lights

  for ( int i= 0; i < number_of_point_lights; i++){
     pointResult += calcPointLight(pointLightz[i], norm, fragPos, viewDirection);
  }

  for ( int i= 0; i < number_of_dir_lights; i++){
     dirResult += calcDirLight(dirLightz[i], norm, viewDirection);
  }

  for ( int i= 0; i < number_of_spot_lights; i++){
     //spotResult += calcSpotLight(spotLightz[i], norm, fragPos, viewDirection);
  }

  vec3 result = (  spotResult + dirResult + pointResult  );///Lägg ttill alla ljus sen.

out_Color = color * vec4(result,1.0f);
out_Color.w = 1.0;
}

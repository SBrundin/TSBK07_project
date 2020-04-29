#version 150

in vec3 ex_Normal;
in vec2 ex_TexCoord;
in vec3 in_Normal;
in vec3 position;

out vec4 out_Color;
uniform sampler2D Tex, TexUp0, TexUp1, TexUp2, sideTex;
uniform int ID;

void main(void)
{
float shade;
vec4 color0, color1, color2;
vec4 color = vec4(1.0, 0.0, 0.0, 1.0);

const vec3 light = vec3(0.58, 0.58, 0.58);
shade = dot(normalize(ex_Normal), light);
shade = clamp(shade,0,1);
if (ID == 0){
  if(in_Normal.y == 0){
    color = texture(sideTex, ex_TexCoord);
  }
  else{
  color0 = min(abs(position.x/30), 1.0) * texture(Tex, 10*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
  color1 = min(abs(1 - position.x/30), 1.0) * texture(Tex, 15*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
  color2 = abs(position.z/80) * texture(Tex, 5.0*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
  color = color0 + color1 + color2;
  }
  }
else if (ID == 1){    //Draws on top
  if(in_Normal.y > 0){
    color0 = min(abs(position.x/30), 1.0) * texture(TexUp0, 10*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
    color1 = min(abs(1 - position.x/30), 1.0) * texture(TexUp1, 15*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
    color2 = abs(position.z/80) * texture(TexUp2, 5.0*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
    color = color0 + color1 + color2;
  }
  else if(in_Normal.y < 0){
    color0 = min(abs(position.x/30), 1.0) * texture(Tex, 10*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
    color1 = min(abs(1 - position.x/30), 1.0) * texture(Tex, 15*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
    color2 = abs(position.z/80) * texture(Tex, 5.0*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
    color = color0 + color1 + color2;
  }
  else{
    color = texture(sideTex, ex_TexCoord);
  }
  }
else if (ID == 2){ //Draws on bottom
  if(in_Normal.y < 0){
    color0 = min(abs(position.x/30), 1.0) * texture(TexUp0, 10*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
    color1 = min(abs(1 - position.x/30), 1.0) * texture(TexUp1, 15*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
    color2 = abs(position.z/80) * texture(TexUp2, 5.0*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
    color = color0 + color1 + color2;
  }
  else if(in_Normal.y > 0){
    color0 = min(abs(position.x/30), 1.0) * texture(Tex, 10*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
    color1 = min(abs(1 - position.x/30), 1.0) * texture(Tex, 15*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
    color2 = abs(position.z/80) * texture(Tex, 5.0*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
    color = color0 + color1 + color2;
  }
  else{
    color = texture(sideTex, ex_TexCoord);
  }
  }
else if (ID == 3){ //Draws special lava top
  if(in_Normal.y < 0){
    color0 = min(abs(position.x/30), 1.0) * texture(Tex, 10*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
    color1 = min(abs(1 - position.x/30), 1.0) * texture(Tex, 15*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
    color2 = abs(position.z/80) * texture(Tex, 5.0*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
    color = color0 + color1 + color2;
  }
  else if(in_Normal.y > 0){
    if (position.x > -3 && position.x < 5 && position.z > 5 && position.z < 15){
      float dist = min(sqrt(pow((position.x - 1.0)/4.0, 2) + pow((position.z - 10.0)/5.0, 2)), 1.0);
      color0 =  dist * min(abs(position.x/30), 1.0) * texture(TexUp0, 10*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
      color0 = color0 + abs(position.z/80) * texture(TexUp0, 5.0*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
      color1 = dist * min(abs(1 - position.x/30), 1.0) * texture(TexUp1, 15*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
      color2 = 5 * (1 - dist) * texture(TexUp2, 5.0*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
      color = color0 + color1 + color2;
    }
    else if (position.x > -9.0 && position.x < 1.0 && position.z > -20.0 && position.z < -15.0){
      float dist = min(sqrt(pow((position.x + 4.0)/5.0, 2) + pow((position.z + 17.5)/2.5, 2)), 1.0);
      color0 =  dist * min(abs(position.x/30), 1.0) * texture(TexUp0, 10*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
      color0 = color0 + abs(position.z/80) * texture(TexUp0, 5.0*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
      color1 = dist * min(abs(1 - position.x/30), 1.0) * texture(TexUp1, 15*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
      color2 = 5 * (1 - dist) * texture(TexUp2, 5.0*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
      color = color0 + color1 + color2;
    }
    else{
      color0 = min(abs(position.x/30), 1.0) * texture(TexUp0, 10*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
      color1 = min(abs(1 - position.x/30), 1.0) * texture(TexUp1, 15*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
      color2 = abs(position.z/80) * texture(TexUp0, 5.0*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
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

out_Color = color;
out_Color.w = 1.0;
}

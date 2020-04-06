#version 150

in vec3 ex_Normal;
in vec2 ex_TexCoord;
in float height;
out vec4 out_Color;
uniform sampler2D snowTex, waterTex, bookTex;
uniform sampler2D texPage;
uniform float t;
float water;
void main(void)
{
const vec3 light = vec3(0.58, 0.58, 0.58);
float shade;
shade = dot(normalize(ex_Normal), light);
shade = clamp(shade,0,1);
water = 0.7 + 0.4*sin(t);
float texture1 = 1;
float snowFloat = 0;
float waterFloat = 0;

if (height > 45){
  texture1 = 0;
  snowFloat = 1;
}
else if (height > 10){
  texture1 = 10/height;
  snowFloat = 1 - texture1;
}
else if(height < water){
waterFloat = 1;
texture1 = 0;
}
out_Color = snowFloat*texture(snowTex, ex_TexCoord)* vec4(shade, shade, shade, 1.0) + texture1*texture(texPage, ex_TexCoord)* vec4(shade, shade, shade, 1.0) + waterFloat*texture(waterTex, ex_TexCoord*10)* vec4(shade, shade, shade, 1.0);
}

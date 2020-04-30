#version 150

in vec3 ex_Normal;
in vec2 ex_TexCoord;
in vec3 in_Normal;
in float width;
in vec3 position;
out vec4 out_Color;
uniform sampler2D Tex, TexUp0, TexUp1, TexUp2, sideTex;

void main(void)
{
float shade;
vec3 temp;
vec4 color, color0, color1, color2;

const vec3 light = vec3(0.58, 0.58, 0.58);
shade = dot(normalize(ex_Normal), light);
shade = clamp(shade,0,1);

if (width > 10.0){
  color = texture(Tex, 20*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
}
else{
  color = texture(TexUp1, ex_TexCoord)*vec4(shade, shade, shade, 1.0);
}
out_Color = color;
out_Color.w = 1.0;
}

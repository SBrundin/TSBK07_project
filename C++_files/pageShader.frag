#version 150

in vec3 ex_Normal;
in vec2 ex_TexCoord;
in vec3 in_Normal;
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

if ( in_Normal.y < 0.0){
  color0 = min(abs(position.x/30), 1.0) * texture(TexUp0, 10*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
  color1 = min(abs(1 - position.x/30), 1.0) * texture(TexUp1, 15*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
  color2 = abs(position.z/80) * texture(TexUp2, 1.0*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
  color = color0 + color1 + color2;
}
else if ( in_Normal.y > 0.0){
  color = texture(Tex, 20*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
}
else{
  color = texture(sideTex, ex_TexCoord)*vec4(shade, shade, shade, 1.0);
}
out_Color = color;

}

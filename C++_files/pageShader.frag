#version 150

in vec3 ex_Normal;
in vec2 ex_TexCoord;
in vec3 in_Normal;
out vec4 out_Color;
uniform sampler2D Tex, sideTex;

void main(void)
{
float shade;
vec4 color;

const vec3 light = vec3(0.58, 0.58, 0.58);
shade = dot(normalize(ex_Normal), light);
shade = clamp(shade,0,1);

if ( in_Normal.y > 0.0){
  color = texture(Tex, ex_TexCoord)*vec4(shade, shade, shade, 1.0);
}
else if ( in_Normal.y < 0.0){
  color = texture(Tex, ex_TexCoord)*vec4(shade, shade, shade, 1.0);
}
else{
  color = texture(sideTex, ex_TexCoord)*vec4(shade, shade, shade, 1.0);
}
out_Color = color;
}

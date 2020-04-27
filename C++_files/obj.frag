#version 150

in vec3 ex_Normal;
in vec2 ex_TexCoord;
in float height;

out vec4 out_Color;
uniform sampler2D Tex;
uniform float timer;
uniform float opac;


void main(void)
{
const vec3 light = vec3(0.58, 0.58, 0.58);
float shade;
shade = dot(normalize(ex_Normal), light);
shade = clamp(shade,0,1);

if(opac > height){
  out_Color = texture(Tex, ex_TexCoord)*vec4(shade, shade, shade, 0.0f);
}
else {
  out_Color = texture(Tex, ex_TexCoord)*vec4(shade, shade, shade, timer);
}

}

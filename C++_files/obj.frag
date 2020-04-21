#version 150

in vec3 ex_Normal;
in vec2 ex_TexCoord;
out vec4 out_Color;
uniform sampler2D Tex;
uniform float timer;


void main(void)
{
const vec3 light = vec3(0.58, 0.58, 0.58);
float shade;
shade = dot(normalize(ex_Normal), light);
shade = clamp(shade,0,1);

out_Color = texture(Tex, 20*ex_TexCoord)*vec4(shade, shade, shade, timer);
}

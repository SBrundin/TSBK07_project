#version 150

in vec3 ex_Normal;
in vec2 ex_TexCoord;
in float height;
out vec4 out_Color;
uniform sampler2D snowTex, waterTex, bookTex;
uniform sampler2D tex;
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

out_Color = texture(tex, 50*ex_TexCoord)*vec4(shade, shade, shade, 1.0);
}

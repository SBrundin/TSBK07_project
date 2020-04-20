#version 150


in vec3 ex_Normal;
in vec2 ex_TexCoord;
in float height;
out vec4 out_Color;
uniform sampler2D tex3;
uniform float t;
void main(void)
{
	const vec3 light = vec3(1.0, 0.0, 0.0);
	float shade = dot(ex_Normal, light);

// Texture * shade (don't shade alpha!):
	vec4 p = texture(tex3, ex_TexCoord);
	out_Color = vec4(p.r*shade, p.g*shade, p.b*shade, p.a);
}

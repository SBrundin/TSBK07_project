#version 150

in  vec3 inPosition;
in vec2 inTexCoord;

uniform mat4 projMatrix;
uniform mat4 mdlMatrix;

out vec2 ex_TexCoord;

void main(void)
{
	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
	ex_TexCoord = inTexCoord;
}

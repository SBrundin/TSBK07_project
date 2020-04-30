#version 150


in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;
out vec3 ex_Normal;
out vec3 in_Normal;
out vec3 position;
out vec2 ex_TexCoord;
out vec3 fragPos;
out vec3 normal;
uniform mat4 myRotX;
uniform mat4 myRotY;
uniform mat4 myRotZ;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 model;

void main(void)
{
	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
	ex_Normal = mat3(transpose(inverse(model)))  * inNormal;
	normal = ex_Normal;
	in_Normal = inNormal;
	ex_TexCoord = inTexCoord;
	position = inPosition;
	fragPos = vec3(model * vec4(inPosition, 1.0f));
}

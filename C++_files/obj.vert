#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec3 ex_Normal;
out vec2 ex_TexCoord;
out vec3 pos;
uniform mat4 myRotX;
uniform mat4 myRotY;
uniform mat4 myRotZ;
uniform float quickfix;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;

void main(void)
{
	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
	ex_Normal = mat3(mdlMatrix) * inNormal;
	ex_TexCoord = inTexCoord;
	vec4 temp = myRotZ * vec4(inPosition, 1.0);
	pos = vec3(temp.x  + quickfix, temp.y, temp.z);
}

#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec3 normal;
out vec3 fragPos;
out vec2 exTexCoord;
out float height;
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
  normal = mat3(transpose(inverse(model))) * inNormal;
	exTexCoord = inTexCoord;
	vec4 temp = myRotZ * vec4(inPosition, 1.0);
	height = temp.y;
	fragPos = vec3(model * vec4(inPosition, 1.0f));
}

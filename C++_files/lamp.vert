#version 150

in  vec3 inPosition;
//in  vec3 inNormal;
//in vec2 inTexCoord;

uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
//out vec3 ex_Normal;
//out vec2 ex_TexCoord;

void main()
{
    gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
}

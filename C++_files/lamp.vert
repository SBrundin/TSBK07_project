#version 150

in  vec3 inPosition;
uniform vec3 lampColour;

uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
out vec3 fragLampColour;

void main()
{
    gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
    fragLampColour = lampColour;
}

#version 150

//In variables
in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

//Uniform variables
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 model;


//To frag
//out vec3 fragColour;
out vec2 exTexCoord;
out vec3 fragPos;
out vec3 normal;

void main()
{
    gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
    fragPos = vec3(model * vec4(inPosition, 1.0f));
    normal = mat3(transpose(inverse(model))) * inNormal;
    exTexCoord = inTexCoord;
}

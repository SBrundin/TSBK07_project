#version 150

//In variables
in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

//Uniform variables
//uniform vec3 lightColour;
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 model;
//uniform vec3 viewPos;

//To frag
out vec3 fragColour;
out vec2 exTexCoord;
out vec3 fragPos;
out vec3 normal;

void main()
{
    gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
    //fragColour = lightColour;
    fragPos = vec3(model * vec4(inPosition, 1.0f));
    normal = mat3(transpose(inverse(model))) * inNormal;
    exTexCoord = inTexCoord;
}

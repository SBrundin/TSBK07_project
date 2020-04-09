#version 150
out vec4 out_color;

in vec3 fragLampColour;

void main()
{
  out_color = vec4(fragLampColour, 1.0);

}

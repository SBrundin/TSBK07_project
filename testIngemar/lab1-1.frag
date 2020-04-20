#version 150

out vec4 out_Color;
uniform vec3 colourArray;

void main(void)
{
	out_Color = vec4(colourArray, 1.0f);
}

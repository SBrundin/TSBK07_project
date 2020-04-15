#version 150

in vec2 ex_TexCoord;

out vec4 out_Color;
uniform sampler2D texUnit, test;
uniform float timer;
uniform int page;

void main(void)
{
  if (page == 0){
  out_Color = timer * texture(test, ex_TexCoord) + (1 - timer) * texture(texUnit, ex_TexCoord);
  }
  else if (page == 1){
  out_Color = (1 - timer) * texture(test, ex_TexCoord) + timer * texture(texUnit, ex_TexCoord);
  }
}

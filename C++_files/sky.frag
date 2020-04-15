#version 150

in vec2 ex_TexCoord;

out vec4 out_Color;
uniform sampler2D tex0, tex1;
//uniform float timer;
uniform int ID;

void main(void)
{
  if (ID == 1){
    //out_Color = timer * texture(test, ex_TexCoord) + (1 - timer) * texture(texUnit, ex_TexCoord);
    out_Color = texture(tex0, ex_TexCoord);
  }
  else if (ID == 2){
  //out_Color = (1 - timer) * texture(test, ex_TexCoord) + timer * texture(texUnit, ex_TexCoord);
  out_Color = texture(tex1, ex_TexCoord);
  }


}

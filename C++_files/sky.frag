#version 150

in vec2 ex_TexCoord;

out vec4 out_Color;
uniform sampler2D tex0, tex1, tex2;
uniform float timer;
uniform int ID;
uniform int dir;

void main(void)
{
  if (ID == 1){
    out_Color = timer * texture(tex1, ex_TexCoord) + (1 - timer) * texture(tex0, ex_TexCoord);
  }
  else if (ID == 2){
    if (dir == 1){
      out_Color = (1 - timer) * texture(tex1, ex_TexCoord) + timer * texture(tex2, ex_TexCoord);
    }
    else{
      out_Color = (1 - timer) * texture(tex1, ex_TexCoord) + timer * texture(tex0, ex_TexCoord);
    }
  }
  else if (ID == 3){
    if (dir == -1){
      out_Color = (1 - timer) * texture(tex2, ex_TexCoord) + timer * texture(tex1, ex_TexCoord);
    }
    else{
      out_Color = timer * texture(tex2, ex_TexCoord) + (1 - timer) * texture(tex2, ex_TexCoord);
    }
  }
}

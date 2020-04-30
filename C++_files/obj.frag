#version 150

in vec3 ex_Normal;
in vec2 ex_TexCoord;
in vec3 pos;

out vec4 out_Color;
uniform sampler2D Tex;
uniform float timer;
uniform vec4 opac;


void main(void)
{
const vec3 light = vec3(0.58, 0.58, 0.58);
float shade;
shade = dot(normalize(ex_Normal), light);
shade = clamp(shade,0,1);
float drawoverheight;
if(pos.x > 0.0){
  drawoverheight = opac.z * abs(pos.x  - 30.0)/30.0 + opac.w*pos.x/30.0;
}
else{
  drawoverheight = opac.y * abs(pos.x + 30.0)/30.0 + opac.x * abs(pos.x)/30.0;
  }

if(drawoverheight > pos.y){
  out_Color = texture(Tex, ex_TexCoord)*vec4(shade, shade, shade, 0.0f);
}
else {
  out_Color = texture(Tex, ex_TexCoord)*vec4(shade, shade, shade, timer);
}

}

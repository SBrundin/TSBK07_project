#include "Object.h"
#include <stdio.h>
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LightHandler.h"
#include "LightSource.h"

LightHandler::LightHandler()
{
  amount_of_lights = 0;
}

/*int LightHandler::addLight(vec3 lightPos,vec3 lightColour,GLfloat constant, GLfloat linear,GLfloat quadratic)
{
  lightVec.push_back(LightSource(lightPos, lightColour, constant, linear, quadratic));
  amount_of_lights = lightVec.size();
  //std::cout << amount_of_lights << std::endl;
  int index = amount_of_lights-1;
  return index;
}

/*getColourByIndex(int index, vec3 col)
{
  lightVec.at(index).setColour(col);
}
*/

void LightHandler::uploadPointLights(GLuint shader){

  glUniform1i(glGetUniformLocation(shader, "number_of_point_lights"), amount_of_lights);

//  for (std::size_t i = 0, max = lightVec.size(); i < max; i ++){
    //Creates all the strings to shader

  /*  std::string colString = "pointLightz[" + std::to_string(i) + "].colour";
    std::string ambString = "pointLightz[" + std::to_string(i) + "].ambient";
    std::string diffString = "pointLightz[" + std::to_string(i) + "].diffuse";
    std::string specString = "pointLightz[" + std::to_string(i) + "].specular";
    std::string constString = "pointLightz[" + std::to_string(i) + "].constant";
    std::string linString = "pointLightz[" + std::to_string(i) + "].linear";
    std::string quaString = "pointLightz[" + std::to_string(i) + "].quadratic";
    //Create all variables that shuld go to shader
    std::string posString = "pointLightz[" + std::to_string(i) + "].position";*/

    //Sending all the stuff to shader

  /*  vec3 *position = &lightVec.at(0).getPosition();
    //glUniform3fv(glGetUniformLocation(shader, posString.c_str()), 1, &position.x);
    vec3 *colour = &lightVec.at(0).getColour();
    vec3 *ambient = &lightVec.at(0).getAmbient();
    vec3 *diffuse = &lightVec.at(0).getDiffuse();
    vec3 *specular = &lightVec.at(0).getSpecular();
    GLfloat constant = lightVec.at(0).getConstant();
    GLfloat linear = lightVec.at(0).getLinear();
    GLfloat quadratic = lightVec.at(0).getQuadratic();
    glUniform3fv(glGetUniformLocation(shader, "pointLightz[0].position"), 1, &(*position).x);
    glUniform3fv(glGetUniformLocation(shader, "pointLightz[0].colour"), 1, &(*colour).x);
    glUniform3fv(glGetUniformLocation(shader, "pointLightz[0].ambient"), 1, &(*ambient).x);
    glUniform3fv(glGetUniformLocation(shader,  "pointLightz[0].diffuse"), 1, &(*diffuse).x);
    glUniform3fv(glGetUniformLocation(shader,  "pointLightz[0].specular"), 1, &(*specular).x);
    glUniform1f(glGetUniformLocation(shader,  "pointLightz[0].constant"), constant);
    glUniform1f(glGetUniformLocation(shader, "pointLightz[0].linear"), linear);
    glUniform1f(glGetUniformLocation(shader, "pointLightz[0].quadratic"), quadratic);*/
//  }

    //std::cout << position.y << std::endl;*/


}

void LightHandler::setPosition(int index, vec3 pos)
{
  lightVec.at(index).setPosition(pos);
}

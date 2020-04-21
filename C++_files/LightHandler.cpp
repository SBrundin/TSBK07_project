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

int LightHandler::addLight(vec3 lightPos,vec3 lightColour,GLfloat constant, GLfloat linear,GLfloat quadratic)
{
  lightVec.push_back(LightSource(lightPos, lightColour, constant, linear, quadratic));
  int index = lightVec.size();
  amount_of_lights = index;
  return index;
}

/*getColourByIndex(int index, vec3 col)
{
  lightVec.at(index).setColour(col);
}
*/

void LightHandler::uploadPointLights(GLuint shader){

  glUniform1f(glGetUniformLocation(shader, "number_of_point_lights"), amount_of_lights);

  for (std::size_t i = 0, max = lightVec.size(); i < max; i ++){
    //Creates all the strings to shader
    std::string posString = "pointLight[" + std::to_string(i) + "].position";
    std::string colString = "pointLight[" + std::to_string(i) + "].colour";
    std::string ambString = "pointLight[" + std::to_string(i) + "].ambient";
    std::string diffString = "pointLight[" + std::to_string(i) + "].diffuse";
    std::string specString = "pointLight[" + std::to_string(i) + "].specular";
    std::string constString = "pointLight[" + std::to_string(i) + "].constant";
    std::string linString = "pointLight[" + std::to_string(i) + "].linear";
    std::string quaString = "pointLight[" + std::to_string(i) + "].quadratic";
    //Create all variables that shuld go to shader
    vec3 position = lightVec.at(i).getPosition();
    vec3 colour = lightVec.at(i).getColour();
    vec3 ambient = lightVec.at(i).getAmbient();
    vec3 diffuse = lightVec.at(i).getDiffuse();
    vec3 specular = lightVec.at(i).getSpecular();
    GLfloat constant = lightVec.at(i).getConstant();
    GLfloat linear = lightVec.at(i).getLinear();
    GLfloat quadratic = lightVec.at(i).getQuadratic();
    //Sending all the stuff to shader
    glUniform3fv(glGetUniformLocation(shader, posString.c_str()), 1, &position.x);
    glUniform3fv(glGetUniformLocation(shader, colString.c_str()), 1, &colour.x);
    glUniform3fv(glGetUniformLocation(shader, ambString.c_str()), 1, &ambient.x);
    glUniform3fv(glGetUniformLocation(shader, diffString.c_str()), 1, &diffuse.x);
    glUniform3fv(glGetUniformLocation(shader, specString.c_str()), 1, &specular.x);
    glUniform1f(glGetUniformLocation(shader, constString.c_str()), constant);
    glUniform1f(glGetUniformLocation(shader, linString.c_str()), linear);
    glUniform1f(glGetUniformLocation(shader, quaString.c_str()), quadratic);

  }
}

void LightHandler::setPosition(int index, vec3 pos)
{
  lightVec.at(index).setPosition(pos);
}

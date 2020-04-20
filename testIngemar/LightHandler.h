#ifndef LIGHTHANDLER_H
#define LIGHTHANDLER_H
#include <stdio.h>
#include "VectorUtils3.h"
//#include "LightSource.h"

class LightHandler
{
public:
    LightHandler();
    //int addLight(LightSource *light); //Adds light and returns index of that light;
    int addLight();
    //void changeLightPos(int index, vec3 pos);
    vec3 getColourArray();
    //void changeLightColour(int index, vec3 col);
    //void sendLightsToShader();



private:
    vec3 *positionArray;
    vec3 *ambientArray;
    vec3 *specularArray;
    vec3 *diffuseArray;
    vec3 *colourArray;
    int *constantArray;
    int *quadraticArray;
    int *linearArray;
    int amount_of_lights;
};

#endif

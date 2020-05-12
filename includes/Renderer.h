//
// Created by Tim Fernandez-Hart on 05/05/2020.
//

#ifndef OXFORD_CAR_VISUALISER_RENDERER_H
#define OXFORD_CAR_VISUALISER_RENDERER_H
#include <GL/glew.h>


#include "Shader.h"

#define ASSERT(x) if (!(x)) __builtin_trap();

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);



class Renderer {
public:

};


#endif //OXFORD_CAR_VISUALISER_RENDERER_H

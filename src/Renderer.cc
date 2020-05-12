//
// Created by Tim Fernandez-Hart on 05/05/2020.
//

#include "Renderer.h"
#include <iostream>
/*
 *  Some OpenGL debug functions.
 */
void GLClearError(){
    while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line){
    while (GLenum error = glGetError()){
        std::cout << "[GL ERROR ]:  " << error  << " " << function
                  << " " << file << " : " << line << std::endl;
        return false;
    }
    return true;


}


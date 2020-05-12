//
// Created by Tim Fernandez-Hart on 05/05/2020.
//

#ifndef OXFORD_CAR_VISUALISER_SHADER_H
#define OXFORD_CAR_VISUALISER_SHADER_H
#include <string>
#include "glm.hpp"

struct ShaderProgrammeSource {
    std::string vertexSource;
    std::string fragmentSource;
};


/*
 *  Use this class to abstract the shader functions. It can load vertex and fragment shaders and
 *  add the appropriate '\n' crap. It loads from a single file and parses it into vertex and fragment
 *  shaders.
 */
class Shader {
private:
    std::string filepath;
    unsigned int rendererId;
    int GetUniformLocation(const std::string& name);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    ShaderProgrammeSource ParseShader(const std::string& filepath);
public:
    Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind();
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string &name, const glm::mat4 &matrix);
};


#endif //OXFORD_CAR_VISUALISER_SHADER_H

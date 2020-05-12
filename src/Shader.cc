//
// Created by Tim Fernandez-Hart on 05/05/2020.
//
#include <iostream>
#include "Shader.h"
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

int Shader::GetUniformLocation(const std::string &name) {
    GLCall( int location = glGetUniformLocation(rendererId, name.c_str()));
    if(location == -1){
        std::cout << "Warning: Uniform " << name << " doesn't exist." << std::endl;
    }
    return location;
}

Shader::Shader(const std::string &filepath) : filepath(filepath), rendererId(0)
{
   ShaderProgrammeSource source = ParseShader(filepath);
   rendererId = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(rendererId));
}

void Shader::Bind() const {
    GLCall(glUseProgram(rendererId));
}

void Shader::Unbind() {
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3) {
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, NULL));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if(result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &result));
        char * message= (char*) alloca(length*sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader ";
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }
    return id;

}

unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string& fragmentShader) {
    GLCall(unsigned int programme = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(programme, vs));
    GLCall(glAttachShader(programme, fs));
    GLCall(glLinkProgram(programme));
    GLCall(glValidateProgram(programme));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return programme;

}


ShaderProgrammeSource Shader::ParseShader(const std::string& filepath) {

    if (filepath == "") {std::cerr << "File name not provided.";}
    std::ifstream stream(filepath);
    if(!stream) {
        std::cout << "Problem opening file." << std::endl;
    }

    enum class ShaderType {
        NONE = -1, VERTEX=0, FRAGMENT=1
    };
    ShaderType type = ShaderType::NONE;
    std::stringstream ss[2];
    std::string line;
    while(getline(stream, line)){
        if(line.find("#shader") != std::string::npos) {
            if(line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if(line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)type] << line << "\n";
        }

    }
    return {ss[0].str(), ss[1].str()};
}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}





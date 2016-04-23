#pragma once
#include <string>
#include <vector>
#include "GL/glew.h"
#include <string>
#include <vector>
#include "GLShader.h"
#include "MultibrotPanel.h"

class GLShaderProgram
{
public:
    GLShaderProgram(MultibrotPanel& canvas, std::vector<GLuint>& shaders);
    GLShaderProgram(const GLShaderProgram&) = delete;
    GLShaderProgram(GLShaderProgram&&) = delete;
    virtual ~GLShaderProgram();
    GLShaderProgram& operator=(const GLShaderProgram) = delete;
    GLShaderProgram& operator=(GLShaderProgram&&) = delete;
    void BindFragmentDataLocations(const std::vector<std::string>& varName);

private:
    void Link();

    MultibrotPanel* m_canvas;
    GLuint m_program;
};


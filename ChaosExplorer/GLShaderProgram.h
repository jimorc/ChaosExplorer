#pragma once
#include <string>
#include <vector>
#include "GL/glew.h"
#include <string>
#include <vector>
#include "GLShader.h"
#include "ChaosPanel.h"

class GLShaderProgram
{
public:
    GLShaderProgram(ChaosPanel& canvas, std::vector<GLuint>& shaders);
    GLShaderProgram(const GLShaderProgram&) = delete;
    GLShaderProgram(GLShaderProgram&&) = delete;
    virtual ~GLShaderProgram();
    GLShaderProgram& operator=(const GLShaderProgram) = delete;
    GLShaderProgram& operator=(GLShaderProgram&&) = delete;
    void BindFragmentDataLocations(const std::vector<std::string>& varName);

private:
    void Link();

    ChaosPanel* m_canvas;
    GLuint m_program;
};


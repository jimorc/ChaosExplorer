#pragma once
#include <string>
#include <memory>
#include "GL/glew.h"

class ChaosPanel;
//#include "ChaosPanel.h"

class GLShader
{
public:
    GLShader(ChaosPanel& canvas, const GLenum shaderType,
        const std::string& shaderSource,
        const std::string& compileErrorString);
    GLShader(const GLShader&) = delete;
    GLShader(GLShader&&) = delete;
    virtual ~GLShader();

    GLShader& operator=(const GLShader&) = delete;
    GLShader& operator=(GLShader&&) = delete;
    GLuint GetShaderHandle() { return m_shader; }

private:
    void CheckShaderCompileStatus(GLuint shader, const std::string& errorMsg);

    ChaosPanel* m_canvas;
    GLuint m_shader;
};


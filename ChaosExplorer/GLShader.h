#pragma once
#include <string>
#include <memory>
#include "GL/glew.h"
#include "MultibrotPanel.h"
class GLShader
{
public:
    GLShader(MultibrotPanel& canvas, const GLenum shaderType,
        const std::string& shaderSource,
        const std::string& compileErrorString);
    GLShader(const GLShader&) = delete;
    GLShader(GLShader&&) = delete;
    virtual ~GLShader() noexcept;

    GLShader& operator=(const GLShader&) = delete;
    GLShader& operator=(GLShader&&) = delete;

private:
    void CheckShaderCompileStatus(GLuint shader, const std::string& errorMsg);

    MultibrotPanel* m_canvas;
    GLuint m_shader;
};


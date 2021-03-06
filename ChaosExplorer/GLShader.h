#pragma once
#include <string>
#include <memory>

class GLShader
{
public:
    GLShader(const GLenum shaderType,
        const std::string& shaderSource,
        const std::string& compileErrorString);
    GLShader(const GLShader&) = delete;
    GLShader(GLShader&&);
    virtual ~GLShader();

    GLShader& operator=(const GLShader&) = delete;
    GLShader& operator=(GLShader&&);
    GLuint GetShaderHandle() { return m_shader; }

private:
    void CheckShaderCompileStatus(GLuint shader, const std::string& errorMsg);

//    ChaosPanel* m_canvas;
    GLuint m_shader;
};


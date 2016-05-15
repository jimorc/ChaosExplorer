#pragma once
#include <string>
#include <vector>
#include <memory>
#include "GL/glew.h"
#include <string>
#include <vector>
#include "GLShader.h"
#include "ChaosPanel.h"

class GLShaderProgram
{
public:
    GLShaderProgram(ChaosPanel& canvas);
    GLShaderProgram(const GLShaderProgram&) = delete;
    GLShaderProgram(GLShaderProgram&&) = delete;
    virtual ~GLShaderProgram();
    GLShaderProgram& operator=(const GLShaderProgram) = delete;
    GLShaderProgram& operator=(GLShaderProgram&&) = delete;
    void Link();
    void Use() { glUseProgram(m_program); }
    GLint GetUniformLocation(std::string& uniform) {
        glGetUniformLocation(GetProgramHandle(), uniform.c_str());
    }
    GLuint GetProgramHandle() { return m_program; }
    void SetProgramHandle(GLuint handle) { m_program = handle; }
    ChaosPanel* GetCanvas() { return m_canvas; }
    GLShader* GetVertexShader() { return m_vertexShader.get(); }

protected:
    virtual void BuildVertexShader();

private:
    std::unique_ptr<GLShader> m_vertexShader;

    ChaosPanel* m_canvas;
    GLuint m_program;

};


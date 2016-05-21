#pragma once
#include <string>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "GLShader.h"

class ChaosPanel;       // forward declaration

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
    GLint GetUniformHandle(const std::string& name) const;

protected:
    virtual void BuildVertexShader();
    void LoadUniformHandles();

private:
    std::unique_ptr<GLShader> m_vertexShader;

    std::map<const std::string, GLint> m_uniforms;

    ChaosPanel* m_canvas;
    GLuint m_program;

};


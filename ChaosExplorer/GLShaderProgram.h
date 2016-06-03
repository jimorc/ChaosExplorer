#pragma once
#include <string>
#include <map>
#include <memory>
#include <wx/wxprec.h>
#if !defined(__gl_h_)
    #include <GL/glew.h>
#endif
#include "GLShader.h"

class GLShaderProgram
{
public:
    GLShaderProgram();
    GLShaderProgram(const GLShaderProgram&) = delete;
    GLShaderProgram(GLShaderProgram&&);
    virtual ~GLShaderProgram();
    GLShaderProgram& operator=(const GLShaderProgram) = delete;
    GLShaderProgram& operator=(GLShaderProgram&&);
    void Link();
    void Use() { glUseProgram(m_program); }
    GLint GetUniformLocation(std::string& uniform) {
        glGetUniformLocation(GetProgramHandle(), uniform.c_str());
    }
    GLuint GetProgramHandle() { return m_program; }
    void SetProgramHandle(GLuint handle) { m_program = handle; }
    GLShader* GetVertexShader() { return m_vertexShader.get(); }
    GLint GetUniformHandle(const std::string& name) const;

protected:
    virtual void BuildVertexShader();
    void LoadUniformHandles();

private:
    std::unique_ptr<GLShader> m_vertexShader;

    std::map<const std::string, GLint> m_uniforms;

    GLuint m_program;

};


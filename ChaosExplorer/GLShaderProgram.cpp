#define _CRT_SECURE_NO_WARNINGS
#include "GLShaderProgram.h"

GLShaderProgram::GLShaderProgram(ChaosPanel& canvas) 
{
}

GLShaderProgram::GLShaderProgram(GLShaderProgram&& prog)
{
    m_vertexShader = std::move(prog.m_vertexShader);
    prog.m_vertexShader.release();
    m_uniforms = prog.m_uniforms;
    prog.m_uniforms.clear();
    m_program = prog.m_program;
    prog.m_program = 0;
}


GLShaderProgram::~GLShaderProgram()
{
    glDeleteProgram(m_program);
}

GLShaderProgram& GLShaderProgram::operator=(GLShaderProgram&& prog)
{
    return std::move(prog);
}


void GLShaderProgram::Link()
{
    glLinkProgram(m_program);
    // check shader compile status, and throw exception if compile failed
    GLint status = GL_TRUE;
    glGetShaderiv(m_program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        // Program Info Log is empty
        // so don't try to read it
        throw std::logic_error("Linking of shader program failed.");
    }

}

// Vertex shader is common among the shader programs.
// It can, however, be overridden if necessary.
void GLShaderProgram::BuildVertexShader(ChaosPanel& canvas)
{
    std::string vertexSource =
        "#version 330 core\n"
        "in vec4 position;"
        "void main()"
        "{"
        "    gl_Position = position;"
        "}";
    m_vertexShader = std::make_unique<GLShader>(canvas, GL_VERTEX_SHADER, vertexSource,
        "Vertex shader did not compile.");
}

// the code in this method is based on an answer to a question on StackOverflow. See
// http://stackoverflow.com/questions/440144/in-opengl-is-there-a-way-to-get-a-list-of-all-uniforms-attribs-used-by-a-shade
// The original answer by Nicol Bolas was edited by Axalo.
void GLShaderProgram::LoadUniformHandles()
{
    // get numver of uniforms
    GLint numActiveUniforms = 0;
    glGetProgramInterfaceiv(m_program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

    std::vector<GLchar> nameData(256);
    std::vector<GLenum> properties;
    properties.push_back(GL_NAME_LENGTH);
    properties.push_back(GL_TYPE);
    properties.push_back(GL_ARRAY_SIZE);
    std::vector<GLint> values(properties.size());
    for (int uniform = 0; uniform < numActiveUniforms; ++uniform) {
        // get the properties for each uniform
        glGetProgramResourceiv(m_program, GL_UNIFORM, uniform, static_cast<GLsizei>(properties.size()),
            &properties[0], static_cast<GLsizei>(values.size()), NULL, &values[0]);
        // get the name
        nameData.resize(properties[0]);  // length of the name
        glGetProgramResourceName(m_program, GL_UNIFORM, uniform, static_cast<GLsizei>(nameData.size()),
            NULL, &nameData[0]);
        std::string name((char*)&nameData[0], nameData.size() - 1);
        // now get the location and add to m_uniforms (a map)
        // must use name.c_str() as index or m_uniforms.at() and m_uniforms.find() do not find the key
        m_uniforms[name.c_str()] = glGetUniformLocation(m_program, name.c_str());
    }
}

// retrieve the uniform handle for the uniform named 'name'
GLint GLShaderProgram::GetUniformHandle(const std::string& name) const
{
    auto iter = m_uniforms.find(name);
    if (iter != m_uniforms.end()) {
        return iter->second;
    }
    else {
        return -1;
    }
}
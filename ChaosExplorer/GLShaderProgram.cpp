#define _CRT_SECURE_NO_WARNINGS
#include "GLShaderProgram.h"

GLShaderProgram::GLShaderProgram(ChaosPanel& canvas) 
    : m_canvas(&canvas)
{
}


GLShaderProgram::~GLShaderProgram()
{
    glDeleteProgram(m_program);
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
void GLShaderProgram::BuildVertexShader()
{
    std::string vertexSource =
        "#version 330 core\n"
        "in vec4 position;"
        "void main()"
        "{"
        "    gl_Position = position;"
        "}";
    m_vertexShader = std::make_unique<GLShader>(*GetCanvas(), GL_VERTEX_SHADER, vertexSource,
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
        glGetProgramResourceiv(m_program, GL_UNIFORM, uniform, properties.size(),
            &properties[0], values.size(), NULL, &values[0]);
        // get the name
        nameData.resize(properties[0]);  // length of the name
        glGetProgramResourceName(m_program, GL_UNIFORM, uniform, nameData.size(), NULL, &nameData[0]);
        std::string name((char*)&nameData[0], nameData.size() - 1);
        // now get the location and add to m_uniforms (a map)
        glGetUniformLocation(m_program, name.c_str());
        m_uniforms[name] = glGetUniformLocation(m_program, name.c_str());
    }
}

// retrieve the uniform handle for the uniform named 'name'
auto GLShaderProgram::GetUniformHandle(const std::string& name) const
{
    auto iter = m_uniforms.find(name);
    if (iter != m_uniforms.end()) {
        return iter->second;
    }
    else {
        return -1;
    }
}
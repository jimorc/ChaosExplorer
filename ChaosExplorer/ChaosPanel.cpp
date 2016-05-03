#define _CRT_SECURE_NO_WARNINGS
#include "GL/glew.h"
#include "GLShaderProgram.h"
#include "ChaosPanel.h"

std::vector<bool> ChaosPanel::m_timers(MaxTimers, false);

ChaosPanel::ChaosPanel(wxWindow* parent, wxWindowID id, const int* attribList,
    const wxSize& size)
    : wxGLCanvas(parent, id, attribList, wxDefaultPosition, size)
{
    m_context = std::make_unique<wxGLContext>(this);
    SetCurrent(*m_context);
    InitializeGLEW();
}


ChaosPanel::~ChaosPanel()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void ChaosPanel::InitializeGLEW()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        const GLubyte* msg = glewGetErrorString(err);
        throw std::runtime_error(reinterpret_cast<const char*>(msg));
    }
}

void ChaosPanel::SetupTriangles(std::vector<glm::vec4>& vert, GLint prog)
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert) * sizeof(vert[0]), &vert[0], GL_STATIC_DRAW);
    GLint posAttrib = glGetAttribLocation(prog, "position");
    glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(posAttrib);
}


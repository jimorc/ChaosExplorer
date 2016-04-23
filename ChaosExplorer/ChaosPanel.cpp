#define _CRT_SECURE_NO_WARNINGS
#include "GL/glew.h"
#include "GLShaderProgram.h"
#include "ChaosPanel.h"

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


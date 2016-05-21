#define _CRT_SECURE_NO_WARNINGS
#include "GL/glew.h"
#include "GLShaderProgram.h"
#include "ChaosPanel.h"

// The vertices that define the two triangles.
// These vertices take up entire view 
std::vector<glm::vec4> ChaosPanel::s_vertices = {
    { 1.0f, 1.0f, 0.0f, 1.0f },
    { -1.0f, 1.0f, 0.0f, 1.0f },
    { 1.0f, -1.0f, 0.0f, 1.0f },
    { -1.0f, -1.0f, 0.0f, 1.0f }
};

std::vector<bool> ChaosPanel::m_timers(MaxTimers, false);

ChaosPanel::ChaosPanel(wxWindow* parent, wxWindowID id, const int* attribList,
    const std::complex<float>ul, const std::complex<float>lr,
    const wxSize& size)
    : wxGLCanvas(parent, id, attribList, wxDefaultPosition, size), m_popup(nullptr),
    m_rightDown({ 0, 0 }), m_leftButtonDown(false), m_leftDown({ 0, 0 }), m_leftUp({ 0, 0 })
{
    SetUpperLeftLowerRight(ul, lr);
    m_context = std::make_unique<wxGLContext>(this);
    SetCurrent(*m_context);
    InitializeGLEW();
    Bind(wxEVT_RIGHT_DOWN, &ChaosPanel::OnRightButtonDown, this);
    Bind(wxEVT_LEFT_DOWN, &ChaosPanel::OnLeftButtonDown, this);
    Bind(wxEVT_MOTION, &ChaosPanel::OnMouseMove, this);
    Bind(wxEVT_LEFT_UP, &ChaosPanel::OnLeftButtonUp, this);
}


ChaosPanel::~ChaosPanel()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);

    // delete popup menu
    if (m_popup != nullptr) {
        delete m_popup;
    }
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

void ChaosPanel::SetupTriangles()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices) * sizeof(s_vertices[0]), &s_vertices[0], 
        GL_STATIC_DRAW);
    GLint posAttrib = glGetAttribLocation(m_program->GetProgramHandle(), "position");
    glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(posAttrib);
}

void ChaosPanel::OnLeftButtonDown(wxMouseEvent& event)
{
    // set left button down position
    m_leftButtonDown = true;
    m_leftDown = event.GetPosition();
    m_leftUp = m_leftDown;
}

void ChaosPanel::OnMouseMove(wxMouseEvent& event)
{
    // as mouse moves when left button is down, set m_leftDown to be upper left
    // and m_leftUp to be lower right positions of selection area.
    if (m_leftButtonDown) {
        m_leftUp = event.GetPosition();
        if (m_leftDown.x > m_leftUp.x) {
            int temp = m_leftDown.x;
            m_leftDown.x = m_leftUp.x;
            m_leftUp.x = temp;
        }
        if (m_leftDown.y > m_leftUp.y) {
            m_leftDown.y = m_leftUp.y;
        }
        m_leftUp.y = m_leftDown.y + (m_leftUp.x - m_leftDown.x);
        SetLeftUp(m_leftUp);
        // redraw the selection square
        Refresh();
    }
}

void ChaosPanel::OnLeftButtonUp(wxMouseEvent& event)
{
    // set final positions of the selection square
    m_leftUp = event.GetPosition();
    m_leftButtonDown = false;
    if (m_leftDown.x > m_leftUp.x) {
        int temp = m_leftDown.x;
        m_leftDown.x = m_leftUp.x;
        m_leftUp.x = temp;
    }
    if (m_leftDown.y > m_leftUp.y) {
        m_leftDown.y = m_leftUp.y;
    }
    m_leftUp.y = m_leftDown.y + (m_leftUp.x - m_leftDown.x);
    // and redraw
    Refresh();
}


#define _CRT_SECURE_NO_WARNINGS
#include "GL/glew.h"
#include "GLSquareShaderProgram.h"
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
    m_squareProgram = std::make_unique<GLSquareShaderProgram>(*this);

    Bind(wxEVT_RIGHT_DOWN, &ChaosPanel::OnRightButtonDown, this);
    Bind(wxEVT_LEFT_DOWN, &ChaosPanel::OnLeftButtonDown, this);
    Bind(wxEVT_MOTION, &ChaosPanel::OnMouseMove, this);
    Bind(wxEVT_LEFT_UP, &ChaosPanel::OnLeftButtonUp, this);
}


ChaosPanel::~ChaosPanel()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_squareVbo);
    glDeleteVertexArrays(1, &m_squareVao);

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

// call this from your child panel's constructor
// If you call from ChaosPanel constructor, exception is thrown in OnPaint.
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

// call this from your child panel's constructor
// If you call from ChaosPanel constructor, exception is thrown in OnPaint.
void ChaosPanel::SetupSquareArrays()
{
    // set GL stuff for the square that will contain the Multibrot image
    glGenVertexArrays(1, &m_squareVao);
    glBindVertexArray(m_squareVao);
    glGenBuffers(1, &m_squareVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_squareVbo);
    GLint posAttrib = glGetAttribLocation(GetSquareShaderProgram()->GetProgramHandle(), "position");
    glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(posAttrib);

}

void ChaosPanel::DrawSquare()
{
    wxSize size = GetSize();
    // draw the square outlining the selected area of the image
    if (m_leftDown.x != m_leftUp.x || m_leftDown.y != m_leftUp.y) {
        glUseProgram(m_squareProgram->GetProgramHandle());
        glBindVertexArray(GetSquareVao());
        float halfSize = static_cast<float>(size.x) / 2.0f;
        float downX = m_leftDown.x - halfSize;
        float downY = halfSize - m_leftDown.y;
        float upX = m_leftUp.x - halfSize;
        float upY = halfSize - m_leftUp.y;
        std::vector<glm::vec4> points;
        points.push_back({ downX, downY, 0.0f, halfSize });
        points.push_back({ downX, upY, 0.0f, halfSize });
        points.push_back({ upX, upY, 0.0f, halfSize });
        points.push_back({ upX, downY, 0.0f, halfSize });
        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(points[0]), &points[0], GL_DYNAMIC_DRAW);
        glDrawArrays(GL_LINE_LOOP, 0, points.size());
    }
}

void ChaosPanel::CalculateUpperLeftAndLowerRight(std::complex<float>& ul, std::complex<float>& lr) const
{
    wxSize size = GetSize();
    float deltaX = m_lowerRight.real() - m_upperLeft.real();
    float deltaY = m_upperLeft.imag() - m_lowerRight.imag();
    float ulReal = m_upperLeft.real() + deltaX * m_leftDown.x / size.x;
    float ulImag = m_upperLeft.imag() - deltaY * m_leftDown.y / size.y;
    float lrReal = m_upperLeft.real() + deltaX * m_leftUp.x / size.x;
    float lrImag = m_upperLeft.imag() - deltaY * m_leftUp.y / size.y;

    ul = { ulReal, ulImag };
    lr = { lrReal, lrImag };
}

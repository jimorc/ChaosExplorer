#pragma once
#include "wx/wxprec.h"
#include <memory>
#include <vector>
#include <complex>
#include <type_traits>
#include <glm/gtc/type_ptr.hpp>
#include "wx/glcanvas.h"
#include "GLShaderProgram.h"
#include "GLSquareShaderProgram.h"

class GLShaderProgram;

const int ID_DRAWFROMSELECTION = 2002;
const int ID_DELETESELECTION = 2003;
const int ID_ANIMATEITERATIONS = 2004;
const int ID_ANIMATEMAGNIFICATION = 2005;
const int ID_POWER3 = 2006;
const int ID_POWER4 = 2007;
const int ID_POWER5 = 2008;
const int ID_POWER6 = 2009;
const int ID_POWER7 = 2010;
const int ID_POWER8 = 2011;
const int ID_POWER9 = 2012;
const int ID_POWER10 = 2013;
const int ID_ANIMATEREALPOWERS = 2014;
const int ID_ANIMATEIMAGINARYPOWERS = 2015;
const int ID_POWER2 = 2016;
const int ID_ANIMATEZ0REAL = 2017;
const int ID_ANIMATEZ0IMAG = 2018;
const int ID_PRECLOSETAB = 2019;
const int ID_JULIA = 2020;

// The vertices that define the two triangles.
// These vertices take up entire view 
static std::vector<glm::vec4> s_vertices = {
    { 1.0f, 1.0f, 0.0f, 1.0f },
    { -1.0f, 1.0f, 0.0f, 1.0f },
    { 1.0f, -1.0f, 0.0f, 1.0f },
    { -1.0f, -1.0f, 0.0f, 1.0f }
};

// Timers
static const int MaxTimers = 10;    // MSW has limited # timers, so we only allow 10 timers at once
static std::vector<bool> timers(MaxTimers);


template <typename T>
class ChaosPanel :
    public wxGLCanvas
{
public:
    ChaosPanel<T>(wxWindow* parent, wxWindowID id, const int* attribList,
        const std::complex<float>ul,
        const std::complex<float>lr, const wxSize& size = { 800, 600 })
        : wxGLCanvas(parent, id, attribList, wxDefaultPosition, size), m_popup(nullptr),
        m_rightDown({ 0, 0 }), m_leftButtonDown(false), m_leftDown({ 0, 0 }), m_leftUp({ 0, 0 })
    {
//        static_assert(meta_detail::has_shader_handler(T(), 0), "Template type T must be derived from GLShaderProgram");
//        static_assert(meta_detail::isBaseOf( T()), "Template type T must be derived from GLShaderProgram");
        static_assert(std::is_base_of<GLShaderProgram, T>::value, "T must be derived from GLShaderProgram");
        SetUpperLeftLowerRight(ul, lr);
        m_context = std::make_unique<wxGLContext>(this);
        SetCurrent(*m_context);
        InitializeGLEW();
        m_squareProgram = std::make_unique<GLSquareShaderProgram>();

        Bind(wxEVT_RIGHT_DOWN, &ChaosPanel::OnRightButtonDown, this);
        Bind(wxEVT_LEFT_DOWN, &ChaosPanel::OnLeftButtonDown, this);
        Bind(wxEVT_MOTION, &ChaosPanel::OnMouseMove, this);
        Bind(wxEVT_LEFT_UP, &ChaosPanel::OnLeftButtonUp, this);
    }

    ChaosPanel(const ChaosPanel&) = delete;
    ChaosPanel(ChaosPanel&&) = delete;
    virtual ~ChaosPanel()
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

    ChaosPanel& operator=(const ChaosPanel&) = delete;
    ChaosPanel& operator=(ChaosPanel&&) = delete;
    void InitializeGLEW()
    {
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            const GLubyte* msg = glewGetErrorString(err);
            throw std::runtime_error(reinterpret_cast<const char*>(msg));
        }
    }

    void SetContext() const noexcept { SetCurrent(*m_context); }


protected:
    virtual void DrawFractal() = 0;
    // call this from your child panel's constructor
    // If you call from ChaosPanel constructor, exception is thrown in OnPaint.
    void SetupTriangles()
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

    GLuint GetVao() { return m_vao; }
    int GetTimer() {
        for (int timer = 0; timer < MaxTimers; ++timer) {
            if (!timers[timer]) {
                return timer;
            }
        }
        return NOTIMERS;
    }
    void ReleaseTimer(int timer) {
        timers[timer] = false;
    }
    static const int NOTIMERS = -1;

    wxMenu* GetPopupMenu() { return m_popup; }
    void SetPopupMenu(wxMenu* popup) { m_popup = popup; }

    std::complex<float> GetUpperLeft() { return m_upperLeft; }
    std::complex<float> GetLowerRight() { return m_lowerRight; }
    void SetUpperLeftLowerRight(std::complex<float> ul, std::complex<float> lr) {
        m_upperLeft = std::complex<float>(std::min(ul.real(), lr.real()),
            std::max(ul.imag(), lr.imag()));
        m_lowerRight = std::complex<float>(std::max(ul.real(), lr.real()),
            std::min(ul.imag(), lr.imag()));
    }
    wxPoint& GetRightDown() { return m_rightDown; }
    void SetRightDown(wxPoint& rDown) { m_rightDown = rDown; }

    virtual void OnRightButtonDown(wxMouseEvent& event) {
        if (m_popup != nullptr) {
            m_rightDown = event.GetPosition();
            PopupMenu(m_popup);
        }
    }
    wxPoint& GetLeftDown() { return m_leftDown; }
    void SetLeftDown(const wxPoint& leftDown) { m_leftDown = leftDown; }
    wxPoint& GetLeftUp() { return m_leftUp; }
    void SetLeftUp(const wxPoint& leftUp) { m_leftUp = leftUp; }
    bool GetLeftButtonDown() { return m_leftButtonDown; }
    void SetLeftButtonDown(bool down) { m_leftButtonDown = down; }
    std::complex<float> GetRightDownPoint() { return m_rightDownPoint; }
    void SetRightDownPoint(std::complex<float>& point) { m_rightDownPoint = point; }

    GLShaderProgram* GetSquareShaderProgram() { return m_squareProgram.get(); }
    T* GetShaderProgram() { return m_program.get(); }
    void CreateShaderProgram() { m_program.reset(new T()); }
    GLuint GetSquareVao() { return m_squareVao; }
    // call this from your child panel's constructor
    // If you call from ChaosPanel constructor, exception is thrown in OnPaint.
    void SetupSquareArrays()
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

    void DrawSquare()
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

    void CalculateUpperLeftAndLowerRight(std::complex<float>& ul, std::complex<float>& lr) const
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

    void OnCloseTab()
    {
        ChaosExplorerWindow* mainWin = dynamic_cast<ChaosExplorerWindow*>(GetParent()->GetParent());
        mainWin->OnCloseTab(wxCommandEvent());
    }


private:
    virtual void SetStatusBarText() = 0;
    virtual void OnPaint(wxPaintEvent& event) = 0;
    virtual void OnLeftButtonDown(wxMouseEvent& event)
    {
        // set left button down position
        m_leftButtonDown = true;
        m_leftDown = event.GetPosition();
        m_leftUp = m_leftDown;
    }

    virtual void OnMouseMove(wxMouseEvent& event)
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

    virtual void OnLeftButtonUp(wxMouseEvent& event)
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

    wxMenu* m_popup;

    std::complex<float> m_upperLeft;
    std::complex<float> m_lowerRight;
    std::complex<float> m_rightDownPoint;
    wxPoint m_rightDown;
    bool m_leftButtonDown;
    wxPoint m_leftDown;
    wxPoint m_leftUp;

    std::unique_ptr<T> m_program;
    std::unique_ptr<wxGLContext> m_context;
    GLuint m_vbo;
    GLuint m_vao;
    std::unique_ptr<GLSquareShaderProgram> m_squareProgram;
    GLuint m_squareVbo;
    GLuint m_squareVao;

};


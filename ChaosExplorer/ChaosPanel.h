#pragma once
#include "wx/wxprec.h"
#include <memory>
#include <complex>
#include <glm/gtc/type_ptr.hpp>
#include "wx/glcanvas.h"

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

class GLShaderProgram;

class ChaosPanel :
    public wxGLCanvas
{
public:
    ChaosPanel(wxWindow* parent, wxWindowID id, const int* attribList,
        const std::complex<float>ul,
        const std::complex<float>lr, const wxSize& size = { 800, 600 });
    ChaosPanel(const ChaosPanel&) = delete;
    ChaosPanel(ChaosPanel&&) = delete;
    virtual ~ChaosPanel();
    ChaosPanel& operator=(const ChaosPanel&) = delete;
    ChaosPanel& operator=(ChaosPanel&&) = delete;
    void InitializeGLEW();
    void SetContext() const noexcept { SetCurrent(*m_context); }


protected:
    virtual void BuildShaderProgram() = 0;
    std::unique_ptr<GLShaderProgram> m_program;
    void SetupTriangles();
    GLuint GetVao() { return m_vao; }
    int GetTimer() {
        for (int timer = 0; timer < MaxTimers; ++timer) {
            if (!m_timers[timer]) {
                return timer;
            }
        }
        return NOTIMERS;
    }
    void ReleaseTimer(int timer) {
        m_timers[timer] = false;
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

private:
    virtual void OnPaint(wxPaintEvent& event) = 0;
    wxMenu* m_popup;

    std::complex<float> m_upperLeft;
    std::complex<float> m_lowerRight;

    wxPoint m_rightDown;

    static std::vector<glm::vec4> s_vertices;
    std::unique_ptr<wxGLContext> m_context;
    GLuint m_vbo;
    GLuint m_vao;
    static std::vector<bool> m_timers;
    static const int MaxTimers = 10;    // MSW has limited # timers, so we only allow 10 timers at once
};


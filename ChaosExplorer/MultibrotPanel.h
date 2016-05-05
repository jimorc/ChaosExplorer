#pragma once
#include <complex>
#include <memory>
#include <chrono>
#include "wx/panel.h"
#include "ChaosPanel.h"

using namespace std::complex_literals;

const int ID_DRAWFROMSELECTION = 2002;
const int ID_DELETESELECTION = 2003;
const int ID_ANIMATEITERATIONS = 2004;
const int ID_ANIMATEMAGNIFICATION = 2005;
const int ID_POWER3 = 2006;

class MultibrotPanel :
    public ChaosPanel
{
public:
    MultibrotPanel(wxWindow* parent, wxWindowID id, const int* attribList,
        const wxSize& size = { 800, 600 },
        std::complex<float> power = 2.0f,
        std::complex<float> ul = -2.5f + 2.0if,
        std::complex<float> lr =  1.5f - 2.0if);
    virtual ~MultibrotPanel();

protected:
    virtual void BuildShaderProgram() override;

private:
    virtual void OnPaint(wxPaintEvent& event) override;
    virtual void OnLeftButtonDown(wxMouseEvent& event);
    virtual void OnMouseMove(wxMouseEvent& event);
    virtual void OnLeftButtonUp(wxMouseEvent& event);
    virtual void OnRightButtonDown(wxMouseEvent& event);
    virtual void OnDrawFromSelection(wxCommandEvent& event);
    virtual void OnDeleteSelection(wxCommandEvent& event);
    virtual void OnMenuOpen(wxMenuEvent& event);
    virtual void OnAnimateIterations(wxCommandEvent& event);
    virtual void AnimateIterations(wxTimerEvent& event);
    virtual void OnAnimateMagnification(wxCommandEvent& event);
    virtual void AnimateMagnification(wxTimerEvent& event);
    virtual void OnMultibrotPower3(wxCommandEvent& event);
    void SetStatusBarText();
    void SetupSquareArrays();
    void CreateMainMenu();

    std::unique_ptr<GLShaderProgram> m_squareProgram;
    GLuint m_squareVbo;
    GLuint m_squareVao;

    std::complex<float> m_power;
    std::complex<float> m_upperLeft;
    std::complex<float> m_lowerRight;
    std::complex<float> m_rightDownPoint;

    bool m_leftButtonDown;
    wxPoint m_leftDown;
    wxPoint m_leftUp;
    wxPoint m_rightDown;

    wxMenu* m_popup;

    static const int m_iterationInterval = 1000 / 6;
    static const int m_magnificationInterval = 1000 / 60;
    int m_timerNumber;
    std::unique_ptr<wxTimer> m_timer;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
    int m_maxIterations;
    int m_zoomCount;
};


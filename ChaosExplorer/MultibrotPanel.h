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

class MultibrotPanel;

using TimerHandler =  void(MultibrotPanel::*) (wxTimerEvent&);

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
    void OnAnimateRealPowers(wxCommandEvent& event);
    void AnimateRealPowers(wxTimerEvent& event);
    void OnAnimateImaginaryPowers(wxCommandEvent& event);
    void AnimateImaginaryPowers(wxTimerEvent& event);
    void OnAnimateZ0Real(wxCommandEvent& event);
    void AnimateZ0Real(wxTimerEvent& event);
    void OnAnimateZ0Imag(wxCommandEvent& event);
    void AnimateZ0Imag(wxTimerEvent& event);
    void SetStatusBarText();
    void SetupSquareArrays();
    void CreateMainMenu();
    wxMenu* CreateMultibrotSubMenu();
    void AddItemToMenu(wxMenu* menu, const int menuId, std::wstring menuText, float power);
    void StartTimer(const int timerInterval, TimerHandler handler);

    std::unique_ptr<GLShaderProgram> m_squareProgram;
    GLuint m_squareVbo;
    GLuint m_squareVao;

    std::complex<float> m_z0;
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
    static const int m_realPowersInterval = 1000 / 20;
    static const int m_z0Interval = 1000 / 20;
    int m_timerNumber;
    std::unique_ptr<wxTimer> m_timer;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
    int m_maxIterations;
    int m_zoomCount;
    int m_powersCount;
    int m_z0Count;
};


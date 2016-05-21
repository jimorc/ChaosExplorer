#pragma once
#include <complex>
#include <memory>
#include <chrono>
#include "wx/event.h"
#include "wx/panel.h"
#include "ChaosPanel.h"

using namespace std::complex_literals;


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
    void OnJulia(wxCommandEvent& event);
    void OnCloseTab();
    void SetStatusBarText();
    void SetupSquareArrays();
    void CreateMainMenu();
    wxMenu* CreateMultibrotSubMenu();
    void AddItemToMenu(wxMenu* menu, const int menuId, std::wstring menuText, float power);
    void StartTimer(const int timerInterval, TimerHandler handler);
    void StopAndReleaseTimer(TimerHandler handler);

    std::unique_ptr<GLShaderProgram> m_squareProgram;
    GLuint m_squareVbo;
    GLuint m_squareVao;

    std::complex<float> m_z0;
    std::complex<float> m_power;
    std::complex<float> m_rightDownPoint;

    bool m_leftButtonDown;
    wxPoint m_leftDown;
    wxPoint m_leftUp;
    wxPoint m_rightDown;

    // rate at which the various animations run
    static const int m_iterationInterval = 1000 / 6;
    static const int m_magnificationInterval = 1000 / 60;
    static const int m_powersInterval = 1000 / 20;
    static const int m_z0Interval = 1000 / 20;

    int m_timerNumber;
    std::unique_ptr<wxTimer> m_timer;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
    int m_maxIterations;
    int m_zoomCount;
    int m_powersCount;
    int m_z0Count;
};


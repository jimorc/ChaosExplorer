#pragma once
#include <complex>
#include "ChaosPanel.h"

using namespace std::complex_literals;


class MandelJuliaPanel :
    public ChaosPanel
{
public:
    MandelJuliaPanel(wxWindow* parent, wxWindowID id, const int* attribList,
        const wxSize& size = { 800, 600 },
        std::complex<float> power = 2.0f,
        std::complex<float> c = 0.0f + 0.0if,
        std::complex<float> ul = -2.0f + 2.0if,
        std::complex<float> lr = 2.0f - 2.0if);
    virtual ~MandelJuliaPanel();

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
    void CreateMainMenu();
    void OnCloseTab();
    void SetupSquareArrays();
    void SetStatusBarText();

    std::unique_ptr<GLShaderProgram> m_squareProgram;
    GLuint m_squareVbo;
    GLuint m_squareVao;

    bool m_leftButtonDown;
    wxPoint m_leftDown;
    wxPoint m_leftUp;

    std::complex<float> m_c;
    std::complex<float> m_p;
    std::complex<float> m_upperLeft;
    std::complex<float> m_lowerRight;

    wxPoint m_rightDown;
};


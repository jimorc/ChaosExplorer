#pragma once
#include <complex>
#include <memory>
#include "wx/panel.h"
#include "ChaosPanel.h"

using namespace std::complex_literals;

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
    void SetupSquareArrays();

    std::unique_ptr<GLShaderProgram> m_squareProgram;
    GLuint m_squareVbo;
    GLuint m_squareVao;

    std::complex<double> m_power;
    std::complex<double> m_upperLeft;
    std::complex<double> m_lowerRight;

    bool m_leftButtonDown;
    wxPoint m_leftDown;
    wxPoint m_leftUp;
};


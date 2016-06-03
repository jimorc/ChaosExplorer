#pragma once
#include <complex>
#include "ChaosPanel.h"
#include "GLMandelJuliaShaderProgram.h"

using namespace std::complex_literals;


class MandelJuliaPanel :
    public ChaosPanel<GLMandelJuliaShaderProgram>
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
    virtual void DrawFractal(GLMandelJuliaShaderProgram* shaderProgram) {}

private:
    virtual void OnPaint(wxPaintEvent& event) override;
    virtual void OnDrawFromSelection(wxCommandEvent& event);
    virtual void OnDeleteSelection(wxCommandEvent& event);
    virtual void OnMenuOpen(wxMenuEvent& event);
    void CreatePopupMenu();
    void OnCloseTab();
    void SetStatusBarText();

    std::complex<float> m_c;
    std::complex<float> m_p;
};


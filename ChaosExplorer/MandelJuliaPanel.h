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
        std::complex<float> c = 0.0f + 0.0if);
    virtual ~MandelJuliaPanel();

protected:
    virtual void BuildShaderProgram() override;

private:
    virtual void OnPaint(wxPaintEvent& event) override;
    virtual void OnRightButtonDown(wxMouseEvent& event);
    void CreateMainMenu();
    void OnCloseTab();

    std::complex<float> m_c;
    std::complex<float> m_p;

    wxMenu* m_popup;
    wxPoint m_rightDown;
};


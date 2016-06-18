#pragma once
#include <complex>
#include "PlottingZ0Panel.h"
#include "GLFractal4JuliaShaderProgram.h"

using namespace std::complex_literals;


class Fractal4JuliaPanel :
    public PlottingZ0Panel<GLFractal4JuliaShaderProgram>
{
public:
    Fractal4JuliaPanel(wxWindow* parent, wxWindowID id, const int* attribList,
        const wxSize& size = { 800, 600 },
        std::complex<float> power = 2.0f,
        std::complex<float> c = 0.0f + 0.0if,
        std::complex<float> ul = -2.0f + 2.0if,
        std::complex<float> lr = 2.0f - 2.0if);
    virtual ~Fractal4JuliaPanel();

private:
    virtual void OnDrawFromSelection(wxCommandEvent& event);
    virtual void OnDeleteSelection(wxCommandEvent& event);
    virtual void OnMenuOpen(wxMenuEvent& event);
    void CreatePopupMenu();
    void OnCloseTab();
};

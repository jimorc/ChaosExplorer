#pragma once
#include "PlottingCPanel.h"
#include "GLFractal3ShaderProgram.h"
#include "Fractal2JuliaPanel.h"

class Fractal3Panel :
    public PlottingCPanel<GLFractal3ShaderProgram, Fractal2JuliaPanel>
{
public:
    Fractal3Panel(wxWindow* parent, wxWindowID id, const int* attribList,
        const wxSize& size = { 800, 600 },
        std::complex<float> power = 2.0f,
        std::complex<float> ul = -2.0f + 2.0if,
        std::complex<float> lr = 2.0f - 2.0if);
    virtual ~Fractal3Panel();

private:
    void CreatePopupMenu();
    void OnDrawFromSelection(wxCommandEvent& event);
    void OnDeleteSelection(wxCommandEvent& event);
    void OnMenuOpen(wxMenuEvent& event);
};


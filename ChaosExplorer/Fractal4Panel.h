#pragma once
#include "PlottingCPanel.h"
#include "GLFractal4ShaderProgram.h"
#include "Fractal4JuliaPanel.h"

class Fractal4Panel :
    public PlottingCPanel<GLFractal4ShaderProgram, Fractal4JuliaPanel>
{
public:
    Fractal4Panel(wxWindow* parent, wxWindowID id, const int* attribList,
        const wxSize& size = { 800, 600 },
        std::complex<float> power = 2.0f,
        std::complex<float> ul = -2.0f + 2.0if,
        std::complex<float> lr = 2.0f - 2.0if);
    virtual ~Fractal4Panel();

private:
    void CreatePopupMenu();
    void OnDrawFromSelection(wxCommandEvent& event);
    void OnDeleteSelection(wxCommandEvent& event);
    void OnMenuOpen(wxMenuEvent& event);
};


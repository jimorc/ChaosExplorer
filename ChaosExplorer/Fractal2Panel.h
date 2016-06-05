#pragma once
#include "PlottingCPanel.h"
#include "GLFractal2ShaderProgram.h"
#include "MandelJuliaPanel.h"

class Fractal2Panel :
    public PlottingCPanel<GLFractal2ShaderProgram, MandelJuliaPanel>
{
public:
    Fractal2Panel(wxWindow* parent, wxWindowID id, const int* attribList,
        const wxSize& size = { 800, 600 },
        std::complex<float> power = 2.0f,
        std::complex<float> ul = -2.0f + 2.0if,
        std::complex<float> lr = 2.0f - 2.0if);
    virtual ~Fractal2Panel();
};


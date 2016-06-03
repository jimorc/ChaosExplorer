#pragma once
#include "wx/wxprec.h"
#include "ChaosPanel.h"

template <typename T>
class PlottingZ0Panel
    : public ChaosPanel<T>
{
public:
    PlottingZ0Panel<T>(wxWindow* parent, wxWindowID id, const int* attribList,
        const wxSize& size = { 800, 600 },
        std::complex<float> power = 2.0f,
        std::complex<float> c = 0.0f,
        std::complex<float> ul = -2.0f + 2.0if,
        std::complex<float> lr = 2.0f - 2.0if)
        : ChaosPanel<T>(parent, id, attribList, ul, lr, size),
        m_c(c), m_p(power)
    {}

   virtual ~PlottingZ0Panel() {}

protected:
    virtual void DrawFractal() override
    {
        T* prog = GetShaderProgram();
        glUseProgram(prog->GetProgramHandle());
        glBindVertexArray(GetVao());
        std::complex<float> upperLeft = GetUpperLeft();
        std::complex<float> lowerRight = GetLowerRight();
        glUniform2f(prog->GetUniformHandle("ul"), upperLeft.real(), upperLeft.imag());
        glUniform2f(prog->GetUniformHandle("lr"), lowerRight.real(), lowerRight.imag());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    std::complex<float> GetC() { return m_c; }
    std::complex<float> GetPower() { return m_p; }

private:
    std::complex<float> m_c;
    std::complex<float> m_p;
};
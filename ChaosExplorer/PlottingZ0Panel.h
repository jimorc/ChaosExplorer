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
    {
        Bind(wxEVT_PAINT, &PlottingZ0Panel<T>::OnPaint, this);
    }

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
    void OnPaint(wxPaintEvent& event)
    {
        wxSize size = GetSize();
        SetContext();
        // set background to black
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // draw the Multibrot image (well, draw the triangles for the display area)
        DrawFractal();
        DrawSquare();

        glFlush();
        SwapBuffers();

        SetStatusBarText();
    }

    void SetStatusBarText()
    {
        ChaosExplorerWindow* win = dynamic_cast<ChaosExplorerWindow*>(GetParent()->GetParent());
        wxStatusBar* statusBar = win->GetStatusBar();
        std::complex<float> upperLeft = GetUpperLeft();
        std::complex<float> lowerRight = GetLowerRight();
        std::wstringstream ss;
        ss << L"C = " << m_c.real();
        m_c.imag() > 0.0f ? ss << L" + " : ss << L" - ";
        ss << abs(m_c.imag()) << L"i";
        ss << L", Power = " << m_p.real();
        m_p.imag() >= 0.0f ? ss << L" + " : ss << L" - ";
        ss << abs(m_p.imag()) << L"i";
        ss << L", Upper Left = " << upperLeft.real();
        upperLeft.imag() > 0.0f ? ss << L" + " : ss << L" - ";
        ss << abs(upperLeft.imag()) << L"i";
        ss << L", Lower Right = " << lowerRight.real();
        lowerRight.imag() > 0.0f ? ss << L" + " : ss << L" - ";
        ss << abs(lowerRight.imag()) << L"i";

        statusBar->SetStatusText(ss.str().c_str());
    }

    std::complex<float> m_c;
    std::complex<float> m_p;
};
#pragma once
#include "wx/wxprec.h"
#include "GLShaderProgram.h"
#include "ChaosPanel.h"
#include <complex>

using namespace std::complex_literals;

template <typename T, typename U>
class PlottingCPanel :
    public ChaosPanel<T>
{
public:
    PlottingCPanel<T, U>(wxWindow* parent, wxWindowID id, const int* attribList,
        const wxSize& size = { 800, 600 },
        std::complex<float> power = 2.0f,
        std::complex<float> ul = -2.5f + 2.0if,
        std::complex<float> lr = 1.5f - 2.0if)
        : ChaosPanel<T>(parent, id, attribList, ul, lr, size),
        m_maxIterations(4 * colors.size()),
        m_power(power), m_z0({ 0.0f, 0.0f })
    {
        // TODO: add check for U being a PlottingZ0Panel
        Bind(wxEVT_PAINT, &PlottingCPanel<T, U>::OnPaint, this);
    }

    virtual ~PlottingCPanel() {}

protected:
    void AddOnJuliaMenuItem(wxMenu *menu)
    {
        menu->Append(ID_JULIA, L"Julia Set");
        Bind(wxEVT_COMMAND_MENU_SELECTED, &PlottingCPanel::OnJulia,
            this, ID_JULIA);
    }

    virtual void DrawFractal() override
    {
        T* shaderProg = GetShaderProgram();
        // draw the image (well, draw the triangles for the display area)
        glUseProgram(shaderProg->GetProgramHandle());
        glBindVertexArray(GetVao());
        glUniform1i(shaderProg->GetUniformHandle("maxIterations"), m_maxIterations);
        glUniform2f(shaderProg->GetUniformHandle("p"), m_power.real(), m_power.imag());
        glUniform2f(shaderProg->GetUniformHandle("z0"), m_z0.real(), m_z0.imag());
        std::complex<float> upperLeft = GetUpperLeft();
        std::complex<float> lowerRight = GetLowerRight();
        glUniform2f(shaderProg->GetUniformHandle("ul"), upperLeft.real(), upperLeft.imag());
        glUniform2f(shaderProg->GetUniformHandle("lr"), lowerRight.real(), lowerRight.imag());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void SetMaxIterations(const int iterations) { m_maxIterations = iterations; }
    void IncrementMaxIterations() { ++m_maxIterations; }
    int GetMaxIterations() { return m_maxIterations; }
    void SetZ0(const std::complex<float>& z0) { m_z0 = z0; }
    std::complex<float> GetZ0() { return m_z0; }
    void SetPower(const std::complex<float>& power) { m_power = power; }
    std::complex<float> GetPower() const { return m_power; }


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

    void SetStatusBarText() override
    {
        ChaosExplorerWindow* win = dynamic_cast<ChaosExplorerWindow*>(GetParent()->GetParent());
        wxStatusBar* statusBar = win->GetStatusBar();
        std::complex<float> upperLeft = GetUpperLeft();
        std::complex<float> lowerRight = GetLowerRight();
        std::wstringstream ss;
        ss << L"Iterations = " << GetMaxIterations();
        ss << L", Power = " << GetPower().real();
        GetPower().imag() >= 0.0f ? ss << L" + " : ss << L" - ";
        ss << abs(GetPower().imag()) << L"i";
        ss << L", Upper Left = " << upperLeft.real();
        upperLeft.imag() > 0.0f ? ss << L" + " : ss << L" - ";
        ss << abs(upperLeft.imag()) << L"i";
        ss << L", Lower Right = " << lowerRight.real();
        lowerRight.imag() > 0.0f ? ss << L" + " : ss << L" - ";
        ss << abs(lowerRight.imag()) << L"i";

        statusBar->SetStatusText(ss.str().c_str());
    }

    void OnJulia(wxCommandEvent& event)
    {
        wxSize size = GetSize();
        std::complex<float> upperLeft = GetUpperLeft();
        std::complex<float> lowerRight = GetLowerRight();
        wxPoint rightDown = GetRightDown();
        float deltaXY = lowerRight.real() - upperLeft.real();
        float x = upperLeft.real() + deltaXY * rightDown.x / size.x;
        float y = upperLeft.imag() - deltaXY * rightDown.y / size.y;
        SetRightDownPoint(std::complex<float>(x, y));

        // create and display a new MandelJuliaPanel for the display
        wxNotebook* nBook = dynamic_cast<wxNotebook*>(GetParent());
        if (nBook == nullptr) {
            throw std::logic_error("Could not retrieve the Notebook for the new JuliaPanel.");
        }
        try {
            U* mPanel = new U(nBook, wxID_ANY, nullptr,
                size, GetPower(),GetRightDownPoint());
            nBook->AddPage(mPanel, L"Julia Set", true);
        }
        catch (std::exception& e) {
            wxMessageBox(e.what(), "Cannot create Julia Set");
        }
    }

    int m_maxIterations;
    std::complex<float> m_z0;
    std::complex<float> m_power;
};


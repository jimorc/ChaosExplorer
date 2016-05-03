#pragma once
#include <complex>
#include <memory>
#include "wx/panel.h"
#include "ChaosPanel.h"

using namespace std::complex_literals;

const int ID_DRAWFROMSELECTION = 2002;

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
    virtual void OnRightButtonDown(wxMouseEvent& event);
    virtual void OnDrawFromSelection(wxCommandEvent& event);
    virtual void OnMenuOpen(wxMenuEvent& event);
    void SetupSquareArrays();
    void CreateMainMenu();

    std::unique_ptr<GLShaderProgram> m_squareProgram;
    GLuint m_squareVbo;
    GLuint m_squareVao;

    std::complex<float> m_power;
    std::complex<float> m_upperLeft;
    std::complex<float> m_lowerRight;

    bool m_leftButtonDown;
    wxPoint m_leftDown;
    wxPoint m_leftUp;

    wxMenu* m_popup;
};


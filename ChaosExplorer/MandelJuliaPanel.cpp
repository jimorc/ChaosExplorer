#include "wx/notebook.h"
#include <stdexcept>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"
#include "GLMandelJuliaShaderProgram.h"
#include "GLSquareShaderProgram.h"
#include "MandelJuliaPanel.h"
#include "ChaosExplorerWindow.h"

extern std::vector<glm::vec4> colors;

MandelJuliaPanel::MandelJuliaPanel(wxWindow* parent, wxWindowID id, const int* attribList,
    const wxSize& size,
    std::complex<float> power,
    std::complex<float> c,
    std::complex<float> ul,
    std::complex<float> lr)
    : PlottingZ0Panel<GLMandelJuliaShaderProgram>(parent, id, attribList, size, power, c, ul, lr)
{
    if (power.real() < 1.0f) {
        throw std::invalid_argument(
            "The real portion of the power argument for Julia Set is less than 1.0");
    }

    // create popup menu
    CreatePopupMenu();

    Bind(wxEVT_PAINT, &MandelJuliaPanel::OnPaint, this);

    // set up GL stuff
    CreateShaderProgram();
    SetupTriangles();
    SetupSquareArrays();
    glUseProgram(GetShaderProgram()->GetProgramHandle());
    GLMandelJuliaShaderProgram* prog = dynamic_cast<GLMandelJuliaShaderProgram*>(GetShaderProgram());
    glUniform2f(prog->GetUniformHandle("c"), c.real(), c.imag());
    glUniform2f(prog->GetUniformHandle("p"), power.real(), power.imag());
    glUniform2f(prog->GetUniformHandle("viewDimensions"), size.x, size.y);
    glUniform4fv(prog->GetUniformHandle("color[0]"), colors.size() * 4, &colors[0].x);
}


MandelJuliaPanel::~MandelJuliaPanel()
{
}

void MandelJuliaPanel::OnPaint(wxPaintEvent& event)
{
    wxSize size = GetSize();
    SetContext();
    // set background to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw the Multibrot image (well, draw the triangles for the display area)
    glUseProgram(GetShaderProgram()->GetProgramHandle());
    glBindVertexArray(GetVao());
    GLMandelJuliaShaderProgram* prog = dynamic_cast<GLMandelJuliaShaderProgram*>(GetShaderProgram());
    std::complex<float> upperLeft = GetUpperLeft();
    std::complex<float> lowerRight = GetLowerRight();
    glUniform2f(prog->GetUniformHandle("ul"), upperLeft.real(), upperLeft.imag());
    glUniform2f(prog->GetUniformHandle("lr"), lowerRight.real(), lowerRight.imag());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    DrawSquare();

    glFlush();
    SwapBuffers();

    SetStatusBarText();
}

void MandelJuliaPanel::CreatePopupMenu()
{
    wxMenu* popup = new wxMenu;
    SetPopupMenu(popup);
    popup->Append(ID_DRAWFROMSELECTION, L"Draw From Selection");
    popup->Append(ID_DELETESELECTION, L"Deselect Selection");
    popup->AppendSeparator();
    popup->Append(ID_PRECLOSETAB, L"Close Tab");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MandelJuliaPanel::OnDrawFromSelection,
        this, ID_DRAWFROMSELECTION);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MandelJuliaPanel::OnDeleteSelection,
        this, ID_DELETESELECTION);
    Bind(wxEVT_COMMAND_MENU_SELECTED, [this](wxCommandEvent& event) {
        // Closing a tab must be done after popup menu has closed.
        // Otherwise, assert fails in wxMenuBase::SetInvokingWindow()
        CallAfter(&ChaosPanel::OnCloseTab); },
        ID_PRECLOSETAB);
    Bind(wxEVT_MENU_OPEN, &MandelJuliaPanel::OnMenuOpen, this);
}

void MandelJuliaPanel::OnDrawFromSelection(wxCommandEvent& event)
{
    std::complex<float> ul;
    std::complex<float> lr;
    CalculateUpperLeftAndLowerRight(ul, lr);
    // create and display a new MultibrotPanel for the display
    wxNotebook* nBook = dynamic_cast<wxNotebook*>(GetParent());
    if (nBook == nullptr) {
        throw std::logic_error("Could not retrieve the Notebook for the new MandelJuliaPanel.");
    }
    MandelJuliaPanel* mPanel = new MandelJuliaPanel(nBook, wxID_ANY, nullptr, GetSize(), GetPower(), GetC(), ul, lr);
    nBook->AddPage(mPanel, L"Mandelbrot-Julia", true);
}

void MandelJuliaPanel::OnDeleteSelection(wxCommandEvent& event)
{
    // to delete the selection, just set leftDown and leftUp positions to the same value
    SetLeftDown({ 0, 0 });
    SetLeftUp({ 0, 0 });
    Refresh();
}

void MandelJuliaPanel::OnMenuOpen(wxMenuEvent& event)
{
    // enable/disable the various popup menu items
    wxPoint leftDown = GetLeftDown();
    wxPoint leftUp = GetLeftUp();
    wxMenu* popup = GetPopupMenu();
    popup->Enable(ID_DRAWFROMSELECTION, leftDown != leftUp);
    popup->Enable(ID_DELETESELECTION, leftDown != leftUp);
    wxNotebook* noteBook = dynamic_cast<wxNotebook*>(GetParent());
    int tabCount = noteBook->GetPageCount();
    popup->Enable(ID_PRECLOSETAB, tabCount > 1);
}

void MandelJuliaPanel::SetStatusBarText()
{
    ChaosExplorerWindow* win = dynamic_cast<ChaosExplorerWindow*>(GetParent()->GetParent());
    wxStatusBar* statusBar = win->GetStatusBar();
    std::complex<float> upperLeft = GetUpperLeft();
    std::complex<float> lowerRight = GetLowerRight();
    std::complex<float> c = GetC();
    std::complex<float> power = GetPower();
    std::wstringstream ss;
    ss << L"C = " << c.real();
    c.imag() > 0.0f ? ss << L" + " : ss << L" - ";
    ss << abs(c.imag()) << L"i";
    ss << L", Power = " << power.real();
    power.imag() >= 0.0f ? ss << L" + " : ss << L" - ";
    ss << abs(power.imag()) << L"i";
    ss << L", Upper Left = " << upperLeft.real();
    upperLeft.imag() > 0.0f ? ss << L" + " : ss << L" - ";
    ss << abs(upperLeft.imag()) << L"i";
    ss << L", Lower Right = " << lowerRight.real();
    lowerRight.imag() > 0.0f ? ss << L" + " : ss << L" - ";
    ss << abs(lowerRight.imag()) << L"i";

    statusBar->SetStatusText(ss.str().c_str());
}

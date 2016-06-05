#include "Fractal3Panel.h"

extern std::vector<glm::vec4> colors;

Fractal3Panel::Fractal3Panel(wxWindow* parent, wxWindowID id, const int* attribList,
    const wxSize& size,
    std::complex<float> power,
    std::complex<float> ul,
    std::complex<float> lr)
    : PlottingCPanel<GLFractal3ShaderProgram, Fractal2JuliaPanel>(parent, id, attribList, size, power, ul, lr)
{
    CreatePopupMenu();
    // set up GL stuff
    CreateShaderProgram();
    SetupTriangles();
    SetupSquareArrays();
    glUseProgram(GetShaderProgram()->GetProgramHandle());
    GLFractal3ShaderProgram* prog = dynamic_cast<GLFractal3ShaderProgram*>(GetShaderProgram());
    glUniform2f(prog->GetUniformHandle("viewDimensions"), size.x, size.y);
    glUniform4fv(prog->GetUniformHandle("color[0]"), colors.size() * 4, &colors[0].x);
}


Fractal3Panel::~Fractal3Panel()
{
}

void Fractal3Panel::CreatePopupMenu()
{
    wxMenu* popup = new wxMenu;
    SetPopupMenu(popup);
    AddOnJuliaMenuItem(popup);
    popup->Append(ID_DRAWFROMSELECTION, L"Draw From Selection");
    popup->Append(ID_DELETESELECTION, L"Deselect Selection");
    popup->AppendSeparator();
    popup->Append(ID_PRECLOSETAB, L"Close Tab");
    // bind the various events related to this menu
    Bind(wxEVT_COMMAND_MENU_SELECTED, &Fractal3Panel::OnDrawFromSelection,
        this, ID_DRAWFROMSELECTION);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &Fractal3Panel::OnDeleteSelection,
        this, ID_DELETESELECTION);
    Bind(wxEVT_COMMAND_MENU_SELECTED, [this](wxCommandEvent& event) {
        // Closing a tab must be done after popup menu has closed.
        // Otherwise, assert fails in wxMenuBase::SetInvokingWindow()
        CallAfter(&ChaosPanel::OnCloseTab); },
        ID_PRECLOSETAB);
}

void Fractal3Panel::OnDrawFromSelection(wxCommandEvent& event)
{
    std::complex<float> ul;
    std::complex<float> lr;
    CalculateUpperLeftAndLowerRight(ul, lr);
    // create and display a new MultibrotPanel for the display
    wxNotebook* nBook = dynamic_cast<wxNotebook*>(GetParent());
    if (nBook == nullptr) {
        throw std::logic_error("Could not retrieve the Notebook for the new FractalPanel.");
    }
    Fractal3Panel* mPanel = new Fractal3Panel(nBook, wxID_ANY, nullptr, GetSize(), GetPower(), ul, lr);
    nBook->AddPage(mPanel, L"c*e^z", true);
}

void Fractal3Panel::OnMenuOpen(wxMenuEvent& event)
{
    wxMenu* popup = GetPopupMenu();
    std::complex<float> upperLeft = GetUpperLeft();
    std::complex<float> lowerRight = GetLowerRight();
    wxPoint leftDown = GetLeftDown();
    wxPoint leftUp = GetLeftUp();
    // enable/disable the various popup menu items
    wxNotebook* noteBook = dynamic_cast<wxNotebook*>(GetParent());
    int tabCount = noteBook->GetPageCount();
    popup->Enable(ID_PRECLOSETAB, tabCount > 1);
}

void Fractal3Panel::OnDeleteSelection(wxCommandEvent& event)
{
    // to delete the selection, just set leftDown and leftUp positions to the same value
    SetLeftDown({ 0, 0 });
    SetLeftUp({ 0, 0 });
    Refresh();
}

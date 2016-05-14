#include "wx/notebook.h"
#include <stdexcept>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"
#include "GLMandelJuliaShaderProgram.h"
#include "MandelJuliaPanel.h"

extern std::vector<glm::vec4> colors;
extern std::vector<glm::vec4> vertices;

MandelJuliaPanel::MandelJuliaPanel(wxWindow* parent, wxWindowID id, const int* attribList,
    const wxSize& size,
    std::complex<float> power,
    std::complex<float> c)
    : ChaosPanel(parent, id, attribList, size), m_c(c), m_p(power),
    m_popup(nullptr)
{
    if (power.real() < 1.0f) {
        throw std::invalid_argument(
            "The real portion of the power argument for Julia Set is less than 1.0");
    }

    // create popup menu
    CreateMainMenu();

    Bind(wxEVT_PAINT, &MandelJuliaPanel::OnPaint, this);
    Bind(wxEVT_RIGHT_DOWN, &MandelJuliaPanel::OnRightButtonDown, this);

    // set up GL stuff
    BuildShaderProgram();
    SetupTriangles(vertices, m_program->GetProgramHandle());
    glUseProgram(m_program->GetProgramHandle());
    GLMandelJuliaShaderProgram* prog = dynamic_cast<GLMandelJuliaShaderProgram*>(m_program.get());
    glUniform2f(prog->GetCHandle(), m_c.real(), m_c.imag());
    glUniform2f(prog->GetPHandle(), m_p.real(), m_p.imag());
    glUniform2f(prog->GetViewDimensionsHandle(), size.x, size.y);
    glUniform4fv(prog->GetColorHandle(), colors.size() * 4, &colors[0].x);
}


MandelJuliaPanel::~MandelJuliaPanel()
{
    // delete popup menu
    if (m_popup != nullptr) {
        delete m_popup;
    }
}

void MandelJuliaPanel::BuildShaderProgram()
{
        m_program = std::make_unique<GLMandelJuliaShaderProgram>(*this);
}

void MandelJuliaPanel::OnPaint(wxPaintEvent& event)
{
    wxSize size = GetSize();
    SetContext();
    // set background to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw the Multibrot image (well, draw the triangles for the display area)
    glUseProgram(m_program->GetProgramHandle());
    glBindVertexArray(GetVao());
    GLMandelJuliaShaderProgram* prog = dynamic_cast<GLMandelJuliaShaderProgram*>(m_program.get());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glFlush();
    SwapBuffers();
}

void MandelJuliaPanel::CreateMainMenu()
{
    m_popup = new wxMenu;
    m_popup->Append(ID_PRECLOSETAB, L"Close Tab");
    Bind(wxEVT_COMMAND_MENU_SELECTED, [this](wxCommandEvent& event) {
        // Closing a tab must be done after popup menu has closed.
        // Otherwise, assert fails in wxMenuBase::SetInvokingWindow()
        CallAfter(&MandelJuliaPanel::OnCloseTab); },
        ID_PRECLOSETAB);
}

void MandelJuliaPanel::OnCloseTab()
{
    wxNotebook* noteBook = dynamic_cast<wxNotebook*>(GetParent());
    int pageNumber = noteBook->GetSelection();
    int pageCount = noteBook->GetPageCount();
    // must change selected tab before deleting a tab/page
    // otherwise, the ChaosPanels get screwed up and one display background only
    if (pageNumber != pageCount - 1) {
        noteBook->ChangeSelection(pageNumber + 1);
    }
    else {
        noteBook->ChangeSelection(pageNumber - 1);
    }
    noteBook->DeletePage(pageNumber);
}

void MandelJuliaPanel::OnRightButtonDown(wxMouseEvent& event)
{
    PopupMenu(m_popup);
}


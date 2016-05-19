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
    : ChaosPanel(parent, id, attribList, size), m_c(c), m_p(power),
    m_leftDown({0, 0}), m_leftUp({ 0, 0}),
    m_upperLeft(ul), m_lowerRight(lr), m_popup(nullptr)
{
    if (power.real() < 1.0f) {
        throw std::invalid_argument(
            "The real portion of the power argument for Julia Set is less than 1.0");
    }

    // create popup menu
    CreateMainMenu();

    Bind(wxEVT_PAINT, &MandelJuliaPanel::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &MandelJuliaPanel::OnLeftButtonDown, this);
    Bind(wxEVT_LEFT_UP, &MandelJuliaPanel::OnLeftButtonUp, this);
    Bind(wxEVT_MOTION, &MandelJuliaPanel::OnMouseMove, this);
    Bind(wxEVT_RIGHT_DOWN, &MandelJuliaPanel::OnRightButtonDown, this);

    // set up GL stuff
    BuildShaderProgram();
    SetupTriangles();
    SetupSquareArrays();
    glUseProgram(m_program->GetProgramHandle());
    GLMandelJuliaShaderProgram* prog = dynamic_cast<GLMandelJuliaShaderProgram*>(m_program.get());
    glUniform2f(prog->GetUniformHandle("c"), m_c.real(), m_c.imag());
    glUniform2f(prog->GetUniformHandle("p"), m_p.real(), m_p.imag());
    glUniform2f(prog->GetUniformHandle("viewDimensions"), size.x, size.y);
    glUniform4fv(prog->GetUniformHandle("color[0]"), colors.size() * 4, &colors[0].x);
}


MandelJuliaPanel::~MandelJuliaPanel()
{
    glDeleteBuffers(1, &m_squareVbo);
    glDeleteVertexArrays(1, &m_squareVao);

    // delete popup menu
    if (m_popup != nullptr) {
        delete m_popup;
    }
}

void MandelJuliaPanel::BuildShaderProgram()
{
    m_program = std::make_unique<GLMandelJuliaShaderProgram>(*this);
    m_squareProgram = std::make_unique<GLSquareShaderProgram>(*this);
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
    glUniform2f(prog->GetUniformHandle("ul"), m_upperLeft.real(), m_upperLeft.imag());
    glUniform2f(prog->GetUniformHandle("lr"), m_lowerRight.real(), m_lowerRight.imag());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


    // draw the square outlining the selected area of the image
    if (m_leftDown.x != m_leftUp.x || m_leftDown.y != m_leftUp.y) {
        glUseProgram(m_squareProgram->GetProgramHandle());
        glBindVertexArray(m_squareVao);
        float halfSize = static_cast<float>(size.x) / 2.0f;
        float downX = m_leftDown.x - halfSize;
        float downY = halfSize - m_leftDown.y;
        float upX = m_leftUp.x - halfSize;
        float upY = halfSize - m_leftUp.y;
        std::vector<glm::vec4> points;
        points.push_back({ downX, downY, 0.0f, halfSize });
        points.push_back({ downX, upY, 0.0f, halfSize });
        points.push_back({ upX, upY, 0.0f, halfSize });
        points.push_back({ upX, downY, 0.0f, halfSize });
        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(points[0]), &points[0], GL_DYNAMIC_DRAW);
        glDrawArrays(GL_LINE_LOOP, 0, points.size());
    }
    glFlush();
    SwapBuffers();

    SetStatusBarText();
}

void MandelJuliaPanel::CreateMainMenu()
{
    m_popup = new wxMenu;
    m_popup->Append(ID_DRAWFROMSELECTION, L"Draw From Selection");
    m_popup->Append(ID_DELETESELECTION, L"Deselect Selection");
    m_popup->AppendSeparator();
    m_popup->Append(ID_PRECLOSETAB, L"Close Tab");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MandelJuliaPanel::OnDrawFromSelection,
        this, ID_DRAWFROMSELECTION);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MandelJuliaPanel::OnDeleteSelection,
        this, ID_DELETESELECTION);
    Bind(wxEVT_COMMAND_MENU_SELECTED, [this](wxCommandEvent& event) {
        // Closing a tab must be done after popup menu has closed.
        // Otherwise, assert fails in wxMenuBase::SetInvokingWindow()
        CallAfter(&MandelJuliaPanel::OnCloseTab); },
        ID_PRECLOSETAB);
    Bind(wxEVT_MENU_OPEN, &MandelJuliaPanel::OnMenuOpen, this);
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

void MandelJuliaPanel::OnLeftButtonDown(wxMouseEvent& event)
{
    // set left button down position
    m_leftButtonDown = true;
    m_leftDown = event.GetPosition();
    m_leftUp = m_leftDown;
}

void MandelJuliaPanel::OnMouseMove(wxMouseEvent& event)
{
    // as mouse moves when left button is down, set m_leftDown to be upper left
    // and m_leftUp to be lower right positions of selection area.
    if (m_leftButtonDown) {
        m_leftUp = event.GetPosition();
        if (m_leftDown.x > m_leftUp.x) {
            int temp = m_leftDown.x;
            m_leftDown.x = m_leftUp.x;
            m_leftUp.x = temp;
        }
        if (m_leftDown.y > m_leftUp.y) {
            m_leftDown.y = m_leftUp.y;
        }
        m_leftUp.y = m_leftDown.y + (m_leftUp.x - m_leftDown.x);
        // redraw the selection square
        Refresh();
    }
}

void MandelJuliaPanel::OnLeftButtonUp(wxMouseEvent& event)
{
    // set final positions of the selection square
    m_leftUp = event.GetPosition();
    m_leftButtonDown = false;
    if (m_leftDown.x > m_leftUp.x) {
        int temp = m_leftDown.x;
        m_leftDown.x = m_leftUp.x;
        m_leftUp.x = temp;
    }
    if (m_leftDown.y > m_leftUp.y) {
        m_leftDown.y = m_leftUp.y;
    }
    m_leftUp.y = m_leftDown.y + (m_leftUp.x - m_leftDown.x);
    // and redraw
    Refresh();
}

void MandelJuliaPanel::SetupSquareArrays()
{
    // set GL stuff for the square that will contain the Multibrot image
    glGenVertexArrays(1, &m_squareVao);
    glBindVertexArray(m_squareVao);
    glGenBuffers(1, &m_squareVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_squareVbo);
    GLint posAttrib = glGetAttribLocation(m_squareProgram->GetProgramHandle(), "position");
    glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(posAttrib);

}

void MandelJuliaPanel::OnDrawFromSelection(wxCommandEvent& event)
{
    // calculate the upper left and lower right locations for the new display
    wxSize size = GetSize();
    float deltaX = m_lowerRight.real() - m_upperLeft.real();
    float deltaY = m_upperLeft.imag() - m_lowerRight.imag();
    float ulReal = m_upperLeft.real() + deltaX * m_leftDown.x / size.x;
    float ulImag = m_upperLeft.imag() - deltaY * m_leftDown.y / size.y;
    float lrReal = m_upperLeft.real() + deltaX * m_leftUp.x / size.x;
    float lrImag = m_upperLeft.imag() - deltaY * m_leftUp.y / size.y;

    std::complex<float> ul(ulReal, ulImag);
    std::complex<float> lr(lrReal, lrImag);

    // create and display a new MultibrotPanel for the display
    wxNotebook* nBook = dynamic_cast<wxNotebook*>(GetParent());
    if (nBook == nullptr) {
        throw std::logic_error("Could not retrieve the Notebook for the new MultibrotPanel.");
    }
    MandelJuliaPanel* mPanel = new MandelJuliaPanel(nBook, wxID_ANY, nullptr, size, m_p, m_c, ul, lr);
    nBook->AddPage(mPanel, L"Mandelbrot-Julia", true);
}

void MandelJuliaPanel::OnDeleteSelection(wxCommandEvent& event)
{
    // to delete the selection, just set leftDown and leftUp positions to the same value
    m_leftDown = m_leftUp = { 0, 0 };
    Refresh();
}

void MandelJuliaPanel::OnMenuOpen(wxMenuEvent& event)
{
    // enable/disable the various popup menu items
    m_popup->Enable(ID_DRAWFROMSELECTION, m_leftDown != m_leftUp);
    m_popup->Enable(ID_DELETESELECTION, m_leftDown != m_leftUp);
    wxNotebook* noteBook = dynamic_cast<wxNotebook*>(GetParent());
    int tabCount = noteBook->GetPageCount();
    m_popup->Enable(ID_PRECLOSETAB, tabCount > 1);
}

void MandelJuliaPanel::SetStatusBarText()
{
    ChaosExplorerWindow* win = dynamic_cast<ChaosExplorerWindow*>(GetParent()->GetParent());
    wxStatusBar* statusBar = win->GetStatusBar();
    std::wstringstream ss;
    ss << L"C = " << m_c.real();
    m_c.imag() > 0.0f ? ss << L" + " : ss << L" - ";
    ss << abs(m_c.imag()) << L"i";
    ss << L", Power = " << m_p.real();
    m_p.imag() >= 0.0f ? ss << L" + " : ss << L" - ";
    ss << abs(m_p.imag()) << L"i";
    ss << L", Upper Left = " << m_upperLeft.real();
    m_upperLeft.imag() > 0.0f ? ss << L" + " : ss << L" - ";
    ss << abs(m_upperLeft.imag()) << L"i";
    ss << L", Lower Right = " << m_lowerRight.real();
    m_lowerRight.imag() > 0.0f ? ss << L" + " : ss << L" - ";
    ss << abs(m_lowerRight.imag()) << L"i";

    statusBar->SetStatusText(ss.str().c_str());
}

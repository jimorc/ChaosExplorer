#define _CRT_SECURE_NO_WARNINGS
#include "wx/notebook.h"
#include <stdexcept>
#include <vector>
#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"
#include "GLMultibrotShaderProgram.h"
#include "GLSquareShaderProgram.h"
#include "MultibrotPanel.h"
#include "ChaosExplorerWindow.h"

// Colours to display Multibrot image in
static std::vector<glm::vec4> colors = {
    { 1.0f, 0.5f, 0.0f, 1.0f },
    { 0.9f, 0.45f, 0.0f, 1.0f },
    { 0.8f, 0.4f, 0.0f, 1.0f },
    { 0.7f, 0.35f, 0.0f, 1.0f },
    { 0.6f, 0.3f, 0.0f, 1.0f },
    { 0.5f, 0.25f, 0.0f, 1.0f },
    { 0.4f, 0.2f, 0.0f, 1.0f },
    { 0.3f, 0.15f, 0.0f, 1.0f },
    { 0.2f, 0.1f, 0.0f, 1.0f },
    { 0.1f, 0.05f, 0.0f, 1.0f },
    { 0.0f, 0.0f, 0.1f, 1.0f },
    { 0.0f, 0.0f, 0.2f, 1.0f },
    { 0.0f, 0.0f, 0.3f, 1.0f },
    { 0.0f, 0.0f, 0.4f, 1.0f },
    { 0.0f, 0.0f, 0.5f, 1.0f },
    { 0.1f, 0.0f, 0.5f, 1.0f },
    { 0.2f, 0.0f, 0.5f, 1.0f },
    { 0.3f, 0.0f, 0.5f, 1.0f },
    { 0.4f, 0.0f, 0.5f, 1.0f },
    { 0.5f, 0.0f, 0.5f, 1.0f },
    { 0.5f, 0.0f, 0.4f, 1.0f },
    { 0.5f, 0.0f, 0.3f, 1.0f },
    { 0.5f, 0.0f, 0.2f, 1.0f },
    { 0.5f, 0.0f, 0.1f, 1.0f },
    { 0.5f, 0.0f, 0.0f, 1.0f },
    { 0.4f, 0.0f, 0.0f, 1.0f },
    { 0.3f, 0.0f, 0.0f, 1.0f },
    { 0.2f, 0.0f, 0.0f, 1.0f },
    { 0.1f, 0.0f, 0.0f, 1.0f },
    { 0.0f, 0.1f, 0.0f, 1.0f },
    { 0.0f, 0.2f, 0.0f, 1.0f },
    { 0.0f, 0.3f, 0.0f, 1.0f },
    { 0.0f, 0.4f, 0.0f, 1.0f },
    { 0.0f, 0.4f, 0.0f, 1.0f },
    { 0.0f, 0.5f, 0.0f, 1.0f },
    { 0.0f, 0.5f, 0.1f, 1.0f },
    { 0.0f, 0.5f, 0.2f, 1.0f },
    { 0.0f, 0.5f, 0.3f, 1.0f },
    { 0.0f, 0.5f, 0.4f, 1.0f },
    { 0.0f, 0.5f, 0.5f, 1.0f },
    { 0.0f, 0.4f, 0.5f, 1.0f },
    { 0.0f, 0.3f, 0.5f, 1.0f },
    { 0.0f, 0.2f, 0.5f, 1.0f },
    { 0.0f, 0.1f, 0.5f, 1.0f },
    { 0.1f, 0.1f, 0.5f, 1.0f },
    { 0.2f, 0.2f, 0.4f, 1.0f },
    { 0.3f, 0.3f, 0.1f, 1.0f },
    { 0.4f, 0.4f, 0.1f, 1.0f },
    { 0.5f, 0.5f, 0.0f, 1.0f },
    { 0.3f, 0.3f, 0.3f, 1.0f }
};

// The vertices that define the two triangles.
// These vertices take up entire view
std::vector<glm::vec4> vertices = {
    { 1.0f, 1.0f, 0.0f, 1.0f },
    { -1.0f, 1.0f, 0.0f, 1.0f },
    { 1.0f, -1.0f, 0.0f, 1.0f },
    { -1.0f, -1.0f, 0.0f, 1.0f }
};

// Constructor
MultibrotPanel::MultibrotPanel(wxWindow* parent, wxWindowID id, const int* attribList,
    const wxSize& size,
    std::complex<float> power,
    std::complex<float> ul,
    std::complex<float> lr)
    : ChaosPanel(parent, id, attribList, size),
    m_power(power), m_leftButtonDown(false), m_leftDown({ 0, 0 }), m_leftUp({ 0, 0 }),
    m_rightDown({ 0, 0 }), m_popup(nullptr), m_maxIterations(4 * colors.size()),
    m_zoomCount(0), m_powersCount(0)
    {
        if (power.real() < 1.0f) {
        throw std::invalid_argument(
            "The real portion of the power argument for Multibrot is less than 1.0");
    }
    m_upperLeft = std::complex<float>(std::min(ul.real(), lr.real()), std::max(ul.imag(), lr.imag()));
    m_lowerRight = std::complex<float>(std::max(ul.real(), lr.real()), std::min(ul.imag(), lr.imag()));
    if (ul.real() == lr.real()) {
        throw std::invalid_argument(
            "The real portions of the upper-left and lower-right corners of the Multibrot display are equal.");
    }
    if (ul.imag() == lr.imag()) {
        throw std::invalid_argument(
            "The imag portions of the upper-left and lower-right corners of the Multibrot display are equal.");
    }


    // create popup menu
    CreateMainMenu();

    Bind(wxEVT_PAINT, &MultibrotPanel::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &MultibrotPanel::OnLeftButtonDown, this);
    Bind(wxEVT_LEFT_UP, &MultibrotPanel::OnLeftButtonUp, this);
    Bind(wxEVT_MOTION, &MultibrotPanel::OnMouseMove, this);

    // set up GL stuff
    BuildShaderProgram();
    SetupTriangles(vertices, m_program->GetProgramHandle());
    SetupSquareArrays();
    glUseProgram(m_program->GetProgramHandle());
    GLMultibrotShaderProgram* prog = dynamic_cast<GLMultibrotShaderProgram*>(m_program.get());
    glUniform2f(prog->GetZ0Handle(), 0.0f, 0.0f);
    glUniform2f(prog->GetULHandle(), m_upperLeft.real(), m_upperLeft.imag());
    glUniform2f(prog->GetLRHandle(), m_lowerRight.real(), m_lowerRight.imag());
    glUniform2f(prog->GetViewDimensionsHandle(), size.x, size.y);
    glUniform4fv(prog->GetColorHandle(), colors.size() *4, &colors[0].x);
}


MultibrotPanel::~MultibrotPanel()
{
    glDeleteBuffers(1, &m_squareVbo);
    glDeleteVertexArrays(1, &m_squareVao);

    // delete popup menu
    if (m_popup != nullptr) {
        delete m_popup;
    }
}

void MultibrotPanel::OnPaint(wxPaintEvent& event)
{
    wxSize size = GetSize();
    SetContext();
    // set background to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw the Multibrot image (well, draw the triangles for the display area)
    glUseProgram(m_program->GetProgramHandle());
    glBindVertexArray(GetVao());
    GLMultibrotShaderProgram* prog = dynamic_cast<GLMultibrotShaderProgram*>(m_program.get());
    glUniform1i(prog->GetMaxIterationsHandle(), m_maxIterations);
    glUniform2f(prog->GetULHandle(), m_upperLeft.real(), m_upperLeft.imag());
    glUniform2f(prog->GetLRHandle(), m_lowerRight.real(), m_lowerRight.imag());
    glUniform2f(prog->GetPHandle(), m_power.real(), m_power.imag());
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


void MultibrotPanel::BuildShaderProgram()
{
    m_program = std::make_unique<GLMultibrotShaderProgram>(*this);
    m_squareProgram = std::make_unique<GLSquareShaderProgram>(*this);
}

void MultibrotPanel::OnLeftButtonDown(wxMouseEvent& event)
{
    // set left button down position
    m_leftButtonDown = true;
    m_leftDown = event.GetPosition();
    m_leftUp = m_leftDown;
}

void MultibrotPanel::OnMouseMove(wxMouseEvent& event)
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

void MultibrotPanel::OnLeftButtonUp(wxMouseEvent& event)
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

void MultibrotPanel::SetupSquareArrays()
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

void MultibrotPanel::CreateMainMenu()
{
    // create Multibrot submenu
    wxMenu* multiMenu = CreateMultibrotSubMenu();
    // create the popup menu
    m_popup = new wxMenu;
    m_popup->AppendSubMenu(multiMenu, L"MultibrotPower");
    m_popup->AppendSeparator();
    m_popup->Append(ID_DRAWFROMSELECTION, L"Draw From Selection");
    m_popup->Enable(ID_DRAWFROMSELECTION, false);
    m_popup->Append(ID_DELETESELECTION, L"Delete Selection");
    m_popup->Enable(ID_DELETESELECTION, false);
    m_popup->AppendSeparator();
    m_popup->Append(ID_ANIMATEITERATIONS, L"Animate Iterations");
    m_popup->Enable(ID_ANIMATEITERATIONS, true);
    m_popup->Append(ID_ANIMATEMAGNIFICATION, L"Animate Magnification");
    m_popup->Enable(ID_ANIMATEMAGNIFICATION, true);
    m_popup->Append(ID_ANIMATEREALPOWERS, L"Animate Real Powers");
    m_popup->Enable(ID_ANIMATEREALPOWERS, true);
    m_popup->Append(ID_ANIMATEIMAGINARYPOWERS, L"Animate Imaginary Powers");
    m_popup->Enable(ID_ANIMATEIMAGINARYPOWERS, true);

    // bind the various events related to this menu
    Bind(wxEVT_RIGHT_DOWN, &MultibrotPanel::OnRightButtonDown, this);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MultibrotPanel::OnDrawFromSelection,
        this, ID_DRAWFROMSELECTION);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MultibrotPanel::OnDeleteSelection,
        this, ID_DELETESELECTION);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MultibrotPanel::OnAnimateIterations,
        this, ID_ANIMATEITERATIONS);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MultibrotPanel::OnAnimateMagnification,
        this, ID_ANIMATEMAGNIFICATION);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MultibrotPanel::OnAnimateRealPowers,
        this, ID_ANIMATEREALPOWERS);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MultibrotPanel::OnAnimateImaginaryPowers,
        this, ID_ANIMATEIMAGINARYPOWERS);
    Bind(wxEVT_MENU_OPEN, &MultibrotPanel::OnMenuOpen, this);
}

wxMenu* MultibrotPanel::CreateMultibrotSubMenu()
{
    wxMenu* multiMenu = new wxMenu;
    AddItemToMenu(multiMenu, ID_POWER2, L"Power=2", 2.0f);
    AddItemToMenu(multiMenu, ID_POWER3, L"Power=3", 3.0f);
    AddItemToMenu(multiMenu, ID_POWER4, L"Power=4", 4.0f);
    AddItemToMenu(multiMenu, ID_POWER5, L"Power=5", 5.0f);
    AddItemToMenu(multiMenu, ID_POWER6, L"Power=6", 6.0f);
    AddItemToMenu(multiMenu, ID_POWER7, L"Power=7", 7.0f);
    AddItemToMenu(multiMenu, ID_POWER8, L"Power=8", 8.0f);
    AddItemToMenu(multiMenu, ID_POWER9, L"Power=9", 9.0f);
    AddItemToMenu(multiMenu, ID_POWER10, L"Power=10", 10.0f);
    return multiMenu;
}

void MultibrotPanel::AddItemToMenu(wxMenu* menu, const int menuId, std::wstring menuText,
    float power)
{
    menu->Append(menuId, menuText.c_str());
    Bind(wxEVT_COMMAND_MENU_SELECTED,
        [this, power](wxCommandEvent&) {
        m_power = { power, 0.0f }; m_upperLeft = { -2.5f, 2.0f }; 
        m_lowerRight = { 1.5f, -2.0f }; Refresh(); },
        menuId);
}

void MultibrotPanel::OnRightButtonDown(wxMouseEvent& event)
{
    if (m_timer == nullptr) {
        m_rightDown = event.GetPosition();
        PopupMenu(m_popup);
    }
}

void MultibrotPanel::OnDrawFromSelection(wxCommandEvent& event)
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
    MultibrotPanel* mPanel = new MultibrotPanel(nBook, wxID_ANY, nullptr, size, m_power, ul, lr);
    nBook->AddPage(mPanel, L"Multibrot", true);
}

void MultibrotPanel::OnMenuOpen(wxMenuEvent& event)
{
    // enable/disable the various popup menu items
    m_popup->Enable(ID_DRAWFROMSELECTION, m_leftDown != m_leftUp);
    m_popup->Enable(ID_DELETESELECTION, m_leftDown != m_leftUp);
}

void MultibrotPanel::OnAnimateIterations(wxCommandEvent& event)
{
    wxBeginBusyCursor();
    // get a new timer number
    m_timerNumber = GetTimer();
    // MSW has a limited number of timers, so we must check that we got one.
    if (m_timerNumber != NOTIMERS) {
        // start the timer and run AnimateIterations each time it generates event
        m_startTime = std::chrono::high_resolution_clock::now();
        m_timer = std::make_unique<wxTimer>(this, m_timerNumber);
        m_timer->Start(m_iterationInterval);
        Bind(wxEVT_TIMER, &MultibrotPanel::AnimateIterations, this);
        m_maxIterations = 1;
    }
}

void MultibrotPanel::AnimateIterations(wxTimerEvent& event)
{
    ++m_maxIterations;
    // if we have reached max iterations, unbind the timer event and stop and delete the timer.
    if (m_maxIterations > 4 * colors.size()) {
        Unbind(wxEVT_TIMER, &MultibrotPanel::AnimateIterations, this);
        m_maxIterations = 4 * colors.size();
        m_timer->Stop();
        wxTimer* timer = m_timer.release();
        delete timer;
        ReleaseTimer(m_timerNumber);
        wxEndBusyCursor();
    }
    Refresh();
}

void MultibrotPanel::OnDeleteSelection(wxCommandEvent& event)
{
    // to delete the selection, just set leftDown and leftUp positions to the same value
    m_leftDown = m_leftUp = { 0, 0 };
    Refresh();
}

void MultibrotPanel::SetStatusBarText()
{
    ChaosExplorerWindow* win = dynamic_cast<ChaosExplorerWindow*>(GetParent()->GetParent());
    wxStatusBar* statusBar = win->GetStatusBar();
    std::wstringstream ss;
    ss << L"Iterations = " << m_maxIterations;
    ss << L", Power = " << m_power.real();
    m_power.imag() >= 0.0f ? ss << L" + " : ss << L" - ";
    ss << abs(m_power.imag()) << L"i";
    ss << L", Upper Left = " << m_upperLeft.real();
    m_upperLeft.imag() > 0.0f ? ss << L" + " : ss << L" - ";
    ss << abs(m_upperLeft.imag()) << L"i";
    ss << L", Lower Right = " << m_lowerRight.real();
    m_lowerRight.imag() > 0.0f ? ss << L" + " : ss << L" - ";
    ss << abs(m_lowerRight.imag()) << L"i";

    statusBar->SetStatusText(ss.str().c_str());
}

void MultibrotPanel::OnAnimateMagnification(wxCommandEvent& event)
{
    wxBeginBusyCursor();
    m_zoomCount = 0;
    wxSize size = GetSize();
    float deltaXY = m_lowerRight.real() - m_upperLeft.real();
    float x = m_upperLeft.real() + deltaXY *m_rightDown.x / size.x;
    float y = m_upperLeft.imag() - deltaXY * m_rightDown.y / size.y;
    m_rightDownPoint = { x, y };
    m_upperLeft = { (x - deltaXY / 2.0f), (y + deltaXY / 2.0f) };
    m_lowerRight = { (x + deltaXY / 2.0f), (y - deltaXY / 2.0f) };
    // get a new timer number
    m_timerNumber = GetTimer();
    // MSW has a limited number of timers, so we must check that we got one.
    if (m_timerNumber != NOTIMERS) {
        // start the timer and run AnimateIterations each time it generates event
        m_startTime = std::chrono::high_resolution_clock::now();
        m_timer = std::make_unique<wxTimer>(this, m_timerNumber);
        m_timer->Start(m_magnificationInterval);
        Bind(wxEVT_TIMER, &MultibrotPanel::AnimateMagnification, this);
    }
    Refresh();
}

void MultibrotPanel::AnimateMagnification(wxTimerEvent& event)
{
    ++m_zoomCount;
    if (m_zoomCount <= 500) {
        float deltaXY = (m_lowerRight.real() - m_upperLeft.real()) * 0.99f / 2.0f;
        m_upperLeft = { (m_rightDownPoint.real() - deltaXY), (m_rightDownPoint.imag() + deltaXY) };
        m_lowerRight = { (m_rightDownPoint.real() + deltaXY), (m_rightDownPoint.imag() - deltaXY) };
        Refresh();
    }
    else {
        Unbind(wxEVT_TIMER, &MultibrotPanel::AnimateMagnification, this);
        m_timer->Stop();
        wxTimer* timer = m_timer.release();
        delete timer;
        ReleaseTimer(m_timerNumber);
        m_zoomCount = 0;
        wxEndBusyCursor();
    }
}

void MultibrotPanel::OnAnimateRealPowers(wxCommandEvent& event)
{
    wxBeginBusyCursor();
    m_power = 1.0f;
    m_timerNumber = GetTimer();
    // MSW has a limited number of timers, so we must check that we got one.
    if (m_timerNumber != NOTIMERS) {
        m_startTime = std::chrono::high_resolution_clock::now();
        m_timer = std::make_unique<wxTimer>(this, m_timerNumber);
        m_timer->Start(m_realPowersInterval);
        Bind(wxEVT_TIMER, &MultibrotPanel::AnimateRealPowers, this);
    }
    Refresh();
}

void MultibrotPanel::AnimateRealPowers(wxTimerEvent& event)
{
    ++m_powersCount;
    if (m_powersCount <= 900) {
        m_power = 1.0f + 0.01 * m_powersCount;
        Refresh();
    }
    else {
        Unbind(wxEVT_TIMER, &MultibrotPanel::AnimateRealPowers, this);
        m_timer->Stop();
        wxTimer* timer = m_timer.release();
        delete timer;
        ReleaseTimer(m_timerNumber);
        m_powersCount = 0;
        wxEndBusyCursor();
    }
}

void MultibrotPanel::OnAnimateImaginaryPowers(wxCommandEvent& event)
{
    m_power = { m_power.real(), -1.0f };
    m_timerNumber = GetTimer();
    // MSW has limited number of timers, so we must check that we got one.
    if (m_timerNumber != NOTIMERS) {
        m_startTime = std::chrono::high_resolution_clock::now();
        m_timer = std::make_unique<wxTimer>(this, m_timerNumber);
        m_timer->Start(m_realPowersInterval);
        Bind(wxEVT_TIMER, &MultibrotPanel::AnimateImaginaryPowers, this);
        wxBeginBusyCursor();
    }
    Refresh();
}

void MultibrotPanel::AnimateImaginaryPowers(wxTimerEvent& event)
{
    ++m_powersCount;
    if (m_powersCount <= 200) {
        m_power = { m_power.real(), -1.0f + 0.01f * m_powersCount };
        Refresh();
    }
    else {
        Unbind(wxEVT_TIMER, &MultibrotPanel::AnimateImaginaryPowers, this);
        m_timer->Stop();
        wxTimer* timer = m_timer.release();
        delete timer;
        ReleaseTimer(m_timerNumber);
        m_powersCount = 0;
        wxEndBusyCursor();
    }
}

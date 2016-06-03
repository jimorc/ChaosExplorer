#define _CRT_SECURE_NO_WARNINGS
#include "wx/notebook.h"
#include <stdexcept>
#include <vector>
#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"
#include "GLSquareShaderProgram.h"
#include "MultibrotPanel.h"
#include "MandelJuliaPanel.h"
#include "ChaosExplorerWindow.h"

// Colours to display Multibrot image in
extern std::vector<glm::vec4> colors = {
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

// Constructor
MultibrotPanel::MultibrotPanel(wxWindow* parent, wxWindowID id, const int* attribList,
    const wxSize& size,
    std::complex<float> power,
    std::complex<float> ul,
    std::complex<float> lr)
    : PlottingCPanel<GLMultibrotShaderProgram>(parent, id, attribList, size, power, ul, lr),
    m_zoomCount(0), m_powersCount(0), m_z0Count(0)
    {
        if (power.real() < 1.0f) {
        throw std::invalid_argument(
            "The real portion of the power argument for Multibrot is less than 1.0");
    }
    if (ul.real() == lr.real()) {
        throw std::invalid_argument(
            "The real portions of the upper-left and lower-right corners of the Multibrot display are equal.");
    }
    if (ul.imag() == lr.imag()) {
        throw std::invalid_argument(
            "The imag portions of the upper-left and lower-right corners of the Multibrot display are equal.");
    }


    // create popup menu
    CreatePopupMenu();

    // set up GL stuff
    CreateShaderProgram();
    SetupTriangles();
    SetupSquareArrays();
    glUseProgram(GetShaderProgram()->GetProgramHandle());
    GLMultibrotShaderProgram* prog = dynamic_cast<GLMultibrotShaderProgram*>(GetShaderProgram());
    glUniform2f(prog->GetUniformHandle("viewDimensions"), size.x, size.y);
    glUniform4fv(prog->GetUniformHandle("color[0]"), colors.size() * 4, &colors[0].x);
}


MultibrotPanel::~MultibrotPanel()
{
}

void MultibrotPanel::CreatePopupMenu()
{
    // create Multibrot submenu
    wxMenu* multiMenu = CreateMultibrotSubMenu();
    // create the popup menu
    wxMenu* popup = new wxMenu;
    SetPopupMenu(popup);
    popup->AppendSubMenu(multiMenu, L"MultibrotPower");
    popup->AppendSeparator();
    popup->Append(ID_JULIA, L"Julia Set");
    popup->Append(ID_DRAWFROMSELECTION, L"Draw From Selection");
    popup->Enable(ID_DRAWFROMSELECTION, false);
    popup->Append(ID_DELETESELECTION, L"Deselect Selection");
    popup->Enable(ID_DELETESELECTION, false);
    popup->AppendSeparator();
    popup->Append(ID_ANIMATEITERATIONS, L"Animate Iterations");
    popup->Enable(ID_ANIMATEITERATIONS, true);
    popup->Append(ID_ANIMATEMAGNIFICATION, L"Animate Magnification");
    popup->Enable(ID_ANIMATEMAGNIFICATION, true);
    popup->Append(ID_ANIMATEREALPOWERS, L"Animate Real Powers");
    popup->Enable(ID_ANIMATEREALPOWERS, true);
    popup->Append(ID_ANIMATEIMAGINARYPOWERS, L"Animate Imaginary Powers");
    popup->Enable(ID_ANIMATEIMAGINARYPOWERS, true);
    popup->Append(ID_ANIMATEZ0REAL, L"Animate Z0 Real");
    popup->Enable(ID_ANIMATEZ0REAL, true);
    popup->Append(ID_ANIMATEZ0IMAG, L"Animage Z0 Imaginary");
    popup->Enable(ID_ANIMATEZ0IMAG, true);
    popup->AppendSeparator();
    popup->Append(ID_PRECLOSETAB, L"Close Tab");
    // bind the various events related to this menu
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MultibrotPanel::OnJulia,
        this, ID_JULIA);
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
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MultibrotPanel::OnAnimateZ0Real,
        this, ID_ANIMATEZ0REAL);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MultibrotPanel::OnAnimateZ0Imag,
        this, ID_ANIMATEZ0IMAG);
    Bind(wxEVT_COMMAND_MENU_SELECTED, [this](wxCommandEvent& event) {
        // Closing a tab must be done after popup menu has closed.
        // Otherwise, assert fails in wxMenuBase::SetInvokingWindow()
        CallAfter(&ChaosPanel::OnCloseTab); },
        ID_PRECLOSETAB);

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
        SetLeftDown({ 0, 0 });
        SetLeftUp({ 0, 0 });
        SetPower({ power, 0.0f }); 
        SetUpperLeftLowerRight(-2.5f + 2.0if, 1.5f - 2.0if);
       SetZ0({ 0.0f, 0.0f }); 
        Refresh(); },
        menuId);
}

void MultibrotPanel::OnRightButtonDown(wxMouseEvent& event)
{
    if (m_timer == nullptr) {
        ChaosPanel::OnRightButtonDown(event);
    }
}

void MultibrotPanel::OnDrawFromSelection(wxCommandEvent& event)
{
    std::complex<float> ul;
    std::complex<float> lr;
    CalculateUpperLeftAndLowerRight(ul, lr);
    // create and display a new MultibrotPanel for the display
    wxNotebook* nBook = dynamic_cast<wxNotebook*>(GetParent());
    if (nBook == nullptr) {
        throw std::logic_error("Could not retrieve the Notebook for the new MultibrotPanel.");
    }
    MultibrotPanel* mPanel = new MultibrotPanel(nBook, wxID_ANY, nullptr, GetSize(), GetPower(), ul, lr);
    nBook->AddPage(mPanel, L"Multibrot", true);
}

void MultibrotPanel::OnMenuOpen(wxMenuEvent& event)
{
    wxMenu* popup = GetPopupMenu();
    std::complex<float> upperLeft = GetUpperLeft();
    std::complex<float> lowerRight = GetLowerRight();
    wxPoint leftDown = GetLeftDown();
    wxPoint leftUp = GetLeftUp();
    // enable/disable the various popup menu items
    popup->Enable(ID_DRAWFROMSELECTION, leftDown != leftUp &&
       GetZ0() == std::complex<float>({0.0f, 0.0f}));
    popup->Enable(ID_DELETESELECTION, leftDown != leftUp);
    popup->Enable(ID_ANIMATEITERATIONS, GetZ0() == std::complex<float>({ 0.0f, 0.0f }));
    popup->Enable(ID_ANIMATEMAGNIFICATION, GetZ0() == std::complex<float>({ 0.0f, 0.0f }));
    popup->Enable(ID_ANIMATEREALPOWERS, GetZ0() == std::complex<float>({ 0.0f, 0.0f }) &&
        upperLeft == std::complex<float>({ -2.5f, 2.0f }) &&
        lowerRight == std::complex<float>({ 1.5f, -2.0f }));
    popup->Enable(ID_ANIMATEIMAGINARYPOWERS, GetZ0() == std::complex<float>({ 0.0f, 0.0f }) &&
        upperLeft == std::complex<float>({ -2.5f, 2.0f }) &&
        lowerRight == std::complex<float>({ 1.5f, -2.0f }));
    popup->Enable(ID_ANIMATEZ0REAL, GetZ0() == std::complex<float>({ 0.0f, 0.0f }) &&
        upperLeft == std::complex<float>({ -2.5f, 2.0f }) &&
        lowerRight == std::complex<float>({ 1.5f, -2.0f }));
    popup->Enable(ID_ANIMATEZ0IMAG, GetZ0() == std::complex<float>({ 0.0f, 0.0f }) &&
        upperLeft == std::complex<float>({ -2.5f, 2.0f }) &&
        lowerRight == std::complex<float>({ 1.5f, -2.0f }));
    wxNotebook* noteBook = dynamic_cast<wxNotebook*>(GetParent());
    int tabCount = noteBook->GetPageCount();
    popup->Enable(ID_PRECLOSETAB, tabCount > 1);
}

void MultibrotPanel::OnAnimateIterations(wxCommandEvent& event)
{
    StartTimer(m_iterationInterval, &MultibrotPanel::AnimateIterations);
    // only set m_maxIterations to 1 if successful obtaining a timer.
    // otherwise, m_maxIterations must be 4 * colors.size().
    if (m_timerNumber != NOTIMERS) {
        SetMaxIterations(1);
    }
}

void MultibrotPanel::AnimateIterations(wxTimerEvent& event)
{
    IncrementMaxIterations();
    // if we have reached max iterations, unbind the timer event and stop and delete the timer.
    if (GetMaxIterations() > 4 * colors.size()) {
        StopAndReleaseTimer(&MultibrotPanel::AnimateIterations);
       SetMaxIterations(4 * colors.size());
    }
    Refresh();
}

void MultibrotPanel::OnDeleteSelection(wxCommandEvent& event)
{
    // to delete the selection, just set leftDown and leftUp positions to the same value
    SetLeftDown({ 0, 0 });
    SetLeftUp({ 0, 0 });
    Refresh();
}

void MultibrotPanel::OnAnimateMagnification(wxCommandEvent& event)
{
    m_zoomCount = 0;
    wxSize size = GetSize();
    std::complex<float> upperLeft = GetUpperLeft();
    std::complex<float> lowerRight = GetLowerRight();
    wxPoint rightDown = GetRightDown();
    float deltaXY = lowerRight.real() - upperLeft.real();
    float x = upperLeft.real() + deltaXY *rightDown.x / size.x;
    float y = upperLeft.imag() - deltaXY * rightDown.y / size.y;
    m_rightDownPoint = { x, y };
    upperLeft = { (x - deltaXY / 2.0f), (y + deltaXY / 2.0f) };
    lowerRight = { (x + deltaXY / 2.0f), (y - deltaXY / 2.0f) };
    SetUpperLeftLowerRight(upperLeft, lowerRight);

    StartTimer(m_magnificationInterval, &MultibrotPanel::AnimateMagnification);
    Refresh();
}

void MultibrotPanel::AnimateMagnification(wxTimerEvent& event)
{
    std::complex<float> upperLeft = GetUpperLeft();
    std::complex<float> lowerRight = GetLowerRight();
    ++m_zoomCount;
    if (m_zoomCount <= 500) {
        float deltaXY = (lowerRight.real() - upperLeft.real()) * 0.99f / 2.0f;
        upperLeft = { (m_rightDownPoint.real() - deltaXY), (m_rightDownPoint.imag() + deltaXY) };
        lowerRight = { (m_rightDownPoint.real() + deltaXY), (m_rightDownPoint.imag() - deltaXY) };
        SetUpperLeftLowerRight(upperLeft, lowerRight);
        Refresh();
    }
    else {
        StopAndReleaseTimer(&MultibrotPanel::AnimateMagnification);
        m_zoomCount = 0;
    }
}

void MultibrotPanel::OnAnimateRealPowers(wxCommandEvent& event)
{
    SetPower(1.0f);
    StartTimer(m_powersInterval, &MultibrotPanel::AnimateRealPowers);
    Refresh();
}

void MultibrotPanel::AnimateRealPowers(wxTimerEvent& event)
{
    ++m_powersCount;
    if (m_powersCount <= 900) {
        SetPower(1.0f + 0.01 * m_powersCount);
        Refresh();
    }
    else {
        StopAndReleaseTimer(&MultibrotPanel::AnimateRealPowers);
        m_powersCount = 0;
    }
}

void MultibrotPanel::OnAnimateImaginaryPowers(wxCommandEvent& event)
{
    SetPower({ GetPower().real(), -1.0f });
    StartTimer(m_powersInterval, &MultibrotPanel::AnimateImaginaryPowers);
    Refresh();
}

void MultibrotPanel::AnimateImaginaryPowers(wxTimerEvent& event)
{
    ++m_powersCount;
    if (m_powersCount <= 200) {
       SetPower({ GetPower().real(), -1.0f + 0.01f * m_powersCount });
        Refresh();
    }
    else {
        StopAndReleaseTimer(&MultibrotPanel::AnimateImaginaryPowers);
        m_powersCount = 0;
    }
}

void MultibrotPanel::OnAnimateZ0Real(wxCommandEvent& event)
{
    SetZ0({ -1.0f, 0.0f });
    m_z0Count = 0;
    StartTimer(m_z0Interval, &MultibrotPanel::AnimateZ0Real);
    Refresh();
}

void MultibrotPanel::AnimateZ0Real(wxTimerEvent& event)
{
    ++m_z0Count;
    if (m_z0Count <= 200) {
       SetZ0({ -1 + 0.01f * m_z0Count, 0.0f });
        Refresh();
    }
    else {
        StopAndReleaseTimer(&MultibrotPanel::AnimateZ0Real);
        m_z0Count = 0;
    }
}

void MultibrotPanel::OnAnimateZ0Imag(wxCommandEvent& event)
{
   SetZ0({ -0.0f, -1.0f });
    m_z0Count = 0;
    StartTimer(m_z0Interval, &MultibrotPanel::AnimateZ0Imag);
    Refresh();
}

void MultibrotPanel::AnimateZ0Imag(wxTimerEvent& event)
{
    ++m_z0Count;
    if (m_z0Count <= 200) {
       SetZ0({ 0.0f, -1 + 0.01f * m_z0Count });
        Refresh();
    }
    else {
        StopAndReleaseTimer(&MultibrotPanel::AnimateZ0Imag);
        m_z0Count = 0;
    }
}

void MultibrotPanel::StartTimer(const int timerInterval, TimerHandler handler)
{
    m_timerNumber = GetTimer();
    // MSW has limited number of timers, so we must check that we got one.
    if (m_timerNumber != NOTIMERS) {
        m_startTime = std::chrono::high_resolution_clock::now();
        m_timer = std::make_unique<wxTimer>(this, m_timerNumber);
        m_timer->Start(timerInterval);
        Bind(wxEVT_TIMER, handler, this);
        wxBeginBusyCursor();
    }
}

void MultibrotPanel::StopAndReleaseTimer(TimerHandler handler)
{
    Unbind(wxEVT_TIMER, handler, this);
    m_timer->Stop();
    wxTimer* timer = m_timer.release();
    delete timer;
    ReleaseTimer(m_timerNumber);
    wxEndBusyCursor();
}

void MultibrotPanel::OnJulia(wxCommandEvent& event)
{
    wxSize size = GetSize();
    std::complex<float> upperLeft = GetUpperLeft();
    std::complex<float> lowerRight = GetLowerRight();
    wxPoint rightDown = GetRightDown();
    float deltaXY = lowerRight.real() - upperLeft.real();
    float x = upperLeft.real() + deltaXY * rightDown.x / size.x;
    float y = upperLeft.imag() - deltaXY * rightDown.y / size.y;
    m_rightDownPoint = { x, y };
    
    // create and display a new MandelJuliaPanel for the display
    wxNotebook* nBook = dynamic_cast<wxNotebook*>(GetParent());
    if (nBook == nullptr) {
        throw std::logic_error("Could not retrieve the Notebook for the new MandelJuliaPanel.");
    }
    try {
        MandelJuliaPanel* mPanel = new MandelJuliaPanel(nBook, wxID_ANY, nullptr,
            size, GetPower(), m_rightDownPoint);
        nBook->AddPage(mPanel, L"Mandelbrot-Julia", true);
    }
    catch (std::exception& e) {
        wxMessageBox(e.what(), "Cannot create Julia Set");
    }
}
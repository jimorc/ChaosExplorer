#include "wx/wxprec.h"
#include "ChaosExplorerWindow.h"
#include "MultibrotPanel.h"
#include "Fractal2Panel.h"
#include "Fractal3Panel.h"
#include "Fractal4Panel.h"

const int ID_FILE_CLOSE_TAB = 2200;
const int ID_FRACTAL_MANDELBROT = 2201;
const int ID_FRACTAL2 = 2202;
const int ID_FRACTAL3 = 2203;
const int ID_FRACTAL4 = 2204;

ChaosExplorerWindow::ChaosExplorerWindow(wxWindow* parent, wxWindowID id, const wxString& title,
    const wxPoint& pos, const wxSize& size,
    long style, const wxString& name)
    : wxFrame(parent, id, title, pos, size, style, name)
{
    m_statusBar = new wxStatusBar(this);
    SetStatusBar(m_statusBar);
    CreateMainMenu();
    m_notebook = new wxNotebook(this, wxID_ANY, wxPoint(0, 0), wxDefaultSize, wxTop | wxNB_MULTILINE);
    MultibrotPanel* win = new MultibrotPanel(m_notebook, wxID_ANY, nullptr,
    { 800, 800 });
    m_notebook->AddPage(win, L"z^p + c", true);
    m_notebook->Fit();

    Fit();
    Centre();
}


ChaosExplorerWindow::~ChaosExplorerWindow()
{
}

void ChaosExplorerWindow::CreateMainMenu()
{
    m_mainMenuBar = new wxMenuBar;
    wxMenu* fileMenu = CreateFileMenu();
    m_mainMenuBar->Append(fileMenu, L"&File");
    wxMenu* fractalMenu = CreateFractalMenu();
    m_mainMenuBar->Append(fractalMenu, L"&Fractals");
    SetMenuBar(m_mainMenuBar);
}

wxMenu* ChaosExplorerWindow::CreateFileMenu()
{
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(ID_FILE_CLOSE_TAB, L"Close Tab");
    fileMenu->Append(wxID_EXIT, L"&Quit\tCtrl+Q");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &ChaosExplorerWindow::OnCloseTab,
        this, ID_FILE_CLOSE_TAB);
    Bind(wxEVT_COMMAND_MENU_SELECTED, [this](wxCommandEvent&) {Destroy(); },
        wxID_EXIT);

    Bind(wxEVT_MENU_OPEN, &ChaosExplorerWindow::OnFileMenuOpen, this);
    return fileMenu;
}

wxMenu* ChaosExplorerWindow::CreateFractalMenu()
{
    wxMenu* fractalMenu = new wxMenu;
    fractalMenu->Append(ID_FRACTAL_MANDELBROT, L"z^p + c");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &ChaosExplorerWindow::OnMandelbrot,
        this, ID_FRACTAL_MANDELBROT);
    fractalMenu->Append(ID_FRACTAL2, L"(z^3+1)/(1+cz^2)");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &ChaosExplorerWindow::OnFractal2,
        this, ID_FRACTAL2);
    fractalMenu->Append(ID_FRACTAL3, L"c*e^z");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &ChaosExplorerWindow::OnFractal3,
        this, ID_FRACTAL3);
    fractalMenu->Append(ID_FRACTAL4, L"z^3-z^2+z+c");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &ChaosExplorerWindow::OnFractal4,
        this, ID_FRACTAL4);

    return fractalMenu;
}

void ChaosExplorerWindow::OnCloseTab(wxCommandEvent& event)
{
    int pageNumber = m_notebook->GetSelection();
    int pageCount = m_notebook->GetPageCount();
    if (pageCount != 1) {
        // must change selected tab before deleting a tab/page
        // otherwise, the MultibrotPanels get screwed up and one display background only
        if (pageNumber != pageCount - 1) {
            m_notebook->ChangeSelection(pageNumber + 1);
        }
        else {
            m_notebook->ChangeSelection(pageNumber - 1);
        }
        m_notebook->DeletePage(pageNumber);
    }
}

void ChaosExplorerWindow::OnFileMenuOpen(wxMenuEvent& event)
{
    m_mainMenuBar->Enable(ID_FILE_CLOSE_TAB, m_notebook->GetPageCount() > 1);
}

void ChaosExplorerWindow::OnMandelbrot(wxCommandEvent& event)
{
    MultibrotPanel* win = new MultibrotPanel(m_notebook, wxID_ANY, nullptr,
    { 800, 800 });
    m_notebook->AddPage(win, L"z^p + c", true);
}

void ChaosExplorerWindow::OnFractal2(wxCommandEvent& event)
{
    try {
        Fractal2Panel* win = new Fractal2Panel(m_notebook, wxID_ANY, nullptr,
        { 800, 800 }, 2.0f, -10.0f + 10.0if, 10.0f - 10.0if);
        m_notebook->AddPage(win, L"(z^3+1)/(1+cz^2)");
        m_notebook->SetSelection(m_notebook->GetPageCount() - 1);
    }
    catch (std::exception& e) {
        wxMessageBox(e.what(), "Error creating Fractal2Panel", MB_OK, this);
    }
}

void ChaosExplorerWindow::OnFractal3(wxCommandEvent& event)
{
    try {
        Fractal3Panel* win = new Fractal3Panel(m_notebook, wxID_ANY, nullptr,
        { 800, 800 });
        m_notebook->AddPage(win, L"c*e^z");
        m_notebook->SetSelection(m_notebook->GetPageCount() - 1);
    }
    catch (std::exception& e) {
        wxMessageBox(e.what(), "Error creating Fractal3Panel", MB_OK, this);
    }
}

void ChaosExplorerWindow::OnFractal4(wxCommandEvent& event)
{
    try {
        Fractal4Panel* win = new Fractal4Panel(m_notebook, wxID_ANY, nullptr,
        { 800, 800 });
        m_notebook->AddPage(win, L"(z^3-z^2+z+c");
        m_notebook->SetSelection(m_notebook->GetPageCount() - 1);
    }
    catch (std::exception& e) {
        wxMessageBox(e.what(), "Error creating Fractal4Panel", MB_OK, this);
    }
}

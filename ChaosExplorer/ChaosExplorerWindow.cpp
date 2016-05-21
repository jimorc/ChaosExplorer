#include "wx/wxprec.h"
#include "ChaosExplorerWindow.h"
#include "MultibrotPanel.h"

const int ID_FILE_CLOSE_TAB = 2200;
const int ID_FILE_CLOSE = 2201;

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
    m_notebook->AddPage(win, L"Multibrot", true);
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

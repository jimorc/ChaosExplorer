#include "wx/wxprec.h"
#include "ChaosExplorerWindow.h"
#include "MultibrotPanel.h"



ChaosExplorerWindow::ChaosExplorerWindow(wxWindow* parent, wxWindowID id, const wxString& title,
    const wxPoint& pos, const wxSize& size,
    long style, const wxString& name)
    : wxFrame(parent, id, title, pos, size, style, name)
{
    m_statusBar = new wxStatusBar(this);
    SetStatusBar(m_statusBar);
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

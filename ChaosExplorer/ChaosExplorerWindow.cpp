#include "wx/wxprec.h"
#include "ChaosExplorerWindow.h"
#include "MultibrotPanel.h"



ChaosExplorerWindow::ChaosExplorerWindow(wxWindow* parent, wxWindowID id, const wxString& title,
    const wxPoint& pos, const wxSize& size,
    long style, const wxString& name)
    : wxFrame(parent, id, title, pos, size, style, name)
{
    m_notebook = new wxNotebook(this, wxID_ANY, wxPoint(0, 0), wxDefaultSize, wxTop | wxNB_MULTILINE);
    MultibrotPanel* win = new MultibrotPanel(m_notebook, wxID_ANY, { 800, 800 }, 2.0l, 362);
    m_notebook->AddPage(win, L"Mandelbrot-1", true);
    m_notebook->Fit();

    Fit();
    Centre();
}


ChaosExplorerWindow::~ChaosExplorerWindow()
{
}

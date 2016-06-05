#pragma once
#include "wx/notebook.h"

class ChaosExplorerWindow :
    public wxFrame
{
public:
    ChaosExplorerWindow(wxWindow* parent, wxWindowID id, const wxString& title,
        const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize,
        long style=wxDEFAULT_FRAME_STYLE, const wxString& name=wxFrameNameStr);
    ChaosExplorerWindow(const ChaosExplorerWindow&) = delete;
    ChaosExplorerWindow(ChaosExplorerWindow&&) = delete;
    virtual ~ChaosExplorerWindow();
    ChaosExplorerWindow& operator=(const ChaosExplorerWindow&) = delete;
    ChaosExplorerWindow& operator=(ChaosExplorerWindow&&) = delete;
    wxStatusBar* GetStatusBar() { return m_statusBar; }
    void OnCloseTab(wxCommandEvent& event);

private:
    void CreateMainMenu();
    wxMenu* CreateFileMenu();
    wxMenu* CreateFractalMenu();
    void OnFileMenuOpen(wxMenuEvent& event);
    void OnMandelbrot(wxCommandEvent& event);
    void OnFractal2(wxCommandEvent& event);
    wxNotebook* m_notebook;
    wxStatusBar* m_statusBar;
    wxMenuBar* m_mainMenuBar;
};


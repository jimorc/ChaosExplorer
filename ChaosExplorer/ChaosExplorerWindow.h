#pragma once
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
};


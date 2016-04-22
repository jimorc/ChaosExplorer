#include "wx/wxprec.h"
#include "ChaosExplorerApp.h"

#ifdef _UNICODE
#ifdef NDEBUG
#pragma comment(lib, "wxbase31u.lib")
#else
#pragma comment(lib, "wxbase31ud.lib")
#endif
#else
#ifndef NDEBUG
#pragma comment(lib, "wxbase31.lib")
#else
#pragma comment(lib, "wxbase31d.lib")
#endif
#endif

ChaosExplorerApp::ChaosExplorerApp()
{
}


ChaosExplorerApp::~ChaosExplorerApp()
{
}

bool ChaosExplorerApp::OnInit()
{
    wxFrame* mainFrame = new wxFrame(nullptr, wxID_ANY, L"ChaosExplorer");
    mainFrame->Show(true);
    return true;
}

wxIMPLEMENT_APP(ChaosExplorerApp);

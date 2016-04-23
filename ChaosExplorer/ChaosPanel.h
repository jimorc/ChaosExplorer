#pragma once
#include "wx/wxprec.h"
#include <memory>
#include "wx/glcanvas.h"

class ChaosPanel :
    public wxGLCanvas
{
public:
    ChaosPanel(wxWindow* parent, wxWindowID id, const int* attribList,
        const wxSize& size = { 800, 600 });
    ChaosPanel(const ChaosPanel&) = delete;
    ChaosPanel(ChaosPanel&&) = delete;
    virtual ~ChaosPanel();
    ChaosPanel& operator=(const ChaosPanel&) = delete;
    ChaosPanel& operator=(ChaosPanel&&) = delete;
    void InitializeGLEW();
    void SetContext() { SetCurrent(*m_context); }

private:
    virtual void OnPaint(wxPaintEvent& event) = 0;
    std::unique_ptr<wxGLContext> m_context;
};


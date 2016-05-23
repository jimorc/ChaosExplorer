#include "PlottingCPanel.h"



PlottingCPanel::PlottingCPanel(wxWindow* parent, wxWindowID id, const int* attribList,
    const wxSize& size,
    std::complex<float> power,
    std::complex<float> ul,
    std::complex<float> lr)
    : ChaosPanel(parent, id, attribList, ul, lr, size)
{
}


PlottingCPanel::~PlottingCPanel()
{
}

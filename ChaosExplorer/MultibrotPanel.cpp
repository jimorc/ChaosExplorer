#define _CRT_SECURE_NO_WARNINGS
#include "MultibrotPanel.h"


MultibrotPanel::MultibrotPanel(wxWindow* parent, wxWindowID id, const wxSize& size,
    std::complex<double> power,
    std::complex<double> ul,
    std::complex<double> lr)
    : wxPanel(parent, id, wxDefaultPosition, size)
{
}


MultibrotPanel::~MultibrotPanel()
{
}

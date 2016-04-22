#define _CRT_SECURE_NO_WARNINGS
#include <stdexcept>
#include "MultibrotPanel.h"


MultibrotPanel::MultibrotPanel(wxWindow* parent, wxWindowID id, const wxSize& size,
    std::complex<double> power,
    std::complex<double> ul,
    std::complex<double> lr)
    : wxPanel(parent, id, wxDefaultPosition, size),
    m_power(power)
{
    if (power.real() < 1.0) {
        throw std::invalid_argument(
            "The real portion of the power argument for Multibrot is less than 1.0");
    }
    m_upperLeft = std::complex<double>(std::min(ul.real(), lr.real()), std::max(ul.imag(), lr.imag()));
    m_lowerRight = std::complex<double>(std::max(ul.real(), lr.real()), std::min(ul.imag(), lr.imag()));
    if (ul.real() == lr.real()) {
        throw std::invalid_argument(
            "The real portions of the upper-left and lower-right corners of the Multibrot display are equal.");
    }
    if (ul.imag() == lr.imag()) {
        throw std::invalid_argument(
            "The imag portions of the upper-left and lower-right corners of the Multibrot display are equal.");
    }
}


MultibrotPanel::~MultibrotPanel()
{
}

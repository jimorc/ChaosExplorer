#pragma once
#include "wx/wxprec.h"
#include "ChaosPanel.h"

template <typename T>
class PlottingZ0Panel
    : public ChaosPanel<T>
{
public:
    PlottingZ0Panel<T>(wxWindow* parent, wxWindowID id, const int* attribList,
        const wxSize& size = { 800, 600 },
        std::complex<float> power = 2.0f,
        std::complex<float> c = 0.0f,
        std::complex<float> ul = -2.0f + 2.0if,
        std::complex<float> lr = 2.0f - 2.0if)
        : ChaosPanel<T>(parent, id, attribList, ul, lr, size),
        m_c(c), m_p(power)
    {}

   virtual ~PlottingZ0Panel() {}

protected:
    std::complex<float> GetC() { return m_c; }
    std::complex<float> GetPower() { return m_p; }

private:
    std::complex<float> m_c;
    std::complex<float> m_p;
};
#include "qam.h"
#include <cmath>
#include <stdint.h>

uint32_t grayRevToBinary(uint32_t x, uint32_t n) {
    uint32_t y = 0;
    for (int i=0; i<n; i++) {
        y <<= 1;
        y |= x&1;
        x >>= 1;
    }
    for (int shift=1; shift<n; shift<<=1)
        y ^= y >> shift;
    return y;
}

QAM::QAM(int Nbpsc) : Constellation(Nbpsc) {
    float scale = 1.f;
    int n = 1;
    if (Nbpsc != 1) {
        // want variance = .5 per channel
        // in fact, var{ 2 range(2^{Nbpsc/2}) - const } = (2^Nbpsc - 1) / 3
        scale = sqrt( 1.5f / ((1<<Nbpsc) - 1) );
        n = Nbpsc >> 1;
    }
    std::vector<float> syms(1<<n);
    for (int i=0; i<1<<n; i++)
        syms[i] = (2*(int)grayRevToBinary(i, n) + 1 - (1<<n)) * scale;
    if (Nbpsc != 1) {
        int k=0;
        for (int i=0; i<1<<n; i++)
            for (int j=0; j<1<<n; j++)
                symbols[k++] = complex(syms[j], syms[i]);
    } else {
        for (int i=0; i<1<<n; i++)
            symbols[i] = syms[i];
    }
}
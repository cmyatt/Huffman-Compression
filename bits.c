#include "bits.h"

uchar set_bit(uchar *x, uchar pos) {
    uchar mask = 1 << pos;
    *x |= mask;
    return *x;
}


uchar clear_bit(uchar *x, uchar pos) {
    uchar mask = 1 << pos;
    *x &= ~mask;
    return *x;
}


uchar modify_bit(uchar x, uchar pos, uchar n) {
    uchar mask = n << pos;
    return (x & ~mask) | (-n & mask);
}


uchar flib_bit(uchar x, uchar pos) {
    uchar mask = 1 << pos;
    return x ^ mask;
}


uchar is_bit_set(uchar x, uchar pos) {
    x >>= pos;
    return (x & 1) != 0;
}

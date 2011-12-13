#ifndef BITS_H
#define BITS_H

typedef unsigned char uchar;

uchar set_bit(uchar *x, uchar pos);
uchar clear_bit(uchar *x, uchar pos);
uchar modify_bit(uchar x, uchar pos, uchar n);
uchar flib_bit(uchar x, uchar pos);
uchar is_bit_set(uchar x, uchar pos);


uchar set_bit(uchar *x, uchar pos) {
    uchar mask = 1 << pos;
    *x |= mask;
}


uchar clear_bit(uchar *x, uchar pos) {
    uchar mask = 1 << pos;
    *x &= ~mask;
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

#endif

#ifndef BITS_H
#define BITS_H

/* Utility functions for accessing
 * specific bits within a byte.
*/

typedef unsigned char uchar;

uchar set_bit(uchar *x, uchar pos);
uchar clear_bit(uchar *x, uchar pos);
uchar modify_bit(uchar x, uchar pos, uchar n);
uchar flib_bit(uchar x, uchar pos);
uchar is_bit_set(uchar x, uchar pos);

#endif

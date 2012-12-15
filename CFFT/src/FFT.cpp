#include <cstdio>
#include "FFT.h"

/*
 * Decimation in time radix-2 FFT with in order output
 *
 * Greg Larmore
 */

void FFT(complex<Q1_15> *data, uint8_t inv) {
  uint16_t i, j, k;
  uint16_t n, m;
  complex<Q1_15> temp;
  //Bit reversal
  for (i = 0; i < LENGTH; i++) {
    j = BRT[i];
    if (j < i) {
      temp = data[i];
      data[i] = data[j];
      data[j] = temp;
    }
  }
  //Radix-2 butterflies
  for (i = 0; i < LENGTH; i += 2) {
    uint16_t a = i;
    uint16_t b = i + 1;
    temp = data[a] - data[b];
    data[a] += data[b];
    data[b] = temp;
  }
  //Larger butterflies
  if (inv) {
    for (n = 2, m = HALF_LENGTH; n < LENGTH; n <<= 1, m >>= 1) {
      for (i = 0; i < LENGTH; i += n << 1) {
        for (j = 0, k = 0; j < n; j++, k += m) {
          uint16_t a = i + j;
          uint16_t b = a + n;
          complex<Q1_15> twidd;
          //Twiddle factor
          switch (k) {
            case 0: //This twiddle is 1 + j0
              twidd = data[b];
              break;
            case HALF_LENGTH: //This twiddle is 0 + j1
              twidd.real(-data[b].imag());
              twidd.imag(data[b].real());
              break;
            default:
              twidd.real(INV_TWIDD[k]);
              twidd.imag(INV_TWIDD[k+1]);
              twidd *= data[b];
              break;
          }
          data[b] = data[a] - twidd;
          data[a] += twidd;
        }
      }
    }
    //Scale by 1/LENGTH
    i = HALF_LENGTH;
    n = 0;
    while (i != 0) {
      i >>= 1;
      n++;
    }
    for (i = 0; i < LENGTH; i++) {
      data[i].real(Q1_15(data[i].real().full()>>n));
      data[i].imag(Q1_15(data[i].imag().full()>>n));
    }
  } else {
    for (n = 2, m = HALF_LENGTH; n < LENGTH; n <<= 1, m >>= 1) {
      for (i = 0; i < LENGTH; i += n << 1) {
        for (j = 0, k = 0; j < n; j++, k += m) {
          uint16_t a = i + j;
          uint16_t b = a + n;
          complex<Q1_15> twidd;
          //Twiddle factor
          switch (k) {
            case 0: //This twiddle is 1 + j0
              twidd = data[b];
              break;
            case HALF_LENGTH: //This twiddle is 0 - j1
              twidd.real(data[b].imag());
              twidd.imag(-data[b].real());
              break;
            default:
              twidd.real(TWIDD[k]);
              twidd.imag(TWIDD[k+1]);
              twidd *= data[b];
              break;
          }
          data[b] = data[a] - twidd;
          data[a] += twidd;
        }
      }
    }
  }
}

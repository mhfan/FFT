#include <cstdio>
#include "FFT.h"

// Copyright (c) 2012, Greg Larmore
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met: 
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer. 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution. 
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// The views and conclusions contained in the software and documentation are those
// of the authors and should not be interpreted as representing official policies, 
// either expressed or implied, of the FreeBSD Project.


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

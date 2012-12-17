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
 * FFT Test
 *
 * Greg Larmore
 */

void print(const complex<Q1_15> data[], int length) {
  int i;
  for (i = 0; i < length; i++) {
    printf("%2.2f + j%2.2f\n", data[i].real().toFloat(), data[i].imag().toFloat());
  }
  printf("\n");
}

int main(void) {
  uint16_t i, count;
  complex<Q1_15> data[LENGTH];
  complex<Q1_15> data_copy[LENGTH];

  //Load data into input vector
  for (i = 0; i < LENGTH; i++) {
    data[i] = (i % 2) ? complex<Q1_15>(CONST_Q1_15(0,0.05), CONST_Q1_15(0,0.01)) : complex<Q1_15>(CONST_Q1_15(0,0.05), -CONST_Q1_15(0,0.01));
  }

  //Copy data for comparison
  for (i = 0; i < LENGTH; i++) {
    data_copy[i] = data[i];
  }

  printf("Welcome\n\n");

  printf("Input\n");
  print(data, LENGTH);
  FFT(data, 0);
  printf("FFT\n");
  print(data, LENGTH);
  FFT(data, 1);
  printf("IFFT (reproduced original)\n");
  print(data, LENGTH);

  //Check for mismatches between the original and the recreated
  count = 0;
  for (i = 0; i < LENGTH; i++) {
    if (data[i] != data_copy[i]) {
      count++;
    }
  }

  if (count != 0) {
    if (count == 1) {
      printf("1 Mismatch\n");
    } else {
      printf("%d Mismatch(es)\n", count);
    }

    return 1;
  }

  return 0;
}

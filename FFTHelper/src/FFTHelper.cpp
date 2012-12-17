#include <cstdio>
#include <complex>
#include "Fixed.h"

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
 * Generates bit reversal table, twiddle factors, and inverse twiddle factors for Q1.15 fixed point radix-2 FFT
 *
 * Greg Larmore
 */

#define LENGTH 256
#define QUARTER_LENGTH LENGTH/4
#define HALF_LENGTH LENGTH/2

using namespace std;

uint16_t BitReverse(uint16_t i) {
  uint16_t mask = HALF_LENGTH;
  uint16_t temp = 0;
  while (mask != 0) {
    temp >>= 1;
    if (i & mask) {
      temp |= HALF_LENGTH;
    }
    mask >>= 1;
  }
  if (!(i % 32) && i != 0) printf("\n");
  printf("%d", temp);
  if (i != LENGTH - 1) printf(", ");
  return temp;
}

complex<Q1_15> Twiddle(uint16_t i) {
  float real = cos(-2.0f*M_PI*i/LENGTH);
  float imag = sin(-2.0f*M_PI*i/LENGTH);
  complex<Q1_15> temp(CONST_Q1_15((int16_t)real, real - (int16_t)real),
            CONST_Q1_15((int16_t)imag, imag - (int16_t)imag));
  if (!(i % 8)) printf("\n");
  printf("0x%04X, 0x%04X", (uint16_t)temp.real().full(), (uint16_t)temp.imag().full());
  if (i != HALF_LENGTH - 1) printf(", ");
  return temp;
}

complex<Q1_15> Inv_Twiddle(uint16_t i) {
  float real = cos(2.0f*M_PI*i/LENGTH);
  float imag = sin(2.0f*M_PI*i/LENGTH);
  complex<Q1_15> temp(CONST_Q1_15((int16_t)real, real - (int16_t)real),
            CONST_Q1_15((int16_t)imag, imag - (int16_t)imag));
  if (!(i % 8)) printf("\n");
  printf("0x%04X, 0x%04X", (uint16_t)temp.real().full(), (uint16_t)temp.imag().full());
  if (i != HALF_LENGTH - 1) printf(", ");
  return temp;
}

int main(void) {
  uint16_t i;
  printf("#if (LENGTH == %d)\n", LENGTH);
  printf("const uint16_t BRT[] = {");
  for (i = 0; i < LENGTH; i++) {
    BitReverse(i);
  }
  printf("};\n");
  printf("const uint16_t TWIDD[] = {");
  for (i = 0; i < HALF_LENGTH; i++) {
    switch (i) {
    case 0:
      printf("0x7FFF, 0x0000, ");
      break;
    case QUARTER_LENGTH:
      if (!(i % 8)) printf("\n");
      printf("0x0000, 0x8000");
      if (i != HALF_LENGTH - 1) printf(", ");
      break;
    default:
      Twiddle(i);
      break;
    }
  }
  printf("};\n");
  printf("const uint16_t INV_TWIDD[] = {");
  for (i = 0; i < HALF_LENGTH; i++) {
    switch (i) {
    case 0:
      printf("0x7FFF, 0x0000, ");
      break;
    case QUARTER_LENGTH:
      if (!(i % 8)) printf("\n");
      printf("0x0000, 0x7FFF");
      if (i != HALF_LENGTH - 1) printf(", ");
      break;
    default:
      Inv_Twiddle(i);
      break;
    }
  }
  printf("};\n");
  printf("#endif");
  return 0;
}

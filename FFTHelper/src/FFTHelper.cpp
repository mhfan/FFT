/*
 * Generates bit reversal table, twiddle factors, and inverse twiddle factors for Q1.15 fixed point radix-2 FFT
 *
 * Greg Larmore
 */

#include <cstdio>
#include <complex>
#include "Fixed.h"

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

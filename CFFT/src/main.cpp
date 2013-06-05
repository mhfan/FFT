#include <cstdio>
#include "FFT.h"

/**
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

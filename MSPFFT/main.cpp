#include <msp430.h>
//#include <stdio.h>
#include "FFT.h"
//#include "UART.h"

/*
 * FFT Test
 *
 * Greg Larmore
 */

int _system_pre_init(void)
{
  /* Insert your low-level initializations here */
  WDTCTL = WDTPW + WDTHOLD; // Stop Watchdog timer
  /*==================================*/
  /* Choose if segment initialization */
  /* should be done or not. */
  /* Return: 0 to omit initialization */
  /* 1 to run initialization */
  /*==================================*/
  return (1);
}

void print(const complex<Q1_15> data[], int length) {
  int i;
  for (i = 0; i < length; i++) {
    printf("%d + j%d\r\n", data[i].real().full(), data[i].imag().full());
  }
  printf("\r\n");
}

int main(void) {
  uint16_t i, count;
  complex<Q1_15> data[LENGTH];
  complex<Q1_15> data_copy[LENGTH];

  MPY32CTL0 = MPYFRAC;  //Put hardware multiplier in Q1.15 fractional mode
  //initUART();

  //Load data into input vector
  for (i = 0; i < LENGTH; i++) {
    data[i] = (i % 2) ? complex<Q1_15>(CONST_Q1_15(0,0.05), CONST_Q1_15(0,0.01)) : complex<Q1_15>(CONST_Q1_15(0,0.05), -CONST_Q1_15(0,0.01));
  }

  //Copy data for comparison
  for (i = 0; i < LENGTH; i++) {
    data_copy[i] = data[i];
  }

  //TODO: Fix print statements (memory problem)
  //printf("Welcome\r\n");

  //print(data, LENGTH);
  __no_operation();
  FFT(data, 0);
  //print(data, LENGTH);
  __no_operation();
  FFT(data, 1);
  //print(data, LENGTH);

  __no_operation();

  //Check for mismatches between the original and the recreated
  count = 0;
  for (i = 0; i < LENGTH; i++) {
    if (data[i] != data_copy[i]) {
      count++;
    }
  }

  if (count != 0) {
    __no_operation();
    return 1;
  }

  _bis_SR_register(GIE + LPM3_bits);

  return 0;
}

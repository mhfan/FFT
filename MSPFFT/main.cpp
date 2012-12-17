#include <msp430.h>
//#include <stdio.h>
#include "FFT.h"
//#include "UART.h"

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

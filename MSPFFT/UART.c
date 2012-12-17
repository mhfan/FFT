#include "UART.h"

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
 * UART interface for MSP430FR5739
 *
 * Greg Larmore
 */

void initUART() {
  PJSEL0 |= BIT4 + BIT5;

  CSCTL0_H = 0xA5;
  CSCTL1 |= DCOFSEL0 + DCOFSEL1;
  CSCTL2 = SELA_0 + SELS_3 + SELM_3;
  CSCTL3 = DIVA_0 + DIVS_3 + DIVM_3;
  CSCTL4 |= XT1DRIVE_0;
  CSCTL4 &= ~XT1OFF;

  do
  {
    CSCTL5 &= ~XT1OFFG;
    SFRIFG1 &= ~OFIFG;
  } while (SFRIFG1 & OFIFG);


  P2SEL0 &= ~(BIT5 + BIT6);
  P2SEL1 |= BIT5 + BIT6;

  UCA1CTL1 |= UCSWRST;
  UCA1CTL1 = UCSSEL_1;
  UCA1BR0 = 3;
  UCA1MCTLW |= 0x5300;
  UCA1BR1 = 0;
  UCA1CTL1 &= ~UCSWRST;
}

int fputc(int _c, register FILE *_fp)
{
  while (!(UCA1IFG & UCTXIFG));
  UCA1TXBUF = (unsigned char)_c;

  return (unsigned char)_c;
}

int fputs(const char *_ptr, register FILE *_fp)
{
  unsigned int i, len;

  len = strlen(_ptr);

  for(i = 0; i < len; i++)
  {
    while (!(UCA1IFG & UCTXIFG));
    UCA1TXBUF = (unsigned char)_ptr[i];
  }

  return len;
}

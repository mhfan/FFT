#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include "UART.h"

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

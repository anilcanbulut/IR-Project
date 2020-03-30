#include <msp430.h>



void main(void)
{

    /*** Watchdog timer and clock Set-Up ***/
    WDTCTL = WDTPW + WDTHOLD;     // Stop watchdog timer
    DCOCTL = 0;                   // Select lowest DCOx and MODx
    BCSCTL1 = CALBC1_16MHZ;       // Set range
    DCOCTL = CALDCO_16MHZ;        // Set DCO step + modulation

    /*** GPIO Set-Up ***/
    P1DIR |= BIT2;                  // P1.0 set as output
    P1SEL |= BIT2;                  // P1.0 selected Timer0_A Out1


    for (;;)            //send all 4 bytes or 32 bits
      {
        IRsendLG(0x88C0051);
        delay_10u(1000000);

      }


}

void IRsendLG ( unsigned long data )
{

    unsigned long mask = 0x8000000;

    IRcarrier(860);            //8.6ms of carrier
    delay_10u(410);            //4.1ms of silence
    IRcarrier(60);              //600us of carrier


  int i;
  for (i=0; i<28; i++)
    {
    if ( ( data & mask) == mask)       {
        delay_10u(160);         //1600us of silence
    }
    else{
        delay_10u(50);          //550us of silence
    }
    IRcarrier(60);              //600us of carrier turn on the carrier for one bit time
    mask >>= 1;
    }

  P1OUT = 0x00;
  TA0CTL &= TASSEL_2 + MC_0 + ID_0 + TACLR;
}

void IRsendNEC ( unsigned long data )
{

    unsigned long mask = 0x80000000;

    IRcarrier(900);            //9 ms of carrier
    delay_10u(440);            //4.5 ms of silence

  int i;
  for (i=0; i<32; i++)
    {
      IRcarrier(56);              //560us of carrier time turn on the carrier for one bit time
    if ( ( data & mask) == mask)       {
        delay_10u(165);         //1690us of silence
    }
    else{
        delay_10u(50);          //560us of silence
    }

    mask >>= 1;
    }
  IRcarrier(56);              //560us of carrier 1 time
  P1OUT = 0x00;
  TA0CTL &= TASSEL_2 + MC_0 + ID_0 + TACLR;
}


void IRcarrier(unsigned int delay)
{

    /*** Timer0_A Set-Up ***/
    TA0CCR0 |= 412-1;             // PWM Period
    TA0CCTL1 |= OUTMOD_7;           // TA0CCR1 output mode = reset/set
    TA0CCR1 |= 97;                 // TA0CCR1 PWM duty cycle
    TA0CTL |= TASSEL_2 + MC_1 + ID_0 + TACLR;      // SMCLK, Up Mode (Counts to TA0CCR0)
    delay_10u(delay);
    TA0CTL &= TASSEL_2 + MC_0 + ID_0 + TACLR;
}

void delay_10u(unsigned int delay10u){
    int i;
      for(i=0; i < delay10u; i++)
        {
          __delay_cycles(10*16);
        }
}

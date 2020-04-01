#include "_ffmc16.h"
#include "extern.h"
#include "monitor.h"


#define ON      1
#define OFF     0


#define INPUT   0
#define OUTPUT  1





void delay(int milliseconds);



int next_state_counter = OFF;
int counter            = 0;
int FirstSensor = 0;
int SecondSensor = 0;
int ThirdSensor = 0;

void delay(int milliseconds)
{
    int t;
    for (t=0;t<milliseconds;t++)
    {
        
    }
}


void main(void)
{
    __set_il(7);
    __EI();    /* enable interrupt(for Accemic) */

    
    IO_ELVR.word       = 0x1500;
    IO_DDR1.bit.D10    = OUTPUT; // VCC PIR1
    IO_DDR1.bit.D11    = OUTPUT; // VCC PIR2
    IO_DDR1.bit.D12    = OUTPUT; // VCC PIR3
    IO_PDR1.byte       = 0x1F;
    IO_PDR2.bit.P21    = ON; //LED1
    IO_PDR2.bit.P22    = ON; //LED2
    IO_PDR2.bit.P23    = ON; //LED3
    //IO_DDR1.bit.D13    = OUTPUT;
    
    IO_DDR2.bit.D21    = OUTPUT; //LED1
    IO_DDR2.bit.D22    = OUTPUT; //LED2
    IO_DDR2.bit.D23    = OUTPUT; //LED3
    
    IO_DDR2.bit.D24    = INPUT; // PIR1
    IO_DDR2.bit.D25    = INPUT; // PIR2
    IO_DDR2.bit.D26    = INPUT; // PIR3

/* external interrupt P24 and P25 start */
    IO_ICR06.byte = 0x00;
    //added IO_ICR08.byte = 0x01; 
    IO_ICR08.byte = 0x01; 
    IO_ENIR.bit.EN4 = 0;
    IO_EIRR.bit.ER4 = 0;
    IO_ENIR.bit.EN4 = 1;

    IO_ENIR.bit.EN5 = 0;
    IO_EIRR.bit.ER5 = 0;
    IO_ENIR.bit.EN5 = 1;
    /* external interrupt P24 and P25 end */

    /* external interrupt P26 start */
    IO_ICR06.byte = 0x00;
    IO_ICR08.byte = 0x01;
    IO_ENIR.bit.EN6 = 0;
    IO_EIRR.bit.ER6 = 0;
    IO_ENIR.bit.EN6 = 1;
    /* external interrupt P26 end */

    //Initial OFF LED
    IO_PDR2.bit.P21 = OFF;
    IO_PDR2.bit.P22 = OFF;
    IO_PDR2.bit.P23 = OFF;

 
while(1)
{   

    if(FirstSensor == 1)
    {
        IO_PDR2.bit.P21 = ON;
        delay(32000);

        IO_PDR2.bit.P21 = OFF;
    }
    else if (SecondSensor == 1)
    {
        IO_PDR2.bit.P22 = ON;
        delay(32000);
    
        IO_PDR2.bit.P22 = OFF;
    }
    else if(ThirdSensor ==1 )
    {
        IO_PDR2.bit.P23 = ON;
        delay(32000);

        IO_PDR2.bit.P23 = OFF;
    }
    else {
        IO_PDR2.bit.P21 = OFF;
        IO_PDR2.bit.P22 = OFF;
        IO_PDR2.bit.P23 = OFF;
    }



} 
}


/*****************************************************************************
interrupt routine
******************************************************************************/

/* ADC interrupt routine */

__interrupt void _firstSecondSensor(void)
{
    if(IO_EIRR.bit.ER4 == 1)
    {
        IO_EIRR.bit.ER4 = 0;
        
        // Changed "FirstSensor to SecondSensor so that led 2 will interact with led3
        SecondSensor = ON; //LED1 ON
        
        
        
    }
    else if (IO_EIRR.bit.ER5 == 1)
    {
        IO_EIRR.bit.ER5 = 0;
        SecondSensor    = OFF; //LED2 ON
        
    }else{
        delay(10);
    }
    
    
    
}
__interrupt void _thirdSensor(void)
{
    if(IO_EIRR.bit.ER6 == 1)
    {
        IO_EIRR.bit.ER6 = 0;
        ThirdSensor    = ON; //LED3 ON
        
    }
    else{
        delay(10);
    }
    
}


/* Vector Table */
#pragma section INTVECT,locate=0xfffc00
#pragma intvect _start    0x8  0x0    /* Reset Vector */

#pragma intvect _firstSecondSensor 24
#pragma intvect _thirdSensor 27


//Brandon Noble April 2020

#include <hidef.h>      //libraries required to use Esduino and use serial communication
#include "derivative.h" 
#include "SCI.h"

char string[20];
unsigned short val;     //used to store the values to be serially communicated
int matVal;
short comm = 0;         //used for serial communication, default off
short mode = 0;         //default mode will be to read X values
int i = 0;              //loop iterator

//Set bus clock to 8MHz as outlined in project specifications
void setClk(void){
  CPMUPROT = 0x26;      //Protection of clock configuration is disabled
  CPMUCLKS = 0x80;      //PLLSEL=1. Select Bus Clock Source:  PLL clock or Oscillator.
  CPMUOSC = 0x80;       //OSCE=1. Select Clock Reference for PLLclk as:fOSC (8 MHz).
  CPMUREFDIV = 0x41;    //Set fREF divider and selects reference clock frequency .  
  CPMUSYNR= 0x05;       //Set Syn divide and selects VCO frequency 
  CPMUPOSTDIV=0x02;     //Set Post Divider 
                          
  CPMUPROT = 1;         //Protection for clock configuration is reenabled 
}
     

void msDelay(unsigned int);

void OutCRLF(void){     //New line for serial communication
  SCI_OutChar(CR);
  SCI_OutChar(LF);
}

void main(void) {
  setClk();
	ATDCTL1 = 0x4F;		    //set for 12-bit resolution
	ATDCTL3 = 0x88;		
	ATDCTL4 = 0x02;		    //prescaler = 2; ADC clock = 8MHz / (2 * (2 + 1)) == 1.33MHz
	ATDCTL5 = 0x36;		    //continuous conversion on channel 06
	
	DDR1AD = 0x0F;        //enable PAD0-PAD3 as output for LEDs
	DDR0AD = 0x0F;        //enable PAD8-11 as output for LEDs
	
	//Enable interrupts to control communication, code from lab 4
	TSCR1 = 0x90;    
  TSCR2 = 0x04;    
  TIOS = 0xFC;          //Enable input for TIC[0] and TIC[1]
  PERT = 0x03;          //Pull up resistors
  TCTL3 = 0x00;    
  TCTL4 = 0x05;         //Configure rising edges for both pins             
  TIE = 0x03;           //Timer interrupt enable
	EnableInterrupts;     
	
  SCI_Init(38400);      //Configure the Baud rate to be the highest possible for this clock speed, provided it has a <6% error
    
  
  for(;;) {
    while(comm == 1) {   //only run while communication is enabled
      val = 0;      
      for(i=0; i<20; i++) {       //averages 20 samples over 0.2s to clean up signal
        val += ATDDR0;
        msDelay(10);     //slight delay between values
      }//end for
      
      val/= 20; 
      SCI_OutUDec(val);
      OutCRLF();
      if(SCI_InStatus()){ //if data is available from MATLAB, read it, otherwise continue
        matVal = (int)SCI_InChar(); //read in the character and convert it to an integer
        //display the BCD by writing the above values to the registers           
        PT1AD = matVal / 10;       //grabs the MSD; for example 9 from the number 95
        PT0AD = matVal % 10;       //grabs the LSD; for example 5 from the number 95
      }//end if
    }//end while 
  }//end for
  
}//end main



void msDelay(unsigned int time) //delay function
{
	unsigned int j,k;
	for(j=0;j<time;j++)
		for(k=0;k<1334;k++);        //calcualted to be 1ms
}



//Used to toggle the serial communication on and off
interrupt  VectorNumber_Vtimch0 void ISR_Vtimch0(void) {
  unsigned int temp;
  //OutCRLF();
  if(comm == 0) {     //enable communication if currently disabled
    SCI_OutString("Start communication");
    OutCRLF();
    comm = 1;
  } 
  
  else {              //disable communication if currently enabled
    SCI_OutString("End communication");
    OutCRLF();
    comm = 0;
  }
  msDelay(200);       //small debounce to prevent accidental presses
  temp = TC0;
}

//Used to toggle the axis being read to either X or Y
interrupt  VectorNumber_Vtimch1 void ISR_Vtimch1(void) {
  unsigned int temp;
  if(mode == 0) {
    mode = 1;
    ATDCTL5 = 0x35;   //change to read AN5, allowing the Y values to be read
    SCI_OutString("Y reading started");
    OutCRLF();
  } 
  else {
   mode = 0;
   ATDCTL5 = 0x36;    //change to read AN6, allowing the X values to be read
   SCI_OutString("X reading started");  
  }
  msDelay(200);       //small debounce to prevent accidental presses
  temp = TC1;
}
  

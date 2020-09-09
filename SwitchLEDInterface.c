// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to toggle an LED
// while a button is pressed and turn the LED on when the
// button is released.  This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.
// January 15, 2016
//      Jon Valvano and Ramesh Yerraballi

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// following refer to spec OR library tm4c123gh6pm.h
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))  //Base address of port E
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))  // PE0, PE1
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))  // offset 0x420
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))		// Enable PE0, PE1
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))		// offset 0x528
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
// select port E
#define SYSCTL_RCGC2_GPIOE      0x00000010  // port F Clock Gating Control

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

// ***** 3. Subroutines Section *****

// PE0, PB0, or PA2 connected to positive logic momentary switch using 10k ohm pull down resistor
// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic
// shown in Lab8_artist.sch (PCB Artist schematic file) or 
// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).

//place the delay function outside mains
void Delay100ms(unsigned long time){
		unsigned long i;
		while(time > 0){
			i = 1333333;  // this number means 100ms
			while(i > 0){
			i = i - 1;
			}
    time = time - 1; // decrements every 100 ms
		}
}
int main(void){ unsigned long volatile delay;
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1, ScopeOn);  // activate grader and set system clock to 80 MHz
	//initialization
	//SYSCTL_RCGC2_R |= 0x00000020;	// activate clock in port F
	SYSCTL_RCGC2_R |= 0x00000010;	// activate clock in port E
	delay = SYSCTL_RCGC2_R; 	// delay for stablizing the clock
	GPIO_PORTE_AMSEL_R = 0x00; 	 // off analog mode
	GPIO_PORTE_PCTL_R = 0x00000000; 	//
	GPIO_PORTE_DIR_R = 0x02;	//PE0=input=0, PE1=output=1 00000010
	GPIO_PORTE_AFSEL_R = 0x00;
	GPIO_PORTE_DEN_R = 0x03; //enable PE0 and PE1 00000011
	
	//set the initial state
	// GPIO_PORTF_DATA_R = 0x14; //when SW1 is OFF, blue LED is ON (normal mode) SW1=OFF=1, LED ON=1 00010100
	GPIO_PORTE_DATA_R = 0x02; //when SW1 is OFF (Positive Logic, PE0=off=0, LED is ON PE1=1) 00000010
  	
  EnableInterrupts();           // enable interrupts for the grader
	
  while(1){
		Delay100ms(1);
		//GPIO_PORTE_DATA_R; 
		if ((GPIO_PORTE_DATA_R&0x01)==0x01){  //if SW is pressed (PE0=1=pressed) 00000000
			GPIO_PORTE_DATA_R = GPIO_PORTE_DATA_R^0x02;  //00000010
			GPIO_PORTE_DATA_R;
		}
		else{
			GPIO_PORTE_DATA_R = 0x02; //not press, remain ON
		}		
	}
    
}
  

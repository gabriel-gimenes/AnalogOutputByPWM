/*
 * File:   main.c
 * Autor: Gabriel Gimenes
 *
 * Data: 22/12/2021
 * Generating analog output by PWM pin
 * PWM information:
 * -> Period = (T2PR+1)*4*Tosc*TMR2PrescaleValue = (79+1)*4*500E-09*1 = 160us
 * -> PWM Frequency = (160us)^-1 = 6.25kHz
 */


#include <xc.h>
#pragma config WDTE = OFF                               /*Watchdog timer off*/
#pragma config LVP = ON                                 /*Low-Voltage Programming On*/
#define _XTAL_FREQ 8000000                             /*Internal Oscillator Clock = 8MHz*/
/*Functions prototype*/
static void PWM3_Initialize(void);
static void PWM3_LoadPercentDutyValue(uint16_t dutyValue);
static void TMR2_Initialize(void);
static void PORT_Initialize(void);
static void Initialize(void);
// <editor-fold defaultstate="collapsed" desc="main file">
void main(void) {
    Initialize();
    PWM3_LoadPercentDutyValue(75);
    
    while(1){
    }
    
    return;
}
// </editor-fold>

/*Initialization PWM3 routine (10 bits resolution)*/
static void PWM3_Initialize(void)
{
    PWM3CON = 0x80;                                     /* Enabling PWM Module and keeping non invert output*/
    PWM3DCH = 0x27;                                     /* Duty Cycle Register 8x MSB initialization*/
    PWM3DCL = 0xC0;                                     /* Duty Cycle Register 2x LSB initialization*/
}
/*Duty Cyle config routine with argument in percentual*/
static void PWM3_LoadPercentDutyValue(uint16_t dutyValue)
{
    dutyValue = dutyValue*320/100;                      /*Calc to convert percent duty cyle in duty value to PWM3 registers*/
    PWM3DCH = (dutyValue & 0x03FC)>>2;                  /*8 MSBs of PWM duty cycle*/
    PWM3DCL = (dutyValue & 0x0003)<<6;                  /*2 LSBs of PWM duty cycle*/
}
/*Timer2 initialization: clock soure to PWM*/
//TODO change the T2PR to 0x3F
static void TMR2_Initialize(void)
{
    T2CLKCON = 0x01;                                    /*Clock source: Fosc/4 = 2MHz -> T = 500ns*/
    T2PR = 0x4F;                                        /*Rollover every 39.5 us -> 79 decimal*/
    T2TMR = 0x00;                                       /*Timer2 Counter*/
    PIR1bits.TMR2IF = 0;                                /*Clear IF flag*/
    T2CONbits.ON = 1;                                   /*ON bit enables timer*/
}
/*PORT initialization using Peripheral Pin Selection*/
static void PORT_Initialize(void)
{
    TRISCbits.TRISC5 = 0x00;                            /*Configuring RC5 as an output*/
    RC5PPS = 0x03;                                      /*Configuring RC5 as PWM3 output*/
}

static void Initialize(void){
    PORT_Initialize();
    PWM3_Initialize();
    TMR2_Initialize();
}

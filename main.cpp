//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

//=====[Declaration and initialization of public global objects]===============

DigitalIn driverSeat(D2);
DigitalIn passengerSeat(D3);
DigitalIn driverBelt(D4);
DigitalIn passengerBelt(D5);
DigitalIn ignitionButton(D6);

DigitalOut ignitionLED(LED1);
DigitalOut engineLED(LED2);

DigitalInOut sirenPin(PE_10);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration and initialization of public global variables]=============

bool driverState = OFF;
bool engineState = OFF; 
bool alarmON = OFF;
bool endPrint = OFF; 

//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInit();
void uartTask();
void ledActivation();

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    inputsInit();
    outputsInit();
    while (true) {
        uartTask();
        ledActivation();
    }
}

//=====[Implementations of public functions]===================================

void inputsInit()
{
    driverSeat.mode(PullDown);
    passengerSeat.mode(PullDown);
    driverBelt.mode(PullDown);
    passengerBelt.mode(PullDown);
    ignitionButton.mode(PullDown);
    sirenPin.mode(OpenDrain);
    sirenPin.input();
}

void outputsInit()
{
    ignitionLED = OFF;
    engineLED = OFF;
}

void ledActivation() {
    if ( driverSeat && passengerSeat && driverBelt && passengerBelt ) {
        ignitionLED = ON;
    }

    if (ignitionLED && ignitionButton) {
        engineLED = ON;
        engineState = ON;
        ignitionLED = OFF;
    }

    if (ignitionLED == OFF && engineLED == OFF && ignitionButton && alarmON == OFF) {
        sirenPin.output();
        sirenPin = LOW;
        alarmON = ON;
    }

}

void uartTask(){
    if ( driverState == OFF && driverSeat ) {
        driverState = ON; 
        uartUsb.write( "Welcome to enhanced alarm system model 218-W24\r\n", 48 );
    }	

    if (ignitionLED == OFF && engineState ){
        engineState = OFF; 
        uartUsb.write( "Engine started\r\n", 16 );
    }

    if (alarmON == ON && ignitionButton == ON && !endPrint){
        alarmON == OFF;
        uartUsb.write("Ignition inhibited\r\n", 20);

        if (driverSeat == OFF) {
            uartUsb.write("Driver seat is not occupied\r\n", 30);
        }

        if (passengerSeat == OFF) {
            uartUsb.write("Passenger seat is not occupied\r\n", 32);
        }

        if (driverBelt == OFF) {
            uartUsb.write("Driver belt is not fastened\r\n", 30);
        }

        if (passengerBelt == OFF) {
            uartUsb.write("Passenger belt is not fastened\r\n", 32);
        }
        endPrint = true;
    }

}


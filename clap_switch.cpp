#include "mbed.h"
#include "clap_switch.h"

ClapSwitch::ClapSwitch(PinName input_pin){
    clap_input = new InterruptIn(input_pin);
    // attach interrupt to input pin
    clap_input -> rise(callback(this, &ClapSwitch::receive_clap));
}

ClapSwitch::~ClapSwitch(){
    delete(clap_input);
}

volatile bool ClapSwitch::clap_received(void){
    return _clap_received;
}

void ClapSwitch::receive_clap(void){
    _clap_received = true;
}
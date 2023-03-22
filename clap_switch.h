#ifndef CLAP_SWITCH
#define CLAP_SWITCH 

#include "mbed.h"

class ClapSwitch{
    public:
        ClapSwitch(PinName input_pin);
        ~ClapSwitch();
        volatile bool clap_received (void);

    private:
        InterruptIn* clap_input;
        volatile bool _clap_received{false};

        void receive_clap(void);
};

#endif
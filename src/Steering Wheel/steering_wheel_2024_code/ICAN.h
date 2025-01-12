#ifndef I_CAN_H
#define I_CAN_H

#include "driver/twai.h"

class ICAN
{
public:
    virtual void setup() = 0;
    virtual void transmit(const twai_message_t &message) = 0;
    virtual ~ICAN() = default;
};

#endif // I_CAN_H
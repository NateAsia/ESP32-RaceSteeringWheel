#ifndef CAN_HANDLER_H
#define CAN_HANDLER_H

#include "ICAN.h"

class CANHandler : public ICAN
{
public:
    CANHandler();
    void setup() override;
    void transmit(const twai_message_t &message) override;

private:
    static const twai_general_config_t g_config;
    static const twai_timing_config_t t_config;
    static const twai_filter_config_t f_config;
};

#endif // CAN_HANDLER_H
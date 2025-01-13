#ifndef I_CAN_H
#define I_CAN_H

#include "driver/twai.h"

/**
 * @class ICAN
 * @brief Interface for CAN communication.
 *
 * This interface defines the basic operations for CAN communication.
 */
class ICAN
{
public:
    /**
     * @brief Set up the CAN interface.
     */
    virtual void setup() = 0;

    /**
     * @brief Transmit a CAN message.
     * 
     * @param message The CAN message to be transmitted.
     */
    virtual void transmit(const twai_message_t &message) = 0;

    /**
     * @brief Virtual destructor.
     *
     * Ensures proper cleanup of derived classes.
     */
    virtual ~ICAN() = default;
};

#endif // I_CAN_H

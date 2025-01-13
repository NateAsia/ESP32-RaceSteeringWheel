#ifndef I_STEERING_WHEEL_H
#define I_STEERING_WHEEL_H

/**
 * @class ISteeringWheel
 * @brief Interface for the steering wheel functionality.
 *
 * This interface defines the basic operations that any steering wheel
 * implementation should support.
 */
class ISteeringWheel {
public:
    /**
     * @brief Set up the steering wheel.
     *
     * This method should handle any initialization required for the steering wheel.
     */
    virtual void setup() = 0;

    /**
     * @brief Start the steering wheel operations.
     *
     * This method should begin the main operational loop of the steering wheel.
     */
    virtual void start() = 0;

    /**
     * @brief Virtual destructor.
     *
     * Ensures proper cleanup of derived classes.
     */
    virtual ~ISteeringWheel() = default;
};

#endif // I_STEERING_WHEEL_H

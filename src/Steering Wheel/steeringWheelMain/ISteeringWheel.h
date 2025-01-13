#ifndef I_STEERING_WHEEL_H
#define I_STEERING_WHEEL_H

class ISteeringWheel {
public:
    virtual void setup() = 0;
    virtual void start() = 0;
    virtual ~ISteeringWheel() = default;
};

#endif // I_STEERING_WHEEL_H
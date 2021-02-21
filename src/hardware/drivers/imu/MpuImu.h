#pragma once

#include <Arduino.h>
#include "hardware/IO.h"
#include "hardware/BusIO.h"
#include "hardware/AbstractHardware.h"
#include <SPI.h>
#include <Wire.h>

namespace IMUDrivers {
    /**
     * MPU6000 / MPU6050 IMU driver.
     *
     * Datasheets:
     * https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf
     * https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
     */
    class MpuImu : public Hardware::Gyroscope, public Hardware::Accelerometer {
    public:
        MpuImu(TwoWire* i2c, uint8_t address, IO::Pin* interruptPin);
        MpuImu(SPIClass* spi, uint32_t csPin, IO::Pin* interruptPin);
        Hardware::ThreeAxisData getRotationData() override;
        Hardware::ThreeAxisData getAccelerationData() override;

    private:
        /**
         * nullptr if we aren't using interrupts.
         */
        IO::Pin* interruptPin;
        BusIO::RegisterBasedDevice* device;
        void initialize(bool disableDeviceI2C);
        void handleInterrupt();
    };
}

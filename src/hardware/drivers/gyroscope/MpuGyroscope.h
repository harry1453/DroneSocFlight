#pragma once

#include <Arduino.h>
#include "hardware/BusIO.h"
#include "hardware/AbstractHardware.h"
#include <SPI.h>
#include <Wire.h>

namespace GyroscopeDrivers {
    /**
     * MPU6000 / MPU6050 gyroscope driver.
     *
     * Datasheets:
     * https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf
     * https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
     */
    class MpuGyroscope : public Hardware::Gyroscope {
    public:
        MpuGyroscope(TwoWire* i2c, uint8_t address);
        MpuGyroscope(SPIClass* spi, uint32_t csPin);
        Hardware::ThreeAxisData getRotationData() override;

    private:
        BusIO::RegisterBasedDevice* device;
        void initialize();
    };
}

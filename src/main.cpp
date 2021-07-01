#include "mbed.h"
#include "USBSerial.h"
#include "LSM303AGRAccSensor.h"

USBSerial serial;
DigitalOut led((PinName)0x6C);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
SPI devSPI(PB_15, NC, PB_13);  // 3-wires SPI on SensorTile  
static LSM303AGRAccSensor accelerometer(&devSPI, PC_4);

FileHandle *mbed::mbed_override_console(int) {
    return &serial;
}

void toggleLed() {
    led = !led;
}

void initializeAccelerometer() {
    uint8_t id;
    int32_t axes[3];
    accelerometer.init(NULL);
    accelerometer.enable();
    accelerometer.read_id(&id);
    printf("LSM303AGR accelerometer           = 0x%X\r\n", id);
    accelerometer.get_x_axes(axes);
    printf("LSM303AGR [acc/mg]:      %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);
}

void readAccelerometer() {
    int32_t axes[3];
    accelerometer.get_x_axes(axes);
    printf("%ld,%ld,%ld\n\r",axes[0],axes[1],axes[2]);
    toggleLed();
}

int main(void)
{
    led = 1;
    initializeAccelerometer();
    queue.call_every(10ms, readAccelerometer);
    queue.dispatch_forever();
}
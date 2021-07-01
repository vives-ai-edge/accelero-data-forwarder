#include "mbed.h"
#include "USBSerial.h"
#include "LSM303AGRAccSensor.h"

USBSerial serial;
DigitalOut led((PinName)0x6C);

volatile bool trig = 0;
Ticker timer;
uint8_t id;
int32_t axes[3];

SPI devSPI(PB_15, NC, PB_13);  // 3-wires SPI on SensorTile  
static LSM303AGRAccSensor accelerometer(&devSPI, PC_4);


FileHandle *mbed::mbed_override_console(int) {
    return &serial;
}

void sample(){
    trig=1;
}

int main(void)
{
    led = 1;
    accelerometer.init(NULL);
    accelerometer.enable();
    accelerometer.read_id(&id);
    printf("LSM303AGR accelerometer           = 0x%X\r\n", id); 
    accelerometer.get_x_axes(axes);
    printf("LSM303AGR [acc/mg]:      %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);
    timer.attach_us(&sample,10100);

    while(true){
        while (!trig){}
        trig = 0;
        accelerometer.get_x_axes(axes);
        printf("%ld,%ld,%ld\n\r",axes[0],axes[1],axes[2]);
        led = !led;
    }
}
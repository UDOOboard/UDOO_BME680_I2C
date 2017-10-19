#include <iostream>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <cstring>
#include <zconf.h>
#include "lib/DFRobot_BME680.h"
#include "lib/DFRobot_BME680_I2C.h"

std::string getIAQClassification(int gas);

int main() {

    int fd_i2c = open("/dev/i2c-3", O_RDWR);

    if (fd_i2c < 0) {
        std::cout << "Error opening file " << std:: endl;

        return 1;
    }

    if (ioctl(fd_i2c, I2C_SLAVE, BME680_I2C_ADDR_PRIMARY) < 0) {
        std::cout << "ioctl error "<< std:: endl;
        return 1;
    }

    DFRobot_BME680_I2C bme(BME680_I2C_ADDR_PRIMARY, fd_i2c);
    bme.begin();
    sleep(1);

    while(true){
        bme.startConvert();
        sleep(1);
        bme.update();

        std::cout << "temperature " << bme.readTempture() << std::endl;
        std::cout << "humidity " << bme.readHumidity() << std::endl;
        std::cout << "pressure " << bme.readPressure() << std::endl;
        std::cout << "altitude " << bme.readAltitude() << std::endl;
        std::cout << "gas resistance " << bme.readGasResistance() << std::endl;
        uint16_t gas = bme.readGas();
        std::cout << "gas " << gas << std::endl;
        std::cout << "iaq " << getIAQClassification(gas) << std::endl;
        sleep(1);
    }
}
std::string getIAQClassification(int gas){
    if(gas <= 50){
        return "good";
    }else if( gas <= 100){
        return "average";
    }else if( gas <= 150){
        return "little bad";
    }else if( gas <= 200){
        return "bad";
    }else if( gas <= 300){
        return "worse";
    }else {
        return "very bad";
    }
}


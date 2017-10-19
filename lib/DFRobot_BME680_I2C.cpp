#include "DFRobot_BME680_I2C.h"

#include <linux/i2c-dev.h>
#include <stdio.h>
#include <zconf.h>
#include <cstdlib>

static int8_t bme680_i2c_read(int fd, uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len) {

    /*
    * Data on the bus should be like
    * |------------+---------------------|
    * | I2C action | Data                |
    * |------------+---------------------|
    * | Start      | -                   |
    * | Write      | (reg_addr)          |
    * | Stop       | -                   |
    * | Start      | -                   |
    * | Read       | (reg_data[0])       |
    * | Read       | (....)              |
    * | Read       | (reg_data[len - 1]) |
    * | Stop       | -                   |
    * |------------+---------------------|
    */

    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    uint8_t reg[1];
    reg[0] = reg_addr;

    if (write(fd, reg, 1) != 1) {
        perror("user_i2c_read_reg");
        rslt = 1;
    }
    if (read(fd, data, len) != len) {
        perror("user_i2c_read_data");
        rslt = 1;
    }

    return rslt;
}


static int8_t bme680_i2c_write(int fd, uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len) {

/*
 * Data on the bus should be like
 * |------------+---------------------|
 * | I2C action | Data                |
 * |------------+---------------------|
 * | Start      | -                   |
 * | Write      | (reg_addr)          |
 * | Write      | (reg_data[0])       |
 * | Write      | (....)              |
 * | Write      | (reg_data[len - 1]) |
 * | Stop       | -                   |
 * |------------+---------------------|
 */

    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    uint8_t reg[16];
    reg[0] = reg_addr;

    for (int i = 1; i < len + 1; i++)
        reg[i] = data[i - 1];

    if (write(fd, reg, len + 1) != len + 1) {
        perror("user_i2c_write");
        rslt = 1;
        exit(1);
    }

    return rslt;
}


static void bme680_delay_ms(uint32_t period) {
    usleep(period * 1000);
}


DFRobot_BME680_I2C::DFRobot_BME680_I2C(uint8_t I2CAddr, int fd) :
        DFRobot_BME680(fd, bme680_i2c_read, bme680_i2c_write, bme680_delay_ms, eBME680_INTERFACE_I2C) {
    DFRobot_BME680::bme680_I2CAddr = I2CAddr;
    DFRobot_BME680::fd = fd;
}



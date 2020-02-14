#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    const char *fileName = "/dev/i2c-1";                        // device's file
    const int  address = 0x18;                                  // default addr of MCP9808
    unsigned char buf[10];
    int fd;

    if ((fd = open(fileName, O_RDWR)) < 0) {
        printf("Failed to open i2c port\n");
        exit(1);
    }
    if (ioctl(fd, I2C_SLAVE, address) < 0) {
        printf("Unable to get bus access to talk to slave\n");
        exit(1);
    }
    buf[0] = 0x05;
    if ((write(fd, buf, 1)) != 1) {
        printf("Error writing to i2c slave\n");
        exit(1);
    }

    sleep(1);
    if (read(fd, buf, 2) != 2) {
        printf("Unable to read from slave\n");
        exit(1);
    } else {
        unsigned char highByte = buf[0];
        unsigned char lowByte = buf[1];
        int data = (highByte <<8) + lowByte;
        unsigned sign_bit = data & 0x1000;
	float result;

        data &= 0x1FFF;                               	// clear flag bits
        result = (float)(data) / 16.0F;
        if (sign_bit)
            result -= 256;                              // if signed we should subtract value (according to its datasheet)
        printf("Val: %f C\n", result);                  // display bearing with decimal place
    }

    return 0;
}

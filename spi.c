#include<stdio.h>
#include<fcntl.h>
#include<string.h>
/* #include<unistd.h> */
#include<stdlib.h>
#include<sys/ioctl.h>
#include<linux/types.h>
#include<linux/spi/spidev.h>
#include<stdint.h>

char rx_buf[100];
char tx_buf[100];

struct spi_ioc_transfer xfer;

int spi_init(char file_name[40]){
    int fd = open(file_name, O_RDWR);

    if (fd <0){
        printf("couldn't able find the device %s\n", file_name);
        return -1;
    }

    xfer.len = 20;                                            // len of command write
    xfer.bits_per_word = 8;
    xfer.cs_change = 0;
    xfer.speed_hz = 100000;
    xfer.tx_buf = (unsigned long)tx_buf;
    xfer.rx_buf = (unsigned long)rx_buf;
    return fd;
}

int main(){
    memset(tx_buf, 0x41, 10);
    memset(tx_buf+10, 0x42, 10);
    int fd = spi_init("/dev/spidev0.0");
    int status = ioctl(fd, SPI_IOC_MESSAGE(2), xfer);
    if (status < 0){
        printf("failed to transfer with status %d\n", status);
        return -1;
    }
    rx_buf[20]=0;
    puts(rx_buf);
    return 0;
}

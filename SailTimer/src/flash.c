#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#define MCP23017_ADDR 0x20

/* MCP23017 registers */
#define IODIRA  0x00
#define GPPUA   0x0C
#define GPIOA   0x12
#define OLATA   0x14

#define LED_PIN     0
#define SWITCH_PIN  1

static int mcp_write_reg(int fd, uint8_t reg, uint8_t value)
{
    uint8_t buf[2] = { reg, value };

    if (write(fd, buf, 2) != 2)
    {
        perror("write");
        return -1;
    }

    return 0;
}

static int mcp_read_reg(int fd, uint8_t reg)
{
    if (write(fd, &reg, 1) != 1)
    {
        perror("write reg addr");
        return -1;
    }

    uint8_t value;

    if (read(fd, &value, 1) != 1)
    {
        perror("read");
        return -1;
    }

    return value;
}

int main(void)
{
    int fd;

    fd = open("/dev/i2c-1", O_RDWR);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    if (ioctl(fd, I2C_SLAVE, MCP23017_ADDR) < 0)
    {
        perror("ioctl");
        return 1;
    }

    /* GPA0 = output, GPA1 = input */
    if (mcp_write_reg(fd, IODIRA, 0xFE) < 0)
        return 1;

    /* Enable pull-up on GPA1 */
    if (mcp_write_reg(fd, GPPUA, (1 << SWITCH_PIN)) < 0)
        return 1;

    uint8_t led_state = 0;
    uint8_t stable_state = 1;      /* pull-up means released */
    uint8_t last_sample = 1;
    int debounce_count = 0;

    printf("Monitoring switch on GPA1\n");

    while (1)
    {
        int gpio = mcp_read_reg(fd, GPIOA);
        if (gpio < 0)
            break;

        uint8_t sample = (gpio & (1 << SWITCH_PIN)) ? 1 : 0;

        if (sample == last_sample)
        {
            if (debounce_count < 5)
                debounce_count++;
        }
        else
        {
            debounce_count = 0;
            last_sample = sample;
        }

        if ((debounce_count >= 5) && (sample != stable_state))
        {
            stable_state = sample;

            if (stable_state == 0)
            {
                printf("Switch PRESSED\n");

                led_state ^= 1;

                if (led_state)
                    mcp_write_reg(fd, OLATA, (1 << LED_PIN));
                else
                    mcp_write_reg(fd, OLATA, 0);
            }
            else
            {
                printf("Switch RELEASED\n");
            }

            fflush(stdout);
        }

        usleep(10000);  /* 10 ms */
    }

    close(fd);
    return 0;
}
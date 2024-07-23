#include "hal/accelerometer.h"
#include "hal/joystick.h"

#define I2C_BUS_1 "/dev/i2c-1"
#define I2C_1 0x18
#define WHO_AM_I 0x0F
#define CTRL_REG1 0x20


#define CONFIG_P17 "config-pin p9_17 i2c > /dev/null"
#define CONFIG_P18 "config-pin p9_18 i2c > /dev/null"

static int desc;

void init_acc() {
    desc = initI2cBus(I2C_BUS_1, I2C_1);
    sleepForMs(200);
    runCommand(CONFIG_P17);
    sleepForMs(200);
    runCommand(CONFIG_P18);
    sleepForMs(200);
    writeI2cReg(desc, CTRL_REG1, 0x27);
}


int get_desc() {
	return desc;
}

// HELPER FUNCTIONS

int initI2cBus(char* bus, int address)
{
	int i2cFileDesc = open(bus, O_RDWR);
	if (i2cFileDesc < 0) {
		printf("I2C DRV: Unable to open bus for read/write (%s)\n", bus);
		perror("Error is:");
		exit(-1);
	}

	int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
	if (result < 0) {
		perror("Unable to set I2C device to slave address.");
		exit(-1);
	}
	return i2cFileDesc;
}

void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value)
{
	unsigned char buff[2];
	buff[0] = regAddr;
	buff[1] = value;
	int res = write(i2cFileDesc, buff, 2);
	if (res != 2) {
		perror("Unable to write i2c register");
		exit(-1);
	}
}

unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr)
{
	int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
	if (res != sizeof(regAddr)) {
		perror("Unable to write i2c register.");
		exit(-1);
	}

	char value = 0;
	res = read(i2cFileDesc, &value, sizeof(value));
	if (res != sizeof(value)) {
		perror("Unable to read i2c register");
		exit(-1);
	}
	return value;
}

// Function to run commands (provided in the assignment pdf)
void runCommand(char* command) {
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");
    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL) {
            break;
        }
    }
    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}
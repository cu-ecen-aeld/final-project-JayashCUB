#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>


#define GPIO_PIN 2 // BCM pin 27 corresponds to WiringPi pin 2
#define led_PIN 0 // BCM pin 17 corresponds to WiringPi pin 0
#define yled_PIN 7 // BCM pin 4 corresponds to WiringPi pin 7
#define buz_pin 3   // BCM pin 22 corresponds to WiringPi pin 3

void readI2C(unsigned char *data, int bytes) {
	int i2cAddress = 0x57;
	int i2cHandle;
	char *deviceName = "/dev/i2c-1";
    
	// Open the I2C bus
	if ((i2cHandle = open(deviceName, O_RDWR)) < 0) {
    	perror("Error opening I2C");
    	exit(1);
	}
    
	// Set the I2C slave address
	if (ioctl(i2cHandle, I2C_SLAVE, i2cAddress) < 0) {
    	perror("Error setting I2C slave address");
    	exit(1);
	}

	// Write a '1' to start ranging session
	unsigned char startSession = 0x01;
	if (write(i2cHandle, &startSession, 1) != 1) {
    	perror("Error writing to I2C");
    	exit(1);
	}
    
	// Wait a few milliseconds (you may need to adjust this)
	usleep(10000); // Wait for 10 milliseconds
    
	// Read the 3-byte response
	if (read(i2cHandle, data, bytes) != bytes) {
    	perror("Error reading from I2C");
    	exit(1);
	}
    
	// Close the I2C bus
	close(i2cHandle);
}

// Signal handler function to handle Ctrl+C
void handleSignal(int signal) {
    printf("\nExiting program.\n");
    exit(0);
}

int main() {
    signal(SIGINT, handleSignal); // Register signal handler for Ctrl+C

    wiringPiSetup(); // Initialize WiringPi library

    pinMode(GPIO_PIN, INPUT); // Set BCM pin 11 as input
    pinMode (led_PIN, OUTPUT) ;         // aka BCM_GPIO pin 17
    pinMode (yled_PIN, OUTPUT) ;
    printf("Reading value of BCM pin 27 as input...\n");
  
    pinMode (buz_pin, OUTPUT) ;
    
    unsigned char data[3];
    
    
    // Loop to read the value of BCM pin 11 indefinitely
    while (1) {
            
        int value = digitalRead(GPIO_PIN); // Read digital value of BCM pin

        printf("BCM pin 27 value: %d\n", value);
        
        if(value ==1)
        {
          printf("buton\n");
          digitalWrite (led_PIN, 0) ;
        }
        else
        {
          digitalWrite (led_PIN, 1) ;
        }
        
        readI2C(data, 3);
    
  // Combine the bytes to get the distance in um
	unsigned int distance = (data[0] << 16) | (data[1] << 8) | data[2];
	// Convert um to mm
	float distance_mm = distance / 1000.0;
  printf("Distance: %.2f mm\n", distance_mm);
        if(distance_mm == 0)
        {
          digitalWrite (yled_PIN, 1) ;
        }
        else if (distance_mm < 50)
        {
          digitalWrite (yled_PIN, 0) ;
          for(int i=0; i<50; i++)
          {
            digitalWrite (buz_pin, 0) ;
            delay(5);
            digitalWrite (buz_pin, 1) ;
            delay(5);
          }
        }
  

        delay(500);
        digitalWrite (yled_PIN, 1) ;
    }

    return 0;
}


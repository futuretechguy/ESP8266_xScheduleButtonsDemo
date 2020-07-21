// un-comment out this line if you want to see debug messages over the serial connection
#define DEBUG 1

// The serial speed used for debugging messages
#define SERIALRATE 115200

// This pin will be held high when the program is running ... you can connect an LED through a 330 Ohm resistor to show the power is on
#define POWERPIN 15

// This pin will be held high when any button is pressed ... you can connect an LED to provide button press feedback to the user
#define PRESSPIN 2

// This is the IP address of the machine running xSchedule
#define SERVER_IP "192.168.1.58"

// This is the xSchedule port
#define WEBPORT 80

// This is the number of buttons connected
#define BUTTONS 3

// Define the pins to use ... each pin maps to a button named ArduinoButton_n where n is the one based index in the table below
// This must be at least as large as the number of buttons above. Excess pins are ignored.
// This list of pins is ordered from best to worst choice. Not all GPIO pins can be used. 
short pins[] = {14, 12, 13, 5, 4, 3, 16};

// NOTE: When wiring up the buttons the buttons should short the pin to ground when pressed. When not pressed the pin should be free floating

#include "my_serial.h"

int main(void){
	serial0_init(9600);
	serial_send(serial_receive());
	return 0;
}

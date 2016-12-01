//Only works for 8bit, no parity, 1 stop bit
void serial0_init(unsigned int baud);
void serial_send(unsigned char);
unsigned char serial_receive();

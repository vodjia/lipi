#ifndef TRANSMIT_H_
#define TRANSMIT_H_

void wait();
void send_char(unsigned char input, struct gpiod_line *light);
void translate_char(unsigned char input, struct gpiod_line *light);
void transmit(char* data, struct gpiod_line *light);

#endif
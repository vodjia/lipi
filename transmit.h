#ifndef TRANSMIT_H
#define TRANSMIT_H

void wait();
void send_char(unsigned char input, struct gpiod_line *light);
void translate_char(unsigned char input, struct gpiod_line *light);
void transmit(char* data, struct gpiod_line *light);

#endif

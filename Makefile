CPPFLAGS += -g
CFLAGS += -Wall -std=c11
LDFLAGS += -lgpiod

TARGETS = receive transmit

RECEIVE_SRCS = receive.c decode.c receiver.c adc0820.c
TRANSMIT_SRCS = transmit.c encode.c transmitter.c led.c

.PHONY: all receive transmit

all: $(TARGETS)

receive:
	$(CC) $(CPPFLAGS) $(CFALGS) $(RECEIVE_SRCS) $(LDFLAGS) -o receive

transmit:
	$(CC) $(CPPFLAGS) $(CFALGS) $(TRANSMIT_SRCS) $(LDFLAGS) -o transmit

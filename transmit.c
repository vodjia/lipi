#include <gpiod.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "transmit.h"

static const unsigned char eight = 8;

static unsigned char start_pattern = 24;
static unsigned char end_pattern = 13;
static unsigned long interval = 1000;

void wait() {
    struct timeval current;
    gettimeofday(&current, NULL);
    usleep(interval - current.tv_usec % interval);
}

void send_char(unsigned char input, struct gpiod_line *light) {
    gpiod_line_set_value(light, (input & 16) != 0);
    wait();
    gpiod_line_set_value(light, (input & 8) != 0);
    wait();
    gpiod_line_set_value(light, (input & 4) != 0);
    wait();
    gpiod_line_set_value(light, (input & 2) != 0);
    wait();
    gpiod_line_set_value(light, (input & 1) != 0);
    wait();
}

// void send_char(unsigned char input, struct gpiod_line *light) {
//     printf("%d", ((input & 16) != 0));
//     printf("%d", ((input & 8) != 0));
//     printf("%d", ((input & 4) != 0));
//     printf("%d", ((input & 2) != 0));
//     printf("%d", ((input & 1) != 0));
// }

void translate_char(unsigned char input, struct gpiod_line *light) {
    unsigned char five_bits = 0;
    switch (input)
    {
    case 0:
        five_bits = 30;
        break;
    case 1:
        five_bits = 9;
        break;
    case 2:
        five_bits = 20;
        break;
    case 3:
        five_bits = 21;
        break;
    case 4:
        five_bits = 10;
        break;
    case 5:
        five_bits = 11;
        break;
    case 6:
        five_bits = 14;
        break;
    case 7:
        five_bits = 15;
        break;
    case 8:
        five_bits = 18;
        break;
    case 9:
        five_bits = 19;
        break;
    case 10:
        five_bits = 22;
        break;
    case 11:
        five_bits = 23;
        break;
    case 12:
        five_bits = 26;
        break;
    case 13:
        five_bits = 27;
        break;
    case 14:
        five_bits = 28;
        break;
    case 15:
        five_bits = 29;
        break;
    default:
        break;
    }
    send_char(five_bits, light);
}


void transmit(char *data, struct gpiod_line *light) {
    send_char(start_pattern, light);
    for (size_t i = 0; i < strlen(data); i++) {
        unsigned char left = data[i] >> 4;
        translate_char(left, light);
        unsigned char right = data[i] & 15;
        translate_char(right, light);
    }
    send_char(end_pattern, light);
    return;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("please input the name of the file to transmit\n");
        return 0;
    }
    struct gpiod_chip *chip;
    struct gpiod_line *light;
    const char *chipname = "gpiochip0";
    chip = gpiod_chip_open_by_name(chipname);
    light = gpiod_chip_get_line(chip, 18);
    gpiod_line_request_output(light, "transmit", 0);
    gpiod_line_set_value(light, 0);
    // printf("here\n");
    size_t len = strlen(argv[1]) +16;
    char file_path[len];
    strcpy(file_path, "./");
    strcpy(file_path + 2, argv[1]);
    // printf("file_path = %s\n", file_path);
    FILE* fp;
    fp = fopen(file_path, "r");
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    // printf("%u \n", size);
    char* buffer = malloc(size + 1);
    size_t count = 0;
    if ((count = fread(buffer, 1, size, fp)) != size) {
        printf("failed in loading file, count = %u \n", count);
        return 0;
    }
    strcpy(buffer + size, "\0");
    while (true) {
        transmit(buffer, light);
        // printf("\n");
        // usleep(100000);
    }
    gpiod_line_set_value(light, 0);
    gpiod_line_release(light);
    gpiod_chip_close(chip);
    // printf("%s\n", buffer);
    free(buffer);
}
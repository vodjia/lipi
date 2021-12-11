#include <gpiod.h>

struct adc0820_pinout {
	int db0;
	int db1;
	int db2;
	int db3;
	union {
		int wr_n;
		int rdy;
	};
	int mode;
	int rd_n;
	int int_n;
	int cs_n;
	int db4;
	int db5;
	int db6;
	int db7;
	int ofl_n;
};

struct adc0820_device {
	struct gpiod_chip *chip;
	struct gpiod_line *db0;
	struct gpiod_line *db1;
	struct gpiod_line *db2;
	struct gpiod_line *db3;
	union {
		struct gpiod_line *wr_n;
		struct gpiod_line *rdy;
	};
	struct gpiod_line *mode;
	struct gpiod_line *rd_n;
	struct gpiod_line *int_n;
	struct gpiod_line *cs_n;
	struct gpiod_line *db4;
	struct gpiod_line *db5;
	struct gpiod_line *db6;
	struct gpiod_line *db7;
	struct gpiod_line *ofl_n;
};

struct adc0820_device *adc0820_new(struct gpiod_chip *chip,
				   struct adc0820_pinout *pinout);

void adc0820_delete(struct adc0820_device *device);

unsigned char adc0820_read(struct adc0820_device *device);

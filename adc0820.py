from gpiozero import *

class ADC0820(CompositeDevice):
    """8-Bit High Speed A/D Converter"""
    def __init__(self, db0=None, db1=None, db2=None, db3=None,
                 db4=None, db5=None, db6=None, db7=None,
                 rd_n=None, int_n=None, cs_n=None, ofl_n=None,
                 wr_n=None, mode=None):
        super(ADC0820, self).__init__(
            db0=DigitalInputDevice(db0),
            db1=DigitalInputDevice(db1),
            db2=DigitalInputDevice(db2),
            db3=DigitalInputDevice(db3),
            db4=DigitalInputDevice(db4),
            db5=DigitalInputDevice(db5),
            db6=DigitalInputDevice(db6),
            db7=DigitalInputDevice(db7),
            rd_n=DigitalOutputDevice(rd_n, False),
            int_n=DigitalInputDevice(int_n, True),
            cs_n=DigitalOutputDevice(cs_n, False),
            ofl_n=DigitalInputDevice(ofl_n, True),
            wr_n=DigitalOutputDevice(wr_n, False),
            mode=DigitalOutputDevice(mode)
        )

    def read(self):
        if self.cs_n.is_active is not True:
            self.cs_n.on()
        if self.mode.value != 0:
            self.mode.off()
        self.rd_n.on()
        while self.int_n.is_active is not True:
            continue
        data = (
            self.db7.value << 7 |
            self.db6.value << 6 |
            self.db5.value << 5 |
            self.db4.value << 4 |
            self.db3.value << 3 |
            self.db2.value << 2 |
            self.db1.value << 1 |
            self.db0.value
        )
        self.rd_n.off()
        return data

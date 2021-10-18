# from gpiozero import AnalogInputDevice
# from gpiozero import SPIBadChannel

# class ADC083x(AnalogInputDevice):
#     """
#     Extends :class:`AnalogInputDevice` to implement an interface for all ADC
#     chips with a protocol similar to the Texas Instruments Incorporated ADC083x
#     series of devices.
#     """

#     def __init__(self, channel=0, bits=8, differential=False, max_voltage=5,
#                  **spi_args):
#         self._channel = channel
#         self._differential = bool(differential)
#         super(ADC083x, self).__init__(bits, max_voltage, **spi_args)

#     @property
#     def channel(self):
#         """
#         The channel to read data from. The ADC0831, ADC0832, ADC0834, and
#         ADC0838 have 1, 2, 4, and 8 channels respectively.
#         """
#         return self._channel

#     @property
#     def differential(self):
#         """
#         If ``True``, the device is operated in differential mode. In this mode
#         one channel (specified by the channel attribute) is read relative to
#         the value of a second channel (implied by the chip's design).

#         Please refer to the device data-sheet to determine which channel is
#         used as the relative base value (for example, when using an
#         :class:`ADC0834` in differential mode, channel 0 is read relative to
#         channel 1).
#         """
#         return self._differential

#     def _read(self):
#         return self._words_to_int(
#             self._spi.transfer(self._send())[-2:], self.bits
#         )

#     def _send(self):
#         """
#         ADC0834 protocol looks like the following:
        
#             Byte        0        1        2
#             ==== ======== ======== ========
#             Tx   00000001 MCCCxxxx xxxxxxxx
#             Rx   xxxxxxxx xxxxx0RR RRRRRRRR

#         The transmit bits start with several preamble "0" bits, the number
#         of which is determined by the amount required to align the last byte
#         of the result with the final byte of output. A start "1" bit is then
#         transmitted, followed by the single/differential bit (M); 1 for
#         single-ended read, 0 for differential read. Next comes three bits for
#         channel (C).
        
#         Read-out begins with a don't care bit (x), then a null bit (0)
#         followed by the result bits (R). All other bits are don't care (x).
#         """
#         return self._int_to_words(
#             (0b10000 | (not self.differential) << 3 | self.channel) << (self.bits + 2)
#         )

# class ADC0834(ADC083x):
#     """
#     The `ADC0834`_ is a 8-bit analog to digital converter with 4 channels (0-3).
#     """
#     def __init__(self, channel=0, differential=False, max_voltage=5, **spi_args):
#         if not 0 <= channel < 4:
#             raise SPIBadChannel('channel must be between 0 and 3')
#         super(ADC0834, self).__init__(channel, 8, differential, max_voltage, **spi_args)

# pd = ADC0834(0)
# print("Hello, World!")
# print(pd.voltage)
# while True:
#     print(pd.voltage)


from gpiozero import AnalogInputDevice
from gpiozero import SPIBadChannel


class ADC083x(AnalogInputDevice):
    """
    Extends :class:`AnalogInputDevice` to implement an interface for all ADC
    chips with a protocol similar to the Texas Instruments Incorporated ADC083x
    series of devices.
    """

    def __init__(self, channel=0, differential=False, bits=8, max_voltage=5,
                 **spi_args):
        self._channel = channel
        self._differential = differential
        super(ADC083x, self).__init__(bits, max_voltage, **spi_args)

    @property
    def channel(self):
        """
        The channel to read data from. The ADC0831, ADC0832, ADC0834, and
        ADC0838 have 1, 2, 4, and 8 channels respectively.
        """
        return self._channel

    @property
    def differential(self):
        """
        If ``True``, the device is operated in differential mode. In this mode
        one channel (specified by the channel attribute) is read relative to
        the value of a second channel (implied by the chip's design).

        Please refer to the device data-sheet to determine which channel is
        used as the relative base value (for example, when using an
        :class:`ADC0834` in differential mode, channel 0 is read relative to
        channel 1).
        """
        return self._differential

    def _read(self):
        return self._words_to_int(
            self._spi.transfer(self._send())[-1:], self.bits
        )

    def _send(self):
        """
        ADC083x protocol looks like the following:

            Byte        0        1
            ==== ======== ========
            Tx   001MPSSx xxxxxxxx
            Rx   xxxxxxx0 RRRRRRRR

        The transmit bits start with several preamble "0" bits, the number
        of which is determined by the amount required to align the last byte
        of the result with the final byte of output. A start "1" bit is then
        transmitted, followed by a 4-bit MUX addressing code. A particular
        input configuration is assigned during the MUX addressing sequence,
        prior to the start of a conversion. This programmability is best
        illustrated by the MUX addressing codes shown in the following
        table:

                 Mux Addressing       Analog Single-Ended Channel #
                 ==============       =============================
            SGL/~DIF ODD/SGN  SELECT   0  1  2  3  4  5  6  7  COM
            ======== =======  ======   =  =  =  =  =  =  =  =  ===
                1       0      0  0    +                        -
                1       0      0  1          +                  -
                1       0      1  0                +            -
                1       0      1  1                      +      -
                1       1      0  0       +                     -
                1       1      0  1             +               -
                1       1      1  0                   +         -
                1       1      1  1                         +   -

        Read-out begins with a don't care bit (x), then a null bit (0)
        followed by the result bits (R). All other bits are don't care (x).
        """
        m = not self.differential
        p = self.channel % 2
        ss = self.channel >> 1
        return self._int_to_words(
            (0b10000 | m << 3 | p << 2 | ss) << (self.bits + 2)
        )


class ADC0834(ADC083x):
    """
    The `ADC0834`_ is a 8-bit analog to digital converter with 4 channels
    (0-3).

    .. _ADC0834: https://www.ti.com/lit/ds/symlink/adc0834-n.pdf
    """
    def __init__(self, channel=0, differential=False, max_voltage=5,
                 **spi_args):
        if not 0 <= channel < 4:
            raise SPIBadChannel('channel must be between 0 and 3')
        super(ADC0834, self).__init__(channel, differential, 8, max_voltage,
                                      **spi_args)
# pd = ADC0834(0, max_voltage=3.3)
# print("Hello, World!")
# print(pd.voltage)
# while True:
#     print(pd.value)
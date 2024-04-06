# CC1352 MSPM0 LED Toggle

This project is a brief example of UART communication between an MSPM0 board and a CC1352 board. A source controller sends characters via UART to the MSPM0 board, which then filters for specific command characters in a tasklet before sending corresponding 4-byte command signals on to a CC1352 board, which then changes its LED based on the command signal.

Once each board has been flashed with the corresponding built binaries, the MSPM0 board receives signals via a UART connection through the micro-USB port, looking for the command characters `t` (toggle LED), `z` (turn off LED), and `x` (turn on LED) (while also echoing all characters regardless of whether they are command or not). If any command characters are detected, the corresponding 4-byte command signals are forwarded via a second UART connection in pins PB6 (TX pin) and PB7 (RX pin) to the CC1352 board. The CC1352 board has its own UART connection set up on pins DIO25 (TX pin) and DIO26 (RX pin), and when it receives any command signals, it performs the corresponding operation.

With this in mind, the following connections are required:

* Female-to-female from PB7 (MSPM0) to DIO25 (CC1352), PB6 (MSPM0) to DIO26 (CC1352)
* Both boards connected with USB-to-micro (only MSPM0 uses the UART connection, both utilize power and ground through cable)

Once these are made, one green and one red light should be lit up on both boards. By sending the above commands (via Putty, for example), the red light on the CC1352 board should turn on and off.

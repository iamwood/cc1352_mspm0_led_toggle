# CC1352 MSPM0 LED Toggle

This project is a brief example of UART communication between an MSPM0 board and a CC1352 board. A source controller sends characters via UART to the MSPM0 board, which then filters for specific command characters in a tasklet before sending those matching characters on to a CC1352 board, which then changes its LED based on the command character.

Once each board has been flashed with the corresponding built binaries, the MSPM0 board receives signals via a UART connection in pins PA28 (TX pin) and PA31 (RX pin), looking for the command characters `t` (toggle LED), `z` (turn off LED), and `x` (turn on LED) (while also echoing all characters regardless of whether they are command or not). If any command characters are detected, those are forwarded via a second UART connection in pins PB6 (TX pin) and PB7 (RX pin) to the CC1352 board. The CC1352 board has its own UART connection set up on pins DIO25 (TX pin) and DIO26 (RX pin), and when it receives any command characters, it performs the corresponding operation.

With this in mind, the following connections are required:

* Female-to-female from PB7 (MSPM0) to DIO25 (CC1352), PB6 (MSPM0) to DIO26 (CC1352)
* Signaling cable from controller to PA28 and PA31 (serial-to-USB cable with Putty connection works well)
* Power and ground for both boards

Once these are made, one green and one red light should be lit up on both boards. By sending the above commands (via Putty, for example), the red light on the CC1352 board should turn on and off.

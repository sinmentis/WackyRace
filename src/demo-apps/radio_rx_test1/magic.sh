#! /bin/sh

export BOARD=racer
make clean 
make
openocd -f ../../mat91lib/sam4s/scripts/sam4s_stlink.cfg -c "program radio_rx_test1.bin verify reset exit"

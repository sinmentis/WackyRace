#! /bin/sh

export BOARD=hat
make clean 
make
openocd -f ../../mat91lib/sam4s/scripts/sam4s_stlink.cfg -c "program adc_test1.bin verify reset exit"

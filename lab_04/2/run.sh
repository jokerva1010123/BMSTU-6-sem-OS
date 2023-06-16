make

sudo insmod md1.ko

sudo insmod md2.ko

sudo dmesg | tail -10 | grep md | grep -v audit

sudo rmmod md2

sudo insmod md3.ko

sudo dmesg | tail -10 | grep md | grep -v audit

sudo rmmod md3

sudo rmmod md1

sudo dmesg | tail -10 | grep md | grep -v audit
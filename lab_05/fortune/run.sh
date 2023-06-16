#!/bin/bash

make
sudo insmod fortune.ko
lsmod | grep fortune

sudo dmesg | grep fortune

echo "12345" > /proc/fortuneFile
echo "dkfjaijf" > /proc/fortuneFile
echo "m k e j" > /proc/fortuneFile

cat /proc/fortuneFile
cat /proc/fortuneFile
cat /proc/fortuneFile

sudo rmmod fortune

sudo dmesg | grep fortune

make disclean
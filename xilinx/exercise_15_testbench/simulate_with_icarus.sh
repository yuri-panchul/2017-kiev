#!/bin/sh

rm -rf sim
mkdir sim
cd sim

iverilog -g2005 -I .. ../*.v
vvp a.out > a.lst

gtkwave dump.vcd
cd .. && rm -rf sim 

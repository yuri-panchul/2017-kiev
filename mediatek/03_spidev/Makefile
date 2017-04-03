#
# Makefile for SPI and GPIO demo program for Linkit board.
#

STAGING_DIR = "$(HOME)"/mediatek/OpenWrt-SDK-ramips-mt7688_gcc-4.8-linaro_uClibc-0.9.33.2.Linux-x86_64/staging_dir
export STAGING_DIR
BINDIR = $(STAGING_DIR)/toolchain-mipsel_24kec+dsp_gcc-4.8-linaro_uClibc-0.9.33.2/bin
INCDIR = $(STAGING_DIR)/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/include
LIBDIR = $(STAGING_DIR)/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/lib
CROSS_PREFIX=mipsel-openwrt-linux-

CC = $(BINDIR)/$(CROSS_PREFIX)gcc

INCFLAGS = -I$(INCDIR)
LIBFLAGS = -L$(LIBDIR)
C_OPTIONS = -O2 -Wall

all: program

.c.o:
	$(CC) $(C_OPTIONS) $(INCFLAGS) -c -o $@ $^

clean:
	$(RM) program *.o *~

program: program.o gpio_interface.o
	$(CC) $(LD_OPTIONS) $(LIBFLAGS) -o $@ $^


install: all
	scp ./program root@mylinkit.local.:program

run: install
	ssh -t root@mylinkit.local. ./program

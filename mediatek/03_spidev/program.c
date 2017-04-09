/*
  SPI and GPIO demo program for Linkit board.
 */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <poll.h>

#include <sys/ioctl.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>

#include <arpa/inet.h>

#include "gpio_interface.h"

#define VALUE_MIN 16
#define VALUE_MAX 4000
#define VALUE_UNIT ((VALUE_MAX - VALUE_MIN) / BOARD_GPIO_NUM)

#define SPI_DEVICE "/dev/spidev32766.1"

#define USE_DUPLEX_SPI 0
#define USE_WRITE_SPI 0

/*
 * Wait for a time given in milliseconds.
 */
void waitmsec(int n)
{
  poll(NULL, 0, n);
}

/*
  All GPIOs used in this program, in the order
  of position on the board.
*/
struct gpio_desc board_gpio[]=
  {
    {-1, 1,  GPIO_DIRECTION_OUT,  GPIO_OFF},
    {-1, 0,  GPIO_DIRECTION_OUT,  GPIO_OFF},
    {-1, 3,  GPIO_DIRECTION_OUT,  GPIO_OFF},
    {-1, 2,  GPIO_DIRECTION_OUT,  GPIO_OFF},
    {-1, 14, GPIO_DIRECTION_OUT,  GPIO_OFF},
    {-1, 15, GPIO_DIRECTION_OUT,  GPIO_OFF},
    {-1, 16, GPIO_DIRECTION_OUT,  GPIO_OFF},
    {-1, 17, GPIO_DIRECTION_OUT,  GPIO_OFF}
  };

#define BOARD_GPIO_NUM (sizeof(board_gpio) / sizeof(board_gpio[0]))

/* Signal handler for program termination, disables GPIO */
void handle_term(int n)
{
  gpio_stop(board_gpio, BOARD_GPIO_NUM);
  printf("\nCleanup finished\n");
  _exit(1);
}

struct sigaction sigact_term =
{
  .sa_handler = handle_term,
  .sa_flags = SA_RESETHAND
};

/*
  main()
 */
int main (int argc, char **argv)
{
  int h, readbytes;

#if USE_WRITE_SPI
  int writebytes;
#endif

  unsigned char param;
  unsigned int param32;
  int speed;
#if USE_DUPLEX_SPI
  struct spi_ioc_transfer xfer;
#endif

#if USE_DUPLEX_SPI || USE_WRITE_SPI
  unsigned short transmit_buf;
#endif
  unsigned short receive_buf;
  int i, value;

  /* Initialize GPIO and set handler to disable them on signal */
  gpio_init(board_gpio, BOARD_GPIO_NUM);

  sigaction(SIGTERM, &sigact_term, NULL);
  sigaction(SIGINT, &sigact_term, NULL);

  /* Open SPI device */
  h = open(SPI_DEVICE, O_RDWR);
  if(h < 0)
    {
      perror("Can't open SPI device");
      return 1;
    }
  printf ("SPI interface opened\n");

  /* Set SPI speed and modes */
  speed = 2500000;
  if(ioctl(h, SPI_IOC_WR_MAX_SPEED_HZ, &speed))
    {
      perror("Can't set SPI speed");
      goto end;
    }

  param32 = SPI_MODE_0;
  if(ioctl(h, SPI_IOC_WR_MODE32, &param32))
    {
      perror("Can't set SPI mode");
      goto end;
    }

  param = 0;
  if(ioctl(h, SPI_IOC_WR_LSB_FIRST, &param))
    {
      perror("Can't set SPI bits order");
      goto end;
    }

  param = 8;
  if(ioctl(h, SPI_IOC_WR_BITS_PER_WORD, &param))
    {
      perror("Can't set SPI word length");
      goto end;
    }

#if USE_DUPLEX_SPI
  transmit_buf = htons(0x8999);

  memset(&xfer, 0, sizeof(xfer));

  xfer.tx_buf = (unsigned long)&transmit_buf;
  xfer.rx_buf = (unsigned long)&receive_buf;
  xfer.len = 2;
#endif

  while(1)
    {
#if USE_DUPLEX_SPI
      /* Duplex SPI transaction */
      if(ioctl(h, SPI_IOC_MESSAGE(1), &xfer) < 0)
	{
	  perror("Can't perform SPI transaction");
	  goto end;
	}
      readbytes = 2;
#else
#if USE_WRITE_SPI
      /* Half-duplex write transaction (received data is ignored) */
      writebytes = write(h, &transmit_buf,2);
#endif
      /* Half-duplex read transaction */
      readbytes = read(h, &receive_buf, 2);
#endif
      if(readbytes == 2)
	{
	  /* Process received data */
	  value = htons(receive_buf);
	  printf("Received 0x%04x = %d\n", value, value);
	  /* Bar display */
	  for(i = 0; i < BOARD_GPIO_NUM; i++)
	    set_gpio(&board_gpio[i],
		     ((value - VALUE_MIN)
		      > (VALUE_UNIT * i))?GPIO_ON:GPIO_OFF);
	  /* Wait */
	  waitmsec(10);
	}
      else
	{
	  if(readbytes < 0)
	    {
	      /* Print message and exit on error */
	      perror("Can't perform SPI transaction");
	      goto end;
	    }
	}
    }

 end:

  /* Cleanup */
  close(h);
  printf ("SPI interface closed\n");
  gpio_stop(board_gpio, BOARD_GPIO_NUM);
  return 0;
}

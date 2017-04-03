/*
  GPIO interface
 */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "gpio_interface.h"

/*
  Write, retry on transient errors.
 */
static int write_persist(int h, const char *buf, size_t len)
{
  int r, offset;
  offset = 0;
  while(1)
    {
      r = write(h, buf + offset, len - offset);
      if( r < 0)
	{
	  if((errno == EAGAIN)
	     || (errno == EWOULDBLOCK)
	     || (errno == EINTR))
	    r = 0;
	  else
	    return r;
	}
      offset += r;
      if(offset >= len)
	return offset;
    }
}

/*
  Read, retry on transient errors.
 */
static int read_persist(int h, char *buf, size_t len)
{
  int r, offset;
  offset = 0;
  while(1)
    {
      r = read(h, buf + offset, len - offset);
      if( r < 0)
	{
	  if((errno == EAGAIN)
	     || (errno == EWOULDBLOCK)
	     || (errno == EINTR))
	    r = 0;
	  else
	    return r;
	}
      offset += r;
      if(offset >= len)
	return offset;
    }
}

/*
  Initialize GPIO, set direction and value.
 */
int gpio_init(struct gpio_desc *gpio, size_t gpio_num)
{
  int gpio_exportfile, gpiodirfile, i, len, counter = 0;
  char tmpname[36], tmpvalue[6];

  char *directions[2]={"in\n", "out\n"};
  int directions_len[2]= {3, 4};

  gpio_exportfile = open("/sys/class/gpio/export", O_WRONLY);
  if(gpio_exportfile < 0)
    return -1;

  for(i = 0; i < gpio_num; i++)
    {
      if( gpio[i].h < 0)
	{
	  snprintf(tmpname, sizeof(tmpname),
		   "/sys/class/gpio/gpio%d/value", gpio[i].num);

	  gpio[i].h = open(tmpname, O_RDWR);
	  if(gpio[i].h < 0)
	    {
	      snprintf(tmpvalue, sizeof(tmpvalue), "%d\n", gpio[i].num);
	      len = strlen(tmpvalue);
	      write_persist(gpio_exportfile, tmpvalue, len);
	      gpio[i].h = open(tmpname, O_RDWR);
	    }
	}
      if(gpio[i].h >= 0)
	{
	  snprintf(tmpname, sizeof(tmpname),
		   "/sys/class/gpio/gpio%d/direction", gpio[i].num);
	  gpiodirfile = open(tmpname, O_WRONLY);
	  if(gpiodirfile < 0)
	    {
	      close(gpio[i].h);
	      gpio[i].h = -1;
	    }
	  else
	    {
	      len = directions_len[gpio[i].direction];
	      if(write_persist(gpiodirfile,
			       directions[gpio[i].direction],
			       len) != len)
		{
		  close(gpio[i].h);
		  gpio[i].h = -1;
		}
	      close(gpiodirfile);
	    }

	  if(gpio[i].h >= 0)
	    {
	      counter ++;
	      if(gpio[i].direction == GPIO_DIRECTION_OUT)
		write_persist(gpio[i].h,
			      (gpio[i].value == GPIO_ON) ? "1\n" : "0\n", 2);
	      else
		{
		  lseek(gpio[i].h, 0, SEEK_SET);
		  if(read_persist(gpio[i].h, tmpvalue, 2) == 2)
		    {
		      if(*tmpvalue == '1')
			gpio[i].value = GPIO_ON;
		      else
			gpio[i].value = GPIO_OFF;
		    }
		  else
		    gpio[i].value = GPIO_OFF;
		}
	    }
	}
    }
  close(gpio_exportfile);
  return counter;
}

/*
  Set GPIO to input, then disable GPIO.
*/
void gpio_stop(struct gpio_desc *gpio, size_t gpio_num)
{
  int gpio_unexportfile, gpiodirfile, i, len;
  char tmpname[36], tmpvalue[6];

  gpio_unexportfile = open("/sys/class/gpio/unexport", O_WRONLY);
  if(gpio_unexportfile < 0)
    return;

  for(i = 0; i < gpio_num; i++)
    {
      if( gpio[i].h >= 0)
	{
	  close(gpio[i].h);
	  gpio[i].h = -1;

	  snprintf(tmpname, sizeof(tmpname),
		   "/sys/class/gpio/gpio%d/direction", gpio[i].num);
	  gpiodirfile = open(tmpname, O_WRONLY);
	  if(gpiodirfile >= 0)
	    {
	      write_persist(gpiodirfile,"in\n", 3);
	      close(gpiodirfile);
	    }

	  snprintf(tmpvalue, sizeof(tmpvalue), "%d\n", gpio[i].num);
	  len = strlen(tmpvalue);
	  write_persist(gpio_unexportfile, tmpvalue, len);
	}
    }
  close(gpio_unexportfile);
}

/*
  Set GPIO output value.
 */
int set_gpio(struct gpio_desc *gpio, enum gpio_value value)
{
  int r;

  if(gpio->direction != GPIO_DIRECTION_OUT)
    return -1;

  if(value != gpio->value)
    {
      r = write_persist(gpio->h, (value == GPIO_ON) ? "1\n" : "0\n", 2);
      if(r == 2)
	{
	  gpio->value = value;
	  return 0;
	}
      else
	return -1;
    }
  return 0;
}

/*
  Get GPIO input value.
 */
int get_gpio(struct gpio_desc *gpio)
{
  char tmpvalue[2];
  if(gpio->direction != GPIO_DIRECTION_IN)
    return -1;

  lseek(gpio->h, 0, SEEK_SET);
  if(read_persist(gpio->h, tmpvalue, 2) == 2)
    {
      if(*tmpvalue == '1')
	gpio->value = GPIO_ON;
      else
	gpio->value = GPIO_OFF;
    }
  else
    gpio->value = GPIO_OFF;
  return gpio->value;
}

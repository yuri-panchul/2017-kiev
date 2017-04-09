/*
  GPIO interface.
 */

/*
  GPIO direction.
 */
enum gpio_dir {
  GPIO_DIRECTION_IN,
  GPIO_DIRECTION_OUT
};

/*
  GPIO value.
 */
enum gpio_value {
  GPIO_OFF,
  GPIO_ON
};

/*
  GPIO descriptor.
 */
struct gpio_desc {
  int h;
  int num;
  enum gpio_dir direction;
  enum gpio_value value;
};

/*
  Initialize GPIO, set direction and value.
 */
int gpio_init(struct gpio_desc *gpio, size_t gpio_num);

/*
  Set GPIO to input, then disable GPIO.
 */
void gpio_stop(struct gpio_desc *gpio, size_t gpio_num);

/*
  Set GPIO output value.
 */
int set_gpio(struct gpio_desc *gpio, enum gpio_value value);

/*
  Get GPIO input value.
 */
int get_gpio(struct gpio_desc *gpio);

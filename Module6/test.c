#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

struct GPIO_tag {
  union {
    unsigned char R;
    struct {
      unsigned char  :4;
      unsigned char bit0:1;
      unsigned char bit1:1;
      unsigned char bit2:1;
      unsigned char bit3:1;
    } B;
  } PORT0;

  union {
    unsigned char R;
    struct {
      unsigned char bit0:1;
      unsigned char bit1:1;
      unsigned char bit2:1;
      unsigned char bit3:1;
      unsigned char bit4:1;
      unsigned char bit5:1;
      unsigned char bit6:1;
      unsigned char bit7:1;
    } B;
  } PORT1;
};

int main(void) {
    
    struct GPIO_tag gpio;
    
    gpio.PORT1.R = 2;
    
    printf("0x%x 0x%x", gpio.PORT1.B.bit0, gpio.PORT1.B.bit1);
    
    
    return 0;
}

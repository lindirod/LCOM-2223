#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
#include "keyboard.h"

int count = 0;
int hookId = 0;

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hookId;
  sys_irqsetpolicy(0, IRQ_REENABLE, &hookId);

  return 0;
}

int (timer_unsubscribe_int)() {
  sys_irqrmpolicy(&hookId);

  return 0;
}

void (timer_int_handler)() {
  count++;
}



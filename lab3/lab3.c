#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include <stdlib.h>
#include "i8042.h"
#include "keyboard.h"


//extern uint8_t scancode;
//int count = 0;

extern uint8_t scancode, stat;

extern int counter;
extern int count;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {

  uint8_t bit_no = 0;
  int ipc_status,r;
  message msg;
  


  kbc_subscribe_int(&bit_no);

  while(scancode != KBD_ESC_BREAK){
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & IRQ_SET) {
            kbc_ih();
            kbc_scancodes();
          }
          break;
        default:
          break; 
      }
    }
  }
  
  kbc_unsubscribe_int();
  kbd_print_no_sysinb(counter);
  return 0;
}



int(kbd_test_poll)() {
  while(scancode != KBD_ESC_BREAK){
    kbc_ih();
    kbc_scancodes();
    tickdelay(micros_to_ticks(DELAY_US));
  }
  kbc_poll();
  kbd_print_no_sysinb(counter);
  return 0;
}


  
int(kbd_test_timed_scan)(uint8_t n) {
  int ipc_status, r;
  message msg;
  uint8_t timer0_int_bit, kbd_int_bit;

  timer_subscribe_int(&timer0_int_bit);
  kbc_subscribe_int(&kbd_int_bit);

   while(scancode != KBD_ESC_BREAK && count < n * 60) {  
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & timer0_int_bit) { 
            timer_int_handler();
          }

          if (msg.m_notify.interrupts & kbd_int_bit) { 
            kbc_ih();
            kbc_scancodes();
            count = 0;
          }
        break;
      default:
        break; 
      }
    }
  }

  kbc_unsubscribe_int();
  timer_unsubscribe_int();
  kbd_print_no_sysinb(counter);

  return 0;
}

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "i8042.h"
#include "mouse.h"

extern uint8_t data, byte_index, packet[3];
extern struct packet pp;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
    uint8_t bit_no = '\0';
    int ipc_status, r;
    message msg;

    if (write_to_mouse(ENABLE_DATA_REPORTING) != 0) {
        printf("Error in enabling data reporting\n");
        return 1;
    } 

    if(mouse_subscribe_int(&bit_no) != 0) {
        printf("There was an error in mouse_subscribe_int function!\n");
        return 1;
    }

    while(cnt > 0) {
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
        }
        if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: 
            if (msg.m_notify.interrupts & bit_no) {
                mouse_ih();
                sync_mouse_bytes();
                if(byte_index == 3) {
                    build_mouse_packet();
                    mouse_print_packet(&pp);
                    byte_index = 0;
                    cnt--;
                }
            }
            break;
            default:
            break; 
        }
        }
    }

    if(mouse_unsubscribe_int() != 0) {
        printf("There was an error in mouse_unsubscribe_int function!\n");
        return 1;
    }

    if (write_to_mouse(DISABLE_DATA_REPORTING) != 0) {
        printf("Error in enabling data reporting");
        return 1;
    } 

    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    /* To be completed */
    printf("%s(%u): under construction\n", __func__, idle_time);
    return 1;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}

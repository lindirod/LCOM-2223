#ifndef _MOUSE_KEYBOARD_H_
#define _MOUSE_KEYBOARD_H_

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)();
void (sync_mouse_bytes)();
void (mouse_ih)();
int (read_kbc_output)(uint8_t port, uint8_t *output, uint8_t mouse);
int (read_kbc_status)(uint8_t *status);
int (write_kbc_command)(uint8_t port, uint8_t command_byte);
int (write_to_mouse)(uint8_t command);
void (build_mouse_packet)();

#endif /*_MOUSE_KEYBOARD_H_*/

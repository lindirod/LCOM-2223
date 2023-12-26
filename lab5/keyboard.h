#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_


#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdint.h>
#include "i8042.h"

int (kbc_subscribe_int)(uint8_t *bit_no);

int (kbc_unsubscribe_int)();

void (kbc_ih)();

void kbc_scancodes();

int kbc_poll();

#endif /*_LCOM_KEYBOARD_H_*/

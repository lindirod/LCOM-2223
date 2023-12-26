// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "vbe_graphics.h"
#include "i8042.h"
#include "keyboard.h"
#include "i8254.h"


extern vbe_mode_info_t vmi_p;
extern char* buffer_aux;
extern uint8_t scancode;
extern int count_time;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  
  if(vg_init(mode) == NULL){
    printf("Error: vg_graphics failed\n");
    return 1;
  }
  
  sleep(delay);
  
  vg_exit();

  return 0;
}

int (kbc_key)() { 

  int ipc_status;
  message msg;
  uint8_t bit_no;

  if (kbc_subscribe_int(&bit_no) != 0){
    printf("Error in subscribe interrupt\n");
    return 1;
  }

  while (scancode != KBD_ESC_BREAK){
    if (driver_receive(ANY, &msg, &ipc_status) != 0) { 
      printf("driver_receive failed");
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & bit_no) 
            kbc_ih();
            break;
        default:
          break; 
      }
    }
  }

  if (kbc_unsubscribe_int() != 0){
    printf("Error in unsubscribe interrupt\n");
    return 1;
  } 
  return 0;
}


int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
   
  if(vg_init(mode) == NULL){
    printf("Error: vg_init failed\n");
    return 1;
  }
  
  if (vg_draw_rectangle(x, y, width, height, color) != 0){
    printf("Error: vg_draw_rectangle\n");
    return 1;
  } 
 
  if (kbc_key() != 0){
    printf("Error: kbc_key\n");
    return 1;
  }
 
  if(vg_exit() != 0){
    printf("Error: vg_exit failed\n");
    return 1;
  }

  return 0;
}


/*   
int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  
  printf("%s(0x%03x, %u, 0x%08x, %d): under construction\n", __func__,
         mode, no_rectangles, first, step);

  return 1;
}
*/

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  xpm_image_t img;

    if (vg_init(0x105) == NULL){
    printf("Error: vg_init failed\n");
    return 1;
  }

  img.bytes = xpm_load(xpm, XPM_INDEXED, &img);

  if (draw_xpm(&img, x, y) != 0){
    printf("Error: vg_draw_rectangle\n");
    return 1;
  } 
 
  if (kbc_key() != 0){
    printf("Error: kbc_key\n");
    return 1;
  }
 
  if(vg_exit() != 0){
    printf("Error: vg_exit failed\n");
    return 1;
  }

  return 0;
}


int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  int ipc_status, r, frameCounter=0;
  message msg;
  uint8_t irq_kbc, irq_timer;
  uint16_t interruptFrames = sys_hz()/fr_rate;
  xpm_image_t img;
  int counter = 0;

  if(kbc_subscribe_int(&irq_kbc) != 0){
    printf("There was an error in kbd_subscribe_int function!\n");
    return 1;
  }

  if(timer_subscribe_int(&irq_timer) != 0){
    printf("There was an error in timer_subscribe_int function!\n");
    return 1;
  }

  void *video_mem = vg_init(0x105);

  if(video_mem == NULL) {
    printf("There was an error in vg_init function!\n");
    vg_exit();
    return 1;
  }

  img.bytes = xpm_load(xpm, XPM_INDEXED, &img);
  draw_xpm(&img,xi,yi);


  while(scancode != KBD_ESC_BREAK) { 
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if(msg.m_notify.interrupts & irq_timer) {
            timer_int_handler();

          if(yi>yf || xi>xf) break;
            if(counter% interruptFrames==0){
              frameCounter++;
              if(speed>0){
                erase_xpm(xi,yi,&img);
                if(xi == xf){
                  if(yi+speed>yf)
                    yi=yf;
                  else
                    yi+=speed;
                }
                else if(yi==yf){
                  if(xi+speed>xf)
                    xi=xf;
                  else
                    xi+=speed;
                }
                  draw_xpm(&img,xi,yi);
              }
              else{
                if(frameCounter == abs(speed)){
                  erase_xpm(xi,yi,&img);
                  if(xi==xf){
                    if(yi+1>yf)
                      yi=yf;
                    else
                      yi++;
                  }
                  else if(yi==yf){
                    if(xi+1>xf)
                      xi=xf;
                    else
                      xi++;
                  }
                  
                    draw_xpm(&img,xi,yi);
                  frameCounter=0;
                }
              }
            }
            
          }

          //Keyboard				
          if (msg.m_notify.interrupts & irq_kbc) { 
            kbc_ih();
          }

          break;
        default:
          break; 	
      }
    }
  }

  if(vg_exit() != 0){
    printf("There was an error in vg_exit function!\n");
    return 1;
  }

  if(timer_unsubscribe_int() != 0){
    printf("There was an error in timer_unsubscribe_int function!\n");
    return 1;
  }

  if(kbc_unsubscribe_int() != 0){
    printf("There was an error in kbd_unsubscribe_int function!\n");
    return 1;
  }


  return 0;
} 


int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}

#ifndef _LCOM_VBE_GRAPHICS_H_
#define _LCOM_VBE_GRAPHICS_H_

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (draw_xpm) (xpm_image_t* image, uint8_t x, uint8_t y);
void erase_xpm(uint16_t x, uint16_t y, xpm_image_t *img);

#endif /*_LCOM_VBE_GRAPHICS_H_*/


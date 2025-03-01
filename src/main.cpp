#include <lvgl.h>
#include <TFT_eSPI.h>
#include "lvgl/ui.h"

#define LVGL_TICK_PERIOD 20

// Set the screen resolution
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;

static lv_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

TFT_eSPI tft = TFT_eSPI();

#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf)
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

// Display flush callback function
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)px_map, w * h, true);
    tft.endWrite();

    lv_display_flush_ready(disp);
}

void setup()
{

    String LVGL_Arduino = "Hello Arduino ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
    Serial.begin(115200);
    lv_init();

    tft.begin();
    tft.setRotation(3);
    /* Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb((lv_tick_get_cb_t)millis);

    /* Register print function for debugging. */
#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print);
#endif
    // Initialize display driver
    lv_display_t *display = lv_display_create(screenWidth, screenHeight);
    lv_display_set_flush_cb(display, my_disp_flush);
    lv_display_set_buffers(display, buf, NULL, screenWidth * 10, LV_DISPLAY_RENDER_MODE_PARTIAL);

#if 0
    /* Create a simple label. */
    lv_obj_t *label = lv_label_create( lv_scr_act() );
    lv_label_set_text( label, "Hello Arduino, I'm LVGL!" );
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
#endif
    Serial.println("Setup done");

    ui_init();
}

void loop()
{
    lv_timer_handler();
    ui_tick();
    delay(5);
}
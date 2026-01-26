/**
 * @file main.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#ifndef _DEFAULT_SOURCE
  #define _DEFAULT_SOURCE /* needed for usleep() */
#endif

#include <stdlib.h>
#include <stdio.h>
#ifdef _MSC_VER
  #include <Windows.h>
#else
  #include <unistd.h>
  #include <pthread.h>
#endif
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"
#include <SDL.h>

#include "hal/hal.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void button_event_callback(lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

#if LV_USE_OS != LV_OS_FREERTOS

int main(int argc, char **argv)
{
  (void)argc; /* Unused */
  (void)argv; /* Unused */

  /* Set DPI awareness to ignore Windows scale setting */
#ifdef _WIN32
  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
#endif

  /* Initialize LVGL */
  lv_init();

  /* Initialize the HAL (display, input devices, tick) for LVGL */
  sdl_hal_init(960, 270);

  /* BEGIN main code */
  SDL_ShowCursor(SDL_DISABLE); /* Hide system cursor */

  /* Initialize screens */
  lv_obj_t * screen_0 = lv_obj_create(NULL);
  lv_obj_t * screen_1 = lv_obj_create(NULL);

  /* Screen 0 */
  lv_obj_set_style_bg_color(screen_0, lv_color_make(0, 128, 255), LV_PART_MAIN);
  lv_obj_set_style_text_font(screen_0, &lv_font_unscii_8, LV_PART_MAIN);
  lv_obj_set_style_text_color(screen_0, lv_color_hex(0xffffff), LV_PART_MAIN);

  lv_obj_t * label = lv_label_create(screen_0);
  lv_label_set_text(label, "103.5 DAWN FM");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t * btn = lv_button_create(screen_0);
  lv_obj_set_pos(btn, 10, 10);
  lv_obj_set_size(btn, 120, 50);
  lv_obj_set_style_bg_color(btn, lv_color_make(255, 128, 0), LV_PART_MAIN);
  lv_obj_add_event_cb(btn, button_event_callback, LV_EVENT_CLICKED, screen_1);

  lv_obj_t * label2 = lv_label_create(btn);
  lv_label_set_text(label2, "Button");
  lv_obj_center(label2);

  /* Screen 1 */
  lv_obj_set_style_bg_color(screen_1, lv_color_make(255, 128, 0), LV_PART_MAIN);
  lv_obj_set_style_text_font(screen_1, &lv_font_unscii_8, LV_PART_MAIN);
  lv_obj_set_style_text_color(screen_1, lv_color_hex(0xffffff), LV_PART_MAIN);

  label = lv_label_create(screen_1);
  lv_label_set_text(label, "103.5 DAWN FM");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  // lv_screen_load_anim(screen_0, LV_SCREEN_LOAD_ANIM_FADE_IN, 500, 0, false);
  lv_screen_load(screen_0);

  /* END main code */

  while(1) {
    /* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/
    uint32_t sleep_time_ms = lv_timer_handler();
    if(sleep_time_ms == LV_NO_TIMER_READY){
      sleep_time_ms =  LV_DEF_REFR_PERIOD;
    }
#ifdef _MSC_VER
    Sleep(sleep_time_ms);
#else
    usleep(sleep_time_ms * 1000);
#endif
  }

  return 0;
}


#endif

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void button_event_callback(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * btn = lv_event_get_target_obj(e);
  lv_obj_t * scr = lv_event_get_user_data(e);
  if(code == LV_EVENT_CLICKED) {
    lv_screen_load_anim(scr, LV_SCREEN_LOAD_ANIM_FADE_IN, 500, 0, false);
  }
}

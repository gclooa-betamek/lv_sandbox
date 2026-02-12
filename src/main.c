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

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

#define SCREEN_CONTENT_PCT 80
#define SCREEN_NAVBAR_PCT  20

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void button_event_callback(lv_event_t * e);

static lv_obj_t * create_content(lv_obj_t * parent);
static lv_obj_t * create_navbar(lv_obj_t * parent);
static lv_obj_t * create_navbar_button(lv_obj_t * parent, lv_obj_t * content, char * label, lv_color_t bg_color);

static lv_obj_t * create_content_home(lv_obj_t * parent);
static lv_obj_t * create_content_navigation(lv_obj_t * parent);
static lv_obj_t * create_content_bluetooth(lv_obj_t * parent);
static lv_obj_t * create_content_settings(lv_obj_t * parent);

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
  sdl_hal_init(SCREEN_WIDTH, SCREEN_HEIGHT);

  /* Hide system cursor */
  SDL_ShowCursor(SDL_DISABLE);

  /* Initialize screen */
  lv_obj_t * screen = lv_obj_create(NULL);
  lv_obj_set_style_text_font(screen, &lv_font_montserrat_24, LV_PART_MAIN);
  lv_obj_set_style_text_color(screen, lv_color_make(255, 255, 255), LV_PART_MAIN);
  lv_screen_load(screen);

  /* Create screen partitions */
  lv_obj_t * content = create_content(screen);
  lv_obj_t * navbar  = create_navbar(screen);

  /* Create content */
  lv_obj_t * content_home = create_content_home(content);
  lv_obj_t * content_navigation = create_content_navigation(content);
  lv_obj_t * content_bluetooth = create_content_bluetooth(content);
  lv_obj_t * content_settings = create_content_settings(content);

  /* Create navbar buttons */
  lv_obj_t * button_home = create_navbar_button(
    navbar, content_home, LV_SYMBOL_HOME "  Home", lv_color_make(0, 120, 240)
  );
  lv_obj_t * button_navigation = create_navbar_button(
    navbar, content_navigation, LV_SYMBOL_GPS "  Navigation", lv_color_make(0, 240, 120)
  );
  lv_obj_t * button_bluetooth = create_navbar_button(
    navbar, content_bluetooth, LV_SYMBOL_BLUETOOTH "  Bluetooth", lv_color_make(240, 120, 0)
  );
  lv_obj_t * button_settings = create_navbar_button(
    navbar, content_settings, LV_SYMBOL_SETTINGS "  Settings", lv_color_make(120, 0, 240)
  );

  while(1) {
    /* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/
    uint32_t sleep_time_ms = lv_timer_handler();
    if(sleep_time_ms == LV_NO_TIMER_READY){
      sleep_time_ms = LV_DEF_REFR_PERIOD;
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
  lv_obj_t * content = lv_event_get_user_data(e);
  lv_obj_t * btn = lv_event_get_target(e);
  lv_event_code_t code = lv_event_get_code(e);
  if(code == LV_EVENT_CLICKED) {
    lv_obj_move_foreground(content);
  }
}

static lv_obj_t * create_content(lv_obj_t * parent)
{
  lv_obj_t * content = lv_obj_create(parent);

  lv_obj_set_size(content, SCREEN_WIDTH, LV_PCT(SCREEN_CONTENT_PCT));
  lv_obj_set_style_border_width(content, 0, LV_PART_MAIN);
  lv_obj_set_style_radius(content, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(content, 0, LV_PART_MAIN);
  lv_obj_align(content, LV_ALIGN_TOP_MID, 0, 0);

  return content;
}

static lv_obj_t * create_navbar(lv_obj_t * parent)
{
  lv_obj_t * navbar = lv_obj_create(parent);

  lv_obj_set_size(navbar, SCREEN_WIDTH, LV_PCT(SCREEN_NAVBAR_PCT));
  lv_obj_set_style_bg_color(navbar, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_set_style_border_width(navbar, 0, LV_PART_MAIN);
  lv_obj_set_style_radius(navbar, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(navbar, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_row(navbar, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_column(navbar, 0, LV_PART_MAIN);

  lv_obj_set_layout(navbar, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(navbar, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(navbar, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_align(navbar, LV_ALIGN_BOTTOM_MID, 0,0);

  return navbar;
}

static lv_obj_t * create_navbar_button(lv_obj_t * parent, lv_obj_t * content, char * label, lv_color_t bg_color)
{
  lv_obj_t * btn = lv_btn_create(parent);
  lv_obj_add_event_cb(btn, button_event_callback, LV_EVENT_ALL, content);

  lv_obj_set_height(btn, LV_PCT(100));
  lv_obj_set_flex_grow(btn, 1);
  lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_radius(btn, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(btn, bg_color, LV_PART_MAIN);

  lv_obj_t * lbl = lv_label_create(btn);
  lv_label_set_text(lbl, label);
  lv_obj_center(lbl);

  return btn;
}

static lv_obj_t * create_content_home(lv_obj_t * parent)
{
  lv_obj_t * content = create_content(parent);

  lv_obj_set_size(content, LV_PCT(100), LV_PCT(100));
  lv_obj_set_style_bg_color(content, lv_color_make(0, 128, 255), LV_PART_MAIN);
  lv_obj_align(content, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t * label = lv_label_create(content);
  lv_label_set_text(label, "103.5 DAWN FM");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  return content;
}

static lv_obj_t * create_content_navigation(lv_obj_t * parent)
{
  lv_obj_t * content = create_content(parent);

  lv_obj_set_size(content, LV_PCT(100), LV_PCT(100));
  lv_obj_set_style_bg_color(content, lv_color_make(0, 255, 128), LV_PART_MAIN);
  lv_obj_align(content, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t * label = lv_label_create(content);
  lv_label_set_text(label, "103.5 DAWN FM");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  return content;
}

static lv_obj_t * create_content_bluetooth(lv_obj_t * parent)
{
  lv_obj_t * content = create_content(parent);

  lv_obj_set_size(content, LV_PCT(100), LV_PCT(100));
  lv_obj_set_style_bg_color(content, lv_color_make(255, 128, 0), LV_PART_MAIN);
  lv_obj_align(content, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t * label = lv_label_create(content);
  lv_label_set_text(label, "103.5 DAWN FM");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  return content;
}

static lv_obj_t * create_content_settings(lv_obj_t * parent)
{
  lv_obj_t * content = create_content(parent);

  lv_obj_set_size(content, LV_PCT(100), LV_PCT(100));
  lv_obj_set_style_bg_color(content, lv_color_make(128, 0, 255), LV_PART_MAIN);
  lv_obj_align(content, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t * label = lv_label_create(content);
  lv_label_set_text(label, "103.5 DAWN FM");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  return content;
}

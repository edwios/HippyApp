#pragma once
#include <pebble.h>

#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1
#define KEY_ROOM 2
#define KEY_SELECTION 3
  // Note: KEY_CLCOLOR starts with 1 (Red)
#define KEY_CLCOLOR 4
#define KEY_NOUPDATE 5
#define KEY_UPDATETYPE 6
  
#define NUM_MENU_SECTIONS 1
#define NUM_MENU_ICONS 0
#define NUM_FIRST_MENU_ITEMS 12
#define NUM_SECOND_MENU_ITEMS 0
  
static Window *main_window;
static TextLayer *time_layer;
static TextLayer *date_layer;
static TextLayer *text_layer;
static TextLayer *weather_layer;
static TextLayer *weather_layer2;
static TextLayer *CheerlightsLayer;
static InverterLayer *inv_layer;
static InverterLayer *inv_layer2;
static Layer *graphics_layer;
static MenuLayer *menu_layer;
static Layer *window_layer;
static char *CLMenu[13] = {"Red","Green","Blue","Cyan","White","Warmwhite","Purple","Magenta","Yellow","Orange","Pink","Oldlace"};
int CLColor;

void force_back_button(Window *window, MenuLayer *menu_layer);
static void click_config_provider(void *context);
static void back_click_handler(ClickRecognizerRef recognizer, void *context);
void new_ccp(void *context);
void force_back_button(Window *window, MenuLayer *menu_layer);
void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data);
static void refresh_watch();

#include "main.h"


// Menus
// A callback is used to specify the amount of sections of menu items
// With this, you can dynamically add and remove sections
static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

// Each section has a number of items;  we use a callback to specify this
// You can also dynamically add and remove items using this
static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      return NUM_FIRST_MENU_ITEMS;

    case 1:
      return NUM_SECOND_MENU_ITEMS;

    default:
      return 0;
  }
}

// A callback is used to specify the height of the section header
static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  // This is a define provided in pebble.h that you may use for the default height
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

// Here we draw what each header is
static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  // Determine which section we're working with
  switch (section_index) {
    case 0:
      // Draw title text in the section header
      menu_cell_basic_header_draw(ctx, cell_layer, "Cheerlights");
      break;

    case 1:
      menu_cell_basic_header_draw(ctx, cell_layer, "Unused");
      break;
  }
}

// This is the menu item draw callback where you specify what each item should look like
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  // Determine which section we're going to draw in
  switch (cell_index->section) {
    case 0:
      // Use the row to specify which item we'll draw
      // menu_cell_title_draw(ctx, cell_layer, CLMenu[cell_index->row]);
    menu_cell_basic_draw(ctx, cell_layer, CLMenu[cell_index->row], (CLColor==(cell_index->row+1))?"Selected":"", NULL);
    break;
  }
}

// We need to save a reference to the ClickConfigProvider originally set by the menu layer
ClickConfigProvider previous_ccp;


static void up_click_handler(ClickRecognizerRef recognizer, void *context) 
{}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) 
{
  layer_remove_from_parent (text_layer_get_layer(time_layer));
  layer_remove_from_parent (text_layer_get_layer(date_layer));
  layer_remove_from_parent (text_layer_get_layer(text_layer));
  layer_remove_from_parent (text_layer_get_layer(weather_layer));
  layer_remove_from_parent (text_layer_get_layer(weather_layer2));
  layer_remove_from_parent (text_layer_get_layer(CheerlightsLayer));
  layer_remove_from_parent (inverter_layer_get_layer(inv_layer));
  layer_remove_from_parent (inverter_layer_get_layer(inv_layer2));
  layer_remove_from_parent (graphics_layer);
  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
    // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(menu_layer, context);
  force_back_button(context, menu_layer);
}


static void select_click_handler(ClickRecognizerRef recognizer, void *context) 
{
//    Window *window = (Window *)context;
}


static void click_config_provider(void *context) 
{
//  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
//  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
//  window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
}

static void back_click_handler(ClickRecognizerRef recognizer, void *context) 
{
  layer_remove_from_parent (menu_layer_get_layer(menu_layer));
  layer_add_child(window_layer, text_layer_get_layer(time_layer));
  layer_add_child(window_layer, text_layer_get_layer(date_layer));
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  layer_add_child(window_layer, text_layer_get_layer(weather_layer));
  layer_add_child(window_layer, text_layer_get_layer(weather_layer2));
  layer_add_child(window_layer, text_layer_get_layer(CheerlightsLayer));
  layer_add_child(window_layer, inverter_layer_get_layer(inv_layer));
  layer_add_child(window_layer, inverter_layer_get_layer(inv_layer2));
  layer_add_child(window_layer, graphics_layer);
  window_set_click_config_provider(main_window, click_config_provider);
}


// This is the new ClickConfigProvider we will set, it just calls the old one and then subscribe
// for back button events.
void new_ccp(void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "calling the new ccp");
  previous_ccp(context);
  window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "done in the new ccp");
}

// Call this from your init function to do the hack
void force_back_button(Window *window, MenuLayer *menu_layer) {
  previous_ccp = window_get_click_config_provider(window);
  window_set_click_config_provider_with_context(window, new_ccp, menu_layer);
}



// Here we capture when a user selects a menu item
void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  // Use the row to specify which item will receive the select action
  CLColor = cell_index->row+1;
  layer_mark_dirty(menu_layer_get_layer(menu_layer));
  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  // Add a key-value pair
  Tuplet value = TupletInteger(KEY_SELECTION, CLColor);
  dict_write_tuplet(iter, &value);  // Send the message!
  app_message_outbox_send();
}

static void graphics_update_proc(Layer *this_layer, GContext *ctx) {
  GPoint p0 = GPoint(0, 100);
  GPoint p0A = GPoint(0, 101);
  GPoint p1 = GPoint(144, 100);
  GPoint p1A = GPoint(144, 101);
  GPoint p2 = GPoint(50, 100);
  GPoint p2A = GPoint(51, 100);
  GPoint p3 = GPoint(50, 168);
  GPoint p3A = GPoint(51, 168);
  GPoint p4 = GPoint(0, 84);
  GPoint p4A = GPoint(0, 81);
  GPoint p5 = GPoint(144, 84);
  GPoint p5A = GPoint(144, 81);

  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, p0, p1);
  graphics_draw_line(ctx, p0A, p1A);
  graphics_draw_line(ctx, p2, p3);
  graphics_draw_line(ctx, p2A, p3A);
  graphics_draw_line(ctx, p4, p5);

}

static void main_window_load(Window *win) {
  GRect window_bounds = layer_get_bounds(window_get_root_layer(win));

  text_layer = text_layer_create(GRect(0,0,144,45));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorBlack);
  text_layer_set_text(text_layer, "Hippy");
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(win), text_layer_get_layer(text_layer));
 
  time_layer = text_layer_create(GRect(0,35,144,45));
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorBlack);
  text_layer_set_text(time_layer, "00:00");
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(win), text_layer_get_layer(time_layer));

  date_layer = text_layer_create(GRect(0,80,144,20));
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorBlack);
  text_layer_set_text(date_layer, "Thu\n4");
  text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(win), text_layer_get_layer(date_layer));

  weather_layer = text_layer_create(GRect(0, 100, 50, 68));
  text_layer_set_background_color(weather_layer, GColorClear);
  text_layer_set_text_color(weather_layer, GColorBlack); 
  text_layer_set_text(weather_layer, "");
  text_layer_set_font(weather_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(weather_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(win), text_layer_get_layer(weather_layer));

  weather_layer2 = text_layer_create(GRect(50, 100, 94, 30));
  text_layer_set_background_color(weather_layer2, GColorClear);
  text_layer_set_text_color(weather_layer2, GColorBlack); 
  text_layer_set_text(weather_layer2, "");
  text_layer_set_font(weather_layer2, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(weather_layer2, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(win), text_layer_get_layer(weather_layer2));

  CheerlightsLayer = text_layer_create(GRect(50, 130, 94, 38));
  text_layer_set_background_color(CheerlightsLayer, GColorClear);
  text_layer_set_text_color(CheerlightsLayer, GColorBlack); 
  text_layer_set_text(CheerlightsLayer, "");
  text_layer_set_font(CheerlightsLayer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(CheerlightsLayer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(win), text_layer_get_layer(CheerlightsLayer));

  inv_layer = inverter_layer_create(GRect(50,132,94,36));
  layer_add_child(window_get_root_layer(win), inverter_layer_get_layer(inv_layer));
  inv_layer2 = inverter_layer_create(GRect(0,0,144,45));
  layer_add_child(window_get_root_layer(win), inverter_layer_get_layer(inv_layer2));

  // Create a Graphics Layer and set the update_proc
  graphics_layer = layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  layer_add_child(window_get_root_layer(win), graphics_layer);
  layer_set_update_proc(graphics_layer, graphics_update_proc);

  //Cheerlights Menu
  // Now we prepare to initialize the menu layer
  // We need the bounds to specify the menu layer's viewport size
  // In this case, it'll be the same as the window's
  window_layer = window_get_root_layer(win);
  GRect bounds = layer_get_frame(window_layer);

  // Create the menu layer
  menu_layer = menu_layer_create(bounds);

  // Set all the callbacks for the menu layer
  menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });

}

static void main_window_unload(Window *win) {
  text_layer_destroy(time_layer);
  text_layer_destroy(date_layer);
  text_layer_destroy(text_layer);
  text_layer_destroy(weather_layer);
  text_layer_destroy(weather_layer2);
  text_layer_destroy(CheerlightsLayer);
  inverter_layer_destroy(inv_layer);
  inverter_layer_destroy(inv_layer2);
  layer_destroy(graphics_layer);
  menu_layer_destroy(menu_layer);
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  // Create a long-lived buffer
  static char buffer[] = "00:00";
  static char datebuf[] = "Mon\n31";
  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
    strftime(datebuf, sizeof("Mon 31"), "%a %e", tick_time);
  // Display this time on the TextLayer
  text_layer_set_text(time_layer, buffer);
  text_layer_set_text(date_layer, datebuf);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  // Get weather update every 30 minutes
  if(tick_time->tm_min % 10 == 0) {
      refresh_watch();
  }
}

void tap_handler(AccelAxisType accel, int32_t direction) {
    // handle tap here
  // Add Menu to the window for display
  // Bug: Menu added but overlayed on yop of the text layers :(
//  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
//  layer_insert_above_sibling (menu_layer_get_layer(menu_layer), text_layer_get_layer(time_layer));

}

static void refresh_watch() {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    // Add a key-value pair
    Tuplet value = TupletInteger(KEY_SELECTION, 0);
    dict_write_tuplet(iter, &value);  // Send the message!

    // Send the message!
    app_message_outbox_send();

}

// App Messages
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char temperature_buffer[8];
  static char conditions_buffer[8];
  static char room_buffer[8];
  static char weather_layer_buffer[10];
  static char weather_layer2_buffer[10];
  static char cheerlights_buffer[10];
  int noupdate = 0;
  int _t = 0;
  int _h = 0;
  int _r = 0;
  char degC = 'C';
  int updatetype = 0;
  
  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
      case KEY_UPDATETYPE:
        updatetype = t->value->int32;
      case KEY_NOUPDATE:
        noupdate = (int)t->value->int32;
        break;
      case KEY_TEMPERATURE:
        _t = (int)t->value->int32;
        break;
      case KEY_CONDITIONS:
        _h = (int)t->value->int32;
        break;
      case KEY_ROOM:
        _r = (int)t->value->int32;
        break;
      case KEY_CLCOLOR:
        CLColor = (int)t->value->int32;
        break;
      default:
        APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
        break;
    }
    // Look for next item
    t = dict_read_next(iterator);
  }
//  APP_LOG(APP_LOG_LEVEL_INFO, "Room %d!", (int)room_buffer);
  if (updatetype == 1) {
    if (noupdate == 1)
      degC = 'c';
    else
      degC = 'C';
    snprintf(temperature_buffer, sizeof(temperature_buffer), "%d%c", _t, degC);
    snprintf(conditions_buffer, sizeof(conditions_buffer), "%d.%d%%", _h/10, _h%10);
  }
  if (updatetype == 2) {
    if (noupdate == 1)
      degC = 'c';
    else
      degC = 'C';
    snprintf(room_buffer, sizeof(room_buffer), "%d%c", _r, degC);
  }
  // Assemble full string and display
  snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s\n%s", temperature_buffer, room_buffer);
  text_layer_set_text(weather_layer, weather_layer_buffer);
  snprintf(weather_layer2_buffer, sizeof(weather_layer2_buffer), "%s", conditions_buffer);
  text_layer_set_text(weather_layer2, weather_layer2_buffer);
  snprintf(cheerlights_buffer, sizeof(cheerlights_buffer), "%s", (CLColor==0)?"NA":CLMenu[CLColor-1]);
  text_layer_set_text(CheerlightsLayer, cheerlights_buffer);
//  APP_LOG(APP_LOG_LEVEL_DEBUG, weather_layer_buffer);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
    // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Sent dict: %d", (int)t->key);
    // Which key was received?
    switch(t->key) {
    case KEY_SELECTION:
      if (t->value->int32 > 0) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Refresh watch face");
        refresh_watch();
      }
      break;
    default:
      break;
    }
    t = dict_read_next(iterator);
  }
}

static void register_callbacks() {
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

}

static void init() {
  main_window = window_create();
  window_set_window_handlers(main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_set_fullscreen(main_window, true);
  //Clicks
  window_set_click_config_provider(main_window, click_config_provider);
  window_stack_push(main_window, true);
  // Make sure the time is displayed from the start
  update_time();
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  accel_tap_service_subscribe(tap_handler);
  // Register message callbacks
  register_callbacks();
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

}

static void deinit() {
    // Destroy Window
    window_destroy(main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
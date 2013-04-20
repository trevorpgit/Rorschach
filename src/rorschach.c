#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID { 0xD1, 0x15, 0x8E, 0x2B, 0x38, 0x8E, 0x43, 0xFA, 0x8E, 0xFC, 0x9C, 0xD0, 0x3E, 0x8B, 0x4E, 0x3F }
PBL_APP_INFO(MY_UUID,
	     "Rorschach", "ZissCo",
	     3, 0, /* App major/minor version */
	     RESOURCE_ID_IMAGE_MENU_ICON,
	     APP_INFO_WATCH_FACE);


Window window;

BmpContainer background_image;

Layer layer;


const int NUMBER_IMAGE_RESOURCE_IDS[10][2] = {
  {RESOURCE_ID_IMAGE_0_WHITE,RESOURCE_ID_IMAGE_0_BLACK},
  {RESOURCE_ID_IMAGE_1_WHITE,RESOURCE_ID_IMAGE_1_BLACK},
  {RESOURCE_ID_IMAGE_2_WHITE,RESOURCE_ID_IMAGE_2_BLACK},
  {RESOURCE_ID_IMAGE_3_WHITE,RESOURCE_ID_IMAGE_3_BLACK},
  {RESOURCE_ID_IMAGE_4_WHITE,RESOURCE_ID_IMAGE_4_BLACK},
  {RESOURCE_ID_IMAGE_5_WHITE,RESOURCE_ID_IMAGE_5_BLACK},
  {RESOURCE_ID_IMAGE_6_WHITE,RESOURCE_ID_IMAGE_6_BLACK},
  {RESOURCE_ID_IMAGE_7_WHITE,RESOURCE_ID_IMAGE_7_BLACK},
  {RESOURCE_ID_IMAGE_8_WHITE,RESOURCE_ID_IMAGE_8_BLACK},
  {RESOURCE_ID_IMAGE_9_WHITE,RESOURCE_ID_IMAGE_9_BLACK}
};

#define TOTAL_TIME_DIGITS 4
RotBmpPairContainer  time_digits_images[TOTAL_TIME_DIGITS];

const int MIRROR_NUMBER_IMAGE_RESOURCE_IDS[10][2] = {
  {RESOURCE_ID_IMAGE_MIRROR_0_WHITE,RESOURCE_ID_IMAGE_MIRROR_0_BLACK},
  {RESOURCE_ID_IMAGE_MIRROR_1_WHITE,RESOURCE_ID_IMAGE_MIRROR_1_BLACK},
  {RESOURCE_ID_IMAGE_MIRROR_2_WHITE,RESOURCE_ID_IMAGE_MIRROR_2_BLACK},
  {RESOURCE_ID_IMAGE_MIRROR_3_WHITE,RESOURCE_ID_IMAGE_MIRROR_3_BLACK},
  {RESOURCE_ID_IMAGE_MIRROR_4_WHITE,RESOURCE_ID_IMAGE_MIRROR_4_BLACK},
  {RESOURCE_ID_IMAGE_MIRROR_5_WHITE,RESOURCE_ID_IMAGE_MIRROR_5_BLACK},
  {RESOURCE_ID_IMAGE_MIRROR_6_WHITE,RESOURCE_ID_IMAGE_MIRROR_6_BLACK},
  {RESOURCE_ID_IMAGE_MIRROR_7_WHITE,RESOURCE_ID_IMAGE_MIRROR_7_BLACK},
  {RESOURCE_ID_IMAGE_MIRROR_8_WHITE,RESOURCE_ID_IMAGE_MIRROR_8_BLACK},
  {RESOURCE_ID_IMAGE_MIRROR_9_WHITE,RESOURCE_ID_IMAGE_MIRROR_9_BLACK}
};

RotBmpPairContainer  mirror_time_digits_images[TOTAL_TIME_DIGITS];

int offset[10][2]=
	{
		//0       1         2        3         4         5       6         7       8       9
		{0,0}, {-1,-11}, {-6,-6}, {-6,-6}, {-12,-13}, {-6,-6}, {2,-3}, {-11,-5}, {0,0}, {-8,-3}
	};
	
int moffset[10][2]=
	{
		//0       1         2        3         4         5       6         7       8       9
		{0,0}, {-1,-11}, {-6,-6}, {-6,-6}, {-12,-13}, {-6,-6}, {-8,-3}, {-11,-5}, {0,0}, {2,-3}
	};

void set_container_image(RotBmpPairContainer  *bmp_container, const int resource_id_white, const int resource_id_black, GPoint origin, int digit, int position) {
 
  layer_remove_from_parent(&bmp_container->layer.layer);
  rotbmp_pair_deinit_container(bmp_container);

  rotbmp_pair_init_container(resource_id_white, resource_id_black, bmp_container);

  GRect frame = layer_get_frame(&bmp_container->layer.layer);
  frame.origin.x = origin.x - 13;
  frame.origin.y = origin.y;
  if (position == 2 && digit == 1) {
	frame.origin.x -= 16;
  }
  layer_set_frame(&bmp_container->layer.layer, frame);

  layer_add_child(&window.layer, &bmp_container->layer.layer);
}


unsigned short get_display_hour(unsigned short hour) {

  if (clock_is_24h_style()) {
    return hour;
  }

  unsigned short display_hour = hour % 12;

  // Converts "0" to "12"
  return display_hour ? display_hour : 12;
}


void update_display(PblTm *current_time) {
  unsigned short display_hour = get_display_hour(current_time->tm_hour);

  int h1 = display_hour/10;
  set_container_image(&time_digits_images[0], NUMBER_IMAGE_RESOURCE_IDS[h1][0],NUMBER_IMAGE_RESOURCE_IDS[h1][1], GPoint(73 + offset[h1][0], 32 + offset[h1][1]), h1, 1);
  set_container_image(&mirror_time_digits_images[0], MIRROR_NUMBER_IMAGE_RESOURCE_IDS[h1][0],MIRROR_NUMBER_IMAGE_RESOURCE_IDS[h1][1], GPoint(42 + moffset[h1][0], 32 + moffset[h1][1]), h1, 2);

  int h2 = display_hour%10;
  set_container_image(&time_digits_images[1], NUMBER_IMAGE_RESOURCE_IDS[h2][0],NUMBER_IMAGE_RESOURCE_IDS[h2][1], GPoint(104 + offset[h2][0], 32 + offset[h2][1]), h2, 2);
  set_container_image(&mirror_time_digits_images[1], MIRROR_NUMBER_IMAGE_RESOURCE_IDS[h2][0],MIRROR_NUMBER_IMAGE_RESOURCE_IDS[h2][1], GPoint(11 + moffset[h2][0], 32 + moffset[h2][1]), h2, 1);

  int m1 = current_time->tm_min/10;
  set_container_image(&time_digits_images[2], NUMBER_IMAGE_RESOURCE_IDS[m1][0],NUMBER_IMAGE_RESOURCE_IDS[m1][1], GPoint(11 + offset[m1][0], 85 + offset[m1][1]), m1, 1);
  set_container_image(&mirror_time_digits_images[2], MIRROR_NUMBER_IMAGE_RESOURCE_IDS[m1][0],MIRROR_NUMBER_IMAGE_RESOURCE_IDS[m1][1], GPoint(104 + moffset[m1][0], 85 + moffset[m1][1]), m1, 2);

  int m2 = current_time->tm_min%10;
  set_container_image(&time_digits_images[3], NUMBER_IMAGE_RESOURCE_IDS[m2][0],NUMBER_IMAGE_RESOURCE_IDS[m2][1], GPoint(42 + offset[m2][0], 85 + offset[m2][1]), m2, 2);
  set_container_image(&mirror_time_digits_images[3], MIRROR_NUMBER_IMAGE_RESOURCE_IDS[m2][0],MIRROR_NUMBER_IMAGE_RESOURCE_IDS[m2][1], GPoint(73 + moffset[m2][0], 85 + moffset[m2][1]), m2, 1);
}


void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)ctx;
  
  update_display(t->tick_time);
}

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Rorschach");
  window_stack_push(&window, false /* Animated */);

  window_set_background_color(&window, GColorWhite);
  
  resource_init_current_app(&APP_RESOURCES);

  layer_mark_dirty(&layer);

  // Avoids a blank screen on watch start.
  PblTm tick_time;

  get_time(&tick_time);
  update_display(&tick_time);

}


void handle_deinit(AppContextRef ctx) {
  (void)ctx;

  bmp_deinit_container(&background_image);

  for (int i = 0; i < TOTAL_TIME_DIGITS; i++) {
    rotbmp_pair_deinit_container(&time_digits_images[i]);
  }

  for (int i = 0; i < TOTAL_TIME_DIGITS; i++) {
    rotbmp_pair_deinit_container(&mirror_time_digits_images[i]);
  }

}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,

    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }
  };
  app_event_loop(params, &handlers);
}

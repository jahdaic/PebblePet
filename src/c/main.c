#include <pebble.h>
#include "helpers.h"
#include "animations.h"
	
// PET VARIABLES
enum stages
{
	EGG = 0,
	BABY = 1,
	TEEN = 2,
	ADULT = 3
};

enum status_codes
{
	DEAD = 0,
	SICK = 1,
	UNHAPPY = 2,
	HEALTHY = 3
};

int stage = EGG;
int health = 10;
int hunger = 5;
int sickness = 0;
int status = HEALTHY;
int poops = 0;
time_t start_time;

unsigned int sick_time = 0;
unsigned int feed_times[5] = {0};
unsigned int work_times[5] = {0};
unsigned int last_hunger = 0;
unsigned int last_poop = 0;
unsigned int last_cleaned = 0;
unsigned int last_poop_sick = 0;

enum time_spans
{
	EGG_DURATION = 60, //1800,	// 30 minutes
	BABY_DURATION = 300, //432000,		// 5 days
	TEEN_DURATION = 1296000,	// 15 days
	POOP_INTERVAL = 300, //7200,		// 2 hours
	HUNGER_INTERVAL = 150, //5400		// 1.5 hours
	MAX_WORK = 3600,
	MAX_HEAL = 3600,
	MAX_FEED = 3600,
	MAX_POOP = 3600,
	MAX_SICK = 43200
};
	
// UI VARIABLES
bool development = true;
enum menu_items
{
	STATS = 0,
	FOOD = 1,
	MEDICINE = 2,
	POOP = 3,
	WORK = 4,
	RESTART = 7
};

Window *main_window;
BitmapLayer *sprite_layer;
BitmapLayer *status_layer;
BitmapLayer *bg_layer;
InverterLayer *selection_layer;
GBitmap *sprite_bitmap = NULL;
GBitmap *status_bitmap = NULL;
GBitmap *bg_bitmap = NULL;
int sprite_animation = 0;
TextLayer *text_layer;
int current_menu_item = STATS;

// PERSISTENT STORAGE KEYS
enum storage_keys
{
	AGE,
	START_TIME,
	STAGE,
	STATUS,
	HEALTH,
	HUNGER,
	SICKNESS,
	POOPS,
	SICK_TIME,
	FEED_TIMES,
	WORK_TIMES,
	LAST_HUNGER,
	LAST_POOP,
	LAST_CLEANED,
	LAST_POOP_SICK
};

static void write_log(void)
{
	if(development)
	{
		char *text_buffer = "12345678\0";
		clock_copy_time_string(text_buffer, strlen(text_buffer) + 1);
		
		APP_LOG(APP_LOG_LEVEL_INFO, strcat(text_buffer, " STAGE: %d, STATUS: %d, HEALTH: %d, HUNGER: %d, SICKNESS: %d, POOPS: %d"), stage, status, health, hunger, sickness, poops);
	}
}

static void check_status(void)
{
}

static void draw(void)
{
	// Animate the Pet Sprite
	gbitmap_destroy(sprite_bitmap);
	sprite_animation++;
	
	if(status == DEAD)
	{
		if(sprite_animation > 2) {sprite_animation = 1;}
		
		switch(sprite_animation)
		{
			case 1:
				sprite_bitmap = gbitmap_create_with_resource(RESOURCE_ID_DEAD_SPRITE_1);
				break;
			case 2:
				sprite_bitmap = gbitmap_create_with_resource(RESOURCE_ID_DEAD_SPRITE_2);
				break;
		}
		
		bitmap_layer_set_alignment(sprite_layer, GAlignCenter);
		
		// Add Text Layer
		text_layer_destroy(text_layer);
		text_layer = text_layer_create(GRect(0, 120, 144, 48));
		layer_add_child(window_get_root_layer(main_window), text_layer_get_layer(text_layer));
		text_layer_set_background_color(text_layer, GColorWhite);
		text_layer_set_text_color(text_layer, GColorBlack);
		text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
		text_layer_set_text(text_layer, "\nTry Again");
		
		// Add Inverted Selection Layer
		inverter_layer_destroy(selection_layer);
		selection_layer = inverter_layer_create(GRect(36, 132, 72, 24));
		layer_add_child(window_get_root_layer(main_window), inverter_layer_get_layer(selection_layer));
	}
	else if(stage == EGG)
	{
		if(sprite_animation > 4) {sprite_animation = 1;}
		
		switch(sprite_animation)
		{
			case 1:
				sprite_bitmap = gbitmap_create_with_resource(RESOURCE_ID_EGG_SPRITE_1);
				break;
			case 2:
				sprite_bitmap = gbitmap_create_with_resource(RESOURCE_ID_EGG_SPRITE_2);
				break;
			case 3:
				sprite_bitmap = gbitmap_create_with_resource(RESOURCE_ID_EGG_SPRITE_1);
				break;
			case 4:
				sprite_bitmap = gbitmap_create_with_resource(RESOURCE_ID_EGG_SPRITE_3);
				break;
		}
	}
	else if(stage == BABY)
	{		
		if(sprite_animation > 6) {sprite_animation = 1;}
		
		switch(sprite_animation)
		{
			case 1:
				sprite_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BABY_SPRITE_1);
				bitmap_layer_set_alignment(sprite_layer, GAlignCenter);
				break;
			case 2:
				sprite_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BABY_SPRITE_1);
				bitmap_layer_set_alignment(sprite_layer, GAlignLeft);
				break;
			case 3:
				sprite_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BABY_SPRITE_2);
				bitmap_layer_set_alignment(sprite_layer, GAlignLeft);
				break;
			case 4:
				sprite_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BABY_SPRITE_2);
				bitmap_layer_set_alignment(sprite_layer, GAlignCenter);
				break;
			case 5:
				sprite_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BABY_SPRITE_2);
				bitmap_layer_set_alignment(sprite_layer, GAlignRight);
				break;
			case 6:
				sprite_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BABY_SPRITE_1);
				bitmap_layer_set_alignment(sprite_layer, GAlignRight);
				break;
		}
	}
	else if(stage == TEEN)
	{		
		if(sprite_animation > 6) {sprite_animation = 1;}
		
		switch(sprite_animation)
		{
			case 1:
				sprite_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TEEN_SPRITE_1);
				bitmap_layer_set_alignment(sprite_layer, GAlignCenter);
				break;
			case 2:
				sprite_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TEEN_SPRITE_1);
				bitmap_layer_set_alignment(sprite_layer, GAlignLeft);
				break;
			case 3:
				sprite_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TEEN_SPRITE_2);
				bitmap_layer_set_alignment(sprite_layer, GAlignLeft);
				break;
			case 4:
				sprite_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TEEN_SPRITE_2);
				bitmap_layer_set_alignment(sprite_layer, GAlignCenter);
				break;
			case 5:
				sprite_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TEEN_SPRITE_2);
				bitmap_layer_set_alignment(sprite_layer, GAlignRight);
				break;
			case 6:
				sprite_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TEEN_SPRITE_1);
				bitmap_layer_set_alignment(sprite_layer, GAlignRight);
				break;
		}
	}
	
	if(status == HEALTHY)
	{
		status_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BLANK_SPRITE);
	}
	else if(status == SICK)
	{
		switch(sprite_animation)
		{
			case 1:
				status_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SICK_SPRITE_1);
				bitmap_layer_set_alignment(status_layer, GAlignCenter);
				break;
			case 2:
				status_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SICK_SPRITE_1);
				bitmap_layer_set_alignment(status_layer, GAlignLeft);
				break;
			case 3:
				status_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SICK_SPRITE_2);
				bitmap_layer_set_alignment(status_layer, GAlignLeft);
				break;
			case 4:
				status_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SICK_SPRITE_2);
				bitmap_layer_set_alignment(status_layer, GAlignCenter);
				break;
			case 5:
				status_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SICK_SPRITE_2);
				bitmap_layer_set_alignment(status_layer, GAlignRight);
				break;
			case 6:
				status_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SICK_SPRITE_1);
				bitmap_layer_set_alignment(status_layer, GAlignRight);
				break;
		}
		
	}
	else if(status == UNHAPPY)
	{
		;
	}
	
	// Write out the time (so I don't have to exit the app to look at the time. I'm lazy)
	if(status != DEAD)
	{
		char *text_buffer = "12345678901234567890\0";
		clock_copy_time_string(text_buffer, strlen(text_buffer) + 1);
		text_layer_set_text(text_layer, text_buffer);
	}
	
	layer_mark_dirty(window_get_root_layer(main_window));
}

static void update_status(void)
{
	if(status != DEAD)
	{
		time_t current_time = time(NULL);
		
		// TRANSFORMATIONS
		if(stage == EGG)
		{
			if((int)current_time > ((int)start_time + EGG_DURATION))
			{
				light_enable_interaction();
				vibes_long_pulse();
				stage = BABY;
				write_log();
			}
		}
		else if(stage == BABY)
		{
			if((int)current_time > ((int)start_time + BABY_DURATION))
			{
				light_enable_interaction();
				vibes_long_pulse();
				stage = TEEN;
				write_log();
			}		
		}
		else if(stage == TEEN)
		{
			if((int)current_time > ((int)start_time + TEEN_DURATION))
			{
				light_enable_interaction();
				vibes_long_pulse();
				stage = ADULT;
			}		
		}
		
		// DEATH
		if(status == SICK && (int)current_time >  ((int)sick_time + MAX_SICK))
		{
			status = DEAD;
			write_log();
		}
		
		// POOPS
		if((int)current_time > ((int)last_poop + POOP_INTERVAL) && stage != EGG)
		{
			if(poops < 4)
			{
				poops++;
				write_log();
			}
			last_poop = current_time;
		}
		
		// HUNGER
		if((int)current_time > ((int)last_hunger + HUNGER_INTERVAL) && (int)current_time > ((int)get_latest(feed_times) + HUNGER_INTERVAL) && stage != EGG)
		{
			if(hunger > 0)
			{
				hunger--;
				write_log();
			}
			last_hunger = current_time;
		}
		
		// SICKNESS
		if(poops >= POOP_INTERVAL/150 && (int)current_time > ((int)last_poop_sick + MAX_POOP))
		{
			if(sickness < 3)
			{
				sickness++;
				last_poop_sick = current_time;
				
				if(sickness == 3)
				{
					status = SICK;
					
				}
				if(!sick_time)
				{
					sick_time = current_time;
				}
				
				write_log();
			}
		}
	}
}

static void app_loop(void *data)
{
	check_status();
	draw();
	update_status();
	
	app_timer_register(1000, app_loop, NULL);
}

void feed_pet(void)
{
	unsigned int current_time = time(NULL);
	
	if(stage != EGG && status != DEAD)
	{
		push(feed_times, time(NULL));
		
		if(hunger < 5)
		{
			hunger++;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		feed_animation(&sprite_animation, status_bitmap, status_layer);
		//sprite_animation = 1;
	}
	
	if((int)current_time < (int)feed_times[1] + MAX_FEED && (int)feed_times[1] != 0)
	{
		if(sickness < 3)
		{
			sickness++;
			
			if(sickness == 3)
			{
				status = SICK;
				
			}
			if(!sick_time)
			{
				sick_time = current_time;
			}
		}
		
		write_log();
	}	
}

void heal_pet(void)
{
	if(stage != EGG && status != DEAD)
	{
		sickness--;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
}

void clean_pet(void)
{
	if(stage != EGG && status != DEAD)
	{
		poops = 0;
		last_cleaned = time(NULL);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}	
}

void work_pet(void)
{
	unsigned int current_time = time(NULL);
	
	if(stage != EGG && status != DEAD)
	{
		push(work_times, time(NULL));
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}	
	
	if((int)current_time < (int)work_times[1] + MAX_WORK && (int)work_times[1] != 0)
	{
		if(sickness < 3)
		{
			sickness++;
			
			if(sickness == 3)
			{
				status = SICK;
				
			}
			if(!sick_time)
			{
				sick_time = current_time;
			}
		}		
	}
}

void draw_menu(void)
{	
	GRect bounds;
	
	switch(current_menu_item)
	{
		case STATS:
			bounds = GRect(0, 120, 36, 24);
			break;
		case FOOD:
			bounds = GRect(36, 120, 36, 24);
			break;
		case MEDICINE:
			bounds = GRect(72, 120, 36, 24);
			break;
		case POOP:
			bounds = GRect(108, 120, 36, 24);
			break;
		case WORK:
			bounds = GRect(0, 144, 36, 24);
			break;
		case 5:
			bounds = GRect(36, 144, 36, 24);
			break;
		case 6:
			bounds = GRect(72, 144, 36, 24);
			break;
		case RESTART:
			bounds = GRect(108, 144, 36, 24);
			break;
	}
	
	APP_LOG(APP_LOG_LEVEL_INFO, "Menu Item: %d", current_menu_item);
		
	inverter_layer_destroy(selection_layer);
	selection_layer = inverter_layer_create(bounds);
	layer_add_child(window_get_root_layer(main_window), inverter_layer_get_layer(selection_layer));
	
	layer_mark_dirty(window_get_root_layer(main_window));
}

static void restart(void)
{
	// Delete All Persistant Storage
	persist_delete(START_TIME);
	persist_delete(STAGE);
	persist_delete(STATUS);
	persist_delete(HEALTH);
	persist_delete(HUNGER);
	persist_delete(SICKNESS);
	persist_delete(POOPS);
	persist_delete(SICK_TIME);
	persist_delete(FEED_TIMES);
	persist_delete(WORK_TIMES);
	persist_delete(LAST_HUNGER);
	persist_delete(LAST_POOP);
	persist_delete(LAST_CLEANED);
	persist_delete(LAST_POOP_SICK);
	
	// Reset Pet Status
	stage = EGG;
	health = 10;
	hunger = 5;
	sickness = 0;
	status = HEALTHY;
	poops = 0;
	start_time = time(NULL);
	
	// Clear All Activity
	sick_time = 0;
	memset(feed_times, 0,sizeof(feed_times));
	memset(work_times,0,sizeof(work_times));
	last_hunger = 0;
	last_poop = 0;
	last_cleaned = 0;
	last_poop_sick = 0;
	
	// Add Text Layer
	text_layer_destroy(text_layer);
	text_layer = text_layer_create(layer_get_frame(window_get_root_layer(main_window)));
	layer_add_child(window_get_root_layer(main_window), text_layer_get_layer(text_layer));
	text_layer_set_background_color(text_layer, GColorClear);
	text_layer_set_text_color(text_layer, GColorWhite);
	text_layer_set_text(text_layer, "Try Again");
	
	// Add Inverted Selection Layer
	inverter_layer_destroy(selection_layer);
	selection_layer = inverter_layer_create(GRect(0, 120, 36, 24));
	layer_add_child(window_get_root_layer(main_window), inverter_layer_get_layer(selection_layer));
}

static void up_press(ClickRecognizerRef recognizer, void *context)
{
	if(status != DEAD)
	{
		if(current_menu_item == STATS)
		{
			current_menu_item = RESTART;
		}
		else
		{
			current_menu_item--;
		}
		
		draw_menu();	
	}
}

static void down_press(ClickRecognizerRef recognizer, void *context)
{
	if(status != DEAD)
	{
		if(current_menu_item == RESTART)
		{
			current_menu_item = STATS;
		}
		else
		{
			current_menu_item++;
		}
		
		draw_menu();
	}
}

static void menu_item_select(ClickRecognizerRef recognizer, void *context)
{
	if(status != DEAD)
	{
		switch(current_menu_item)
		{
			case STATS:
				;
				break;
			case FOOD:
				feed_pet();
				break;
			case MEDICINE:
				heal_pet();
				break;
			case POOP:
				clean_pet();
				break;
			case WORK:
				work_pet();
				break;
			case 5:
				;
				break;
			case 6:
				;
				break;
			case RESTART:
				restart();
				break;
		}
	}
	else
	{
		restart();
	}
	
	write_log();
}

static void load_data(void)
{
	start_time = persist_read_int(START_TIME);
	stage = persist_read_int(STAGE);
	status = persist_read_int(STATUS);
	health = persist_read_int(HEALTH);
	hunger = persist_read_int(HUNGER);
	sickness = persist_read_int(SICKNESS);
	poops = persist_read_int(POOPS);
	sick_time = persist_read_int(SICK_TIME);
	persist_read_data(FEED_TIMES, feed_times, ARRAY_LENGTH(feed_times) * sizeof(unsigned int));
	persist_read_data(WORK_TIMES, work_times, ARRAY_LENGTH(work_times) * sizeof(unsigned int));
	last_hunger = persist_read_int(LAST_HUNGER);
	last_poop = persist_read_int(LAST_POOP);
	last_cleaned = persist_read_int(LAST_CLEANED);
	last_poop_sick = persist_read_int(LAST_POOP_SICK);
	
	// Calculate /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	write_log();
}

static void save_data(void)
{
	persist_write_int(START_TIME, (int)start_time);
	persist_write_int(STAGE, (int)stage);
	persist_write_int(STATUS, (int)status);
	persist_write_int(HEALTH, (int)health);
	persist_write_int(HUNGER, (int)hunger);
	persist_write_int(SICKNESS, (int)sickness);
	persist_write_int(POOPS, (int)poops);
	persist_write_int(SICK_TIME, (int)sick_time);
	persist_write_data(FEED_TIMES, feed_times, ARRAY_LENGTH(feed_times) * sizeof(unsigned int));
	persist_write_data(WORK_TIMES, work_times, ARRAY_LENGTH(work_times) * sizeof(unsigned int));
	persist_write_int(LAST_HUNGER, (int)last_hunger);
	persist_write_int(LAST_POOP, (int)last_poop);
	persist_write_int(LAST_CLEANED, (int)last_cleaned);
	persist_write_int(LAST_POOP_SICK, (int)last_poop_sick);
}

static void set_callbacks(Window *window)
{
	window_single_click_subscribe(BUTTON_ID_UP, up_press);
	window_single_click_subscribe(BUTTON_ID_DOWN, down_press);
	window_single_click_subscribe(BUTTON_ID_SELECT, menu_item_select);
}

static void window_load(Window *window)
{			
	// Initial Setup of the Window Layers
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_frame(window_layer);
	
	// Add Background Layer
	bg_layer = bitmap_layer_create(bounds);
	layer_add_child(window_layer, bitmap_layer_get_layer(bg_layer));	
	bg_bitmap = gbitmap_create_with_resource(RESOURCE_ID_NIGHT_BG);
	bitmap_layer_set_bitmap(bg_layer, bg_bitmap);
	
	// Add Sprite Layer
	sprite_layer = bitmap_layer_create(GRect(22, 70, 100, 50));
	layer_add_child(window_layer, bitmap_layer_get_layer(sprite_layer));	
	bitmap_layer_set_background_color(sprite_layer, GColorClear);
	bitmap_layer_set_alignment(sprite_layer, GAlignCenter);	
	sprite_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BLANK_SPRITE);
	bitmap_layer_set_bitmap(sprite_layer, sprite_bitmap);
	
	// Add Status Layer
	status_layer = bitmap_layer_create(GRect(22, 70, 100, 50));
	layer_add_child(window_layer, bitmap_layer_get_layer(status_layer));	
	bitmap_layer_set_background_color(status_layer, GColorClear);
	bitmap_layer_set_alignment(status_layer, GAlignCenter);	
	status_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BLANK_SPRITE);
	bitmap_layer_set_bitmap(status_layer, status_bitmap);
	
	// Add test Text Layer
	text_layer = text_layer_create(bounds);
	layer_add_child(window_layer, text_layer_get_layer(text_layer));
	text_layer_set_background_color(text_layer, GColorClear);
	text_layer_set_text_color(text_layer, GColorWhite);
	text_layer_set_text(text_layer, "");
	
	// Add Inverted Selection Layer
	selection_layer = inverter_layer_create(GRect(0, 120, 36, 24));
	layer_add_child(window_layer, inverter_layer_get_layer(selection_layer));
			
	app_timer_register(1000, app_loop, NULL);
}

static void window_unload(Window *window) {
	text_layer_destroy(text_layer);
}

void init(void)
{
	// Setup Main Window
	main_window = window_create();	
	window_set_background_color(main_window, GColorWhite);
	window_set_fullscreen(main_window, true);
	window_set_window_handlers(main_window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload
	});
	
	window_set_click_config_provider(main_window, (ClickConfigProvider)set_callbacks);
		
	// If app is run for the first time, make a note of the exact time
	if(!persist_exists(START_TIME))
	{
		start_time = time(NULL);
	}
	
	// Load saved game data
	if(persist_exists(START_TIME))
	{
		load_data();
	}
	
	// Push Window to Stack and start program
	window_stack_push(main_window, true);
}

void deinit(void)
{
	save_data();
	window_destroy(main_window);
	text_layer_destroy(text_layer);
}

int main(void)
{
	init();
	app_event_loop();
	deinit();
}

#include <pebble.h>
#include <pebble_fonts.h>

static Window *window;
static TextLayer *instruction_layer;
static TextLayer *message_layer;

// Random number generator -- Still need to seed with the time
int random(int max)
{
	static long seed = 100;
	seed = (((seed * 214013L + 2531011L) >> 16) & 32767);
	return ((seed % max)+1);
}

// Get number as Char so it can be displayed as a text layer
char *nums[6] = {"1","2","3","4","5","6"};

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    // Initialize a random number each time button is pressed
    int rand = random(6);
    // Output the random number
    text_layer_set_text(message_layer, nums[rand -1]);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    text_layer_set_text(message_layer, " ");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    text_layer_set_text(message_layer, "Drew\nis\nAwesome");
}

static void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    
    message_layer = text_layer_create(GRect(0,0,144,100));
    text_layer_set_background_color(message_layer, GColorWhite);
    text_layer_set_text_color(message_layer, GColorBlack);
    text_layer_set_text_alignment(message_layer, GTextAlignmentCenter);
    text_layer_set_overflow_mode(message_layer, GTextOverflowModeWordWrap);
    // Set the font size and type
    text_layer_set_font(message_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));
    layer_add_child(window_layer, text_layer_get_layer(message_layer));
    
    
    instruction_layer = text_layer_create(GRect(0,100,144,60));
    text_layer_set_background_color(instruction_layer, GColorBlack);
    text_layer_set_text_color(instruction_layer, GColorWhite);
    text_layer_set_text(instruction_layer, "Press button to \n role the die.");
    text_layer_set_text_alignment(instruction_layer, GTextAlignmentCenter);
    text_layer_set_overflow_mode(instruction_layer, GTextOverflowModeWordWrap);
    // Set the font size and type
    text_layer_set_font(instruction_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
    layer_add_child(window_layer, text_layer_get_layer(instruction_layer));
}

static void window_unload(Window *window) {
    text_layer_destroy(instruction_layer);
    text_layer_destroy(message_layer);
    
}

static void init(void) {
    window = window_create();
    window_set_click_config_provider(window, click_config_provider);
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    const bool animated = true;
    window_stack_push(window, animated);
}

static void deinit(void) {
    window_destroy(window);
}

int main(void) {
    init();
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
    
    app_event_loop();
    deinit();
}
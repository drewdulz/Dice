/* Notes to work on:
 - draw dice or use bitmap?
 - clean up layers and functions
 - 
 
 */

// Pebble screen size is 144 x 168 pixels.

#include <pebble.h>
#include <pebble_fonts.h>


static Window *window;
static TextLayer *instruction_layer;
static TextLayer *count_layer;
static TextLayer *message_layer;
static TextLayer *intro_layer;
static BitmapLayer *dice_view_layer;
static GBitmap *image;



int num_rolls = 0;
char counter_text[5]; //Can count up to 999 (3 digits plus signs & \0)
int roll;


// Random number generator
int randomnum()
{
    return (rand() % 6);
}

// Function used for displaying the counter
void update_text_layer_with_int(int num, TextLayer* text_layer){
    snprintf(counter_text, (5)*sizeof(char), "%d", num);
    text_layer_set_text(text_layer, counter_text);
}


// Display image of dice roll
void display_roll(int roll){
    Layer *window_layer = window_get_root_layer(window);

    if (num_rolls==0) {
        dice_view_layer = bitmap_layer_create(GRect(0,0,144,105));
        bitmap_layer_set_background_color(dice_view_layer, GColorBlack);
        bitmap_layer_set_alignment(dice_view_layer, GAlignCenter);
        layer_add_child(window_layer, bitmap_layer_get_layer(dice_view_layer));

        // Vibrate long pulse
        vibes_short_pulse();
        
        if (roll==1) {
            image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DICE1);
        } else if (roll==2) {
            image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DICE2);
        } else if (roll==3) {
            image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DICE3);
        } else if (roll==4) {
            image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DICE4);
        } else if (roll==5) {
            image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DICE5);
        } else {
            image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DICE6);
        }
        
        bitmap_layer_set_bitmap(dice_view_layer, image);
        
    } else {

        // Setup dice_view_layer where dice image is displayed
        gbitmap_destroy(image);
        
        // Vibrate long pulse
        vibes_short_pulse();
    
        if (roll==1) {
            image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DICE1);
        } else if (roll==2) {
            image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DICE2);
        } else if (roll==3) {
            image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DICE3);
        } else if (roll==4) {
            image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DICE4);
        } else if (roll==5) {
            image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DICE5);
        } else {
            image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DICE6);
        }

        bitmap_layer_set_bitmap(dice_view_layer, image);
    }
}


void accel_tap_handler(AccelAxisType axis, int32_t direction) {
    if(ACCEL_AXIS_Y == 1 || ACCEL_AXIS_Y == -1){
        roll = randomnum();
        // Output the random number via dice image
        display_roll(roll);
        num_rolls += 1;
        update_text_layer_with_int(num_rolls,count_layer);
        
    }
}

// This function used for manual override. It's normally done through the accelerometer.
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    roll = randomnum();
    // Output the random number via dice image
    display_roll(roll);
    num_rolls += 1;
    update_text_layer_with_int(num_rolls,count_layer);
    
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    // Insert up button stuff here
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    // Insert down button stuff here
}

static void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    
    // Setup the instruction layer
    instruction_layer = text_layer_create(GRect(0,105,120,55));
    text_layer_set_background_color(instruction_layer, GColorBlack);
    text_layer_set_text_color(instruction_layer, GColorWhite);
    text_layer_set_text(instruction_layer, "Roll Count:  ");
    text_layer_set_text_alignment(instruction_layer, GTextAlignmentRight);
    text_layer_set_overflow_mode(instruction_layer, GTextOverflowModeWordWrap);
    text_layer_set_font(instruction_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
    layer_add_child(window_layer, text_layer_get_layer(instruction_layer));
    
    // Setup the count layer (where the count is displayed)
    count_layer = text_layer_create(GRect(120,105,24,55));
    text_layer_set_background_color(count_layer, GColorBlack);
    text_layer_set_text_color(count_layer, GColorWhite);
    text_layer_set_text(count_layer, "0");
    text_layer_set_text_alignment(count_layer, GTextAlignmentLeft);
    text_layer_set_overflow_mode(count_layer, GTextOverflowModeWordWrap);
    text_layer_set_font(count_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
    layer_add_child(window_layer, text_layer_get_layer(count_layer));
    
    // Setup intro layer
    intro_layer = text_layer_create(GRect(0,0,144,105));
    text_layer_set_background_color(intro_layer, GColorBlack);
    text_layer_set_text_color(intro_layer, GColorWhite);
    text_layer_set_text(intro_layer, "Shake to Roll!");
    text_layer_set_text_alignment(intro_layer, GTextAlignmentCenter);
    text_layer_set_overflow_mode(intro_layer, GTextOverflowModeWordWrap);
    text_layer_set_font(intro_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
    layer_add_child(window_layer, text_layer_get_layer(intro_layer));
}

static void window_unload(Window *window) {
    text_layer_destroy(instruction_layer);
    text_layer_destroy(message_layer);
    bitmap_layer_destroy(dice_view_layer);
    
}

static void init(void) {
    srand(time(NULL));
    window = window_create();
    window_set_click_config_provider(window, click_config_provider);
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    const bool animated = true;
    window_stack_push(window, animated);
    // Subscribe to accelerometer readings
    accel_tap_service_subscribe(&accel_tap_handler);
}

static void deinit(void) {
    window_destroy(window);
    bitmap_layer_destroy(dice_view_layer);
}

int main(void) {
    init();
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
    
    app_event_loop();
    deinit();
}
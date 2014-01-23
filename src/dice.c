/* Notes to work on:
 - draw dice or use bitmap?
 - clean up layers and functions
 - 
 
 */


#include <pebble.h>
#include <pebble_fonts.h>


static Window *window;
static TextLayer *instruction_layer;
static TextLayer *message_layer;

int num_rolls = 0;
char counter_text[5]; //Can count up to 999 (3 digits plus signs & \0)


// Random number generator -- Still need to seed with the time
int random()
{
    return (rand() % 6);
}

void update_text_layer_with_int(int num, TextLayer* text_layer){
    snprintf(counter_text, (5)*sizeof(char), "%d", num);
    text_layer_set_text(text_layer, counter_text);
}

// Get number as Char so it can be displayed as a text layer
char *nums[6] = {"1","2","3","4","5","6"};

void dice_draw(Layer *message_layer, GContext *context) {
    graphics_context_set_fill_color(context, GColorBlack);
    graphics_fill_rect(context, GRect(5,5,5,5), 0, GCornerNone);
    graphics_context_set_fill_color(context, GColorWhite);
    graphics_fill_circle(context, GPoint(10,10), 4);
}

void accel_tap_handler(AccelAxisType axis, int32_t direction) {
    if(ACCEL_AXIS_Y == 1 || ACCEL_AXIS_Y == -1){
        //Output random number on wrist shake
        text_layer_set_text(message_layer, nums[random()]);
        //update counter
        num_rolls += 1;
        update_text_layer_with_int(num_rolls,instruction_layer);
        
    }
}

// This function used for manual override. It's done through the accelerometer.
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    // Output the random number
    text_layer_set_text(message_layer, nums[random()]);
    num_rolls += 1;
    //text_layer_set_text(instruction_layer, num_rolls);
    update_text_layer_with_int(num_rolls,instruction_layer);
    //Draw a rectangle
    //dice_draw(message_layer, context);
    //graphics_context_set_stroke_color(context, GColorBlack);
    //graphics_context_set_fill_color(context, GColorBlack);
    //graphics_draw_round_rect(context, GRect(5,5,5,5), 2);
    //graphics_draw_circle(context, GPoint(10,10), 5);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    text_layer_set_text(message_layer, " ");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    text_layer_set_text(message_layer, " ");
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
    text_layer_set_text(instruction_layer, "Shake wrist to\nrole the die.");
    text_layer_set_text_alignment(instruction_layer, GTextAlignmentCenter);
    text_layer_set_overflow_mode(instruction_layer, GTextOverflowModeWordWrap);
    // Set the font size and type
    text_layer_set_font(instruction_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
    layer_add_child(window_layer, text_layer_get_layer(instruction_layer));
    
    //Draw the square
    //message_layer->update_proc = dice_draw;
}

static void window_unload(Window *window) {
    text_layer_destroy(instruction_layer);
    text_layer_destroy(message_layer);
    
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
}

int main(void) {
    init();
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
    
    app_event_loop();
    deinit();
}
// #include "Keyboard.h"
#include "button.h"
#include "helpers.h"

// Buttons
const int BUTTON_1 = 45;
const int BUTTON_2 = 42;
Button b1(BUTTON_1);
Button b2(BUTTON_2);

const int ROW_PINS[] = {0, 1, 2, 3, 8, 9, 10, 11, 33, 34, 35, 36};
const int COL_PINS[] = {4, 5, 6, 7, 12, 13, 14, 15, 37, 38, 39};
const int ROWS = 0;
const int COLUMNS = 1;
const int NUM_COL_PINS = 11;
const int NUM_ROW_PINS = 12;
const int MAX_DIM_SIZE = max(NUM_ROW_PINS, NUM_COL_PINS);

// LED PINS
const int DATA_PIN = 17;
const int SHIFT_LED_PIN = 40;
const int CAPS_LED_PIN = 41;

char debug_row_read[] = "000000000000";
char debug_col_read[] = "000000000000";
char debug_row_drive[] = "000000000000";
char debug_col_drive[] = "000000000000";

int mask = 1;
unsigned long out = 0;
unsigned long last_out = 0;
int data_available = 0;
int last_data_available = 0;
int caps_lock_on = 0;
int caps_lock_last_pressed = 0;

unsigned long CAPS_LOCK_KEY = 114;
unsigned long SHIFT_KEY = 54;
// Set to whatever the shift key row and column readings
// turn out to be based on connection of keyboard to controller
const int SHIFT_ROW_READ = 16;   // 4th row (0 idxed)
const int SHIFT_COL_READ = 64;  // 6th column (0 idxed)

int shift_with_char = 0;
int shift_was_pressed = 0;
int shift_current_char = 0;
unsigned long key_id = 0;
int lower_case_letters = 1;

uint32_t delay_timer;
uint32_t repeat_timer;
uint8_t key_repeat = 0;
int REPEAT_DELAY = 320; // 300
int REPEAT_RATE = 200; // 400

void drive(int dimension)
{
    int row_val = (int) (dimension == ROWS);
    int col_val = (int) (dimension == COLUMNS);
    int row_pin_mode = (dimension == ROWS) ? OUTPUT : INPUT_PULLDOWN;
    int col_pin_mode = (dimension == COLUMNS) ? OUTPUT : INPUT_PULLDOWN;
    
    // Pull rows high
    for (int i = 0; i < NUM_ROW_PINS; i++) {
        pinMode(ROW_PINS[i], row_pin_mode);
        digitalWrite(ROW_PINS[i], row_val);
    }
    
    // Pull columms low
    for (int i = 0; i < NUM_COL_PINS; i++) {
        pinMode(COL_PINS[i], col_pin_mode);
        digitalWrite(COL_PINS[i], col_val);
    }
}

long read(int dimension)
{
    long read_val = 0;
    int start_power = 11;
    bool row_dim = (dimension == ROWS);
    int num_pins = row_dim ? NUM_ROW_PINS : NUM_COL_PINS;
    const int* pin_arr = row_dim ? ROW_PINS : COL_PINS;
    char* debug_arr = row_dim ? debug_row_read : debug_col_read;
    char* driven_arr = row_dim ? debug_col_drive : debug_row_drive;

    // Write values of sheet driven high to debug array
    for (int i = 0; i < (row_dim ? NUM_COL_PINS : NUM_ROW_PINS); i++) {
        driven_arr[i + ((dimension + 1) % 2)] = digitalRead((row_dim ? COL_PINS : ROW_PINS)[i]) ? '1' : '0';
    }

    for (int i = 0; i < num_pins; i++) {
        int pin_val = digitalRead(pin_arr[i]);
        // Write individual values to debug array
        debug_arr[i + dimension] = pin_val ? '1' : '0';
        read_val += pow(2, start_power - dimension - i) * pin_val;
    }
    return read_val;
}

int num_bits_set(int num)
{
    int mask = 1;
    int num_bits = MAX_DIM_SIZE;
    int result = 0;
    int i;
    for (i = 0; i < num_bits; i++)
    {
        result += (int) ((num & mask) > 0);
        mask = mask << 1;
    }
    return result;
}

int shift_pressed_with_key(long row_read, long col_read)
{
    // Check if shift key pressed with single other key
    int row_bits, col_bits;
    
    row_bits = num_bits_set(row_read);
    col_bits = num_bits_set(col_read);
    if (row_bits > 2 || col_bits > 2) return 0;
    if (row_read == SHIFT_ROW_READ && col_read == SHIFT_COL_READ) return 0;

    // Check if shift key in row_bits    
    int row_sub_shift = (row_read & (~SHIFT_ROW_READ));
    int in_row_bits = (row_sub_shift != row_read);

    // Check if shift key in col_bits
    int col_sub_shift = (col_read & (~SHIFT_COL_READ));
    int in_col_bits = (col_sub_shift != col_read);
    
    return (in_row_bits && in_col_bits);
}

void drive_row() { drive(ROWS); }
void drive_col() { drive(COLUMNS); }
long read_row() { return read(ROWS); }
long read_col() { return read(COLUMNS); }

unsigned long keypad_scan(void)
{
    long row_num, row_read = 0;
    long col_num, col_read = 0;
    int only_one_key;
    unsigned long result;
    
    /* Drive rows, read columns */
    drive_row();
    delay(10);    // Wait for value to stabilize / key press event
    col_read = read_col();
    col_num = logb2(col_read);
    
    /* Drive columns, read rows*/
    drive_col();
    delay(10);    // Wait for value to stabilize / key press event
    row_read = read_row();
    row_num = logb2(row_read);
    
    /* combine results */
    result = 12 * row_num + col_num;
    shift_with_char = shift_pressed_with_key(row_read, col_read);
    shift_current_char = shift_was_pressed && shift_with_char;
    only_one_key = (num_bits_set(row_read) == 1) && (num_bits_set(col_read) == 1);
    
    if (result == CAPS_LOCK_KEY && !caps_lock_last_pressed && only_one_key) {
        caps_lock_on = !caps_lock_on;
        digitalWrite(CAPS_LED_PIN, caps_lock_on);
        data_available = 0;   
    } else if (shift_current_char) {
        // Get key pressed without shift key
        row_num = logb2(row_read & (~SHIFT_ROW_READ));
        col_num = logb2(col_read & (~SHIFT_COL_READ));
        result = 12 * row_num + col_num;
    } else if (result != CAPS_LOCK_KEY) {
        data_available = (col_num != -1 && row_num != -1);        
    }
    
    if (result == SHIFT_KEY || shift_current_char) {
        shift_was_pressed = 1;
        data_available = shift_current_char && (col_num != -1 && row_num != -1);
    } else shift_was_pressed = 0;
    
    digitalWrite(DATA_PIN, data_available);
    digitalWrite(SHIFT_LED_PIN, shift_was_pressed || shift_current_char);
    caps_lock_last_pressed = (result == CAPS_LOCK_KEY) && only_one_key;
    result = data_available ? result : 0;    
    return result;
}


#include "hidkeyboard.h"
#include "Wire.h"
#if CFG_TUD_HID
// #define KEYBOARD_I2C_ADDR 0X5f

HIDkeyboard dev;

void setup()
{
    Serial.begin(115200);
    pinMode(BUTTON_1, INPUT_PULLUP);
    pinMode(BUTTON_2, INPUT_PULLUP);
    pinMode(DATA_PIN, OUTPUT);
    pinMode(SHIFT_LED_PIN, OUTPUT);
    pinMode(CAPS_LED_PIN, OUTPUT);

    // Enable pins
    for (int i = 0; i < NUM_ROW_PINS; i++) { pinMode(ROW_PINS[i], INPUT_PULLDOWN); }
    for (int i = 0; i < NUM_COL_PINS; i++) { pinMode(COL_PINS[i], INPUT_PULLDOWN); }

    Wire.begin(7,8);
    dev.begin();
    delay_timer = millis();
    repeat_timer = millis();
}

void loop()
{   
    // Debug: show row and column readings of pressed key
    if (b1.update()) {
        Serial.println("rows");
        Serial.println(debug_row_read);
        Serial.println("cols");
        Serial.println(debug_col_read);
    }

    // Debug: show values of row and columns when driven high
    if (b2.update()) {
        Serial.println("rows");
        Serial.println(debug_row_drive);
        Serial.println("cols");
        Serial.println(debug_col_drive);
    }
    
    mask = 1;
    data_available = 0;
    key_id = keypad_scan();
    out = char_lookup(key_id);
    lower_case_letters = !caps_lock_on;
    
    if (shift_current_char) lower_case_letters = !lower_case_letters;
    
    // Make lowercase if caps lock is off or shift not held
    if (out >= 65 && out <= 90 && lower_case_letters)
    {
        out = out | 0b100000;
    }

    if (!data_available) {
        delay_timer = millis();
        key_repeat = 0;
    } else if (millis() - delay_timer >= REPEAT_DELAY) {
        key_repeat = 1;
    }

    if (
        ((last_out != out) && data_available && !last_data_available)
        || (data_available && last_out == out && (millis() - repeat_timer >= REPEAT_RATE))
    ) {
        Serial.printf("key_id: %d\n", key_id);
        Serial.printf("out: %d\n", out);
        if (out != -1) {
            dev.sendChar(out);
            repeat_timer = millis();
        }
    }
    last_data_available = data_available;
    last_out = out;
}

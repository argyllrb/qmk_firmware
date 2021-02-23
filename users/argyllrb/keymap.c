#include QMK_KEYBOARD_H
#define _MAIN 0
#define LAYER1 1
#define LAYER2 2

/*
enum layers {
     _MAIN,
     LAYER1,
     LAYER2,
};
*/

// Custom keys
enum custom_keys {
    MOD_ENC_RES,
    TO_LAYER0,
    TO_LAYER1,
    TO_LAYER2
};

// Tap Dance declarations
enum {
    //TD_LAYER_SWITCH,
    TD_LAYER_SWITCH0,
    TD_LAYER_SWITCH1,
    TD_LAYER_SWITCH2
};

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for A, twice for CapsLock
    // [TD_LAYER_SWITCH] = ACTION_TAP_DANCE_DOUBLE(KC_A, KC_CAPS),
    [TD_LAYER_SWITCH0] = ACTION_TAP_DANCE_LAYER_MOVE(KC_X, _MAIN),
    [TD_LAYER_SWITCH1] = ACTION_TAP_DANCE_LAYER_MOVE(KC_Y, LAYER1),
    [TD_LAYER_SWITCH2] = ACTION_TAP_DANCE_LAYER_MOVE(KC_Z, LAYER2),
};


uint8_t midiChan = 0;
// ROW is Layer position, COL is Encoder position
// e.g. Layer 1, Encoder 2 is ccValArray[0][1]
uint8_t ccValArray[3][5] = {0}; 
uint8_t ccNoArray[3][5] = {
  //{Enc0, Enc1, Enc2, Enc3, Enc4}
  {0, 71, 0,  72, 0},
  {0, 73, 0,  74, 0},
  {0, 75, 0,  76, 0}
};

static bool resolutionIsDefault = true;
//uint8_t layerNo = 0;

//#define ENCODER_RESOLUTIONS_LOW {1,1,1,1,1}
//uint8_t encoder_resolutions[] = ENCODER_RESOLUTIONS;
//uint8_t encoder_resolutions_low[] = {1,1,1,1,1};
#ifdef ENCODER_RESOLUTIONS
//static uint8_t encoder_resolutions_default[] = ENCODER_RESOLUTIONS;
#else
//static uint8_t encoder_resolutions_default[] = {4,4,4,4,4};
//#define ENCODER_RESOLUTIONS {4,4,4,4,4}
#endif

//static uint8_t encoder_resolutions_low[] = {1,1,1,1,1};
//static uint8_t encoder_resolutions[] = encoder_resolutions_default;
//#define ENCODER_RESOLUTIONS_LOW {1,1,1,1,1}

/*
void matrix_scan_user(void) {
}
*/



bool encoder_resolution_is_default_user(void) {
  
  return resolutionIsDefault;


}

/*
int8_t encoder_update_resolution_user(int8_t index) {
  
  // Return default resolution

  // Return resolution low

  return 0;
}
*/

void encoder_update_user(uint8_t index, bool clockwise) {

  // Dummy keypress record inorder to process midi for the encoder  
  //keyevent_t pressedevent = { .pressed = true };
  //keyrecord_t dummyrecord = { .event = pressedevent };
  /*
  process_midi(MI_MODSU, &dummyrecord);

  // Alternative dummy records
  keyrecord_t pressed = {{{0,0},true,0}, {0,0,0,0,0}};
  keyrecord_t depressed = {{{0,0},false,0}, {0,0,0,0,0}};
  */

  uint8_t layerNo = biton32(layer_state);


  
  if (index == 0) { /* First encoder*/
    if (clockwise) {
      tap_code(KC_1);
    } else {
      tap_code(KC_2);
    }
  } else if (index == 1) { /* Second encoder*/
    /*
    switch (biton32layer(layer_state)) {
      case 2:
        ccNo = ccNoArray[layer_state][index];
        break;
      case 1:

        break;
      default:

        break;
    }
    */
    if (clockwise) {
      ccValArray[layerNo][index]++;
    } else {
      ccValArray[layerNo][index]--;
    }
    process_midi_send_cc(midiChan, ccNoArray[layerNo][index], ccValArray[layerNo][index]); 

  } else if (index == 2) { /* Third encoder*/
    if (clockwise) {
      tap_code(KC_5);
    } else {
      tap_code(KC_6);
    }
  } else if (index == 3) { /* Fourth encoder*/
    if (clockwise) {
      //tap_code(KC_7);
      if (ccValArray[layerNo][index] < 127) {
        ccValArray[layerNo][index]++;
      } else {ccValArray[layerNo][index]=127;}
         
    } else {
      //tap_code(KC_8);
      if (ccValArray[layerNo][index] > 0) {
        ccValArray[layerNo][index]--;
      } else {ccValArray[layerNo][index]=0;}
      
    }
    process_midi_send_cc(midiChan, ccNoArray[layerNo][index], ccValArray[layerNo][index]); 

  } else if (index == 4) { /* Fifth encoder*/
    if (clockwise) {
      tap_code(KC_9);
    } else {
      tap_code(KC_0);
    }
  }
}

// Custom Keys

bool process_record_user (uint16_t keycode, keyrecord_t *record) {

  switch(keycode) {

    case MOD_ENC_RES:
      
      if (record->event.pressed) {
        //encoder_resolutions[] = ENCODER_RESOLUTIONS_LOW; //encoder_resolutions_low;
	      //encoder_update_resolution((int[])ENCODER_RESOLUTIONS_LOW);
        resolutionIsDefault = false;
	    } else {
        //encoder_resolutions[] = ENCODER_RESOLUTIONS;
        //encoder_update_resolution((int[])ENCODER_RESOLUTIONS);
        resolutionIsDefault = true;
	    }
      break;
    /*
    case TO_LAYER0:
      layerNo = 0;
      // set layer?
      break;

    case TO_LAYER1:
      layerNo = 1;
      // set layer?
      break;

    case TO_LAYER2:
      layerNo = 2;
      //default_layer_set(layerNo);
      break;
    */

  }

  return true;
}

// Tap dance is TD(TD_LAYER_SWITCH1)
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = { //button closest to usb is first
  [_MAIN] = LAYOUT_ortho_1x5(
    // Mod-Tap shortcut for Right Control - Hold for Right Control, Tap for Keycode
    // TD - See Tap Dance actions above - tap once for keycode, double tap for next layer
    MOD_ENC_RES, KC_1, KC_2, KC_3, TD(TD_LAYER_SWITCH1)
  ),
  [LAYER1] = LAYOUT_ortho_1x5(
    KC_B, KC_4, KC_5, KC_6, TD(TD_LAYER_SWITCH2)
  ),
  [LAYER2] = LAYOUT_ortho_1x5(
    KC_C, KC_7, KC_8, KC_9, TD(TD_LAYER_SWITCH0)
  ),
};

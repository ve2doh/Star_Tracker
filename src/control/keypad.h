#ifndef KEYPAD_H
#define KEYPAD_H

#include <IRremote.h>

#define KP_KEY_A             0x61A018E7
#define KP_KEY_B             0x61A048B7
#define KP_KEY_C             0x61A038C7
#define KP_KEY_D             0x61A0B847  
#define KP_KEY_E             0x61A008F7 
#define KP_KEY_F             0x61A0E817
#define KP_KEY_1             0x61A000FF 
#define KP_KEY_2             0x61A0807F
#define KP_KEY_3             0x61A040BF
#define KP_KEY_4             0x61A0C03F  
#define KP_KEY_5             0x61A020DF
#define KP_KEY_6             0x61A0A05F
#define KP_KEY_7             0x61A0609F  
#define KP_KEY_8             0x61A0E01F
#define KP_KEY_9             0x61A010EF
#define KP_KEY_0             0x61A0906F
#define KP_KEY_UP_ARROW      0x61A050AF  
#define KP_KEY_DOWN_ARROW    0x61A0D02F
#define KP_KEY_RIGHT_ARROW   0x61A030CF
#define KP_KEY_LEFT_ARROW    0x61A0B04F
#define KP_KEY_OK            0x61A018E7
#define KP_UPDATE_MS         200     
class Keypad {

    public:

        Keypad() : _recv(IRrecv(KEYPAD_IR_PIN)) {}

        void initialize() {
            _recv.enableIRIn();
            _recv.blink13(false);
        }

        // returns true if the given key was pressed for longer than SHORT_HOLD_TIME_MS
        inline boolean pushed(uint32_t key_code) { return pressed_internal(key_code, SHORT_HOLD_TIME_MS); }

        // returns true if the given key was pressed for longer than LONG_HOLD_TIME_MS
        inline boolean pressed(uint32_t key_code) { return pressed_internal(key_code, LONG_HOLD_TIME_MS); }

        void update() {

            _hold_time = 0;

            if (_last_used_key == 0 && (millis() - _last_update) < KP_UPDATE_MS) return;
            _last_update = millis();

            uint32_t new_key = get_key();
          
            if (new_key == _used_key) return;

            #ifdef DEBUG_KEYS
                Serial.print(F("Keypad change: "));
                Serial.print(_used_key, HEX);
                Serial.print(F("-->"));
                Serial.println(new_key, HEX);
            #endif

            _last_used_key = _used_key;
            _used_key = new_key;
            _hold_time = millis() - _press_time;
            _press_time = millis();
        }

    private:

        inline bool pressed_internal(uint32_t key_code, unsigned long duration) {
            if (key_code != _last_used_key) return false; 
            if (_hold_time >= duration) {
                _hold_time = 0;
                return true;
            } else return false;
        }

        uint32_t get_key() {

            if (!_recv.decode()) return 0;

            _recv.resume();

            return (_recv.results.value == 0xFFFFFFFF ? _used_key : _recv.results.value) ;
        }

        IRrecv _recv;

        uint32_t _used_key = 0;         // currently pressed key
        uint32_t _last_used_key = 0;    // key pressed before previous key change

        unsigned long _hold_time = 0;   // millis since last key change
        unsigned long _press_time = 0;  // millis of last key change

        unsigned long _last_update = 0; // millis since last key check
};

#endif

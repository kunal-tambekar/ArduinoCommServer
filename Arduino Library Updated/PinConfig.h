#ifndef PINCONFIG_H
#define PINCONFIG_H

class PinConfig{
public:
    PinConfig(){ }

    PinConfig(int epin, String spin,int mode, String mk, String mv){
        esp_pin= epin;
        sensor_pin = spin;
        pin_mode = mode;
        misc_key = mk;
        misc_val = mv;
    }

    int esp_pin ;
    String sensor_pin;
    int pin_mode;
    String misc_key;
    String misc_val;

    void setSPin(const String& s){
        this->sensor_pin = s;
    }

    void setMK(const String& mk){
        this->misc_key = mk;
    }

    void setMV(const String& mv){
        this->misc_val = mv;
    }
};

#endif PINCONFIG_H

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd (0x27, 16, 2);


unsigned long ONE_SECOND = 1000;
unsigned long ONE_MINUTE = ONE_SECOND * 60;
unsigned long ONE_HOUR = ONE_MINUTE * 60;


void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("setup");
  Serial.println("setup");
}


class Timer {
  public:
    Timer(String _name, int display_column_index, int pin, unsigned long on_time, unsigned long off_time);
    void tick();
    String name;

  private:
    int _display_column_index;
    bool _is_on;
    int _pin;
    unsigned long _on_time;  // milliseconds
    unsigned long _off_time;
    unsigned long _next_time;
    unsigned long _prev_time;
};

Timer::Timer(String _name, int display_column_index, int pin, unsigned long on_time, unsigned long off_time) {
  pinMode(pin, OUTPUT);
  name = _name;
  _display_column_index = display_column_index;
  _pin = pin;
  _on_time = on_time;
  _off_time = off_time;
  _next_time = 0;
  _prev_time = millis();
  _is_on = false;
}

void Timer::tick() {
  if (millis() >= _next_time) {
    lcd.setCursor(0, _display_column_index);
    lcd.print("                ");  // write blank text to line (dont use lcd.clear)
    lcd.setCursor(0, _display_column_index);
 
    if (_is_on == true) {  // on to off
      lcd.print(name + ":I:" + String(_off_time / ONE_SECOND));
      Serial.print("disabled pin #");
      Serial.println(_pin);
      digitalWrite(_pin, HIGH);
      _is_on = false;
      _next_time = millis() + _off_time;
    } else {  // off to on
      lcd.print(name + ":R:" + String(_on_time / ONE_SECOND));
      Serial.print("enabled pin #");
      Serial.println(_pin);
      digitalWrite(_pin, LOW);
      _is_on = true;
      _next_time = millis() + _on_time;
    }
    Serial.println(_next_time);
  }
}



Timer K1("PMP", 0, 2, ONE_MINUTE * 5, ONE_MINUTE * 12);
Timer K3("FAN", 1, 3, ONE_HOUR, ONE_HOUR * 4);


void loop() {
  K1.tick();
  K3.tick();
}

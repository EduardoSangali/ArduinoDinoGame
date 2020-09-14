/*
 * ---------------------------------------- 
 * -- Created by Eduardo Sangali - 2019 --
 * ---------------------------------------
*/

 
#include <EEPROM.h>
#include <LiquidCrystal.h>

//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int btn = 7;
int pos_eeprom = 0;
int interv = 350;
int time_jump = 700;
int x = 16;
int y = 13;
int z = 7;
int score = 0;
int cact_count1 = 9;
int cact_count2 = 26;
int cact_count3 = 31;
boolean coordenada = 1;
boolean jumping = false;
boolean pressed = false;
boolean current_sprite = 0;
unsigned long t1 = millis();
unsigned long t_jumping;
int opc = 0;
int i = 0;

byte person[2][8] = {
  { B01110,
    B01010,
    B01110,
    B00101,
    B11111,
    B10100,
    B01111,
    B11000
  },

  { B01110,
    B01010,
    B01110,
    B10100,
    B11111,
    B00101,
    B11110,
    B00010
  }
};

byte cloud[3][8] = {

  { B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B01110,
    B11111,
    B00000
  },

  { B00000,
    B00000,
    B00000,
    B01100,
    B11110,
    B00000,
    B00000,
    B00000
  },

  { B00000,
    B00000,
    B00000,
    B01110,
    B11111,
    B00000,
    B00000,
    B00000
  }
};

byte cacto[3][8] = {
  { B00000,
    B00100,
    B00100,
    B10100,
    B10100,
    B11101,
    B00111,
    B00100
  },

  { B00000,
    B00100,
    B00101,
    B00101,
    B10101,
    B11111,
    B00100,
    B00100
  },

  { B00000,
    B00000,
    B00100,
    B00100,
    B00101,
    B10111,
    B11100,
    B00100
  }
};

void setup() {
  pinMode(btn, INPUT);
  lcd.createChar(0, person[0]);
  lcd.createChar(1, person[1]);
  lcd.createChar(2, cloud[0]);
  lcd.createChar(3, cloud[1]);
  lcd.createChar(4, cloud[2]);
  lcd.createChar(5, cacto[0]);
  lcd.createChar(6, cacto[1]);
  lcd.createChar(7, cacto[2]);
  lcd.begin(16, 2);
  lcd.clear();
  Serial.begin(9600);
}

void loop() {
  //Serial.println(opc);
  switch (opc) {
    //Initial Screen
    case 0:
      score = 0;
      lcd.setCursor(2, 0);
      lcd.print("PRESS BUTTON");
      lcd.setCursor(1, 1);
      lcd.print("to start game!");

      if (digitalRead(btn) == HIGH && !pressed) {
        pressed = true;
        t1 = millis();

      }else if (digitalRead(btn) == LOW && pressed) {
        pressed = false;

        if (millis() - t1 > 1000) {
          delay(500);
          lcd.clear();
          opc = 2;
        } else {
          delay(500);
          lcd.clear();
          opc = 3;
        }
      }
      break;

    //Game Over Screen
    case 1:
      lcd.setCursor(3, 0);
      lcd.print("GAME OVER!");
      lcd.setCursor(6, 1);
      lcd.print("+.+");

      if (digitalRead(btn) == HIGH) {
        lcd.clear();
        opc = 2;
        delay(250);
      }
      break;

    //Score Screen
    case 2:
      if (score < EEPROM.read(pos_eeprom)) {
        lcd.setCursor(0, 0);
        lcd.print("YOUR SCORE:");
        lcd.setCursor(13, 0);
        lcd.print(score);
        lcd.setCursor(0, 1);
        lcd.print("BEST SCORE: ");
        lcd.setCursor(13, 1);
        lcd.print(EEPROM.read(pos_eeprom));
      } else {
        lcd.setCursor(0, 0);
        lcd.print("CONGRATULATION!");
        lcd.setCursor(0, 1);
        lcd.print("NEW RECORD: ");
        lcd.setCursor(12, 1);
        lcd.print(EEPROM.read(pos_eeprom));
      }

      if (digitalRead(btn) == HIGH) {
        lcd.clear();
        score = 0;
        opc = 0;
        delay(250);
      }
      break;

    //Game Screen
    case 3:
      interv = 350 - score * 5;
      time_jump = 750 - score * 5;

      lcd.setCursor(7, 0);
      lcd.print(score);

      //Serial.println(score);

      if (jumping && millis() - t_jumping >= time_jump)
      {
        jumping = false;
        lcd.clear();
        lcd.setCursor(0, 1);
      }

      if (!jumping)
      {
        coordenada = 1;
        lcd.setCursor(0, coordenada);
        lcd.write(current_sprite);
      }
      else
      {
        coordenada = 0;
        lcd.setCursor(0, coordenada);
        lcd.write(current_sprite);
      }

      if (millis() - t1 >= interv)
      {
        if (x < 0) x = 16;
        if (y < 0) y = 16;
        if (z < 0) z = 16;
        if (cact_count1 < 0) cact_count1 = 27;
        if (cact_count2 < 0) cact_count2 = 16;
        if (cact_count3 < 0) cact_count3 = 16;

        lcd.clear();
        t1 = millis();
        current_sprite = !current_sprite;

        lcd.setCursor(x, 0);
        lcd.write(2);
        lcd.setCursor(y, 0);
        lcd.write(3);
        lcd.setCursor(z, 0);
        lcd.write(4);
        lcd.setCursor(cact_count1, 1);
        lcd.write(5);
        lcd.setCursor(cact_count2, 1);
        lcd.write(6);
        lcd.setCursor(cact_count3, 1);
        lcd.write(7);
        x--;
        y--;
        z--;
        cact_count1--;
        cact_count2--;
        cact_count3--;

        if (!coordenada && cact_count1 == -1 || !coordenada && cact_count2 == -1 || !coordenada && cact_count3 == -1) {
          score++;
        }
      }

      if (digitalRead(btn) == HIGH && !jumping && !pressed) {
        jumping = true;
        pressed = true;
        t_jumping = millis();
        lcd.setCursor(0, 1);
        lcd.print(" ");
      } else if (digitalRead(btn) == LOW) pressed = false;

      if (coordenada && cact_count1 == -1 || coordenada && cact_count2 == -1 || coordenada && cact_count3 == -1) {
        lcd.clear();
        opc = 1;

        if (score > EEPROM.read(pos_eeprom)) {
          EEPROM.write(pos_eeprom, score);
        }
      }
      break;
  }
}

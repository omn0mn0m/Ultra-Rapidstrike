/****************************************************************************
 * GNU GPL v3.0 License                                                     *
 * Copyright (c) 2018 Nam Tran                                              *
 *                                                                          *
 * This file is part of Ultra-Rapdistrike.                                  *
 *                                                                          *
 *   Ultra-Rapidstrike is free software: you can redistribute it and/or     *
 *   modify it under the terms of the GNU Lesser General Public License as  *
 *   published by the Free Software Foundation, either version 3 of the     *
 *   License, or (at your option) any later version.                        *
 *                                                                          *
 *   Ultra-Rapidstrike is distributed in the hope that it will be useful,   *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU Lesser General Public License for more details.                    *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with Ultra-Rapidstrike.  If not, see                     *
 *   <http://www.gnu.org/licenses/>.
 ****************************************************************************/

// Arduino Include
#include <Arduino.h>

// Hardware Includes
#include <Servo.h>

// Softwarwe Includes
#include "config.h"

Servo flywheels;

bool pusher_homed;

// Methods
bool isPressed(int pin);

/**
 * Setup code to run once
 */
void
setup() {
  pinMode(PUSHER_ON_PIN, OUTPUT);
  pinMode(PUSHER_OFF_PIN, OUTPUT);

  pinMode(PUSHER_CYCLE_PIN, INPUT_PULLUP);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(REV_PIN, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);

  flywheels.attach(FLYWHEEL_PIN);
  flywheels.write(1000);

  Serial.begin(9600);

  while (!isPressed(PUSHER_CYCLE_PIN)) {
    analogWrite(PUSHER_ON_PIN, 40);
    delay(50);
    analogWrite(PUSHER_ON_PIN, 0);
    delay(10);
    digitalWrite(PUSHER_OFF_PIN, HIGH);
    delay(100);
    digitalWrite(PUSHER_OFF_PIN, LOW);
    Serial.println(isPressed(PUSHER_CYCLE_PIN));
  }

  analogWrite(PUSHER_ON_PIN, 0);
  pusher_homed = true;
}

/**
 * Main code to run repeatedly
 */
void
loop() {
  if (isPressed(REV_PIN)) {
    flywheels.write(2000);
  } else {
    flywheels.write(1000);
  }

  if (isPressed(TRIGGER_PIN)) {
    analogWrite(PUSHER_ON_PIN, 255);
    pusher_homed = false;
  } else {
    if (!pusher_homed) {
      while (!isPressed(PUSHER_CYCLE_PIN)) {
        analogWrite(PUSHER_ON_PIN, 40);
      }

      analogWrite(PUSHER_ON_PIN, 0);
      delay(10);
      digitalWrite(PUSHER_OFF_PIN, HIGH);
      delay(100);
      digitalWrite(PUSHER_OFF_PIN, LOW);

      pusher_homed = true;
    } else {
      digitalWrite(PUSHER_ON_PIN, LOW);
    }
  }
}

/**
 * Returns whether a switch is pressed
 *
 * @return True if
 */
bool
isPressed(int pin) {
  return !digitalRead(pin);
}

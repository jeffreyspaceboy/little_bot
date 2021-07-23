#! /usr/bin/env python3

import RPi.GPIO as GPIO #Raspberry Pi
import time

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

class Motor:
    def __init__(self, gpio_speed, gpio_dir_1, gpio_dir_2, gpio_enc_a, gpio_enc_b, rpm=100.0, max_power=100.0, enc_rev=1200, pwm_frequency=20.0):
        self.rpm = rpm
        self.rps = rpm/60
        self.max_power = max_power
        self.frequency = pwm_frequency
        
        self.gpio_speed = gpio_speed
        self.gpio_dir_1 = gpio_dir_1
        self.gpio_dir_2 = gpio_dir_2
        self.gpio_enc_a = gpio_enc_a
        self.gpio_enc_b = gpio_enc_b

        #  Configure GPIO
        GPIO.setup(self.gpio_speed,  GPIO.OUT)
        GPIO.setup(self.gpio_dir_1, GPIO.OUT)
        GPIO.setup(self.gpio_dir_2, GPIO.OUT)
        
        GPIO.setup(self.gpio_enc_a, GPIO.IN)
        GPIO.setup(self.gpio_enc_b, GPIO.IN)
        GPIO.add_event_detect(self.gpio_enc_a, GPIO.BOTH)
        GPIO.add_event_callback(self.gpio_enc_a, self.refresh_encoder_callback)
        GPIO.add_event_detect(self.gpio_enc_b, GPIO.BOTH)
        GPIO.add_event_callback(self.gpio_enc_b, self.refresh_encoder_callback)

        self.enc_count = 0
        self.enc_state = GPIO.input(self.gpio_enc_a)
        self.enc_last_state = self.enc_state

        self.pwm_speed = GPIO.PWM(self.gpio_speed, self.frequency)
        self.stop()

    def __del__(self):
        self.stop()
        GPIO.remove_event_detect(self.gpio_enc_a)
        GPIO.remove_event_detect(self.gpio_enc_b)

    def spin(self, power):
        if(power > self.max_power):
            power = self.max_power
        if(power < -self.max_power):
            power = -self.max_power
        if(power > 0):
            GPIO.output(self.gpio_dir_1, GPIO.HIGH)
            GPIO.output(self.gpio_dir_2, GPIO.LOW)
            self.pwm_speed.start(power)
        elif(power < 0):
            GPIO.output(self.gpio_dir_1, GPIO.LOW)
            GPIO.output(self.gpio_dir_2, GPIO.HIGH)
            self.pwm_speed.start(power)
        else:
            self.stop()

    def stop (self):
        GPIO.output(self.gpio_dir_1, GPIO.LOW)
        GPIO.output(self.gpio_dir_2, GPIO.LOW)
        self.pwm_speed.stop()

    def reset_encoder(self):
        self.enc_count = 0

    def refresh_encoder_callback(self):
        self.enc_state = GPIO.input(self.gpio_enc_a)
        if(self.enc_state != self.enc_last_state):
            if(GPIO.input(self.gpio_enc_b) != self.enc_state):
                self.enc_count = self.enc_count + 1
            else:
                self.enc_count = self.enc_count - 1
        self.enc_last_state = self.enc_state

def main():
    left_motor = Motor(gpio_speed = 2, gpio_dir_1 = 4, gpio_dir_2 = 3, gpio_enc_a = 27, gpio_enc_b = 17, frequency=20, max_speed=100)
    right_motor = Motor(gpio_speed = 13, gpio_dir_1 = 6, gpio_dir_2 = 5, gpio_enc_a = 26, gpio_enc_b = 19, frequency=20, max_speed=100)

    left_motor.spin(100)
    right_motor.spin(100)
    time.sleep(0.859) #Should drive 1 foot
    left_motor.stop()
    right_motor.stop()

if __name__ == '__main__':
    main()
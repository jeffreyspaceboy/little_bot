/*---MOTOR_C---*/
/*----------------------------------------------------------------------------*/
/*    Module:       Motor.c                                                   */
/*    Author:       Jeffrey Fisher II                                         */
/*    Created:      2021-12-06                                                */
/*----------------------------------------------------------------------------*/

/* LOCAL INCLUDES */
#include "../include/Motor.h"

/* NON-STANDARD INCLUDES */
#include <pigpio.h>

/* STANDARD INCLUDES */
#include <stdlib.h>
#include <string.h>


Motor motor_init(char motor_name[NAME_MAX_SIZE], uint8_t gpio_enable_pin, uint8_t gpio_phase_a_pin, uint8_t gpio_phase_b_pin, Encoder *new_encoder, int reverse){
    Motor new_motor = {
        .name = "",
        .gpio_enable = gpio_enable_pin,
        .gpio_phase_a = (reverse == 0) ? gpio_phase_a_pin : gpio_phase_b_pin,
        .gpio_phase_b = (reverse == 0) ? gpio_phase_b_pin : gpio_phase_a_pin,
        .max_power = MOTOR_DEFAULT_MAX_POWER,
        .encoder = new_encoder,
    };
    strncpy(new_motor.name, motor_name, sizeof(new_motor.name));
    gpioSetMode(new_motor.gpio_enable, PI_OUTPUT);
    gpioSetMode(new_motor.gpio_phase_a, PI_OUTPUT);
    gpioSetMode(new_motor.gpio_phase_b, PI_OUTPUT);
    motor_stop(&new_motor);
    encoder_start(new_motor.encoder);
    return new_motor;
}

int motor_del(Motor *motor){ 
    return encoder_del(motor->encoder) || motor_stop(motor);  
}


int motor_spin(Motor *motor, int power){
    if(abs(power) > motor->max_power){
        gpioPWM(motor->gpio_enable, motor->max_power);
    }else{
        gpioPWM(motor->gpio_enable, abs(power));
    }

    if(power == 0){
        return motor_stop(motor);
    }else if(power > 0){
        gpioWrite(motor->gpio_phase_a, 1);
        gpioWrite(motor->gpio_phase_b, 0);
    }else{
        gpioWrite(motor->gpio_phase_a, 0);
        gpioWrite(motor->gpio_phase_b, 1);
    }
    return SUCCESS;
}

int motor_stop(Motor *motor){
    gpioWrite(motor->gpio_enable, 0);
    gpioWrite(motor->gpio_phase_a, 0);
    gpioWrite(motor->gpio_phase_b, 0);
    return SUCCESS;
}

int motor_set_max_power(Motor *motor, int new_max_power){
    motor->max_power = new_max_power;
    return new_max_power;
}


float motor_get_rotations(Motor *motor){
    return encoder_get_rotations(motor->encoder);
}

float motor_get_angle_degrees(Motor *motor){
    return encoder_get_angle_degrees(motor->encoder);
}

float motor_get_angle_radians(Motor *motor){
    return encoder_get_angle_radians(motor->encoder);
}

float motor_get_rps(Motor *motor){
    return motor->encoder->rps;
}
/*---MOTOR_C---*/
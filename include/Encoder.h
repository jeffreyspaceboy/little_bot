/*---ENCODER_H---*/
#ifndef ENCODER_H
#define ENCODER_H
/*----------------------------------------------------------------------------*/
/*    Module:       Encoder.h                                                 */
/*    Author:       Jeffrey Fisher II                                         */
/*    Created:      2021-12-07                                                */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#include "Definitions.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct{
    char name[NAME_MAX_SIZE];
    int gpio_phase_a, gpio_phase_b;
    int prev_gpio, level_phase_a, level_phase_b, ticks, prev_ticks;
    uint32_t prev_tick_us;
    double rpm, ratio;
} Encoder;

typedef void (*gpioISRFuncEx_t)(int gpio, int level, uint32_t tick, void *data);

Encoder encoder_init(char encoder_name[NAME_MAX_SIZE], int gpio_phase_a_pin, int gpio_phase_b_pin, float encoder_ratio, bool reverse);
int encoder_del(Encoder *encoder);

int encoder_start(Encoder *encoder);
int encoder_reset(Encoder *encoder);
double encoder_refresh_rpm(Encoder *encoder, uint32_t current_tick_us);
void encoder_event_callback(int gpio, int level, uint32_t tick, void *data);

#ifdef __cplusplus
}
#endif
#endif
/*---ENCODER_H---*/
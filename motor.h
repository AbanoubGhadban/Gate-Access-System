/*
 * motor.h
 *
 *  Created on: Dec 28, 2019
 *      Author: abanoub
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#define MOTOR_CTL GPIO_PORTE_DATA_R
#define MOTOR_DIR1 (1U << 3)
#define MOTOR_DIR2 (1U << 4)
#define MOVING_TIME_MS 500

void motor_init(void);
void door_open(void);
void door_close(void);

#endif /* MOTOR_H_ */

// include directives

// RIGHT WHEEL IS MOTOR 2 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// speed is a value between 0 and 65535


/* Standard includes */
#include "project.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "serial.h"
#include "movement.h"



static xComPortHandle pxPort = NULL;
#define comRX_BLOCK_TIME			( ( TickType_t ) 0xffff )

/* initiate functions */

void turn_wheel(char side, int volt) {
    // turning right wheel
    if (side == 'r'){
        Standby_Write(1);
        PWM_M2_WriteCompare(abs(volt));
        
        if (volt > 0)
        {
            M2_FWD_Write(0);
            M2_BWD_Write(1);
        }
        else if (volt < 0)
        {
            M2_FWD_Write(1);
            M2_BWD_Write(0);
        }
        else
        {
            M2_FWD_Write(0);
            M2_BWD_Write(0);
        }
    }
    
    //turning left wheel
    if (side == 'l'){
        Standby_Write(1);
        PWM_M1_WriteCompare(abs(volt));
        
        if (volt > 0)
        {
            M1_FWD_Write(0);
            M1_BWD_Write(1);
        }
        else if (volt < 0)
        {
            M1_FWD_Write(1);
            M1_BWD_Write(0);
        }
        else
        {
            M1_FWD_Write(0);
            M1_BWD_Write(0);
        }
    }
}
void mov_update_error(wheel_data *left, wheel_data *right) {
    //left->cur_dest = left->cur_dest + left->inc;
    //right->cur_dest = right->cur_dest + right->inc;
    
    left->time_prev = left->time;
    left->time = 357912 - Timer_1_ReadCounter() / 12000;
    
    right->time_prev = right->time;
    right->time = 357912 - Timer_1_ReadCounter() / 12000;
    
    
    left->error_prev = left->error;
    right->error_prev = right->error;
    
    left->error = left->cur_dest - (-64*M1QuadDec_GetCounter());
    right->error = right->cur_dest - (-64*M2QuadDec_GetCounter());
    
    left->error_sum = left->error_sum + left->error;
    right->error_sum = right->error_sum + right->error;

}
void mov_get_PID(wheel_data *left, wheel_data *right, PID_data *k) {
    int motor_voltage;
    motor_voltage =  ( k->Kp * left->error ) + ( k->Ki * left->error_sum ) + ( k->Kd * (left->error - left->error_prev) * (left->time - left->time_prev) );

    if (motor_voltage < -65535){
        motor_voltage = -65535;}
    else if (motor_voltage > 65535){
        motor_voltage = 65535;}
    
    left->motor_volt = motor_voltage;
        
    motor_voltage =  ( k->Kp * right->error ) + ( k->Ki * right->error_sum ) + ( k->Kd * (right->error - right->error_prev) * (right->time - right->time_prev) );

    if (motor_voltage < -65535){
        motor_voltage = -65535;}
    else if (motor_voltage > 65535){
        motor_voltage = 65535;}
    
    right->motor_volt = motor_voltage;
}    
void mov_Adj_Volt(wheel_data *left, wheel_data *right) {
    turn_wheel('l', left->motor_volt);
    turn_wheel('r', right->motor_volt);
}
void initialise_wheel_data(wheel_data *w){
    w->cur_dest = 0;
    w->error = 0;
    w->error_prev = 0;
    w->error_sum = 0;
    w->inc = 0;
    w->motor_volt = 0;
    w->prev_dest = 0;
    w->time_prev = 0;
    w->time = 0;
}
void update_inc( wheel_data *left, wheel_data *right, unsigned char *buffer){
    
    int int0 = (unsigned char) buffer[0] - 48;
    int int1 = (unsigned char) buffer[1] - 48;
    int int2 = (unsigned char) buffer[2] - 48;
    int int3 = (unsigned char) buffer[3] - 48;
    int int4 = (unsigned char) buffer[4] - 48;
    int int5 = (unsigned char) buffer[5] - 48;
    int int6 = (unsigned char) buffer[6] - 48;
    int int7 = (unsigned char) buffer[7] - 48;
    
    
    left->cur_dest = int0*1000 + int1*100 + int2*10 + int3*1;
    right->cur_dest = int4*1000 + int5*100 + int6*10 + int7*1;
}
void update_k( PID_data *K, unsigned char *buffer){
    
    int int0 = (unsigned char) buffer[0] - 48;
    int int1 = (unsigned char) buffer[1] - 48;
    int int2 = (unsigned char) buffer[2] - 48;
    int int3 = (unsigned char) buffer[3] - 48;
    int int4 = (unsigned char) buffer[4] - 48;
    int int5 = (unsigned char) buffer[5] - 48;
    int int6 = (unsigned char) buffer[6] - 48;
    int int7 = (unsigned char) buffer[7] - 48;
    
    
    K->Kp = (int2*100000 + int3*10000 + int4*1000 + int5*100 +int6*10 +int7*1)/100;
    K->Kd =0;
    K->Ki = 0;
}
/* [] END OF FILE */

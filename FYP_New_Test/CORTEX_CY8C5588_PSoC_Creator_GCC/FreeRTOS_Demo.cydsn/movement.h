#include <stdint.h>

typedef struct {
    long int prev_dest;
    long int cur_dest;
    int error_prev;
    int error;
    int error_sum;
    int motor_volt;
    int inc;
    uint32_t time;
    long int time_prev;
} wheel_data;

typedef struct {
    double Ki;
    double Kp;
    double Kd;
} PID_data;


extern void mov_update_error(wheel_data *left, wheel_data *right);
extern void mov_get_PID(wheel_data *left, wheel_data *right, PID_data *k);          
extern void mov_Adj_Volt(wheel_data *left, wheel_data *right);
extern void initialise_wheel_data(wheel_data *w);
extern void update_inc( wheel_data *left, wheel_data *right, unsigned char *buffer);
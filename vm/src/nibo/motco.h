
// aktueller Status
extern int16_t motco_speed_l;
extern int16_t motco_speed_r;
extern int16_t motco_ticks_l;
extern int16_t motco_ticks_r;

extern float motco_pos_x;
extern float motco_pos_y;
extern float motco_pos_ori;

uint8_t motco_update();

uint8_t motco_stop();
uint8_t motco_setPWM(int16_t left, int16_t right);
uint8_t motco_setSpeed(int16_t left, int16_t right);
uint8_t motco_setParameters(int8_t ki, int8_t kp, int8_t kd);
uint8_t motco_resetOdometry(int16_t left, int16_t right);

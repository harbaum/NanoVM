
enum {
  DISTCO_PASSIVE = 0,
  DISTCO_RC5TX   = 1,
  DISTCO_MEASURE = 2,
};

extern uint8_t distco_mode;
extern uint8_t distco_distance[5];
extern uint16_t distco_rc5_cmd;

/*
extern uint8_t distco_rc5_frame[5];
extern uint8_t distco_rc5_size;
*/

uint8_t distco_update();

uint8_t distco_stop();
uint8_t distco_startMeasure();
uint8_t distco_transmitRC5(uint16_t rc5);

#include "iodefs.h"
#include "motco.h"
#include "i2c_master.h"

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) (((uint8_t)hi*(uint16_t)0x100)+(uint8_t)lo)

int16_t motco_speed_l;
int16_t motco_speed_r;

int16_t motco_ticks_l;
int16_t motco_ticks_r;

float motco_pos_x;
float motco_pos_y;
float motco_pos_ori;

uint8_t motco_transmit()
{
  if (i2c_start_transmission()==0)
    return 0;
  if (i2c_wait_transmission()==0)
    return 0;
  if (i2c_status()!=I2C_SUCCESS)
    return 0;
  return 1;
}


uint8_t motco_update()
{
  i2c_size=9;
  i2c_buf[0] = I2C_RX(MOTCO_I2C_ID);
  if (motco_transmit()==0)
    return 0;
  motco_ticks_l = MAKE_WORD(i2c_buf[1], i2c_buf[2]);
  motco_ticks_r = MAKE_WORD(i2c_buf[3], i2c_buf[4]);
  motco_speed_l = MAKE_WORD(i2c_buf[5], i2c_buf[6]);
  motco_speed_r = MAKE_WORD(i2c_buf[7], i2c_buf[8]);
  return 1;
}

uint8_t motco_stop()
{
  i2c_size = 2;
  i2c_buf[0] = I2C_TX(MOTCO_I2C_ID);
  i2c_buf[1] = 0x00;
  return motco_transmit();
}

uint8_t motco_setPWM(int16_t left, int16_t right)
{
  i2c_size = 6;
  i2c_buf[0] = I2C_TX(MOTCO_I2C_ID);
  i2c_buf[1] = 0x01;
  i2c_buf[2] = HIBYTE(left);
  i2c_buf[3] = LOBYTE(left);
  i2c_buf[4] = HIBYTE(right);
  i2c_buf[5] = LOBYTE(right);
  return motco_transmit();
}

uint8_t motco_setSpeed(int16_t left, int16_t right)
{
  i2c_size = 6;
  i2c_buf[0] = I2C_TX(MOTCO_I2C_ID);
  i2c_buf[1] = 0x02;
  i2c_buf[2] = HIBYTE(left);
  i2c_buf[3] = LOBYTE(left);
  i2c_buf[4] = HIBYTE(right);
  i2c_buf[5] = LOBYTE(right);
  return motco_transmit();
}

uint8_t motco_setParameters(int8_t ki, int8_t kp, int8_t kd)
{
  i2c_size = 5;
  i2c_buf[0] = I2C_TX(MOTCO_I2C_ID);
  i2c_buf[1] = 0x03;
  i2c_buf[2] = ki;
  i2c_buf[3] = kp;
  i2c_buf[4] = kd;
  return motco_transmit();
}

uint8_t motco_resetOdometry(int16_t left, int16_t right)
{
  i2c_size = 6;
  i2c_buf[0] = I2C_TX(MOTCO_I2C_ID);
  i2c_buf[1] = 0x04;
  i2c_buf[2] = HIBYTE(left);
  i2c_buf[3] = LOBYTE(left);
  i2c_buf[4] = HIBYTE(right);
  i2c_buf[5] = LOBYTE(right);
  return motco_transmit();
}

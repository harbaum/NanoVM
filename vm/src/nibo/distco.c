#include "iodefs.h"
#include "distco.h"
#include "i2c_master.h"

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) (((uint8_t)hi*(uint16_t)0x100)+(uint8_t)lo)

uint8_t distco_mode;
uint8_t distco_distance[5];
uint8_t distco_rc5_count;
//uint8_t distco_rc5_frame[5];
//uint8_t distco_rc5_size;
uint16_t distco_rc5_cmd;

uint8_t distco_transmit()
{
  if (i2c_start_transmission()==0)
    return 0;
  if (i2c_wait_transmission()==0)
    return 0;
  if (i2c_status()!=I2C_SUCCESS)
    return 0;
  return 1;
}

uint8_t distco_update()
{
  i2c_buf[1]=0xaf;
  i2c_buf[2]=0xfe;
  i2c_buf[3]=0xaf;
  i2c_buf[4]=0xfe;
  i2c_buf[5]=0xaf;
  i2c_buf[6]=0xfe;
  i2c_buf[7]=0xaf;
  i2c_buf[8]=0xfe;

  i2c_size = 9;
  i2c_buf[0] = I2C_RX(DISTCO_I2C_ID);
  if (!distco_transmit())
    return 0;

  distco_distance[0] = i2c_buf[1];
  distco_distance[1] = i2c_buf[2];
  distco_distance[2] = i2c_buf[3];
  distco_distance[3] = i2c_buf[4];
  distco_distance[4] = i2c_buf[5];
  distco_rc5_count = i2c_buf[6];

  uint16_t cmd = MAKE_WORD(i2c_buf[7], i2c_buf[8]);
  if (cmd)
    distco_rc5_cmd=cmd;
  return 1;
}

uint8_t distco_stop()
{
  i2c_size = 2;
  i2c_buf[0] = I2C_TX(DISTCO_I2C_ID);
  i2c_buf[1] = 0x00;
  return distco_transmit();
}

uint8_t distco_startMeasure()
{
  i2c_size = 2;
  i2c_buf[0] = I2C_TX(DISTCO_I2C_ID);
  i2c_buf[1] = 0x01;
  return distco_transmit();
}

uint8_t distco_transmitRC5(uint16_t rc5)
{
  i2c_size = 4;
  i2c_buf[0] = I2C_TX(DISTCO_I2C_ID);
  i2c_buf[1] = 0x02;
  i2c_buf[2] = HIBYTE(rc5);
  i2c_buf[3] = LOBYTE(rc5);
  return distco_transmit();
}


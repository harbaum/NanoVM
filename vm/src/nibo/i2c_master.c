#include "iodefs.h"
#include "i2c_master.h"
#include <avr/interrupt.h>
#include "delay.h"

volatile uint8_t i2c_state;

volatile uint8_t i2c_size;
volatile uint8_t i2c_pos;

uint8_t i2c_buf[I2C_BUF_SIZE];
volatile uint8_t i2c_last_TWSR;

enum
{
  I2C_Idle,
  I2C_Address,
  I2C_Rx,
  I2C_Rx_Ack,
  I2C_Tx,
  I2C_Tx_Ack,
  I2C_Locked
};


uint8_t i2c_start_transmission(void)
{
  uint8_t res = 0;
  cli();
  do{
    if (i2c_state == I2C_BUSY)
    {
      i2c_state = I2C_ERROR+1;
      break;
    }
  
    if (TWCR & _BV(TWSTO))
    {
      i2c_state = I2C_ERROR+2;
      break;
    }
    if (TWCR & _BV(TWSTA))
    {
      TWCR = 0;
      TWCR = _BV(TWEN);
      i2c_state = I2C_ERROR+8;
      break;
    }
    i2c_state = I2C_BUSY;
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWSTA);
    res = 1;
  } while(0);
  sei();
  return res;
}

uint8_t i2c_wait_transmission(void)
{
  for (uint16_t i=0; i<10000; ++i)
  {
    if (TWCR & _BV(TWIE))
    {
      delay_us(1);
    }
    else
    {
      return 1;
    }
  }
  cli();
  TWCR = 0;
  TWCR = _BV(TWEN);
  if (i2c_state < I2C_ERROR)
    i2c_state = I2C_ERROR+3;
  sei();
  return 0;
}


uint8_t i2c_status(void)
{
  uint8_t res = i2c_state;
  return res;
}


void i2c_init(void)
{
  TWCR = 0;
  TWBR = I2C_TWBR_INIT;
  TWDR = 0xff;
  TWCR = _BV(TWEN);
  i2c_state = I2C_IDLE;
}


ISR(TWI_vect)
{
  uint8_t res = TWSR & 0xf8;
  i2c_last_TWSR = res;
  switch (res)
  {
    case 0x08: // Start condition
    case 0x10: // Repeated start condition
      i2c_pos=0;
    case 0x18: // W-Address ACK received
    case 0x28: // Data tx and ACK received
      if (i2c_pos<i2c_size)
      {
        TWDR = i2c_buf[i2c_pos++];
        TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
      }
      else
      {
        i2c_state = I2C_SUCCESS;
        TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTO);
      }
      break;
    
    case 0x50: // Data rx and ACK sent
      i2c_buf[i2c_pos++] = TWDR;
    case 0x40: // R-Address ACK received
      if (i2c_pos+1==i2c_size)
      {
        // Receive last byte, and send NAK
        TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
      }
      else
      {
        // Receive next byte, and send ACK
        TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
      }
      break;
    
    case 0x58: // Data rx and NACK sent
      // Last byte was received
      i2c_buf[i2c_pos] = TWDR;
      i2c_state = I2C_SUCCESS;
      TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTO);
      break;
      
    case 0x38: // Arbitration was lost
      TWCR = _BV(TWEN) | _BV(TWINT);
      i2c_state = I2C_ERROR+5;
      break;

    case 0x00: // Bus Error
    case 0x20: // W-Address NACK received
    case 0x48: // R-Address NACK received
    case 0x30: // Data tx and NACK received
      TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTO);
      i2c_state = I2C_ERROR+6;
      break;
    
    default:
      TWCR = _BV(TWEN) | _BV(TWINT);
      i2c_state = I2C_ERROR+7;
  }
}

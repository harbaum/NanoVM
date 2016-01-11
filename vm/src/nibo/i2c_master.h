
extern volatile uint8_t i2c_size;


extern uint8_t i2c_buf[I2C_BUF_SIZE];

enum {
  I2C_BUSY,
  I2C_IDLE,
  I2C_SUCCESS,
  I2C_ERROR=0x10
};

void i2c_init(void);

uint8_t i2c_start_transmission(void);

uint8_t i2c_status(void);
extern volatile uint8_t i2c_last_TWSR;
extern volatile uint8_t i2c_pos;

uint8_t i2c_wait_transmission(void);

#define I2C_TX(ADDR) ((ADDR)<<1)
#define I2C_RX(ADDR) (((ADDR)<<1)+1)

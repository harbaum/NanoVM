#include "nibo/iodefs.h"
#include "nibo/delay.h"
#include <avr/io.h>


#define AVRISP_UDR   UDR0
#define AVRISP_UCSRA UCSR0A
#define AVRISP_UCSRB UCSR0B
#define AVRISP_UBRRH UBRR0H
#define AVRISP_UBRRL UBRR0L


void avrisp_init()
{
  // set up serial port
  // 9600 baud
  AVRISP_UBRRH = 0;
  AVRISP_UBRRL = 103;
  AVRISP_UCSRA &= ~0x02;
  AVRISP_UCSRB = _BV(TXEN) | _BV(RXEN);

}

/* | Enter programming mode            | 'P' |       |      | 13d |   1  |
;* | Report autoincrement address      | 'a' |       |      | 'Y' |      |
;* | Set address                       | 'A' | ah al |      | 13d |   2  |
;* | Write program memory, low byte    | 'c' |    dd |      | 13d |   3  |
;* | Write program memory, high byte   | 'C' |    dd |      | 13d |   3  |
;* | Issue Page Write                  | 'm' |       |      | 13d |      |
;* | Read program memory               | 'R' |       |dd(dd)|     |   4  |
;* | Write data memory                 | 'D' |    dd |      | 13d |      |
;* | Read data memory                  | 'd' |       |   dd |     |      |
;* | Chip erase                        | 'e' |       |      | 13d |      |
;* | Write lock bits                   | 'l' |    dd |      | 13d |      |
;* | Write fuse bits                   | 'f' |    dd |      | 13d |  11  |
;* | Read fuse and lock bits           | 'F' |       |   dd |     |  11  |
;* | Leave programming mode            | 'L' |       |      | 13d |   5  |
;* | Select device type                | 'T' |    dd |      | 13d |   6  |
;* | Read signature bytes              | 's' |       | 3*dd |     |      |
;* | Return supported device codes     | 't' |       | n*dd | 00d |   7  |
;* | Return software identifier        | 'S' |       | s[7] |     |   8  |
;* | Return sofware version            | 'V' |       |dd dd |     |   9  |
;* | Return hardware version           | 'v' |       |dd dd |     |   9  |
;* | Return programmer type            | 'p' |       |   dd |     |  10  |
;* | Set LED                           | 'x' |    dd |      | 13d |  12  |
;* | Clear LED                         | 'y' |    dd |      | 13d |  12  |
;* | Universial command                | ':' |  3*dd |   dd | 13d |      |
;* | New universal command             | '.' |  4*dd |   dd | 13d |      |
;* | Special test command              | 'Z' |  2*dd |   dd |     |      |
*/

uint8_t deviceType;
uint8_t addrh;
uint8_t addrl;
uint8_t pgm_mode;

enum {
  ISPSW_MAJOR='2',
  ISPSW_MINOR='3',
  ISPHW_MAJOR='1',
  ISPHW_MINOR='0'
};

enum {
  AVRDEV_tn12    = 0x55,
  AVRDEV_tn15    = 0x56,
  AVRDEV_S1200D  = 0x13,
  AVRDEV_S2313A  = 0x20,
  AVRDEV_S4414A  = 0x28,
  AVRDEV_S8515A  = 0x38,
  AVRDEV_S2323A  = 0x48,
  AVRDEV_S2343A  = 0x4C,
  AVRDEV_S2333A  = 0x34,
  AVRDEV_S4433A  = 0x30,
  AVRDEV_S4434   = 0x6C,
  AVRDEV_S8535   = 0x68,
  AVRDEV_S8555   = 0x65,
  AVRDEV_m161    = 0x60,
  AVRDEV_m163    = 0x64,
  AVRDEV_m32     = 0x72,
  AVRDEV_m603    = 0x42,
  AVRDEV_m103    = 0x41
};

/*/
uint8_t exchange(uint8_t val) {
  SPDR = val;
  while(!SPSR & _BV(SPIF))
    ;
  val = SPDR;
  return val;
}
*/

uint8_t exchange(uint8_t val) {
  uint8_t res=0x00;
  for (uint8_t i=0; i<8; ++i) {
    set_output_bitval(IO_ISP_MOSI, (val&0x80));
    val <<= 1;
    res <<= 1;
    res |= get_input_bit(IO_ISP_MISO)?0x01:0x00;

    set_output_bit(IO_ISP_SCK);
    delay_us(8);
    clear_output_bit(IO_ISP_SCK);
    delay_us(8);
  }
  return res;
}

void avrisp_putchar(char val) {
  while (!(AVRISP_UCSRA & _BV(UDRE)));
  AVRISP_UDR = val;
}

char avrisp_getchar(void){
  while (!(AVRISP_UCSRA & _BV(RXC)));
  char val = AVRISP_UDR;
  return val;
}

void activateReset() {
  clear_output_bit(IO_RESET_5);
  clear_output_bit(IO_RESET_3);
  clear_output_bit(IO_RESET_A);
  clear_output_bit(IO_RESET_B);
}

void deactivateReset() {
  set_output_bit(IO_RESET_5);
  set_output_bit(IO_RESET_3);
  set_output_bit(IO_RESET_A);
  set_output_bit(IO_RESET_B);
}

void enterProgrammingMode()
{
  activate_output_bit(IO_RESET_5);
  activate_output_bit(IO_RESET_3);
  activate_output_bit(IO_RESET_A);
  activate_output_bit(IO_RESET_B);
  activate_output_bit(IO_ISP_SCK);
  activate_output_bit(IO_ISP_MOSI);
  deactivate_output_bit(IO_ISP_MISO);
  
  clear_output_bit(IO_ISP_SCK);
  //SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR1) | _BV(SPR0);

  deactivateReset();
  delay_ms(10);
  activateReset();

  delay_ms(10);
  delay_ms(10);
  
  exchange(0xac);          // SPI (byte 1)
  exchange(0x53);          // SPI (byte 2)
  
  if ((deviceType>=0x20) && (deviceType<=0x7f))
  {
    uint8_t count=32;
    do
    {
      if (exchange(0x00)==0x53) // SPI (byte 3)
      {
        IO_LEDS_GREEN_PORT = 0xfc;
        break;
      }
      exchange(0x00);          // SPI (byte 4)
      exchange(0xac);          // SPI (byte 1)
      exchange(0x53);          // SPI (byte 2)
    } while (count--);
  }
  else
  {
    exchange(0x00); // SPI (byte 3)
  }
  exchange(0x00);          // SPI (byte 4)
  delay_ms(10);
  avrisp_putchar(13);
}

void leaveProgrammingMode(void){
  deactivate_output_bit(IO_ISP_SCK);
  deactivate_output_bit(IO_ISP_MOSI);
  deactivateReset();
  IO_LEDS_GREEN_PORT = 0x00;
  delay_ms(10);
  avrisp_putchar(13);
}


void selectDeviceType(void) {
  deviceType = avrisp_getchar();
  avrisp_putchar(13);
}


void returnSoftwareIdentifier(void) {
  avrisp_putchar('A');
  avrisp_putchar('V');
  avrisp_putchar('R');
  avrisp_putchar(' ');
  avrisp_putchar('I');
  avrisp_putchar('S');
  avrisp_putchar('P');
}

void returnSofwareVersion(void) {
  avrisp_putchar(ISPSW_MAJOR);
  avrisp_putchar(ISPSW_MINOR);
}

void returnHardwareVersion(void) {
  avrisp_putchar(ISPHW_MAJOR);
  avrisp_putchar(ISPHW_MINOR);
}

void returnSupportedDeviceCodes(void) {
  avrisp_putchar(AVRDEV_tn12);
  avrisp_putchar(AVRDEV_tn15);
  avrisp_putchar(AVRDEV_S1200D);
  avrisp_putchar(AVRDEV_S2313A);
  avrisp_putchar(AVRDEV_S4414A);
  avrisp_putchar(AVRDEV_S8515A);
  avrisp_putchar(AVRDEV_S2323A);
  avrisp_putchar(AVRDEV_S2343A);
  avrisp_putchar(AVRDEV_S2333A);
  avrisp_putchar(AVRDEV_S4434);
  avrisp_putchar(AVRDEV_S8535);
  avrisp_putchar(AVRDEV_S8555);
  avrisp_putchar(AVRDEV_m161);
  avrisp_putchar(AVRDEV_m163);
  avrisp_putchar(AVRDEV_m32);
  avrisp_putchar(AVRDEV_m103);
  avrisp_putchar(AVRDEV_m603);
  avrisp_putchar(0x00);
}

void returnProgrammerType(void){
  avrisp_putchar('S');
}

void reportAutoincrementAddress(void){
  avrisp_putchar('Y');
}

void setLED(void){
  avrisp_getchar();
  avrisp_putchar(13);
}

void clearLED(void){
  avrisp_getchar();
  avrisp_putchar(13);
}


void setAddress(void){
  addrh = avrisp_getchar();
  addrl = avrisp_getchar();
  avrisp_putchar(13);
}


void writeProgrammemoryLowByte(void){
  uint8_t val = avrisp_getchar();
  exchange(0x40);
  exchange(addrh);
  exchange(addrl);
  exchange(val);
          
  if (!pgm_mode)
    delay_ms(6);
  avrisp_putchar(13);
}


void writeProgrammemoryHighByte(void){
  uint8_t val = avrisp_getchar();
  exchange(0x48);
  exchange(addrh);
  exchange(addrl);
  exchange(val);
  
  addrl++;
  if (addrl==0)
    addrh++;
  
  if (!pgm_mode)
    delay_ms(6);
  avrisp_putchar(13);
}


void issuePageWrite(void){
  exchange(0x4c);
  exchange(addrh);
  exchange(addrl);
  exchange(0x00);
  delay_ms(6);
  avrisp_putchar(13);
}


void readProgramMemory(void){
  exchange(0x28);
  exchange(addrh);
  exchange(addrl);
  uint8_t val = exchange(0x00);
  avrisp_putchar(val);
  exchange(0x20);
  exchange(addrh);
  exchange(addrl);
  val = exchange(0x00);
  avrisp_putchar(val);
  addrl++;
  if (addrl==0)
    addrh++;
}

void writeDataMemory(void){
  uint8_t val = avrisp_getchar();
        
  exchange(0xc0);
  exchange(addrh);
  exchange(addrl);
  exchange(val);
  
  delay_ms(6);
  
  addrl++;
  if (addrl==0)
    addrh++;
  avrisp_putchar(13);
}

void readDataMemory(void){
  exchange(0xa0);
  exchange(addrh);
  exchange(addrl);
  uint8_t val = exchange(0x00);
  avrisp_putchar(val);
  addrl++;
  if (addrl==0)
    addrh++;
}

void chipErase(void){
  exchange(0xac);
  exchange(0x80);
  exchange(0x04);
  exchange(0x00);
  delay_ms(10);
  delay_ms(10);
  delay_ms(10);
  avrisp_putchar(13);
}

void writeLockBits(void){
  uint8_t val = avrisp_getchar();
  exchange(0xac);
  exchange(0xe0);
  exchange(0x00);
  exchange(val);
  delay_ms(10);
  avrisp_putchar(13);
}

void writeFuseBits(void){
  IO_LEDS_RED_PORT = 10 <<2;
}

void readFuseAndLockBits(void){
  IO_LEDS_RED_PORT = 11 <<2;
}


void readSignatureBytes(void){
  exchange(0x30);
  exchange(0x00);
  exchange(0x02);
  uint8_t val = exchange(0x00);
  avrisp_putchar(val);

  exchange(0x30);
  exchange(0x00);
  exchange(0x01);
  val = exchange(0x00);
  avrisp_putchar(val);
  
  exchange(0x30);
  exchange(0x00);
  exchange(0x00);
  val = exchange(0x00);
  avrisp_putchar(val);
}

void universialCommand(void){
  uint8_t cmd1 = avrisp_getchar();
  uint8_t cmd2 = avrisp_getchar();
  uint8_t cmd3 = avrisp_getchar();
  uint8_t cmd4 = 0x00;
  exchange(cmd1);
  exchange(cmd2);
  exchange(cmd3);
  exchange(cmd4);
  delay_ms(10);
  avrisp_putchar(13);
}

void newUniversalCommand(void){
  uint8_t cmd1 = avrisp_getchar();
  uint8_t cmd2 = avrisp_getchar();
  uint8_t cmd3 = avrisp_getchar();
  uint8_t cmd4 = avrisp_getchar();
  exchange(cmd1);
  exchange(cmd2);
  exchange(cmd3);
  uint8_t res = exchange(cmd4);
  avrisp_putchar(res);
  delay_ms(10);
  avrisp_putchar(13);
}


void avrisp_run()
{
  while(1)
  {
    char c = avrisp_getchar();
    switch (c)
    {
      case 'P': enterProgrammingMode(); break;
      case 'a': reportAutoincrementAddress(); break;
      case 'A': setAddress(); break;
      case 'c': writeProgrammemoryLowByte(); break;
      case 'C': writeProgrammemoryHighByte(); break;
      case 'm': issuePageWrite (); break;
      case 'R': readProgramMemory(); break;
      case 'D': writeDataMemory (); break;
      case 'd': readDataMemory(); break;
      case 'e': chipErase(); break;
      case 'l': writeLockBits(); break;
      case 'f': writeFuseBits(); break;
      case 'F': readFuseAndLockBits(); break;
      case 'L': leaveProgrammingMode(); break;
      case 'T': selectDeviceType(); break;
      case 's': readSignatureBytes(); break;
      case 't': returnSupportedDeviceCodes(); break;
      case 'S': returnSoftwareIdentifier(); break;
      case 'V': returnSofwareVersion(); break;
      case 'v': returnHardwareVersion(); break;
      case 'p': returnProgrammerType(); break;
      case 'x': setLED(); break;
      case 'y': clearLED(); break;
      case ':': universialCommand(); break;
      case '.': newUniversalCommand(); break;
      default: avrisp_putchar('?');
    }
  }
}


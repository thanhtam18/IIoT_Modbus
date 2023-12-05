#ifndef _MODBUS_H
#define _MODBUS_H
#include <inttypes.h>
#include "Arduino.h"

typedef struct
{
    uint8_t u8id;          
    uint8_t u8fct;        
    uint16_t u16RegAdd;   
    uint16_t u16CoilsNo;  
    uint16_t *au16reg;     
}
modbus_t;

enum
{
    RESPONSE_SIZE = 6,
    EXCEPTION_SIZE = 3,
    CHECKSUM_SIZE = 2
};

enum MESSAGE
{
    ID  = 0, 
    FUNC, 
    ADD_HI,
    ADD_LO,
    NB_HI, 
    NB_LO,
    BYTE_CNT  
};

enum MB_FC
{
    MB_FC_NONE                     = 0,  
    MB_FC_READ_COILS               = 1,	
    MB_FC_READ_DISCRETE_INPUT      = 2,	
    MB_FC_READ_REGISTERS           = 3,	
    MB_FC_READ_INPUT_REGISTER      = 4,
    MB_FC_WRITE_COIL               = 5,	
    MB_FC_WRITE_REGISTER           = 6,	
    MB_FC_WRITE_MULTIPLE_COILS     = 15,	
    MB_FC_WRITE_MULTIPLE_REGISTERS = 16	
};

enum COM_STATES
{
    COM_IDLE                     = 0,
    COM_WAITING                  = 1

};

enum ERR_LIST
{
    ERR_NOT_MASTER                = -1,
    ERR_POLLING                   = -2,
    ERR_BUFF_OVERFLOW             = -3,
    ERR_BAD_CRC                   = -4,
    ERR_EXCEPTION                 = -5
};

enum
{
    NO_REPLY = 255,
    EXC_FUNC_CODE = 1,
    EXC_ADDR_RANGE = 2,
    EXC_REGS_QUANT = 3,
    EXC_EXECUTE = 4
};

const unsigned char fctsupported[] =
{
    MB_FC_READ_COILS,
    MB_FC_READ_DISCRETE_INPUT,
    MB_FC_READ_REGISTERS,
    MB_FC_READ_INPUT_REGISTER,
    MB_FC_WRITE_COIL,
    MB_FC_WRITE_REGISTER,
    MB_FC_WRITE_MULTIPLE_COILS,
    MB_FC_WRITE_MULTIPLE_REGISTERS
};

#define T35  5
#define  MAX_BUFFER  64	

class Modbus
{
private:
    Stream *port; 
    uint8_t u8id;
    uint8_t u8txenpin; 
    uint8_t u8state;
    uint8_t u8lastError;
    uint8_t au8Buffer[MAX_BUFFER];
    uint8_t u8BufferSize;
    uint8_t u8lastRec;
    uint16_t *au16regs;
    uint16_t u16InCnt, u16OutCnt, u16errCnt;
    uint16_t u16timeOut;
    uint32_t u32time, u32timeOut, u32overTime;
    uint8_t u8regsize;

    void sendTxBuffer();
    int8_t getRxBuffer();
    uint16_t calcCRC(uint8_t u8length);
    uint8_t validateAnswer();
    uint8_t validateRequest();
    void get_FC1();
    void get_FC3();
    int8_t process_FC1( uint16_t *regs, uint8_t u8size );
    int8_t process_FC3( uint16_t *regs, uint8_t u8size );
    int8_t process_FC5( uint16_t *regs, uint8_t u8size );
    int8_t process_FC6( uint16_t *regs, uint8_t u8size );
    int8_t process_FC15( uint16_t *regs, uint8_t u8size );
    int8_t process_FC16( uint16_t *regs, uint8_t u8size );
    void buildException( uint8_t u8exception );

public:
    Modbus(uint8_t u8id, Stream& port, uint8_t u8txenpin =0);

    void start();
    void setTimeOut( uint16_t u16timeOut); 
    uint16_t getTimeOut(); 
    boolean getTimeOutState(); 
    int8_t query( modbus_t telegram ); 
    int8_t poll(); 
    int8_t poll( uint16_t *regs, uint8_t u8size ); 
    uint16_t getInCnt();
    uint16_t getOutCnt();
    uint16_t getErrCnt(); 
    uint8_t getID(); 
    uint8_t getState();
    uint8_t getLastError(); 
    void setID( uint8_t u8id ); 
    void setTxendPinOverTime( uint32_t u32overTime );
    void end();

    Modbus(uint8_t u8id=0, uint8_t u8serno=0, uint8_t u8txenpin=0) __attribute__((deprecated));


    template<typename T_Stream>
    void begin(T_Stream* port_, long u32speed_) __attribute__((deprecated));

    
    template<typename T_Stream>
    void begin(T_Stream* port_, long u32speed_, uint8_t u8txenpin_) __attribute__((deprecated));

    
    void begin(long u32speed = 19200) __attribute__((deprecated));
};

#endif

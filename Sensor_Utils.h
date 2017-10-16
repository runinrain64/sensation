
#define SENSOR_UTIL_CRC_INIT    0x71

#ifndef DbgPrint
    extern Serial SpiDebug;
    extern Mutex debug_mutex;
    #define DbgPrint(...)   { debug_mutex.lock(); SpiDebug.printf(__VA_ARGS__); debug_mutex.unlock();}
#endif

uint8_t CalCrc8( uint8_t inCrc, uint8_t inData );
uint8_t CalCrc8Block( uint8_t crc, uint8_t *data, uint8_t len );
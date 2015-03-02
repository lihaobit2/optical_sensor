#ifndef SI114X_SYS_OUT
#define SI114X_SYS_OUT


typedef struct Si114xPortConfig
{
    uint8_t i2c_bus_select;
	uint8_t i2c_address;
	GPIO_Port_TypeDef irqPort;
	int irqPin;
}Si114xPortConfig_t;



#endif

#include "rf.hpp"

// Radio Config
Wacky::Nrf24 radio;
Wacky::Nrf24::status status;

// LED Config


bool rf_detection()
{
	bool result = 1;
	status = radio.begin (77, 0xCCCECCCECC);

	if (status != Wacky::Nrf24::OK)
    {
       result = 0;
	}
	return result;
}

bool rf_send_buffer(int8_t* RF_buffer)
{
	bool result = 1;
	
	if (! radio.write (RF_buffer, sizeof(RF_buffer))) 
	{
		result = 0;  					// no receiver
	}
	return result;
}

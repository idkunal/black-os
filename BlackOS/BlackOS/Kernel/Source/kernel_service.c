#include "kernel_service.h"
#include "board_serial.h"
#include "kernel.h"
#include "gpio.h"

// Attribute for the SVC handler
#define KERNEL_SERVICE_NAKED __attribute__((naked))

// Macro for calling the SVC handler with the specified SVC number
#define KERNEL_SERVICE(svc_number) asm volatile ("svc %[arg]" : : [arg] "I" (svc_number))

//--------------------------------------------------------------------------------------------------//

// Implementation of the kernel services
// Note that service 0-10 should not be used since these
// provides kernel services

void KERNEL_SERVICE_NOINLINE service_serial_print(char* data)
{
	KERNEL_SERVICE(SERVICE_SERIAL_PRINT);
}

void KERNEL_SERVICE_NOINLINE service_thread_delay(uint32_t ticks)
{
	KERNEL_SERVICE(SERVICE_THREAD_DELAY);
}

void KERNEL_SERVICE_NOINLINE service_get_pin_value(Pio* hardware, uint8_t pin, uint8_t* value)
{
	KERNEL_SERVICE(SERVICE_GET_PIN_VALUE);
}

//--------------------------------------------------------------------------------------------------//

// IMPORTANT!!!!
// The kernel services runs in from an interrupt handler with low priority
// (lowest of the cortex-m7 implemented interrupts) and should therefore execute
// as fast as possible. If a service takes long to execute, it will destroy
// the system, and the kernel functionality

void kernel_service_handler(uint32_t* svc_argv)
{
	// Extract the SVC number from the parameter list
	uint32_t svc_number = ((char *)svc_argv[6])[-2];
	
	// Check the SVC number to determine which function to run
	
	uint8_t* tmp = (uint8_t*)svc_argv[2];
	
	switch (svc_number)
	{
		case SERVICE_THREAD_DELAY:
		kernel_thread_delay((uint32_t)svc_argv[0]);
		break;
		
		case SERVICE_SERIAL_PRINT:
		board_serial_print((char*)svc_argv[0]);
		break;
		
		case SERVICE_GET_PIN_VALUE:
		if ((gpio_get_pin_value_status_register((Pio*)svc_argv[0]) & (1 << (uint8_t)svc_argv[1])))
		{
			*tmp = 1;
		}
		else
		{
			*tmp = 0;
		}
		break;
	}
}

//--------------------------------------------------------------------------------------------------//
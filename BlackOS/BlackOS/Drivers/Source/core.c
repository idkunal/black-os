#include "core.h"
#include "cmsis_gcc.h"
#include "core_cm7.h"

//--------------------------------------------------------------------------------------------------//

void core_enter_critical_section(uint32_t volatile *atomic)
{
	*atomic = __get_PRIMASK();
	__disable_irq();
	__DMB();
}

//--------------------------------------------------------------------------------------------------//

void core_leave_critical_section(uint32_t volatile *atomic)
{
	__DMB();
	__set_PRIMASK(*atomic);
}

//--------------------------------------------------------------------------------------------------//
#include "watchdog.h"
#include "core.h"

//--------------------------------------------------------------------------------------------------//

void watchdog_disable(void)
{
	CRITICAL_SECTION_ENTER()
	WDT->WDT_MR |= (1 << WDT_MR_WDDIS_Pos);
	CRITICAL_SECTION_LEAVE()
}

//--------------------------------------------------------------------------------------------------//
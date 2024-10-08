/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP 2.0
 *
 *    Copyright (C) 2023-2024 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2024/02.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef MTKBSP_CPU_STM32F4

/*
 *	int_stm32f4.c
 *
 *	Interrupt controller (STM32CUBE STM32F4 )
 */
#include <tk/tkernel.h>
#include <tk/syslib.h>

#include <sysdepend/stm32_cube/lib/libtk/cpu/core/armv7m/int_armv7m.h>

/*----------------------------------------------------------------------*/
/*
 * EXTI (Extended interrupt controller) functions
 * 
 */
/*
 * Enable interrupt 
 */
LOCAL void EnableInt_exti( UINT intno, INT level )
{
	if(intno < N_EXTIEVT) {
		*(_UW*)EXTI_IMR |= (UW)(1<<intno);
	}	
}

LOCAL void DisableInt_exti( UINT intno )
{
	if(intno < N_EXTIEVT) {
		*(_UW*)EXTI_IMR &= ~(UW)(1<<intno);
	}
}

LOCAL void ClearInt_exti( UINT intno )
{
	if(intno < N_EXTIEVT) {
		*(_UW*)EXTI_PR |= (UW)(1<<intno);
	}
}

LOCAL BOOL CheckInt_exti( UINT intno )
{
	UW	pif;

	if(intno < N_EXTIEVT) {
		pif = *(_UW*)EXTI_PR & (UW)(1<<intno);
	}
	return pif?TRUE:FALSE;
}

LOCAL void SetIntMode_exti(UINT intno, UINT mode)
{
	if(mode & IM_HI) {
		if(intno < N_EXTIEVT) {
			*(_UW*)EXTI_RTSR |= (UW)(1<<intno);
		}
	}
	if(mode & IM_LOW) {
		if(intno <N_EXTIEVT) {
			*(_UW*)EXTI_FTSR |= (UW)(1<<intno);
		}
	}
}

/*----------------------------------------------------------------------*/
/*
 * Interrupt control API
 * 
 */
/*
 * Enable interrupt 
 */
EXPORT void EnableInt( UINT intno, INT level )
{
	if( intno <= MAX_NVIC_INTNO) {
		EnableInt_nvic( intno, level);
	} else if(intno >= MIN_EXTI_INTNO && intno <= MAX_EXTI_INTNO) {
		EnableInt_exti( intno - MIN_EXTI_INTNO, level);
	}
}

/*
 * Disable interrupt 
 */
EXPORT void DisableInt( UINT intno )
{
	if( intno <= MAX_NVIC_INTNO) {
		DisableInt_nvic( intno);
	} else if(intno >= MIN_EXTI_INTNO && intno <= MAX_EXTI_INTNO) {
		DisableInt_exti( intno - MIN_EXTI_INTNO);
	}
}

/*
 * Clear interrupt
 */
EXPORT void ClearInt(UINT intno)
{
	if( intno <= MAX_NVIC_INTNO) {
		ClearInt_nvic( intno);
	} else if(intno >= MIN_EXTI_INTNO && intno <= MAX_EXTI_INTNO) {
		ClearInt_exti( intno - MIN_EXTI_INTNO);
	}
}

/*
 * Issue EOI to interrupt controller
 */
EXPORT void EndOfInt(UINT intno)
{
	/* No opetarion. */
}

/*
 * Check active state
 */
EXPORT BOOL CheckInt( UINT intno )
{
	BOOL rtncd;

	if( intno <= MAX_NVIC_INTNO) {
		rtncd = CheckInt_nvic( intno);
	} else if(intno >= MIN_EXTI_INTNO && intno <= MAX_EXTI_INTNO) {
		rtncd = CheckInt_exti( intno - MIN_EXTI_INTNO);
	} else {
		rtncd = FALSE;
	}
	return rtncd;
}

/*
 * Set interrupt mode
 */
EXPORT void SetIntMode(UINT intno, UINT mode)
{
	if(intno >= MIN_EXTI_INTNO && intno <= MAX_EXTI_INTNO) {
		SetIntMode_exti( intno - MIN_EXTI_INTNO, mode);
	}
}
#endif /* SDK_CPU_STM32F4 */
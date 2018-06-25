/*******************************************************************************
* File Name: MClock.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_MClock_H)
#define CY_CLOCK_MClock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void MClock_Start(void) ;
void MClock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void MClock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void MClock_StandbyPower(uint8 state) ;
void MClock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 MClock_GetDividerRegister(void) ;
void MClock_SetModeRegister(uint8 modeBitMask) ;
void MClock_ClearModeRegister(uint8 modeBitMask) ;
uint8 MClock_GetModeRegister(void) ;
void MClock_SetSourceRegister(uint8 clkSource) ;
uint8 MClock_GetSourceRegister(void) ;
#if defined(MClock__CFG3)
void MClock_SetPhaseRegister(uint8 clkPhase) ;
uint8 MClock_GetPhaseRegister(void) ;
#endif /* defined(MClock__CFG3) */

#define MClock_Enable()                       MClock_Start()
#define MClock_Disable()                      MClock_Stop()
#define MClock_SetDivider(clkDivider)         MClock_SetDividerRegister(clkDivider, 1u)
#define MClock_SetDividerValue(clkDivider)    MClock_SetDividerRegister((clkDivider) - 1u, 1u)
#define MClock_SetMode(clkMode)               MClock_SetModeRegister(clkMode)
#define MClock_SetSource(clkSource)           MClock_SetSourceRegister(clkSource)
#if defined(MClock__CFG3)
#define MClock_SetPhase(clkPhase)             MClock_SetPhaseRegister(clkPhase)
#define MClock_SetPhaseValue(clkPhase)        MClock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(MClock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define MClock_CLKEN              (* (reg8 *) MClock__PM_ACT_CFG)
#define MClock_CLKEN_PTR          ((reg8 *) MClock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define MClock_CLKSTBY            (* (reg8 *) MClock__PM_STBY_CFG)
#define MClock_CLKSTBY_PTR        ((reg8 *) MClock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define MClock_DIV_LSB            (* (reg8 *) MClock__CFG0)
#define MClock_DIV_LSB_PTR        ((reg8 *) MClock__CFG0)
#define MClock_DIV_PTR            ((reg16 *) MClock__CFG0)

/* Clock MSB divider configuration register. */
#define MClock_DIV_MSB            (* (reg8 *) MClock__CFG1)
#define MClock_DIV_MSB_PTR        ((reg8 *) MClock__CFG1)

/* Mode and source configuration register */
#define MClock_MOD_SRC            (* (reg8 *) MClock__CFG2)
#define MClock_MOD_SRC_PTR        ((reg8 *) MClock__CFG2)

#if defined(MClock__CFG3)
/* Analog clock phase configuration register */
#define MClock_PHASE              (* (reg8 *) MClock__CFG3)
#define MClock_PHASE_PTR          ((reg8 *) MClock__CFG3)
#endif /* defined(MClock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define MClock_CLKEN_MASK         MClock__PM_ACT_MSK
#define MClock_CLKSTBY_MASK       MClock__PM_STBY_MSK

/* CFG2 field masks */
#define MClock_SRC_SEL_MSK        MClock__CFG2_SRC_SEL_MASK
#define MClock_MODE_MASK          (~(MClock_SRC_SEL_MSK))

#if defined(MClock__CFG3)
/* CFG3 phase mask */
#define MClock_PHASE_MASK         MClock__CFG3_PHASE_DLY_MASK
#endif /* defined(MClock__CFG3) */

#endif /* CY_CLOCK_MClock_H */


/* [] END OF FILE */

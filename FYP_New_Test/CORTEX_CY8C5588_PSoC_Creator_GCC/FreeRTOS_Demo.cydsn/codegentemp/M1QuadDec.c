/*******************************************************************************
* File Name: M1QuadDec.c  
* Version 3.0
*
* Description:
*  This file provides the source code to the API for the Quadrature Decoder
*  component.
*
* Note:
*  None.
*   
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "M1QuadDec.h"

#if (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT)
    #include "M1QuadDec_PVT.h"
#endif /* M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT */

uint8 M1QuadDec_initVar = 0u;


/*******************************************************************************
* Function Name: M1QuadDec_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default QuadDec configuration provided with customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void M1QuadDec_Init(void) 
{
    #if (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT)
        /* Disable Interrupt. */
        CyIntDisable(M1QuadDec_ISR_NUMBER);
        /* Set the ISR to point to the M1QuadDec_isr Interrupt. */
        (void) CyIntSetVector(M1QuadDec_ISR_NUMBER, & M1QuadDec_ISR);
        /* Set the priority. */
        CyIntSetPriority(M1QuadDec_ISR_NUMBER, M1QuadDec_ISR_PRIORITY);
    #endif /* M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: M1QuadDec_Enable
********************************************************************************
*
* Summary:
*  This function enable interrupts from Component and also enable Component's
*  ISR in case of 32-bit counter.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void M1QuadDec_Enable(void) 
{
    uint8 enableInterrupts;

    M1QuadDec_SetInterruptMask(M1QuadDec_INIT_INT_MASK);

    /* Clear pending interrupts. */
    (void) M1QuadDec_GetEvents();
    
    enableInterrupts = CyEnterCriticalSection();

    /* Enable interrupts from Statusi register */
    M1QuadDec_SR_AUX_CONTROL |= M1QuadDec_INTERRUPTS_ENABLE;

    CyExitCriticalSection(enableInterrupts);        

    #if (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT)
        /* Enable Component interrupts */
        CyIntEnable(M1QuadDec_ISR_NUMBER);
    #endif /* M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: M1QuadDec_Start
********************************************************************************
*
* Summary:
*  Initializes UDBs and other relevant hardware.
*  Resets counter, enables or disables all relevant interrupts.
*  Starts monitoring the inputs and counting.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  M1QuadDec_initVar - used to check initial configuration, modified on
*  first function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void M1QuadDec_Start(void) 
{
    #if (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT)
        M1QuadDec_Cnt8_Start();
        M1QuadDec_Cnt8_WriteCounter(M1QuadDec_COUNTER_INIT_VALUE);
    #else
        /* (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_16_BIT) || 
        *  (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT) 
        */
        M1QuadDec_Cnt16_Start();
        M1QuadDec_Cnt16_WriteCounter(M1QuadDec_COUNTER_INIT_VALUE);
    #endif /* M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT */
    
    #if (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT)        
       M1QuadDec_count32SoftPart = 0;
    #endif /* M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT */

    if (M1QuadDec_initVar == 0u)
    {
        M1QuadDec_Init();
        M1QuadDec_initVar = 1u;
    }

    M1QuadDec_Enable();
}


/*******************************************************************************
* Function Name: M1QuadDec_Stop
********************************************************************************
*
* Summary:
*  Turns off UDBs and other relevant hardware.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void M1QuadDec_Stop(void) 
{
    uint8 enableInterrupts;

    #if (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT)
        M1QuadDec_Cnt8_Stop();
    #else 
        /* (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_16_BIT) ||
        *  (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT)
        */
        M1QuadDec_Cnt16_Stop();    /* counter disable */
    #endif /* (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT) */
 
    enableInterrupts = CyEnterCriticalSection();

    /* Disable interrupts interrupts from Statusi register */
    M1QuadDec_SR_AUX_CONTROL &= (uint8) (~M1QuadDec_INTERRUPTS_ENABLE);

    CyExitCriticalSection(enableInterrupts);

    #if (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT)
        CyIntDisable(M1QuadDec_ISR_NUMBER);    /* interrupt disable */
    #endif /* M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: M1QuadDec_GetCounter
********************************************************************************
*
* Summary:
*  Reports the current value of the counter.
*
* Parameters:
*  None.
*
* Return:
*  The counter value. Return type is signed and per the counter size setting.
*  A positive value indicates clockwise movement (B before A).
*
* Global variables:
*  M1QuadDec_count32SoftPart - used to get hi 16 bit for current value
*  of the 32-bit counter, when Counter size equal 32-bit.
*
*******************************************************************************/
int32 M1QuadDec_GetCounter(void) 
{
    int32 count;
    uint16 tmpCnt;

    #if (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT)
        int16 hwCount;

        CyIntDisable(M1QuadDec_ISR_NUMBER);

        tmpCnt = M1QuadDec_Cnt16_ReadCounter();
        hwCount = (int16) ((int32) tmpCnt - (int32) M1QuadDec_COUNTER_INIT_VALUE);
        count = M1QuadDec_count32SoftPart + hwCount;

        CyIntEnable(M1QuadDec_ISR_NUMBER);
    #else 
        /* (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT) || 
        *  (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_16_BIT)
        */
        #if (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT)
            tmpCnt = M1QuadDec_Cnt8_ReadCounter();
        #else /* (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_16_BIT) */
            tmpCnt = M1QuadDec_Cnt16_ReadCounter();
        #endif  /* M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT */

        count = (int32) ((int32) tmpCnt -
                (int32) M1QuadDec_COUNTER_INIT_VALUE);

    #endif /* M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT */ 

    return (count);
}


/*******************************************************************************
* Function Name: M1QuadDec_SetCounter
********************************************************************************
*
* Summary:
*  Sets the current value of the counter.
*
* Parameters:
*  value:  The new value. Parameter type is signed and per the counter size
*  setting.
*
* Return:
*  None.
*
* Global variables:
*  M1QuadDec_count32SoftPart - modified to set hi 16 bit for current
*  value of the 32-bit counter, when Counter size equal 32-bit.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void M1QuadDec_SetCounter(int32 value) 
{
    #if ((M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT) || \
         (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_16_BIT))
        uint16 count;
        
        if (value >= 0)
        {
            count = (uint16) value + M1QuadDec_COUNTER_INIT_VALUE;
        }
        else
        {
            count = M1QuadDec_COUNTER_INIT_VALUE - (uint16)(-value);
        }
        #if (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT)
            M1QuadDec_Cnt8_WriteCounter(count);
        #else /* (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_16_BIT) */
            M1QuadDec_Cnt16_WriteCounter(count);
        #endif  /* M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT */
    #else /* (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT) */
        CyIntDisable(M1QuadDec_ISR_NUMBER);

        M1QuadDec_Cnt16_WriteCounter(M1QuadDec_COUNTER_INIT_VALUE);
        M1QuadDec_count32SoftPart = value;

        CyIntEnable(M1QuadDec_ISR_NUMBER);
    #endif  /* (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT) ||
             * (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_16_BIT)
             */
}


/*******************************************************************************
* Function Name: M1QuadDec_GetEvents
********************************************************************************
* 
* Summary:
*   Reports the current status of events. This function clears the bits of the 
*   status register.
*
* Parameters:
*  None.
*
* Return:
*  The events, as bits in an unsigned 8-bit value:
*    Bit      Description
*     0        Counter overflow.
*     1        Counter underflow.
*     2        Counter reset due to index, if index input is used.
*     3        Invalid A, B inputs state transition.
*
*******************************************************************************/
uint8 M1QuadDec_GetEvents(void) 
{
    return (M1QuadDec_STATUS_REG & M1QuadDec_INIT_INT_MASK);
}


/*******************************************************************************
* Function Name: M1QuadDec_SetInterruptMask
********************************************************************************
*
* Summary:
*  Enables / disables interrupts due to the events.
*  For the 32-bit counter, the overflow, underflow and reset interrupts cannot
*  be disabled, these bits are ignored.
*
* Parameters:
*  mask: Enable / disable bits in an 8-bit value, where 1 enables the interrupt.
*
* Return:
*  None.
*
*******************************************************************************/
void M1QuadDec_SetInterruptMask(uint8 mask) 
{
    #if (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT)
        /* Underflow, Overflow and Reset interrupts for 32-bit Counter are always enable */
        mask |= (M1QuadDec_COUNTER_OVERFLOW | M1QuadDec_COUNTER_UNDERFLOW |
                 M1QuadDec_COUNTER_RESET);
    #endif /* M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT */

    M1QuadDec_STATUS_MASK = mask;
}


/*******************************************************************************
* Function Name: M1QuadDec_GetInterruptMask
********************************************************************************
*
* Summary:
*  Reports the current interrupt mask settings.
*
* Parameters:
*  None.
*
* Return:
*  Enable / disable bits in an 8-bit value, where 1 enables the interrupt.
*  For the 32-bit counter, the overflow, underflow and reset enable bits are
*  always set.
*
*******************************************************************************/
uint8 M1QuadDec_GetInterruptMask(void) 
{
    return (M1QuadDec_STATUS_MASK & M1QuadDec_INIT_INT_MASK);
}


/* [] END OF FILE */

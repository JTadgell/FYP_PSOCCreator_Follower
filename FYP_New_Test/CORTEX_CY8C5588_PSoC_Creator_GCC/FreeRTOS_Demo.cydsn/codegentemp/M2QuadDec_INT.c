/*******************************************************************************
* File Name: M2QuadDec_INT.c
* Version 3.0
*
* Description:
*  This file contains the Interrupt Service Routine (ISR) for the Quadrature
*  Decoder component.
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

#include "M2QuadDec.h"
#include "M2QuadDec_PVT.h"


volatile int32 M2QuadDec_count32SoftPart = 0;


/*******************************************************************************
* FUNCTION NAME: void M2QuadDec_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for Quadrature Decoder Component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  M2QuadDec_count32SoftPart - modified to update hi 16 bit for current
*  value of the 32-bit counter, when Counter size equal 32-bit.
*  M2QuadDec_swStatus - modified with the updated values of STATUS
*  register.
*
*******************************************************************************/
CY_ISR( M2QuadDec_ISR )
{
   uint8 M2QuadDec_swStatus;

   M2QuadDec_swStatus = M2QuadDec_STATUS_REG;

    #ifdef M2QuadDec_ISR_ENTRY_CALLBACK
        M2QuadDec_ISR_EntryCallback();
    #endif /* M2QuadDec_ISR_ENTRY_CALLBACK */

    /* User code required at start of ISR */
    /* `#START M2QuadDec_ISR_START` */

    /* `#END` */

    if (0u != (M2QuadDec_swStatus & M2QuadDec_COUNTER_OVERFLOW))
    {
        M2QuadDec_count32SoftPart += (int32) M2QuadDec_COUNTER_MAX_VALUE;
    }
    else if (0u != (M2QuadDec_swStatus & M2QuadDec_COUNTER_UNDERFLOW))
    {
        M2QuadDec_count32SoftPart -= (int32) M2QuadDec_COUNTER_INIT_VALUE;
    }
    else
    {
        /* Nothing to do here */
    }
    
    if (0u != (M2QuadDec_swStatus & M2QuadDec_COUNTER_RESET))
    {
        M2QuadDec_count32SoftPart = 0;
    }
    
    /* User code required at end of ISR */
    /* `#START M2QuadDec_ISR_END` */

    /* `#END` */

    #ifdef M2QuadDec_ISR_EXIT_CALLBACK
        M2QuadDec_ISR_ExitCallback();
    #endif /* M2QuadDec_ISR_EXIT_CALLBACK */
}


/* [] END OF FILE */

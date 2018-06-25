/*******************************************************************************
* File Name: M2QuadDec_PM.c
* Version 3.0
*
* Description:
*  This file contains the setup, control and status commands to support 
*  component operations in low power mode.  
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

static M2QuadDec_BACKUP_STRUCT M2QuadDec_backup = {0u};


/*******************************************************************************
* Function Name: M2QuadDec_SaveConfig
********************************************************************************
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void M2QuadDec_SaveConfig(void) 
{
    #if (M2QuadDec_COUNTER_SIZE == M2QuadDec_COUNTER_SIZE_8_BIT)
        M2QuadDec_Cnt8_SaveConfig();
    #else 
        /* (M2QuadDec_COUNTER_SIZE == M2QuadDec_COUNTER_SIZE_16_BIT) || 
         * (M2QuadDec_COUNTER_SIZE == M2QuadDec_COUNTER_SIZE_32_BIT)
         */
        M2QuadDec_Cnt16_SaveConfig();
    #endif /* (M2QuadDec_COUNTER_SIZE == M2QuadDec_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: M2QuadDec_RestoreConfig
********************************************************************************
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void M2QuadDec_RestoreConfig(void) 
{
    #if (M2QuadDec_COUNTER_SIZE == M2QuadDec_COUNTER_SIZE_8_BIT)
        M2QuadDec_Cnt8_RestoreConfig();
    #else 
        /* (M2QuadDec_COUNTER_SIZE == M2QuadDec_COUNTER_SIZE_16_BIT) || 
         * (M2QuadDec_COUNTER_SIZE == M2QuadDec_COUNTER_SIZE_32_BIT) 
         */
        M2QuadDec_Cnt16_RestoreConfig();
    #endif /* (M2QuadDec_COUNTER_SIZE == M2QuadDec_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: M2QuadDec_Sleep
********************************************************************************
* 
* Summary:
*  Prepare Quadrature Decoder Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  M2QuadDec_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void M2QuadDec_Sleep(void) 
{
    if (0u != (M2QuadDec_SR_AUX_CONTROL & M2QuadDec_INTERRUPTS_ENABLE))
    {
        M2QuadDec_backup.enableState = 1u;
    }
    else /* The Quadrature Decoder Component is disabled */
    {
        M2QuadDec_backup.enableState = 0u;
    }

    M2QuadDec_Stop();
    M2QuadDec_SaveConfig();
}


/*******************************************************************************
* Function Name: M2QuadDec_Wakeup
********************************************************************************
*
* Summary:
*  Prepare Quadrature Decoder Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  M2QuadDec_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void M2QuadDec_Wakeup(void) 
{
    M2QuadDec_RestoreConfig();

    if (M2QuadDec_backup.enableState != 0u)
    {
        #if (M2QuadDec_COUNTER_SIZE == M2QuadDec_COUNTER_SIZE_8_BIT)
            M2QuadDec_Cnt8_Enable();
        #else 
            /* (M2QuadDec_COUNTER_SIZE == M2QuadDec_COUNTER_SIZE_16_BIT) || 
            *  (M2QuadDec_COUNTER_SIZE == M2QuadDec_COUNTER_SIZE_32_BIT) 
            */
            M2QuadDec_Cnt16_Enable();
        #endif /* (M2QuadDec_COUNTER_SIZE == M2QuadDec_COUNTER_SIZE_8_BIT) */

        /* Enable component's operation */
        M2QuadDec_Enable();
    } /* Do nothing if component's block was disabled before */
}


/* [] END OF FILE */


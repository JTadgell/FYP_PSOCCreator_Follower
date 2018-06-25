/*******************************************************************************
* File Name: M1QuadDec_PM.c
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

#include "M1QuadDec.h"

static M1QuadDec_BACKUP_STRUCT M1QuadDec_backup = {0u};


/*******************************************************************************
* Function Name: M1QuadDec_SaveConfig
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
void M1QuadDec_SaveConfig(void) 
{
    #if (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT)
        M1QuadDec_Cnt8_SaveConfig();
    #else 
        /* (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_16_BIT) || 
         * (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT)
         */
        M1QuadDec_Cnt16_SaveConfig();
    #endif /* (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: M1QuadDec_RestoreConfig
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
void M1QuadDec_RestoreConfig(void) 
{
    #if (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT)
        M1QuadDec_Cnt8_RestoreConfig();
    #else 
        /* (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_16_BIT) || 
         * (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT) 
         */
        M1QuadDec_Cnt16_RestoreConfig();
    #endif /* (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: M1QuadDec_Sleep
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
*  M1QuadDec_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void M1QuadDec_Sleep(void) 
{
    if (0u != (M1QuadDec_SR_AUX_CONTROL & M1QuadDec_INTERRUPTS_ENABLE))
    {
        M1QuadDec_backup.enableState = 1u;
    }
    else /* The Quadrature Decoder Component is disabled */
    {
        M1QuadDec_backup.enableState = 0u;
    }

    M1QuadDec_Stop();
    M1QuadDec_SaveConfig();
}


/*******************************************************************************
* Function Name: M1QuadDec_Wakeup
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
*  M1QuadDec_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void M1QuadDec_Wakeup(void) 
{
    M1QuadDec_RestoreConfig();

    if (M1QuadDec_backup.enableState != 0u)
    {
        #if (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT)
            M1QuadDec_Cnt8_Enable();
        #else 
            /* (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_16_BIT) || 
            *  (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT) 
            */
            M1QuadDec_Cnt16_Enable();
        #endif /* (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT) */

        /* Enable component's operation */
        M1QuadDec_Enable();
    } /* Do nothing if component's block was disabled before */
}


/* [] END OF FILE */


/*******************************************************************************
* File Name: M1QuadDec_Cnt16_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "M1QuadDec_Cnt16.h"

static M1QuadDec_Cnt16_backupStruct M1QuadDec_Cnt16_backup;


/*******************************************************************************
* Function Name: M1QuadDec_Cnt16_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  M1QuadDec_Cnt16_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void M1QuadDec_Cnt16_SaveConfig(void) 
{
    #if (!M1QuadDec_Cnt16_UsingFixedFunction)

        M1QuadDec_Cnt16_backup.CounterUdb = M1QuadDec_Cnt16_ReadCounter();

        #if(!M1QuadDec_Cnt16_ControlRegRemoved)
            M1QuadDec_Cnt16_backup.CounterControlRegister = M1QuadDec_Cnt16_ReadControlRegister();
        #endif /* (!M1QuadDec_Cnt16_ControlRegRemoved) */

    #endif /* (!M1QuadDec_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: M1QuadDec_Cnt16_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  M1QuadDec_Cnt16_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void M1QuadDec_Cnt16_RestoreConfig(void) 
{      
    #if (!M1QuadDec_Cnt16_UsingFixedFunction)

       M1QuadDec_Cnt16_WriteCounter(M1QuadDec_Cnt16_backup.CounterUdb);

        #if(!M1QuadDec_Cnt16_ControlRegRemoved)
            M1QuadDec_Cnt16_WriteControlRegister(M1QuadDec_Cnt16_backup.CounterControlRegister);
        #endif /* (!M1QuadDec_Cnt16_ControlRegRemoved) */

    #endif /* (!M1QuadDec_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: M1QuadDec_Cnt16_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  M1QuadDec_Cnt16_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void M1QuadDec_Cnt16_Sleep(void) 
{
    #if(!M1QuadDec_Cnt16_ControlRegRemoved)
        /* Save Counter's enable state */
        if(M1QuadDec_Cnt16_CTRL_ENABLE == (M1QuadDec_Cnt16_CONTROL & M1QuadDec_Cnt16_CTRL_ENABLE))
        {
            /* Counter is enabled */
            M1QuadDec_Cnt16_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            M1QuadDec_Cnt16_backup.CounterEnableState = 0u;
        }
    #else
        M1QuadDec_Cnt16_backup.CounterEnableState = 1u;
        if(M1QuadDec_Cnt16_backup.CounterEnableState != 0u)
        {
            M1QuadDec_Cnt16_backup.CounterEnableState = 0u;
        }
    #endif /* (!M1QuadDec_Cnt16_ControlRegRemoved) */
    
    M1QuadDec_Cnt16_Stop();
    M1QuadDec_Cnt16_SaveConfig();
}


/*******************************************************************************
* Function Name: M1QuadDec_Cnt16_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  M1QuadDec_Cnt16_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void M1QuadDec_Cnt16_Wakeup(void) 
{
    M1QuadDec_Cnt16_RestoreConfig();
    #if(!M1QuadDec_Cnt16_ControlRegRemoved)
        if(M1QuadDec_Cnt16_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            M1QuadDec_Cnt16_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!M1QuadDec_Cnt16_ControlRegRemoved) */
    
}


/* [] END OF FILE */

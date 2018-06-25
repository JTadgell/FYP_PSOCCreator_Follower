/*******************************************************************************
* File Name: Standby.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Standby_ALIASES_H) /* Pins Standby_ALIASES_H */
#define CY_PINS_Standby_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define Standby_0			(Standby__0__PC)
#define Standby_0_INTR	((uint16)((uint16)0x0001u << Standby__0__SHIFT))

#define Standby_INTR_ALL	 ((uint16)(Standby_0_INTR))

#endif /* End Pins Standby_ALIASES_H */


/* [] END OF FILE */

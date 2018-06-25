/*******************************************************************************
* File Name: M1QuadDec.h  
* Version 3.0
*
* Description:
*  This file provides constants and parameter values for the Quadrature
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

#if !defined(CY_QUADRATURE_DECODER_M1QuadDec_H)
#define CY_QUADRATURE_DECODER_M1QuadDec_H

#include "cyfitter.h"
#include "CyLib.h"
#include "cytypes.h"

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component QuadDec_v3_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#define M1QuadDec_COUNTER_SIZE               (32u)
#define M1QuadDec_COUNTER_SIZE_8_BIT         (8u)
#define M1QuadDec_COUNTER_SIZE_16_BIT        (16u)
#define M1QuadDec_COUNTER_SIZE_32_BIT        (32u)

#if (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT)
    #include "M1QuadDec_Cnt8.h"
#else 
    /* (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_16_BIT) || 
    *  (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT) 
    */
    #include "M1QuadDec_Cnt16.h"
#endif /* M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT */

extern uint8 M1QuadDec_initVar;


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define M1QuadDec_COUNTER_RESOLUTION         (4u)


/***************************************
*       Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
} M1QuadDec_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  M1QuadDec_Init(void) ;
void  M1QuadDec_Start(void) ;
void  M1QuadDec_Stop(void) ;
void  M1QuadDec_Enable(void) ;
uint8 M1QuadDec_GetEvents(void) ;
void  M1QuadDec_SetInterruptMask(uint8 mask) ;
uint8 M1QuadDec_GetInterruptMask(void) ;
int32 M1QuadDec_GetCounter(void) ;
void  M1QuadDec_SetCounter(int32 value)
;
void  M1QuadDec_Sleep(void) ;
void  M1QuadDec_Wakeup(void) ;
void  M1QuadDec_SaveConfig(void) ;
void  M1QuadDec_RestoreConfig(void) ;

#if (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT)
    CY_ISR_PROTO(M1QuadDec_ISR);
#endif /* M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT */


/***************************************
*           API Constants
***************************************/

#if (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT)
    #define M1QuadDec_ISR_NUMBER             ((uint8) M1QuadDec_isr__INTC_NUMBER)
    #define M1QuadDec_ISR_PRIORITY           ((uint8) M1QuadDec_isr__INTC_PRIOR_NUM)
#endif /* M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT */


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define M1QuadDec_GLITCH_FILTERING           (1u)
#define M1QuadDec_INDEX_INPUT                (0u)


/***************************************
*    Initial Parameter Constants
***************************************/

#if (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT)
    #define M1QuadDec_COUNTER_INIT_VALUE    (0x80u)
#else 
    /* (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_16_BIT) ||
    *  (M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_32_BIT)
    */
    #define M1QuadDec_COUNTER_INIT_VALUE    (0x8000u)
    #define M1QuadDec_COUNTER_MAX_VALUE     (0x7FFFu)
#endif /* M1QuadDec_COUNTER_SIZE == M1QuadDec_COUNTER_SIZE_8_BIT */


/***************************************
*             Registers
***************************************/

#define M1QuadDec_STATUS_REG                 (* (reg8 *) M1QuadDec_bQuadDec_Stsreg__STATUS_REG)
#define M1QuadDec_STATUS_PTR                 (  (reg8 *) M1QuadDec_bQuadDec_Stsreg__STATUS_REG)
#define M1QuadDec_STATUS_MASK                (* (reg8 *) M1QuadDec_bQuadDec_Stsreg__MASK_REG)
#define M1QuadDec_STATUS_MASK_PTR            (  (reg8 *) M1QuadDec_bQuadDec_Stsreg__MASK_REG)
#define M1QuadDec_SR_AUX_CONTROL             (* (reg8 *) M1QuadDec_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)
#define M1QuadDec_SR_AUX_CONTROL_PTR         (  (reg8 *) M1QuadDec_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)


/***************************************
*        Register Constants
***************************************/

#define M1QuadDec_COUNTER_OVERFLOW_SHIFT     (0x00u)
#define M1QuadDec_COUNTER_UNDERFLOW_SHIFT    (0x01u)
#define M1QuadDec_COUNTER_RESET_SHIFT        (0x02u)
#define M1QuadDec_INVALID_IN_SHIFT           (0x03u)
#define M1QuadDec_COUNTER_OVERFLOW           ((uint8) (0x01u << M1QuadDec_COUNTER_OVERFLOW_SHIFT))
#define M1QuadDec_COUNTER_UNDERFLOW          ((uint8) (0x01u << M1QuadDec_COUNTER_UNDERFLOW_SHIFT))
#define M1QuadDec_COUNTER_RESET              ((uint8) (0x01u << M1QuadDec_COUNTER_RESET_SHIFT))
#define M1QuadDec_INVALID_IN                 ((uint8) (0x01u << M1QuadDec_INVALID_IN_SHIFT))

#define M1QuadDec_INTERRUPTS_ENABLE_SHIFT    (0x04u)
#define M1QuadDec_INTERRUPTS_ENABLE          ((uint8)(0x01u << M1QuadDec_INTERRUPTS_ENABLE_SHIFT))
#define M1QuadDec_INIT_INT_MASK              (0x0Fu)


/******************************************************************************************
* Following code are OBSOLETE and must not be used starting from Quadrature Decoder 2.20
******************************************************************************************/
#define M1QuadDec_DISABLE                    (0x00u)


#endif /* CY_QUADRATURE_DECODER_M1QuadDec_H */


/* [] END OF FILE */

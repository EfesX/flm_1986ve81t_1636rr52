/**
  ******************************************************************************
  * @file    mdr32f8_ssp.c
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    03/09/2015
  * @brief   This file contains all the SSP firmware functions.
  ******************************************************************************
  * <br><br>
  *
  *
  * <h2><center>&copy; COPYRIGHT 2015 Milandr</center></h2>
  ******************************************************************************
  * FILE mfr32f8_ssp.c
  */

/* Includes ------------------------------------------------------------------*/
#include "mdr32f8_ssp.h"
#include "MDR1986VE8T.h"	

#define ASSERT_INFO_FILE_ID FILEID__MDR32F8_SSP_C

/** @addtogroup __MDR32F8_StdPeriph_Driver
  * @{
  */

/** @defgroup SSP
  * @{
  */

/** @defgroup SSP_Private_Defines
  * @{
  */

#define CR1_EN_Set                 ((uint16_t)0x0002)  /*!< SSP Enable Mask */
#define CR1_EN_Reset               ((uint16_t)0xFFFD)  /*!< SSP Disable Mask */

/** @} */ /* End of group SSP_Private_Defines */

/** @defgroup SSP_Private_Functions SSP Private Functions
  * @{
  */

/**
  * @brief  Resets the SSPx peripheral registers to their default reset values.
  * @param  SSPx: Select the SSP peripheral.
  *         This parameter can be one of the following values: SSP1, SSP2.
  * @retval None
  */
void SSP_DeInit(MDR_SSP_TypeDef* SSPx)
{
  /* Check the parameters */
  SSPx->CR0   = 0;
  SSPx->CR1   = 0;
  SSPx->CPSR  = 0;
  SSPx->IMSC  = 0;
  SSPx->DMACR = 0;

  /* Clear SSP ICR[RTIC] and SSP ICR[RORIC] bits */
  SSPx->ICR = SSP_IT_RT | SSP_IT_ROR;

}

/**
  * @brief  Initializes the SSPx peripheral according to the specified
  *         parameters in the SSP_InitStruct.
  * @param  SSPx: Select the SSP peripheral.
  *         This parameter can be one of the following values:
  *         SSP1, SSP2.
  * @param  SSP_InitStruct: pointer to a SSP_InitTypeDef structure
  *         that contains the configuration information for the specified SSP peripheral.
  * @retval None
  */
void SSP_Init(MDR_SSP_TypeDef* SSPx, const SSP_InitTypeDef* SSP_InitStruct)
{
  uint32_t tmpreg;

  /* SSPx CPSR Configuration */
  SSPx->CPSR = SSP_InitStruct->SSP_CPSDVSR;

  /* SSPx CR0 Configuration */
  tmpreg = ((SSP_InitStruct->SSP_SCR) << SSP_CR0_SCR_Pos)
         + SSP_InitStruct->SSP_SPH
         + SSP_InitStruct->SSP_SPO
         + SSP_InitStruct->SSP_FRF
         + SSP_InitStruct->SSP_WordLength;
  SSPx->CR0 = tmpreg;

  /* SSPx CR1 Configuration */
  tmpreg = SSP_InitStruct->SSP_HardwareFlowControl + SSP_InitStruct->SSP_Mode;
  SSPx->CR1 = tmpreg;
}

/**
  * @brief  Fills each SSP_InitStruct member with its default value.
  * @param  SSP_InitStruct: pointer to a SSP_InitTypeDef structure
  *         that is to be initialized.
  * @retval None
  */
void SSP_StructInit(SSP_InitTypeDef* SSP_InitStruct)
{
  /* SSP_InitStruct members default value */
  /* Initialize the SPI_Mode member */
  SSP_InitStruct->SSP_Mode = SSP_ModeMaster;
  /* initialize the SSP_WordLength member */
  SSP_InitStruct->SSP_WordLength = SSP_WordLength8b;
  /* Initialize the SSP_SPO member */
  SSP_InitStruct->SSP_SPO = SSP_SPO_Low;
  /* Initialize the SSP_SPH member */
  SSP_InitStruct->SSP_SPH = SSP_SPH_1Edge;
  /* Initialize the SSP_FRF member */
  SSP_InitStruct->SSP_FRF = SSP_FRF_SPI_Motorola;
  /* Initialize the SSP max speed */
  SSP_InitStruct->SSP_CPSDVSR = 2;
  SSP_InitStruct->SSP_SCR = 0;

}

/**
  * @brief  Enables or disables the specified SSP peripheral.
  * @param  SSPx: Select the SSP peripheral.
  *         This parameter can be one of the following values:
  *         SSP1, SSP2.
  * @param  NewState: new state of the SSPx peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SSP_Cmd(MDR_SSP_TypeDef* SSPx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_SSP_ALL_PERIPH(SSPx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected SSP by setting the SSE bit in the CR1 register */
    SSPx->CR1 |= CR1_EN_Set;
  }
  else
  {
    /* Disable the selected SSP by clearing the SSE bit in the CR1 register */
    SSPx->CR1 &= CR1_EN_Reset;
  }
}

/**
  * @brief  Transmits single data through the SSPx peripheral.
  * @param  SSPx: Select the SSP peripheral.
  *         This parameter can be one of the following values:
  *         SSP1, SSP2.
  * @param  Data: the data to transmit.
  * @retval None
  */
void SSP_SendData(MDR_SSP_TypeDef* SSPx, uint16_t Data)
{
  /* Check the parameters */
  assert_param(IS_SSP_ALL_PERIPH(SSPx));

  /* Transmit Data */
  SSPx->DR = Data;

}

/**
  * @brief  Returns the most recent received data by the SSPx peripheral.
  * @param  SSPx: Select the SSP peripheral.
  *         This parameter can be one of the following values:
  *         SSP1, SSP2.
  * @retval The received data (7:0) and error flags (15:8).
  */
uint16_t SSP_ReceiveData(MDR_SSP_TypeDef* SSPx)
{
  /* Check the parameters */
  assert_param(IS_SSP_ALL_PERIPH(SSPx));

  /* Receive Data */
  return ((uint16_t)(SSPx->DR));
}

/** @} */ /* End of group SSP_Private_Functions */

/** @} */ /* End of group SSP */

/** @} */ /* End of group __MDR32F8_StdPeriph_Driver */

/******************* (C) COPYRIGHT 2015 Milandr *********************************
*
* END OF FILE mdr32f8_ssp.c */


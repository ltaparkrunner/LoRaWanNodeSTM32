/**
  ******************************************************************************
  * @file           : nucleo_l476rg_errno.h
  * @brief          : Error Code
  ******************************************************************************
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MU_L476VG_ERRNO_H
#define __MU_L476VG_ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

/* BSP Common Error codes */
#define MU_ERROR_NONE                    0
#define MU_ERROR_NO_INIT                -1
#define MU_ERROR_WRONG_PARAM            -2
#define MU_ERROR_BUSY                   -3
#define MU_ERROR_PERIPH_FAILURE         -4
#define MU_ERROR_COMPONENT_FAILURE      -5
#define MU_ERROR_UNKNOWN_FAILURE        -6
#define MU_ERROR_UNKNOWN_COMPONENT      -7
#define MU_ERROR_SPI_FAILURE            -8
#define MU_ERROR_CLOCK_FAILURE          -9
#define MU_ERROR_MSP_FAILURE            -10
#define MU_ERROR_FEATURE_NOT_SUPPORTED      -11

/* BSP BUS error codes */

#define MU_ERROR_SPI_TRANSACTION_FAILURE    -100
#define MU_ERROR_SPI_ARBITRATION_LOSS       -101
#define MU_ERROR_SPI_ACKNOWLEDGE_FAILURE    -102
#define MU_ERROR_SPI_PROTOCOL_FAILURE       -103

#define MU_ERROR_SPI_MODE_FAULT             -104
#define MU_ERROR_SPI_FRAME_ERROR            -105
#define MU_ERROR_SPI_CRC_ERROR              -106
#define MU_ERROR_SPI_DMA_FAILURE            -107

#ifdef __cplusplus
}
#endif

#endif /* __MU_L476VG_ERRNO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern void oled_init(void);
extern void oled_update(void);
extern void oled_draw_pixel(uint8_t, uint8_t, uint8_t);
extern void oled_draw_bitmap(uint8_t, uint8_t, uint8_t*, uint8_t, uint8_t);
extern HAL_StatusTypeDef ssd1306_send_cmd(uint8_t);

uint8_t image_rabbit[366] = { 0X22,0X01,0X2D,0X00,0X40,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0X40,0X20,
0X20,0XC0,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0XC0,0X60,0X20,0X20,0XC0,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X07,0X7C,0X80,0X00,0X00,0X3F,
0XC0,0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0XC0,0X60,0X30,0X18,0X08,0X04,0X04,0X02,0X03,0X00,0X00,0X00,0X01,0X01,0X01,
0X01,0X01,0X01,0X01,0X00,0X00,0X00,0X01,0X01,0X02,0X02,0X04,0X08,0X10,0X20,0X40,
0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XE0,0X1C,0X03,0X00,0X00,
0X00,0X10,0X10,0X10,0X18,0X18,0X08,0X08,0X08,0X08,0X08,0X00,0X00,0X00,0X00,0X00,
0X08,0X08,0X08,0X08,0X08,0X08,0X08,0X08,0X08,0X08,0X00,0X00,0X00,0X01,0X0E,0XF0,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X1C,0X60,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0XC0,0X38,0X07,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X02,0X04,0X0C,0X88,0X78,0X10,
0X90,0X10,0X20,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X10,0X90,0X10,
0X78,0X88,0X08,0X04,0X06,0X03,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,
0X80,0X80,0X80,0X80,0X80,0X00,0X00,0X00,0X00,0XFC,0X07,0XF0,0X1F,0X80,0X80,0X80,
0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0X80,0X0E,0XF0,0X07,0X7C,
0X80,0X00,0X80,0X80,0X00,0XC0,0X00,0X00,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X01,0X01,0X01,0X01,0X01,0X01,0X01,0X01,0X00,0X00,0X00,0X01,0X01,
0X01,0X01,0X01,0X01,0X01,0X01,0X01,0X01,0X01,0X01,0X01,0X01,0X01,0X01,0X01,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,};

uint8_t image_hi[526] = { 0X22,0X01,0X41,0X00,0X40,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X40,0XC0,0X80,0X80,0X80,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0XE0,0XFE,0XFE,0X3C,0X08,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X80,0X80,0X80,0X80,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X07,0X07,0X07,
0X00,0X00,0X80,0XE0,0X78,0X1E,0X0F,0X0F,0X0C,0X0C,0X04,0X04,0X06,0X06,0X06,0X02,
0X02,0X03,0X03,0X03,0X03,0X03,0X03,0X01,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X04,0X1C,0XFC,0XF8,0X08,0X04,0X04,0X04,
0X04,0X06,0XE7,0XFF,0XFE,0X3E,0X0C,0X00,0X01,0X03,0X0F,0X1E,0X3E,0X1C,0X00,0X00,
0X00,0X00,0X00,0X01,0X00,0X1C,0XFC,0XFC,0X0C,0X04,0X04,0X04,0XC4,0X86,0X82,0X82,
0X02,0X02,0X03,0X03,0X03,0XFF,0XFF,0XFE,0X1E,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XF0,0XC0,0X40,
0X40,0X60,0X60,0X7F,0X7F,0X03,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XC0,0X60,0X08,
0X00,0X40,0X40,0XC0,0XE0,0XE0,0XF8,0XFF,0X7F,0X60,0X20,0X20,0X20,0X20,0X21,0X23,
0X23,0X23,0X20,0X20,0X20,0X30,0XFF,0XFF,0XFF,0X30,0X30,0X30,0X38,0X38,0X38,0X30,
0X30,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0XC0,0XC0,0XF0,0XF8,0X7C,0X0F,0X03,0X00,
0X00,0X00,0X00,0X00,0X80,0XF0,0XFC,0X3F,0X01,0X00,0X00,0X00,0X00,0X00,0X02,0X06,
0X1E,0X9C,0X9C,0X80,0X00,0X80,0XE0,0XFF,0XFF,0X80,0X80,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X1F,0X3F,0X1F,0X01,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X0F,0X0F,0X07,0X03,0X03,0X03,0X01,0X01,0X01,0X01,0X01,0X81,
0X81,0X01,0X01,0X01,0X01,0XC1,0XF1,0XFF,0X7F,0X07,0X01,0X01,0X03,0X03,0X03,0X07,
0X06,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X03,0X07,0X0F,0X1F,0X0F,0X07,0X03,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,};





/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  oled_init();
  oled_draw_bitmap(0, 0, image_rabbit, 45, 8);
  oled_draw_bitmap(46, 0, image_hi, 65, 8);
  oled_update();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

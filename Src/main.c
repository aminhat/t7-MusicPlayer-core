/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "stdio.h"
#include "string.h"

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
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USB_PCD_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


//-----------Global variables-----------\\


#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0



//number to 7448
GPIO_TypeDef * pin = GPIOD;
uint16_t bit0 = GPIO_PIN_4;
uint16_t bit1 = GPIO_PIN_5;
uint16_t bit2 = GPIO_PIN_6;
uint16_t bit3 = GPIO_PIN_7;

//digit to be turned on
uint16_t bitActive0 = GPIO_PIN_2;
uint16_t bitActive1 = GPIO_PIN_3;
uint16_t bitActive2 = GPIO_PIN_1;
uint16_t bitActive3 = GPIO_PIN_0;


//Digits
uint8_t digits[4] = {0, 1, 2, 3};

//UART
char received_data[50];
uint8_t data_index = 0;
char receive;
char transmit_data[50];
uint8_t uart_mode = 1;

//LEDs
GPIO_TypeDef * ledg = GPIOE;
uint16_t led0 = GPIO_PIN_8;
uint16_t led1 = GPIO_PIN_9;
uint16_t led2 = GPIO_PIN_10;
uint16_t led3 = GPIO_PIN_11;
uint16_t led4 = GPIO_PIN_12;
uint16_t led5 = GPIO_PIN_13;
uint16_t led6 = GPIO_PIN_14;
uint16_t led7 = GPIO_PIN_15;

//Tone strcuture
typedef struct
{
    uint16_t frequency;
    uint16_t duration;
} Tone;

//Buzzer

TIM_HandleTypeDef *buzzer_pwm_timer = &htim2;	// Point to PWM Timer configured in CubeMX
uint32_t buzzer_pwm_channel = TIM_CHANNEL_2;   // Select configured PWM channel number
const Tone *volatile melody_ptr;
volatile uint16_t melody_tone_count = 0;
volatile uint16_t current_tone_number = 0;
volatile uint32_t current_tone_end = 0;
volatile uint16_t volume = 10; // (0 - 1000)
const uint8_t number_of_songs = 4;
const uint16_t quarter_duration = 306;
const Tone * Songs[5];
uint8_t current_song = 0;
enum songs {
	MARIO,

};


//melodies
const Tone super_mario_bros[] = {
	{2637,306}, // E7 x2
	{   0,153}, // x3 <-- Silence
	{2637,153}, // E7
	{   0,153}, // x3
	{2093,153}, // C7
	{2637,153}, // E7
	{   0,153}, // x3
	{3136,153}, // G7
	{   0,459}, // x3
	{1586,153}, // G6
	{   0,459}, // x3

	{2093,153}, // C7
	{   0,306}, // x2
	{1586,153}, // G6
	{   0,306}, // x2
	{1319,153}, // E6
	{   0,306}, // x2
	{1760,153}, // A6
	{   0,153}, // x1
	{1976,153}, // B6
	{   0,153}, // x1
	{1865,153}, // AS6
	{1760,153}, // A6
	{   0,153}, // x1

	{1586,204}, // G6
	{2637,204}, // E7
	{3136,204}, // G7
	{3520,153}, // A7
	{   0,153}, // x1
	{2794,153}, // F7
	{3136,153}, // G7
	{   0,153}, // x1
	{2637,153}, // E7
	{   0,153}, // x1
	{2093,153}, // C7
	{2349,153}, // D7
	{1976,153}, // B6
	{   0,306}, // x2

	{2093,153}, // C7
	{   0,306}, // x2
	{1586,153}, // G6
	{   0,306}, // x2
	{1319,153}, // E6
	{   0,306}, // x2
	{1760,153}, // A6
	{   0,153}, // x1
	{1976,153}, // B6
	{   0,153}, // x1
	{1865,153}, // AS6
	{1760,153}, // A6
	{   0,153}, // x1

	{1586,204}, // G6
	{2637,204}, // E7
	{3136,204}, // G7
	{3520,153}, // A7
	{   0,153}, // x1
	{2794,153}, // F7
	{3136,153}, // G7
	{   0,153}, // x1
	{2637,153}, // E7
	{   0,153}, // x1
	{2093,153}, // C7
	{2349,153}, // D7
	{1976,153}, // B6

	{   0,  0}	// <-- Disable PWM
};

const Tone mario2[] = {
    // Super Mario Bros theme
    // Score available at https://musescore.com/user/2123/scores/2145
    // Theme by Koji Kondo
    {NOTE_E5, quarter_duration >> 1},
	{NOTE_E5, quarter_duration >> 1},
	{REST, quarter_duration >> 1},
	{NOTE_E5, quarter_duration >> 1},
	{REST, quarter_duration >> 1},
	{NOTE_C5, quarter_duration >> 1},
	{NOTE_E5, quarter_duration >> 1}, //1
    {NOTE_G5, quarter_duration},
	{REST, quarter_duration},
	{NOTE_G4, quarter_duration >> 1},
	{REST, quarter_duration},
    {NOTE_C5, quarter_duration + (quarter_duration >> 1)},
	{NOTE_G4, quarter_duration >> 1},
	{REST, quarter_duration},
	{NOTE_E4, quarter_duration + (quarter_duration >> 1)}, // 3
    {NOTE_A4, quarter_duration},
	{NOTE_B4, quarter_duration}, {NOTE_AS4, quarter_duration >> 1}, {NOTE_A4, quarter_duration},
    {NOTE_G4, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_E5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_G5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_A5, quarter_duration}, {NOTE_F5, quarter_duration >> 1}, {NOTE_G5, quarter_duration >> 1},
    {REST, quarter_duration >> 1}, {NOTE_E5, quarter_duration}, {NOTE_C5, quarter_duration >> 1}, {NOTE_D5, quarter_duration >> 1}, {NOTE_B4, quarter_duration + (quarter_duration >> 1)},
    {NOTE_C5, quarter_duration + (quarter_duration >> 1)}, {NOTE_G4, quarter_duration >> 1}, {REST, quarter_duration}, {NOTE_E4, quarter_duration + (quarter_duration >> 1)}, // repeats from 3
    {NOTE_A4, quarter_duration}, {NOTE_B4, quarter_duration}, {NOTE_AS4, quarter_duration >> 1}, {NOTE_A4, quarter_duration},
    {NOTE_G4, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_E5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_G5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_A5, quarter_duration}, {NOTE_F5, quarter_duration >> 1}, {NOTE_G5, quarter_duration >> 1},
    {REST, quarter_duration >> 1}, {NOTE_E5, quarter_duration}, {NOTE_C5, quarter_duration >> 1}, {NOTE_D5, quarter_duration >> 1}, {NOTE_B4, quarter_duration + (quarter_duration >> 1)},

    {REST, quarter_duration}, {NOTE_G5, quarter_duration >> 1}, {NOTE_FS5, quarter_duration >> 1}, {NOTE_F5, quarter_duration >> 1}, {NOTE_DS5, quarter_duration}, {NOTE_E5, quarter_duration >> 1}, //7
    {REST, quarter_duration >> 1}, {NOTE_GS4, quarter_duration >> 1}, {NOTE_A4, quarter_duration >> 1}, {NOTE_C4, quarter_duration >> 1}, {REST, quarter_duration >> 1}, {NOTE_A4, quarter_duration >> 1}, {NOTE_C5, quarter_duration >> 1}, {NOTE_D5, quarter_duration >> 1},
    {REST, quarter_duration}, {NOTE_DS5, quarter_duration}, {REST, quarter_duration >> 1}, {NOTE_D5, quarter_duration + (quarter_duration >> 1)},
    {NOTE_C5, quarter_duration << 1}, {REST, quarter_duration << 1},

    {REST, quarter_duration}, {NOTE_G5, quarter_duration >> 1}, {NOTE_FS5, quarter_duration >> 1}, {NOTE_F5, quarter_duration >> 1}, {NOTE_DS5, quarter_duration}, {NOTE_E5, quarter_duration >> 1}, //repeats from 7
    {REST, quarter_duration >> 1}, {NOTE_GS4, quarter_duration >> 1}, {NOTE_A4, quarter_duration >> 1}, {NOTE_C4, quarter_duration >> 1}, {REST, quarter_duration >> 1}, {NOTE_A4, quarter_duration >> 1}, {NOTE_C5, quarter_duration >> 1}, {NOTE_D5, quarter_duration >> 1},
    {REST, quarter_duration}, {NOTE_DS5, quarter_duration}, {REST, quarter_duration >> 1}, {NOTE_D5, quarter_duration + (quarter_duration >> 1)},
    {NOTE_C5, quarter_duration << 1}, {REST, quarter_duration << 1},

    {NOTE_C5, quarter_duration >> 1}, {NOTE_C5, quarter_duration}, {NOTE_C5, quarter_duration >> 1}, {REST, quarter_duration >> 1}, {NOTE_C5, quarter_duration >> 1}, {NOTE_D5, quarter_duration}, //11
    {NOTE_E5, quarter_duration >> 1}, {NOTE_C5, quarter_duration}, {NOTE_A4, quarter_duration >> 1}, {NOTE_G4, quarter_duration << 1},

    {NOTE_C5, quarter_duration >> 1}, {NOTE_C5, quarter_duration}, {NOTE_C5, quarter_duration >> 1}, {REST, quarter_duration >> 1}, {NOTE_C5, quarter_duration >> 1}, {NOTE_D5, quarter_duration >> 1}, {NOTE_E5, quarter_duration >> 1}, //13
    {REST, 1},
    {NOTE_C5, quarter_duration >> 1}, {NOTE_C5, quarter_duration}, {NOTE_C5, quarter_duration >> 1}, {REST, quarter_duration >> 1}, {NOTE_C5, quarter_duration >> 1}, {NOTE_D5, quarter_duration},
    {NOTE_E5, quarter_duration >> 1}, {NOTE_C5, quarter_duration}, {NOTE_A4, quarter_duration >> 1}, {NOTE_G4, quarter_duration << 1},
    {NOTE_E5, quarter_duration >> 1}, {NOTE_E5, quarter_duration >> 1}, {REST, quarter_duration >> 1}, {NOTE_E5, quarter_duration >> 1}, {REST, quarter_duration >> 1}, {NOTE_C5, quarter_duration >> 1}, {NOTE_E5, quarter_duration},
    {NOTE_G5, quarter_duration}, {REST, quarter_duration}, {NOTE_G4, quarter_duration}, {REST, quarter_duration},
    {NOTE_C5, quarter_duration + (quarter_duration >> 1)}, {NOTE_G4, quarter_duration >> 1}, {REST, quarter_duration}, {NOTE_E4, quarter_duration + (quarter_duration >> 1)}, // 19

    {NOTE_A4, quarter_duration}, {NOTE_B4, quarter_duration}, {NOTE_AS4, quarter_duration >> 1}, {NOTE_A4, quarter_duration},
    {NOTE_G4, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_E5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_G5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_A5, quarter_duration}, {NOTE_F5, quarter_duration >> 1}, {NOTE_G5, quarter_duration >> 1},
    {REST, quarter_duration >> 1}, {NOTE_E5, quarter_duration}, {NOTE_C5, quarter_duration >> 1}, {NOTE_D5, quarter_duration >> 1}, {NOTE_B4, quarter_duration + (quarter_duration >> 1)},

    {NOTE_C5, quarter_duration + (quarter_duration >> 1)}, {NOTE_G4, quarter_duration >> 1}, {REST, quarter_duration}, {NOTE_E4, quarter_duration + (quarter_duration >> 1)}, // repeats from 19
    {NOTE_A4, quarter_duration}, {NOTE_B4, quarter_duration}, {NOTE_AS4, quarter_duration >> 1}, {NOTE_A4, quarter_duration},
    {NOTE_G4, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_E5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_G5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_A5, quarter_duration}, {NOTE_F5, quarter_duration >> 1}, {NOTE_G5, quarter_duration >> 1},
    {REST, quarter_duration >> 1}, {NOTE_E5, quarter_duration}, {NOTE_C5, quarter_duration >> 1}, {NOTE_D5, quarter_duration >> 1}, {NOTE_B4, quarter_duration + (quarter_duration >> 1)},

    {NOTE_E5, quarter_duration >> 1}, {NOTE_C5, quarter_duration}, {NOTE_G4, quarter_duration >> 1}, {REST, quarter_duration}, {NOTE_GS4, quarter_duration}, //23
    {NOTE_A4, quarter_duration >> 1}, {NOTE_F5, quarter_duration}, {NOTE_F5, quarter_duration >> 1}, {NOTE_A4, quarter_duration << 1},
    {NOTE_D5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_A5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_A5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_A5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_G5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_F5, (quarter_duration >> 1) + (quarter_duration >> 2)},

    {NOTE_E5, quarter_duration >> 1}, {NOTE_C5, quarter_duration}, {NOTE_A4, quarter_duration >> 1}, {NOTE_G4, quarter_duration << 1}, //26
    {NOTE_E5, quarter_duration >> 1}, {NOTE_C5, quarter_duration}, {NOTE_G4, quarter_duration >> 1}, {REST, quarter_duration}, {NOTE_GS4, quarter_duration},
    {NOTE_A4, quarter_duration >> 1}, {NOTE_F5, quarter_duration}, {NOTE_F5, quarter_duration >> 1}, {NOTE_A4, quarter_duration << 1},
    {NOTE_B4, quarter_duration >> 1}, {NOTE_F5, quarter_duration}, {NOTE_F5, quarter_duration >> 1}, {NOTE_F5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_E5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_D5, (quarter_duration >> 1) + (quarter_duration >> 2)},
    {NOTE_C5, quarter_duration >> 1}, {NOTE_E4, quarter_duration}, {NOTE_E4, quarter_duration >> 1}, {NOTE_C4, quarter_duration << 1},

    {NOTE_E5, quarter_duration >> 1}, {NOTE_C5, quarter_duration}, {NOTE_G4, quarter_duration >> 1}, {REST, quarter_duration}, {NOTE_GS4, quarter_duration}, //repeats from 23
    {NOTE_A4, quarter_duration >> 1}, {NOTE_F5, quarter_duration}, {NOTE_F5, quarter_duration >> 1}, {NOTE_A4, quarter_duration << 1},
    {NOTE_D5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_A5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_A5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_A5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_G5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_F5, (quarter_duration >> 1) + (quarter_duration >> 2)},

    {NOTE_E5, quarter_duration >> 1}, {NOTE_C5, quarter_duration}, {NOTE_A4, quarter_duration >> 1}, {NOTE_G4, quarter_duration << 1}, //26
    {NOTE_E5, quarter_duration >> 1}, {NOTE_C5, quarter_duration}, {NOTE_G4, quarter_duration >> 1}, {REST, quarter_duration}, {NOTE_GS4, quarter_duration},
    {NOTE_A4, quarter_duration >> 1}, {NOTE_F5, quarter_duration}, {NOTE_F5, quarter_duration >> 1}, {NOTE_A4, quarter_duration << 1},
    {NOTE_B4, quarter_duration >> 1}, {NOTE_F5, quarter_duration}, {NOTE_F5, quarter_duration >> 1}, {NOTE_F5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_E5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_D5, (quarter_duration >> 1) + (quarter_duration >> 2)},
    {NOTE_C5, quarter_duration >> 1}, {NOTE_E4, quarter_duration}, {NOTE_E4, quarter_duration >> 1}, {NOTE_C4, quarter_duration << 1},
    {NOTE_C5, quarter_duration >> 1}, {NOTE_C5, quarter_duration}, {NOTE_C5, quarter_duration >> 1}, {REST, quarter_duration >> 1}, {NOTE_C5, quarter_duration >> 1}, {NOTE_D5, quarter_duration >> 1}, {NOTE_E5, quarter_duration >> 1},
    {REST, quarter_duration << 2},

    {NOTE_C5, quarter_duration >> 1}, {NOTE_C5, quarter_duration}, {NOTE_C5, quarter_duration >> 1}, {REST, quarter_duration >> 1}, {NOTE_C5, quarter_duration >> 1}, {NOTE_D5, quarter_duration}, //33
    {NOTE_E5, quarter_duration >> 1}, {NOTE_C5, quarter_duration}, {NOTE_A4, quarter_duration >> 1}, {NOTE_G4, quarter_duration << 1},
    {NOTE_E5, quarter_duration >> 1}, {NOTE_E5, quarter_duration >> 1}, {REST, quarter_duration >> 1}, {NOTE_E5, quarter_duration >> 1}, {REST, quarter_duration >> 1}, {NOTE_C5, quarter_duration >> 1}, {NOTE_E5, quarter_duration},
    {NOTE_G5, quarter_duration}, {REST, quarter_duration}, {NOTE_G4, quarter_duration}, {REST, quarter_duration},
    {NOTE_E5, quarter_duration >> 1}, {NOTE_C5, quarter_duration}, {NOTE_G4, quarter_duration >> 1}, {REST, quarter_duration}, {NOTE_GS4, quarter_duration},
    {NOTE_A4, quarter_duration >> 1}, {NOTE_F5, quarter_duration}, {NOTE_F5, quarter_duration >> 1}, {NOTE_A4, quarter_duration << 1},
    {NOTE_D5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_A5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_A5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_A5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_G5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_F5, (quarter_duration >> 1) + (quarter_duration >> 2)},

    {NOTE_E5, quarter_duration >> 1}, {NOTE_C5, quarter_duration}, {NOTE_A4, quarter_duration >> 1}, {NOTE_G4, quarter_duration << 1}, //40
    {NOTE_E5, quarter_duration >> 1}, {NOTE_C5, quarter_duration}, {NOTE_G4, quarter_duration >> 1}, {REST, quarter_duration}, {NOTE_GS4, quarter_duration},
    {NOTE_A4, quarter_duration >> 1}, {NOTE_F5, quarter_duration}, {NOTE_F5, quarter_duration >> 1}, {NOTE_A4, quarter_duration << 1},
    {NOTE_B4, quarter_duration >> 1}, {NOTE_F5, quarter_duration}, {NOTE_F5, quarter_duration >> 1}, {NOTE_F5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_E5, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_D5, (quarter_duration >> 1) + (quarter_duration >> 2)},
    {NOTE_C5, quarter_duration >> 1}, {NOTE_E4, quarter_duration}, {NOTE_E4, quarter_duration >> 1}, {NOTE_C4, quarter_duration << 1},

    // Game over sound
    {NOTE_C5, quarter_duration + (quarter_duration >> 1)},
	{NOTE_G4, quarter_duration + (quarter_duration >> 1)}, {NOTE_E4, quarter_duration}, //45
    {NOTE_A4, (quarter_duration >> 1) + (quarter_duration >> 2)},
	{NOTE_B4, (quarter_duration >> 1) + (quarter_duration >> 2)},
	{NOTE_A4, (quarter_duration >> 1) + (quarter_duration >> 2)},
	{NOTE_GS4, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_AS4, (quarter_duration >> 1) + (quarter_duration >> 2)}, {NOTE_GS4, (quarter_duration >> 1) + (quarter_duration >> 2)},
    {NOTE_G4, quarter_duration >> 1},
	{NOTE_D4, quarter_duration >> 1},
	{NOTE_E4, (quarter_duration << 1) + quarter_duration},
	{ REST, 0}
};


//playing songs functions

void PWM_Start()
{
    HAL_TIM_PWM_Start(buzzer_pwm_timer, buzzer_pwm_channel);
}

void PWM_Change_Tone(uint16_t pwm_freq, uint16_t volume) // pwm_freq (1 - 20000), volume (0 - 1000)
{
    if (pwm_freq == 0 || pwm_freq > 20000)
    {
        __HAL_TIM_SET_COMPARE(buzzer_pwm_timer, buzzer_pwm_channel, 0);
    }
    else
    {
        const uint32_t internal_clock_freq = HAL_RCC_GetSysClockFreq();
        const uint16_t prescaler = 1;
        const uint32_t timer_clock = internal_clock_freq / prescaler;
        const uint32_t period_cycles = timer_clock / pwm_freq;
        const uint32_t pulse_width = volume * period_cycles / 1000 / 2;

        buzzer_pwm_timer->Instance->PSC = prescaler - 1;
        buzzer_pwm_timer->Instance->ARR = period_cycles - 1;
        buzzer_pwm_timer->Instance->EGR = TIM_EGR_UG;
        __HAL_TIM_SET_COMPARE(buzzer_pwm_timer, buzzer_pwm_channel, pulse_width); // buzzer_pwm_timer->Instance->CCR2 = pulse_width;
    }
}

void Change_Melody(const Tone *melody, uint16_t tone_count)
{
    melody_ptr = melody;
    melody_tone_count = tone_count;
    current_tone_number = 0;
}

void Update_Melody()
{
    if ((HAL_GetTick() > current_tone_end) && (current_tone_number < melody_tone_count))
    {
        const Tone active_tone = *(melody_ptr + current_tone_number);
        PWM_Change_Tone(active_tone.frequency, volume);
        current_tone_end = HAL_GetTick() + active_tone.duration;
        current_tone_number++;
    }
}






uint8_t strcmpwithlength(const char * str1, const char * str2, const uint8_t len)
{
	for(int i = 0 ; i < len; ++i) {
		if(str1[i] != str2[i])
			return 0;
	}
	return 1;
}

//
//void uart_log(uint8_t state) {
//	if(log_on == 0)
//		return;
//
//	switch (state) {
//		case 1:
//
//			break;
//		default:
//			break;
//	}
//	//HAL_UART_Transmit_IT(&huart1, &transmit_data, strlen(transmit_data));
//	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
//	HAL_UART_Transmit(&huart1, transmit_data, strlen(transmit_data), 200);
//}
//


void display_digit(uint8_t num, uint8_t digit, uint8_t dcpoint)
{
	// Which of the four digits is this
	// Active high 7-segment, low pin -> digit on
    HAL_GPIO_WritePin(pin, bitActive0, digit == 0 ? 0 : 1);
    HAL_GPIO_WritePin(pin, bitActive1, digit == 1 ? 0 : 1);
    HAL_GPIO_WritePin(pin, bitActive2, digit == 2 ? 0 : 1);
    HAL_GPIO_WritePin(pin, bitActive3, digit == 3 ? 0 : 1);

    // ABCD BCD Output
    HAL_GPIO_WritePin(pin, bit0, (num == 1 || num == 3 || num == 5 || num == 7 || num == 9));
    HAL_GPIO_WritePin(pin, bit1, (num == 2 || num == 3 || num == 6 || num == 7));
    HAL_GPIO_WritePin(pin, bit2, (num == 4 || num == 5 || num == 6 || num == 7));
    HAL_GPIO_WritePin(pin, bit3, (num == 8 || num == 9));

    // Is decimal point on or off for digit?
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, dcpoint == 1 ? 1 : 0);
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2) {

		Update_Melody();

		static uint8_t i = 0;
		display_digit(i, i, 1);
		++i;
		i = i % 4;

	}
}

//
//
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	static last_interrupt = 0;
//	if(HAL_GetTick() - last_interrupt < 200)
//		return;
//
//	last_interrupt = HAL_GetTick();
//
//	if(GPIO_Pin == GPIO_PIN_11) { //increase selected digit
//		input_led_state = 1;
//		if(digit_confirmed[selected_digit] == 0 && input_permision) {
//			digits[selected_digit] = (digits[selected_digit] + 1) % 10;
//		}
//	}
//	else if(GPIO_Pin == GPIO_PIN_10) { //next digit
//		if(input_permision)
//		selected_digit = (selected_digit + 1) % 4;
//
//
//	}
//	else if(GPIO_Pin == GPIO_PIN_15) { //confirm digit
//		input_led_state = 1;
//		if(digit_confirmed[selected_digit]) {
//			--digit_confirmed_no;
//			digit_confirmed[selected_digit] = 0;
//		} else {
//			++digit_confirmed_no;
//			digit_confirmed[selected_digit] = 1;
//			if(digit_confirmed_no == 4) {
//				if(digits[0] == password[0] && digits[1] == password[1] && digits[2] == password[2] && digits[3] == password[3]) {
//					digits[0] = digits[1] = digits[2] = digits[3] = 0;
//					corct_pass_led_state = 1;
//					wrong_password_no = 0;
//					uart_log(4);
//				} else {
//					wrong_pass_led_state = 1;
//					++wrong_password_no;
//					if(wrong_password_no > 2) {
//						input_permision = 0;
//					}
//					uart_log(5);
//				}
//				digit_confirmed[0] = digit_confirmed[1] = digit_confirmed[2] = digit_confirmed[3] = 0;
//				digit_confirmed_no = 0;
//				input_led_state = 0;
//			}
//		}
//	} else if(GPIO_Pin == GPIO_PIN_0) {
//		wrong_password_no = 0;
//		input_permision = 1;
//		buzzerChangeTone(0, 0);
//	}
//}
//
//
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart->Instance == USART1) {
//		HAL_UART_Receive_IT(&huart1, &receive, 1);
//		receive_data[data_index] = receive;
//		++data_index;
//		if(receive == '\n') {
//			if(strcmpwithlength(receive_data, SET_PASS, 8)) {
//				if(data_index == 15
//				   && receive_data[8] == '('
//				   && receive_data[9] >= '0' && receive_data[9] <= '9'
//				   && receive_data[10] >= '0' && receive_data[10] <= '9'
//				   && receive_data[11] >= '0' && receive_data[11] <= '9'
//				   && receive_data[12] >= '0' && receive_data[12] <= '9'
//				   && receive_data[13] == ')') {
//
//					//repeated password
//					if(receive_data[12] - '0' == password[3] &&
//						receive_data[11] - '0' == password[2] &&
//						receive_data[10] - '0' == password[1] &&
//						receive_data[9] - '0' == password[0]) {
//						log_state = 2;
//
//					} else {
//						HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
//						password[3] = receive_data[12] - '0';
//						password[2] = receive_data[11] - '0';
//						password[1] = receive_data[10] - '0';
//						password[0] = receive_data[9] - '0';
//						log_state = 3;
//						}
//					} else {
//						log_state = 2;
//				}
//
//			} else if(strcmpwithlength(receive_data, LOG_ON, strlen(LOG_ON))) {
//				log_on = 1;
//				log_state = 6;
//
//			} else if(strcmpwithlength(receive_data, LOG_OFF, strlen(LOG_OFF))) {
//				log_state = 7;
////				log_on = 0;
//
//			} else if(strcmpwithlength(receive_data, ALERT_ON, strlen(ALERT_ON))) {
//				buzzer_on = 2;
//				buzzerChangeTone(1500, 100);
//				log_state = 11;
//
//			} else if(strcmpwithlength(receive_data, ALERT_OFF, strlen(ALERT_OFF))) {
//				buzzer_on = 0;
//				log_state = 10;
//
//			} else if(strcmpwithlength(receive_data, SET_VOLUME, strlen(SET_VOLUME))) {
//				if(data_index == 14
//				   && receive_data[10] == '('
//				   && receive_data[11] >= '0' && receive_data[11] <= '6'
//				   && receive_data[12] == ')') {
//
//					buzzer_volume = (receive_data[11] - '0') * 20;
//					log_state = 9;
//
//					} else {
//						log_state = 2;
//				}
//			} else if(strcmpwithlength(receive_data, SET_ALERT, strlen(SET_ALERT))) {
//				if(data_index == 16
//				   && receive_data[9] == '('
//				   && receive_data[10] >= '0' && receive_data[10] <= '9'
//				   && receive_data[11] >= '0' && receive_data[11] <= '9'
//				   && receive_data[12] >= '0' && receive_data[12] <= '9'
//				   && receive_data[13] >= '0' && receive_data[13] <= '9'
//				   && receive_data[14] == ')') {
//
//						HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
//						buzzer_wrong_alert_frq = (receive_data[10] - '0') * 1000 + (receive_data[11] - '0') * 100 + (receive_data[12] - '0') * 10 + (receive_data[13] - '0');
//						log_state = 8;
//
//					} else {
//						log_state = 2;
//					}
//			} else {
//				log_state = 1;
//			}
//			uart_log(log_state);
//			data_index = 0;
//		}
//	}
//}
//
//


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
  MX_SPI1_Init();
  MX_USB_PCD_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  Update_Melody();
  HAL_TIM_Base_Start_IT(&htim2);
  PWM_Start();
//  Change_Melody(super_mario_bros, ARRAY_LENGTH(super_mario_bros));
  Change_Melody(mario2, ARRAY_LENGTH(mario2));
  HAL_UART_Transmit(&huart1, "salam\r", 6, HAL_MAX_DELAY);


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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 4800;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void)
{

  /* USER CODE BEGIN USB_Init 0 */

  /* USER CODE END USB_Init 0 */

  /* USER CODE BEGIN USB_Init 1 */

  /* USER CODE END USB_Init 1 */
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_Init 2 */

  /* USER CODE END USB_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : DRDY_Pin MEMS_INT3_Pin MEMS_INT4_Pin MEMS_INT1_Pin
                           MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = DRDY_Pin|MEMS_INT3_Pin|MEMS_INT4_Pin|MEMS_INT1_Pin
                          |MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : CS_I2C_SPI_Pin LD4_Pin LD3_Pin LD5_Pin
                           LD7_Pin LD9_Pin LD10_Pin LD8_Pin
                           LD6_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD1 PD2 PD3
                           PD4 PD5 PD6 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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

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

//--------strcutures
typedef struct
{
    uint16_t frequency;
    uint16_t duration;
} Tone;

typedef struct
{
	Tone * melody;
	uint16_t melody_length;
} song;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

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

#define C4  261
#define D4  294
#define E4  329
#define F4  349
#define G4  392
#define A4  440
#define B4  493
#define C5  523


#define MAX_SAMPLE_NUMBER 100

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

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
static void MX_ADC1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//--------enums
enum songs {
	MARIO,

};
enum states {
	PAUSE,
	PLAYING,
	CHANGING_VOLUME,
	CHANGING_SONG,

};

//-----------Global variables-----------\\


//--------general controlling variables
uint8_t current_state = PAUSE;
uint32_t potensiometer_value;
uint8_t previous_state = PAUSE;

//--------number to 7448
GPIO_TypeDef * pin = GPIOD;
uint16_t bit0 = GPIO_PIN_4;
uint16_t bit1 = GPIO_PIN_5;
uint16_t bit2 = GPIO_PIN_6;
uint16_t bit3 = GPIO_PIN_7;

//--------digit to be turned on
uint16_t bitActive0 = GPIO_PIN_2;
uint16_t bitActive1 = GPIO_PIN_3;
uint16_t bitActive2 = GPIO_PIN_1;
uint16_t bitActive3 = GPIO_PIN_0;

//--------Digits
uint8_t digits[4] = {0, 1, 2, 3};

//--------UART
char received_data[50];
uint8_t data_index = 0;
char receive;
char transmit_data[50];
uint8_t uart_mode = 1;
uint8_t log_state = 0;
const char * MUSIC_SET = "MUSIC_SET";
const char * CHANGE_VOLUME = "CHANGE_VOLUME";

//--------LEDs
GPIO_TypeDef * ledg = GPIOE;
uint16_t led0 = GPIO_PIN_8;
uint16_t led1 = GPIO_PIN_9;
uint16_t led2 = GPIO_PIN_10;
uint16_t led3 = GPIO_PIN_11;
uint16_t led4 = GPIO_PIN_12;
uint16_t led5 = GPIO_PIN_13;
uint16_t led6 = GPIO_PIN_14;
uint16_t led7 = GPIO_PIN_15;

//--------Buzzer
TIM_HandleTypeDef *buzzer_pwm_timer = &htim2;	// Point to PWM Timer configured in CubeMX
uint32_t buzzer_pwm_channel = TIM_CHANNEL_2;   // Select configured PWM channel number
const Tone *volatile melody_ptr;
volatile uint16_t melody_tone_count = 0;
volatile uint16_t current_tone_number = 0;
volatile uint32_t current_tone_end = 0;
volatile uint16_t volume = 10; // (0 - 1000)
const uint8_t number_of_songs = 6;
const uint16_t quarter_duration = 204;
const uint16_t tempo2 = 2 * quarter_duration;
const uint16_t tempo4 = quarter_duration;
const uint16_t tempo8 = quarter_duration >> 1;
const uint16_t tempo16 = quarter_duration >> 2;
const uint16_t tempom4 = quarter_duration + (quarter_duration >> 1);
const uint16_t tempom8 = (quarter_duration >> 1) + (quarter_duration >> 2);
const uint16_t tempom16 = (quarter_duration >> 1) + (quarter_duration >> 2);

uint8_t current_song = 0;

//--------melodies
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
	{NOTE_B4, quarter_duration},
	{NOTE_AS4, quarter_duration >> 1},
	{NOTE_A4, quarter_duration},
    {NOTE_G4, (quarter_duration >> 1) + (quarter_duration >> 2)},
	{NOTE_E5, (quarter_duration >> 1) + (quarter_duration >> 2)},
	{NOTE_G5, (quarter_duration >> 1) + (quarter_duration >> 2)},
	{NOTE_A5, quarter_duration},
	{NOTE_F5, quarter_duration >> 1},
	{NOTE_G5, quarter_duration >> 1},
    {REST, quarter_duration >> 1},
	{NOTE_E5, quarter_duration},
	{NOTE_C5, quarter_duration >> 1},
	{NOTE_D5, quarter_duration >> 1},
	{NOTE_B4, quarter_duration + (quarter_duration >> 1)},
    {NOTE_C5, quarter_duration + (quarter_duration >> 1)},
	{NOTE_G4, quarter_duration >> 1},
	{REST, quarter_duration},
	{NOTE_E4, quarter_duration + (quarter_duration >> 1)}, // repeats from 3
    {NOTE_A4, quarter_duration},
	{NOTE_B4, quarter_duration},
	{NOTE_AS4, quarter_duration >> 1},
	{NOTE_A4, quarter_duration},
    {NOTE_G4, (quarter_duration >> 1) + (quarter_duration >> 2)},
	{NOTE_E5, (quarter_duration >> 1) + (quarter_duration >> 2)},
	{NOTE_G5, (quarter_duration >> 1) + (quarter_duration >> 2)},
	{NOTE_A5, quarter_duration}, {NOTE_F5, quarter_duration >> 1},
	{NOTE_G5, quarter_duration >> 1},
    {REST, quarter_duration >> 1},
	{NOTE_E5, quarter_duration},
	{NOTE_C5, quarter_duration >> 1},
	{NOTE_D5, quarter_duration >> 1},
	{NOTE_B4, quarter_duration + (quarter_duration >> 1)},

    {REST, quarter_duration},
	{NOTE_G5, quarter_duration >> 1},
	{NOTE_FS5, quarter_duration >> 1},
	{NOTE_F5, quarter_duration >> 1},
	{NOTE_DS5, quarter_duration},
	{NOTE_E5, quarter_duration >> 1}, //7
    {REST, quarter_duration >> 1},
	{NOTE_GS4, quarter_duration >> 1},
	{NOTE_A4, quarter_duration >> 1},
	{NOTE_C4, quarter_duration >> 1},
	{REST, quarter_duration >> 1},
	{NOTE_A4, quarter_duration >> 1},
	{NOTE_C5, quarter_duration >> 1},
	{NOTE_D5, quarter_duration >> 1},
    {REST, quarter_duration},
	{NOTE_DS5, quarter_duration},
	{REST, quarter_duration >> 1},
	{NOTE_D5, quarter_duration + (quarter_duration >> 1)},
    {NOTE_C5, quarter_duration << 1},
	{REST, quarter_duration << 1},

    {REST, quarter_duration},
	{NOTE_G5, quarter_duration >> 1},
	{NOTE_FS5, quarter_duration >> 1},
	{NOTE_F5, quarter_duration >> 1},
	{NOTE_DS5, quarter_duration},
	{NOTE_E5, quarter_duration >> 1}, //repeats from 7
    {REST, quarter_duration >> 1},
	{NOTE_GS4, quarter_duration >> 1},
	{NOTE_A4, quarter_duration >> 1},
	{NOTE_C4, quarter_duration >> 1},
	{REST, quarter_duration >> 1},
	{NOTE_A4, quarter_duration >> 1},
	{NOTE_C5, quarter_duration >> 1},
	{NOTE_D5, quarter_duration >> 1},
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

const Tone imperial_march[] = {
	 {NOTE_A4, tempom4}, {NOTE_A4, tempom4}, {NOTE_A4, tempo16}, {NOTE_A4, tempo16}, {NOTE_A4, tempo16}, {NOTE_A4, tempo16}, {NOTE_F4, tempo8}, {REST, tempo8},
	{NOTE_A4, tempom4}, {NOTE_A4, tempom4}, {NOTE_A4, tempo16}, {NOTE_A4, tempo16}, {NOTE_A4, tempo16}, {NOTE_A4, tempo16}, {NOTE_F4, tempo8}, {REST, tempo8},
	{NOTE_A4, tempo4}, {NOTE_A4, tempo4}, {NOTE_A4, tempo4}, {NOTE_F4, tempom8}, {NOTE_C5, tempo16},
	{NOTE_A4, tempo4}, {NOTE_F4, tempom8}, {NOTE_C5, tempo16}, {NOTE_A4, tempo2},

	{NOTE_E5, tempo4}, {NOTE_E5, tempo4}, {NOTE_E5, tempo4}, {NOTE_F5, tempom8}, {NOTE_C5, tempo16},
	{NOTE_A4, tempo4}, {NOTE_F4, tempom8}, {NOTE_C5, tempo16}, {NOTE_A4, tempo2},

	{NOTE_A5, tempo4}, {NOTE_A4, tempom8}, {NOTE_A4, tempo16}, {NOTE_A5, tempo4}, {NOTE_GS5, tempom8}, {NOTE_G5, tempo16},
	{NOTE_DS5, tempo16}, {NOTE_D5, tempo16}, {NOTE_DS5, tempo8}, {REST, tempo8}, {NOTE_A4, tempo8}, {NOTE_DS5, tempo4}, {NOTE_D5, tempom8}, {NOTE_CS5, tempo16},

	{NOTE_C5, tempo16}, {NOTE_B4, tempo16}, {NOTE_C5, tempo16}, {REST, tempo8}, {NOTE_F4, tempo8}, {NOTE_GS4, tempo4}, {NOTE_F4, tempom8}, {NOTE_A4, tempom16},
	{NOTE_C5, tempo4}, {NOTE_A4, tempom8}, {NOTE_C5, tempo16}, {NOTE_E5, tempo2},

	{NOTE_A5, tempo4}, {NOTE_A4, tempom8}, {NOTE_A4, tempo16}, {NOTE_A5, tempo4}, {NOTE_GS5, tempom8}, {NOTE_G5, tempo16},
	{NOTE_DS5, tempo16}, {NOTE_D5, tempo16}, {NOTE_DS5, tempo8}, {REST, tempo8}, {NOTE_A4, tempo8}, {NOTE_DS5, tempo4}, {NOTE_D5, tempom8}, {NOTE_CS5, tempo16},

	{NOTE_C5, tempo16}, {NOTE_B4, tempo16}, {NOTE_C5, tempo16}, {REST, tempo8}, {NOTE_F4, tempo8}, {NOTE_GS4, tempo4}, {NOTE_F4, tempom8}, {NOTE_A4, tempom16},
	{NOTE_A4, tempo4}, {NOTE_F4, tempom8}, {NOTE_C5, tempo16}, {NOTE_A4, tempo2}
};

const Tone pink_panther[] = {
		{REST, 500},   // Example: a rest for 500 milliseconds
		    {NOTE_E5, 500}, {NOTE_B4, 375}, {NOTE_C5, 125}, {NOTE_D5, 125},
		    {NOTE_C5, 250}, {NOTE_A4, 125}, {NOTE_A4, 125}, {NOTE_A4, 250},
		    {NOTE_E5, 500}, {NOTE_B4, 375}, {NOTE_C5, 125}, {NOTE_D5, 125},
		    {NOTE_E5, 500}, {NOTE_D5, 375}, {NOTE_C5, 125}, {NOTE_B4, 125},
		    {NOTE_A4, 250}, {NOTE_A4, 125}, {NOTE_A4, 125}, {NOTE_A4, 250},
		    {NOTE_E5, 500}, {NOTE_B4, 375}, {NOTE_C5, 125}, {NOTE_D5, 125},
		    {NOTE_C5, 250}, {NOTE_A4, 125}, {NOTE_A4, 125}, {NOTE_A4, 750}, {REST, 250},
		    {NOTE_A4, 500}, {NOTE_D5, 375}, {NOTE_F5, 125}, {NOTE_A5, 125},
		    {NOTE_G5, 250}, {NOTE_FS5, 125}, {NOTE_F5, 125}, {NOTE_FS5, 250},
		    {NOTE_AS4, 500}, {NOTE_D5, 375}, {NOTE_F5, 125}, {NOTE_A5, 125},
		    {NOTE_G5, 250}, {NOTE_FS5, 125}, {NOTE_F5, 125}, {NOTE_FS5, 500},
		    {REST, 250}, {NOTE_AS4, 500}, {NOTE_D5, 375}, {NOTE_F5, 125}, {NOTE_A5, 125},
		    {NOTE_G5, 250}, {NOTE_FS5, 125}, {NOTE_F5, 125}, {NOTE_FS5, 250},
		    {NOTE_A4, 250}, {NOTE_AS4, 125}, {NOTE_A4, 125}, {NOTE_G4, 125},
		    {NOTE_A4, 500}, {NOTE_AS4, 375}, {NOTE_A4, 125}, {NOTE_G4, 125},
		    {NOTE_A4, 500}, {NOTE_AS4, 375}, {NOTE_A4, 125}, {NOTE_G4, 125},
		    {NOTE_A4, 250}, {NOTE_AS4, 125}, {NOTE_A4, 125}, {NOTE_G4, 125},
		    {REST, 500},
};

const Tone game_of_thrones[] = {
		{NOTE_G4, 250}, {NOTE_C4, 250}, {NOTE_DS4, 125}, {NOTE_F4, 125}, {NOTE_G4, 250}, {NOTE_C4, 250}, {NOTE_DS4, 125}, {NOTE_F4, 125}, //1
		    {NOTE_G4, 250}, {NOTE_C4, 250}, {NOTE_DS4, 125}, {NOTE_F4, 125}, {NOTE_G4, 250}, {NOTE_C4, 250}, {NOTE_DS4, 125}, {NOTE_F4, 125},
		    {NOTE_G4, 250}, {NOTE_C4, 250}, {NOTE_E4, 125}, {NOTE_F4, 125}, {NOTE_G4, 250}, {NOTE_C4, 250}, {NOTE_E4, 125}, {NOTE_F4, 125},
		    {NOTE_G4, 250}, {NOTE_C4, 250}, {NOTE_E4, 125}, {NOTE_F4, 125}, {NOTE_G4, 250}, {NOTE_C4, 250}, {NOTE_E4, 125}, {NOTE_F4, 125},
		    {NOTE_G4, 500}, {NOTE_C4, 500}, //5

		    {NOTE_DS4, 125}, {NOTE_F4, 125}, {NOTE_G4, 250}, {NOTE_C4, 250}, {NOTE_DS4, 125}, {NOTE_F4, 125}, //6
		    {NOTE_D4, 1000}, //7 and 8
		    {NOTE_F4, 500}, {NOTE_AS3, 500},
		    {NOTE_DS4, 125}, {NOTE_D4, 125}, {NOTE_F4, 250}, {NOTE_AS3, 500},
		    {NOTE_DS4, 125}, {NOTE_D4, 125}, {NOTE_C4, 1000}, //11 and 12

		    //repeats from 5
		    {NOTE_G4, 500}, {NOTE_C4, 500}, //5

		    {NOTE_DS4, 125}, {NOTE_F4, 125}, {NOTE_G4, 250}, {NOTE_C4, 250}, {NOTE_DS4, 125}, {NOTE_F4, 125}, //6
		    {NOTE_D4, 1000}, //7 and 8
		    {NOTE_F4, 500}, {NOTE_AS3, 500},
		    {NOTE_DS4, 125}, {NOTE_D4, 125}, {NOTE_F4, 250}, {NOTE_AS3, 500},
		    {NOTE_DS4, 125}, {NOTE_D4, 125}, {NOTE_C4, 1000}, //11 and 12
		    {NOTE_G4, 500}, {NOTE_C4, 500}, //5

		    {NOTE_DS4, 125}, {NOTE_F4, 125}, {NOTE_G4, 250}, {NOTE_C4, 250}, {NOTE_DS4, 125}, {NOTE_F4, 125}, //6
		    {NOTE_D4, 1000}, //7 and 8
		    {NOTE_F4, 500}, {NOTE_AS3, 500},
		    {NOTE_DS4, 125}, {NOTE_D4, 125}, {NOTE_F4, 250}, {NOTE_AS3, 500},
		    {NOTE_DS4, 125}, {NOTE_D4, 125}, {NOTE_C4, 1000}, //11 and 12
		    {NOTE_G4, 500}, {NOTE_C4, 500}, {NOTE_DS4, 125}, {NOTE_F4, 125}, {NOTE_G4, 250}, {NOTE_C4, 250}, {NOTE_DS4, 125}, {NOTE_F4, 125},

		    {NOTE_D4, 2000}, //15
		    {NOTE_F4, 500}, {NOTE_AS3, 500},
		    {NOTE_D4, 250}, {NOTE_DS4, 250}, {NOTE_D4, 250}, {NOTE_AS3, 250},
		    {NOTE_C4, 1000},
		    {NOTE_C5, 2000},
		    {NOTE_AS4, 2000},
		    {NOTE_C4, 2000},
		    {NOTE_G4, 2000},
		    {NOTE_DS4, 2000},
		    {NOTE_DS4, 500}, {NOTE_F4, 500},
		    {NOTE_G4, 1000},

		    {NOTE_C5, 2000}, //28
		    {NOTE_AS4, 2000},
		    {NOTE_C4, 2000},
		    {NOTE_G4, 2000},
		    {NOTE_DS4, 2000},
		    {NOTE_DS4, 500}, {NOTE_D4, 500},
		    {NOTE_C5, 1000}, {NOTE_G4, 1000}, {NOTE_GS4, 250}, {NOTE_AS4, 250}, {NOTE_C5, 1000}, {NOTE_G4, 1000}, {NOTE_GS4, 250}, {NOTE_AS4, 250},
		    {NOTE_C5, 1000}, {NOTE_G4, 1000}, {NOTE_GS4, 250}, {NOTE_AS4, 250}, {NOTE_C5, 1000}, {NOTE_G4, 1000}, {NOTE_GS4, 250}, {NOTE_AS4, 250},

		    {0, 500}, {NOTE_GS5, 250}, {NOTE_AS5, 250}, {NOTE_C6, 500}, {NOTE_G5, 500}, {NOTE_GS5, 250}, {NOTE_AS5, 250},
		    {NOTE_C6, 500}, {NOTE_G5, 250}, {NOTE_GS5, 250}, {NOTE_AS5, 250}, {NOTE_C6, 500}, {NOTE_G5, 500}, {NOTE_GS5, 250}, {NOTE_AS5, 250}
};

const Tone smoothCriminal[] = {
	{A4, 150}, {A4, 150}, {A4, 150}, {G4, 150}, {A4, 150}, {A4, 150}, {G4, 150},
	{A4, 150}, {A4, 150}, {G4, 150}, {A4, 150}, {A4, 150}, {G4, 150}, {A4, 150},
	  {A4, 150}, {G4, 150}, {A4, 150}, {G4, 150}, {F4, 150}, {A4, 150}, {A4, 150},
	  {G4, 150}, {A4, 150}, {A4, 150}, {G4, 150}, {A4, 150}, {A4, 150}, {G4, 150},
	  {A4, 150}, {G4, 150}, {F4, 150}, {A4, 150}, {A4, 150}, {G4, 150}, {A4, 150},
	  {A4, 150}, {G4, 150}, {A4, 150}, {G4, 150}, {F4, 150}, {A4, 150}, {A4, 150},
	  {G4, 150}, {A4, 150}, {A4, 150}, {G4, 150}, {A4, 150}, {A4, 150}, {G4, 150},
	  {A4, 150}, {G4, 150}, {F4, 150}, {A4, 150}, {A4, 150}, {G4, 150}, {A4, 150},
	  {A4, 150}, {G4, 150}, {A4, 150}, {G4, 150}, {F4, 150}, {A4, 150}, {A4, 150},
	  {G4, 150}, {A4, 150}, {A4, 150}, {G4, 150}, {A4, 150}, {A4, 150}, {G4, 150},
	  {A4, 150}, {G4, 150}, {F4, 150}, {A4, 150}, {A4, 150}, {G4, 150}, {A4, 150},
	  {A4, 150}, {G4, 150}, {A4, 150}, {G4, 150}, {F4, 150}, {A4, 150}, {A4, 150},
	  {G4, 150}, {A4, 150}, {A4, 150}, {G4, 150}, {A4, 150}, {A4, 150}, {G4, 150},
	  {A4, 150}, {G4, 150}, {F4, 150}, {A4, 150}, {A4, 150}, {G4, 150}, {A4, 150}
};



const song songs[] = {
		{super_mario_bros, ARRAY_LENGTH(super_mario_bros)},
		{mario2, ARRAY_LENGTH(mario2)},
		{imperial_march, ARRAY_LENGTH(imperial_march)},
		{pink_panther, ARRAY_LENGTH(pink_panther)},
		{game_of_thrones, ARRAY_LENGTH(game_of_thrones)},
		{smoothCriminal, ARRAY_LENGTH(smoothCriminal)}
};


//-----------Functions-----------\\


//--------playing songs functions
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


void Change_Song(uint8_t song_number)
{
	current_song = song_number;
	Change_Melody(songs[current_song].melody, songs[current_song].melody_length);
}


void Change_Melody(const Tone *melody, uint16_t tone_count)
{
    melody_ptr = melody;
    melody_tone_count = tone_count;
    current_tone_number = 0;
    current_tone_end = 0;
}

void Update_Melody()
{
    if ((HAL_GetTick() > current_tone_end) && (current_tone_number < melody_tone_count))
    {
        const Tone active_tone = *(melody_ptr + current_tone_number);
        PWM_Change_Tone(active_tone.frequency, volume);
        current_tone_end = HAL_GetTick() + active_tone.duration;
        current_tone_number++;
    } else if(current_tone_number >= melody_tone_count) {
    	Change_Song((current_song + 1) % number_of_songs);
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

void uart_log(uint8_t state) {
	uint32_t time = HAL_GetTick();

	switch (state) {
		case 1: // inside () is wrong for MUSIC_SET
			sprintf(transmit_data, "--[ERROR][Music not found][%d]\r\n>>", time);
			break;
		case 2: //music number changed
			sprintf(transmit_data, "--[INFO][Music changed to %d][%d]\r\n>>", current_song, time);
			break;
		case 3: //
			sprintf(transmit_data, "--[ERROR][Volume not valid][%d]\r\n>>", time);
			break;
		case 4: //
			sprintf(transmit_data, "--[INFO][Volume changed to %d][%d]\r\n>>", volume, time);
			break;
		case 100:
			sprintf(transmit_data, "--[ERROR][Invalid Command][%d]\r\n>>", time);
		default:
			break;
	}
	HAL_UART_Transmit(&huart1, transmit_data, strlen(transmit_data), 200);
}

unsigned int a = 0;

void updateDigits()
{
	switch(current_state) {
	case PAUSE   :
	case PLAYING :
		digits[3] = current_tone_number % 10;
		digits[2] = (current_tone_number / 10) % 10;
		digits[1] = (current_tone_number / 100) % 10;
		digits[0] = current_song;
		break;
	case CHANGING_VOLUME :
		digits[3] = potensiometer_value % 10;
		digits[2] = (potensiometer_value / 10) % 10;
		digits[1] = (potensiometer_value / 100) % 10;
		digits[0] = 0;
		break;
	case CHANGING_SONG :
		digits[0] = (potensiometer_value * number_of_songs / 100);
	}
}


//--------Interrupts Functions--------\\

//TIMERS
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1) {
		// Flicker seven segment digits
		static uint8_t i = 0;
		static uint16_t blinking = 0;

		switch (current_state) {
		case PLAYING :
			updateDigits();
			display_digit(digits[i], i, i == 0);
			++i;
			i = i % 4;
			break;
		case PAUSE :
			display_digit(digits[i], (blinking < 300) ? i : 5, i == 0); // digit 5 is equal to no digit so it will be off
			++i;
			i %= 4;
			++blinking;
			blinking %= 600;
			break;
		case CHANGING_VOLUME :
			updateDigits();
			display_digit(digits[i], i, 0);
			++i;
			i = (i % 3)+1;
			// call adc interrupt _ it cannot be in its self interrupt handler because its priority is less
			// than timer and seems like it will be ignored
			HAL_ADC_Start_IT(&hadc1);
			break;
		case CHANGING_SONG :
			updateDigits();
			display_digit(digits[0], 0, 0);
			HAL_ADC_Start_IT(&hadc1);
			break;
		}
	}
	else if (htim->Instance == TIM2) {
		if(current_state == PLAYING || (previous_state == PLAYING && current_state == CHANGING_VOLUME)) {

			Update_Melody();
		}
		else
			PWM_Change_Tone(0, 0);
	}
}

//--------Buttons
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	static last_interrupt = 0;
	static pin11_last_state;
	static pin10_last_state = 0;
	static pin15_last_state = 0;

	if(HAL_GetTick() - last_interrupt < 150)
		return;

	last_interrupt = HAL_GetTick();
	if(GPIO_Pin == GPIO_PIN_11) { // C11 buttion : [.] [] []
		if(current_state == PAUSE) {
			current_state = PLAYING;
		} else if(current_state == PLAYING) {
			current_state = PAUSE;
		}
	} else if (GPIO_Pin == GPIO_PIN_10) { // C10 buttion : [] [.] []
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == 1) {
			if(pin10_last_state == 1) {
				pin10_last_state = 0;
				Change_Song((potensiometer_value * number_of_songs / 100));
				current_state = previous_state;
				return;
			}
			pin10_last_state = 1;
			previous_state = current_state;
			current_state = CHANGING_SONG;
		} else {
			pin10_last_state = 0;
			Change_Song((potensiometer_value * number_of_songs / 100));
			current_state = previous_state;

		}
	} else if (GPIO_Pin == GPIO_PIN_15) { // A15 buttion : [] [] [.]
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == 1) {
			if(pin15_last_state == 1) {
				pin15_last_state = 0;
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 0);
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1);
				current_state = previous_state;
				return;
			}
			pin15_last_state = 1;
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 0);
			previous_state = current_state;
			current_state = CHANGING_VOLUME;

		} else {
			pin15_last_state = 0;
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 0);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1);
			current_state = previous_state;
		}
	}
	updateDigits();
}

//--------ADC
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	if(hadc->Instance == ADC1) {
		static uint8_t sample_no = 0;
		static uint32_t samples_sum = 0;
		static uint16_t val;
		val = HAL_ADC_GetValue(&hadc1);
		samples_sum += val;
		++sample_no;
		if(sample_no == MAX_SAMPLE_NUMBER) {
			potensiometer_value = samples_sum / MAX_SAMPLE_NUMBER / 40;
			if(current_state == CHANGING_VOLUME) {
				volume = potensiometer_value;
				uart_log(4);
			} else
				uart_log(2);
			sample_no = 0;
			samples_sum = 0;
		}

	}
}

//--------UART
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1) {
		HAL_UART_Receive_IT(&huart1, &receive, 1);
		received_data[data_index] = receive;
		++data_index;

		if(receive == '\r') {
			if(strcmpwithlength(received_data, MUSIC_SET, 9)) {
				if(data_index == 13
				    && received_data[9] == '('
				    && received_data[10] >= '0' && received_data[10] <= '9'
				    && received_data[11] == ')')
				{
					current_song = received_data[10] - '0';
					log_state = 2; // music number changed


				} else {
					log_state = 1;
				}

			} else if(strcmpwithlength(received_data, CHANGE_VOLUME, 13)) {
				if(data_index <= 19 & data_index >= 17) {
						       if(received_data[13] == '('
								&& received_data[14] >= '0' && received_data[14] <= '9'
								&& received_data[15] == ')') {
						    	   volume = received_data[14] - '0';
						    	   log_state = 4; // volume changed

						       } else if(received_data[13] == '('
								&& received_data[14] >= '0' && received_data[14] <= '9'
								&& received_data[15] >= '0' && received_data[15] <= '9'
								&& received_data[16] == ')') {
						    	   volume = (received_data[14] - '0') * 10 + received_data[15] - '0';
						    	   log_state = 4; // volume changed

						       } else if(received_data[13] == '('
								&& received_data[14] >= '0' && received_data[14] <= '9'
								&& received_data[15] >= '0' && received_data[15] <= '9'
								&& received_data[16] >= '0' && received_data[16] <= '9'
								&& received_data[17] == ')') {
						    	   volume = (received_data[14] - '0') * 100 + (received_data[15] - '0') * 10 + received_data[16] - '0';
						    	   log_state = 4; // volume changed
						       } else {
						    	   log_state = 3; // inside () is wrong
						       }

				} else {
					log_state = 3; // inside () is wrong
				}
			} else {
				log_state = 100;
			}
			uart_log(log_state);
			data_index = 0;
		}
	}
}




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
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  Update_Melody();
  htim2.Instance->PSC = 480000;
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim1);

  PWM_Start();

//  Change_Melody(super_mario_bros, ARRAY_LENGTH(super_mario_bros));
//  Change_Melody(songs[0].melody, songs[0].melody_length);
  Change_Song(0);
  HAL_UART_Receive_IT(&huart1, &receive, 1);
  HAL_ADC_Start_IT(&hadc1);

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  TIM1->CCR1 = 10;
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
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_TIM1
                              |RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_601CYCLES_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

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
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 480;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 10;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

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
  htim2.Init.Prescaler = 10;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10;
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
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

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

  /*Configure GPIO pin : PD12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PF6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PC10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

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

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

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

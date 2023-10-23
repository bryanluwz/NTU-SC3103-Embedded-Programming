#include "uart.h"

void uart3_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  // Enable the gpio clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); // Enable the Usart clock
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART2);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	// UsartNVIC configuration
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	// Preempt priority
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	// Preempt priority
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	// Enable the IRQ channel
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// Initialize the VIC register with the specified parameters
	NVIC_Init(&NVIC_InitStructure);
	// USART Initialization Settings
	USART_InitStructure.USART_BaudRate = bound;					// Baud rate
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; // The word length is 8 bit data
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// A stop bit
	USART_InitStructure.USART_Parity = USART_Parity_No;			// No parity bits
	// No hardware data flow control
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// Sending and receiving mode
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);	   // Initialize serial port 3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // Open the serial port to accept interrupts
	USART_Cmd(USART3, ENABLE);					   // Enable serial port 3
}

static u32 notes[] = {
	2272, // A - 440 Hz
	2024, // B - 494 Hz
	3822, // C - 261 Hz
	3401, // D - 294 Hz
	3030, // E - 330 Hz
	2865, // F - 349 Hz
	2551, // G - 392 Hz
	1136, // a - 880 Hz
	1012, // b - 988 Hz
	1912, // c - 523 Hz
	1703, // d - 587 Hz
	1517, // e - 659 Hz
	1432, // f - 698 Hz
	1275, // g - 784 Hz
};
static u8 *song =
			  (uint8_t *)"e2,d2,e2,d2,e2,B2,d2,c2,A2_C2,E2,A2,B2_E2,G2,B2,c4_E2,e2,d2,e2,d2,e2,B2,d2
	,
		  c2, A2_C2, E2, A2, B2_E2, c2, B2, A4 ";
	uint32_t
	getNote(uint8_t ch)
{
	if (ch >= 'A' && ch <= 'G')
		return notes[ch - 'A'];
	if (ch >= 'a' && ch <= 'g')
		return notes[ch - 'a' + 7];
	return 0;
}
uint32_t getDuration(uint8_t ch)
{
	if (ch < '0' || ch > '9')
		return 500;
	/* number of ms */
	return (ch - '0') * 250;
}
uint32_t getPause(uint8_t ch)
{
	switch (ch)
	{
	case '+':
		return 0;
	case ',':
		return 5;
	case '.':
		return 20;
	case '_':
		return 30;
	default:
		return 5;
	}
}
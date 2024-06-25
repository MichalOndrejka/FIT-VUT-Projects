/*
 * Predmet: IMP (Mikroprocesorové a vstavané systémy)
 * Téma: Zjdenodušená hra had
 * Autor: Michal Ondrejka (xondre15)
 * Dátum: 13.12.2023
*/

/* Header file with all the essential definitions for a given type of MCU */
#include "MK60D10.h"

/* Macros for bit-level registers manipulation */
#define GPIO_PIN_MASK	0x1Fu
#define GPIO_PIN(x)		(((1)<<(x & GPIO_PIN_MASK)))

#define R0 1 << 26
#define R1 1 << 24
#define R2 1 << 9
#define R3 1 << 25
#define R4 1 << 28
#define R5 1 << 7
#define R6 1 << 27
#define R7 1 << 29

#define A0 1 << 8
#define A1 1 << 10
#define A2 1 << 6
#define A3 1 << 11

#define BTN_SW2     0x400
#define BTN_SW4     0x8000000

#define pushed      (BTN_SW2 | BTN_SW4)

#define BTN_IRQC    0x0A

int time = 0;
int speed = 150;

#define TIMER(time) (time % speed == 0)

typedef enum direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
} direction_enum;

typedef struct snake {
    unsigned int head;
    unsigned int body1;
    unsigned int body2;
    unsigned int body3;
    direction_enum direction;
} snake_struct;

snake_struct snake;

/* Configuration of the necessary MCU peripherals */
void SystemConfig() {
	/* Turn on all port clocks */
	SIM->SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTE_MASK;

	/* Set corresponding PTA pins (column activators of 74HC154) for GPIO functionality */
	PORTA->PCR[8] = ( 0|PORT_PCR_MUX(0x01) );  // A0
	PORTA->PCR[10] = ( 0|PORT_PCR_MUX(0x01) ); // A1
	PORTA->PCR[6] = ( 0|PORT_PCR_MUX(0x01) );  // A2
	PORTA->PCR[11] = ( 0|PORT_PCR_MUX(0x01) ); // A3

	/* Set corresponding PTA pins (rows selectors of 74HC154) for GPIO functionality */
	PORTA->PCR[26] = ( 0|PORT_PCR_MUX(0x01) );  // R0
	PORTA->PCR[24] = ( 0|PORT_PCR_MUX(0x01) );  // R1
	PORTA->PCR[9] = ( 0|PORT_PCR_MUX(0x01) );   // R2
	PORTA->PCR[25] = ( 0|PORT_PCR_MUX(0x01) );  // R3
	PORTA->PCR[28] = ( 0|PORT_PCR_MUX(0x01) );  // R4
	PORTA->PCR[7] = ( 0|PORT_PCR_MUX(0x01) );   // R5
	PORTA->PCR[27] = ( 0|PORT_PCR_MUX(0x01) );  // R6
	PORTA->PCR[29] = ( 0|PORT_PCR_MUX(0x01) );  // R7

	/* Set corresponding PTE pins (output enable of 74HC154) for GPIO functionality */
	PORTE->PCR[28] = ( 0|PORT_PCR_MUX(0x01) ); // #EN

	/* Change corresponding PTA port pins as outputs */
	PTA->PDDR = GPIO_PDDR_PDD(0x3F000FC0);

	/* Change corresponding PTE port pins as outputs */
	PTE->PDDR = GPIO_PDDR_PDD( GPIO_PIN(28) );
}

unsigned char currR(unsigned int headRow) {
    unsigned char currR = 0x0;
    if (headRow & R0) {
        currR |= (1 << 0);
    }
    if (headRow & R1) {
        currR |= (1 << 1);
    }
    if (headRow & R2) {
        currR |= (1 << 2);
    }
    if (headRow & R3) {
        currR |= (1 << 3);
    }
    if (headRow & R4) {
        currR |= (1 << 4);
    }
    if (headRow & R5) {
        currR |= (1 << 5);
    }
    if (headRow & R6) {
        currR |= (1 << 6);
    }
    if (headRow & R7) {
        currR |= (1 << 7);
    }
    return currR;
}

unsigned int nextR(unsigned char nextRow) {
    unsigned int nextR = 0;
    if (nextRow & (1 << 0)) {
        nextR |= R0;
    }
    if (nextRow & (1 << 1)) {
        nextR |= R1;
    }
    if (nextRow & (1 << 2)) {
        nextR |= R2;
    }
    if (nextRow & (1 << 3)) {
        nextR |= R3;
    }
    if (nextRow & (1 << 4)) {
        nextR |= R4;
    }
    if (nextRow & (1 << 5)) {
        nextR |= R5;
    }
    if (nextRow & (1 << 6)) {
        nextR |= R6;
    }
    if (nextRow & (1 << 7)) {
        nextR |= R7;
    }
    return nextR;
}

unsigned char currA(unsigned int headColumn) {
    unsigned char currA = 0x0;
    if (headColumn & A0) {
        currA |= (1 << 0);
    }
    if (headColumn & A1) {
        currA |= (1 << 1);
    }
    if (headColumn & A2) {
        currA |= (1 << 2);
    }
    if (headColumn & A3) {
        currA |= (1 << 3);
    }
    return currA;
}

unsigned int nextA(unsigned char nextColumn) {
    int nextA = 0;
    if (nextColumn & (1 << 0)) {
        nextA |= A0;
    }
    if (nextColumn & (1 << 1)) {
        nextA |= A1;
    }
    if (nextColumn & (1 << 2)) {
        nextA |= A2;
    }
    if (nextColumn & (1 << 3)) {
        nextA |= A3;
    }
    return nextA;

}

// Update snake position
void Update() {
    unsigned char headR = currR(snake.head);
    unsigned char headA = currA(snake.head);
    unsigned int new_headR;
    unsigned int new_headA;
    snake.body3 = snake.body2;
    snake.body2 = snake.body1;
    snake.body1 = snake.head;
    switch (snake.direction) {
        case UP:
            new_headR = nextR(headR >> 1);
            if (new_headR == 0) {
                new_headR = nextR(1 << 7);
            }
            new_headA = nextA(headA);
            break;
        case RIGHT:
            new_headR = nextR(headR);
            new_headA = nextA(headA + 1);
            break;
        case DOWN:
            new_headR = nextR(headR << 1);
            if (new_headR == 0) {
                new_headR = nextR(1 << 0);
            }
            new_headA = nextA(headA);
            break;
        case LEFT:
            new_headR = nextR(headR);
            new_headA = nextA(headA - 1);
            break;
    }
    snake.head = new_headR | new_headA;
}

// Interruption for buttons
void PORTE_IRQHandler() {
    if (GPIOE_PDIR & pushed == 0) {
        PORTE->ISFR = pushed;
        return;
    }

    if (GPIOE->PDIR & BTN_SW2) {
        switch (snake.direction) {
            case UP:
                snake.direction = LEFT;
                break;
            case RIGHT:
                snake.direction = UP;
                break;
            case DOWN:
                snake.direction = RIGHT;
                break;
            case LEFT:
                snake.direction = DOWN;
                break;
        }
    }
    else if (GPIOE->PDIR & BTN_SW4) {
        switch (snake.direction) {
            case UP:
                snake.direction = RIGHT;
                break;
            case RIGHT:
                snake.direction = DOWN;
                break;
            case DOWN:
                snake.direction = LEFT;
                break;
            case LEFT:
                snake.direction = UP;
                break;
        }
    }

    PORTE->ISFR = pushed;
}

// Interruption for timer
void PIT0_IRQHandler() {
    PIT->CHANNEL[0].TFLG = 0x1;
    if (TIMER(time)) {
        Update();
    }
    int segment  = time % 4;
    PTE->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(28));
    if (segment  == 0) {
       PTA->PDOR = snake.head ;
    } else if (segment  == 1) {
        PTA->PDOR = snake.body1 ;
    } else if (segment  == 2) {
        PTA->PDOR = snake.body2 ;
    } else if (segment  == 3) {
        PTA->PDOR = snake.body3 ;
    }
    PTE->PDOR &= 0xFFFFFFFF;
    time++;
}

void GameConfig() {
    // SNAKE
    snake.body3 =  R4 | 0;
    snake.body2 = R4 | 1;
    snake.body1 = R4 | A0;
    snake.head =  R4 | A1;
    snake.direction = RIGHT;

    // BUTTONS
    PORTE->PCR[10] = PORT_PCR_MUX(0x01);
    PORTE->PCR[27] = PORT_PCR_MUX(0x01);
    PORTE->PCR[10] |= PORT_PCR_IRQC(BTN_IRQC);
    PORTE->PCR[27] |= PORT_PCR_IRQC(BTN_IRQC);
    PTE->PDDR = GPIO_PDDR_PDD(GPIO_PIN(28));
    NVIC_EnableIRQ(PORTE_IRQn);
    NVIC_SetPriority(PORTE_IRQn, 0);

    // TIMER
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
    PIT->MCR &= ~PIT_MCR_MDIS_MASK;
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
    PIT->CHANNEL[0].LDVAL = 0x5fff;
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
    NVIC_EnableIRQ(PIT0_IRQn);
    NVIC_SetPriority(PIT0_IRQn, 1);
}

int main()
{
    SystemConfig();
    GameConfig();
    for (;;) {
    }
    return 0;
}

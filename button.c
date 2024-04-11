/*
 * button.c
 *
 *  Created on: Apr 11, 2024
 *      Author: iot00
 */
#include <stdbool.h>
#include <stdio.h>
#include "io.h"
#include "button.h"

// 입력이 들어오는 버튼의 핀 번호로 interrupt를 확인
#define D_USER_BTN_EXTI_NO                13

static bool flag;                                                        // 버튼이 눌린지 여부
static uint8_t edge_rising_or_falling;                // 버튼이 눌렸는지 떨어졌는지를 확인하기 위한 변수
static uint16_t button_no;                                        // 눌린 버튼이 몇번째인지 확인하기 위한 변수
static uint32_t prev_tick;                                        // 버튼이 너무 짧은 시간에 여러번 눌려서 채터링 발생을 방지하기 위한 이전 틱 저장


// user button이 있는 13번 핀의 콜백 함수
static void button_callback_13(uint8_t rf, void *arg);

// 버튼 스레드 초기화&콜백함수 등록
void button_init(void)
{
        // 처음 초기 시간 값 할당
        prev_tick = HAL_GetTick();
        // 버튼 콜백 함수를 External 인터럽트 방식으로 등록
        io_exti_regcbf(D_USER_BTN_EXTI_NO, button_callback_13);
}

// 버튼이 눌렸다고 플래그가 바뀌었을때
// 눌렸는지 떨어졌는지와 버튼이 저장된 번호를 출력하는 함수
void button_thread(void *arg)
{
        if (flag == true) {
                flag = false;
                if (button_no == D_USER_BTN_EXTI_NO) {
                        printf("rf:%d, no:%d\r\n", edge_rising_or_falling,  button_no);
                }
        }
}

// 버튼이 눌려 interrupt가 발생했을때 호출되는 함수
static void button_callback_13(uint8_t rf, void *arg)
{
        volatile uint32_t curr_tick = HAL_GetTick();

        // 이전에 버튼이 눌렸던 시간과 현재시간을 비교해서 일정시간이 지났다면
        //                        (왜? LED 충전 시간 지연 때문이었는데 LED 관련 코드가 없어짐)
        if (curr_tick - prev_tick > 120) {
                prev_tick = curr_tick;
                // 변경된 비트 값으로 눌렸는지 떨어졌는지를 설정함
                edge_rising_or_falling = rf;
                // 변경된 핀의 번호로 버튼 번호를 설정함
                button_no = *(uint16_t *)arg;
                // 버튼이 눌렸다는 플래그를 true로 바꿈
                flag = true;
        }
}

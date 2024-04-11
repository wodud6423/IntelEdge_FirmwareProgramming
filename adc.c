/*
 * adc.c
 *
 *  Created on: Apr 9, 2024
 *      Author: iot00
 */

#include <stdio.h>
#include "adc.h"

// 오류가 안나게 하기 위한 더미 함수
static void adc_dummy(void *);

// 숫자를 저장하는 구조체 배열 선언
// 실제로는 다양한 adc기능들을 담게 될 adc리스트
static ADC_T gAdcObjs[] = {
        { .cbf = adc_dummy         },
        { .cbf = NULL                        }
};

// adc 초기화 함수(아직은 구현x)
void adc_init(void)
{
}

// adc의 call back function을 등록하기 위함!
void adc_regcbf(uint16_t idx, ADC_CBF cbf)
{
        gAdcObjs[idx].cbf = cbf;
}

// adc에 있는 값을 갱신해주는 함수
void adc_thread(void *arg)
{
        (void)arg;

        static uint16_t value = 100;

        for (int i=0; gAdcObjs[i].cbf != NULL; i++) {
                value+=100;
                gAdcObjs[i].value = value;
                gAdcObjs[i].cbf((void *)&gAdcObjs[i].value);
                // 콜백함수의 인자로 바뀐 값을 인자로 넣어주는데, 이는 상위의 함수에
                // 해당 값이 Update된것을 알리기 위함이다. (왜냐하면 상위의 함수들은 얘의 값이 업데이트 됬는지 모르니까~)
        }
}

//--------------
// adc_hw()
// 오류를 막기 위한 dummy 정의부
static void adc_dummy(void *)
{
        printf("I'm ADC dummy\r\n");
        return;
}

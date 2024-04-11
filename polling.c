/*
 * pooling_thread.c
 *
 *  Created on: Apr 11, 2024
 *      Author: iot00
 */


// polling이라는 현재 층이 존재하는 이유는
// 갖가지 thread의 실행 타이밍을 모아서 한번에 조작하기 위함


// 따라서 원래라면 버튼 또한 이곳으로 편입되어야했으나
// 아까 설명하시기를 더 헷갈릴까봐 아직 이쪽에 포함하지 않으셨다고함.


#include <stdio.h>
#include <stdbool.h>
#include "adc.h"
#include "polling.h"

// adc콜백 함수 선언부
static void adc_callback(void *arg);

// 스레드 구조체
typedef struct {
        uint32_t period, count; // 주기, 타이머(Polling방식을 위한 변수)
        bool flag; // 스레드 작업분할을 위한 플래그(동시 작업을 방지하기위한 변수로 봐도 무방하다)
        void (*cbf)(void *); // 함수인자를 포인터형식으로 받는 콜백함수
} THR_T; // thread type

// 스레드(polling방식) 리스트
volatile THR_T gThrObjs[]  = {
        // adc의 실행 주기를 500ms, 콜백함수를 설정
        { .period = 500,         .cbf = adc_thread   // adc스레드의 실행주기: 500ms | 실행 콜백함수: adc 스레드            },
        { .cbf = NULL                        }
};

// adc 스레드(작업)을 초기화&콜백함수를 등록해주는 polling 초기화 함수
void polling_init(void)
{
        adc_init();
        // adc의 콜백함수를 등록
        adc_regcbf(0, adc_callback);
}


// 해당 레이어에 포함되어 있는 요소들의 flag를 확인하여 처리할 시기가 된 함수를 호출
void polling_thread(void *arg)
{
        static uint16_t thr_idx = 0;

        // 여러 작업을 한번에 모아 처리하는 특성상
        // app단에 바로 올라가 있는 uart나 button에 비해 우선 순위가 낮은 것으로 보임
        //                (왜? 배열에 들어가 있는 것 중 호출 1회당 thr_idx거 하나만 보고 나가기 때문에)
        // 더구나 당연히 인터럽트 방식에 비해서 느리기 때문에 
        // 인터럽트 방식의 스레드가 발생하였을때, 해당 작업이 문맥교환으로 밀려날수 있음
  
        if (gThrObjs[thr_idx].flag == true) { //플래그가 true인 thr_idx의 작업을 처리
                gThrObjs[thr_idx].flag = false;
                gThrObjs[thr_idx].cbf(NULL); // 해당 작업의 콜백함수로 해당 작업에 설정된 함수를 실행
        }
        thr_idx++; // "자 다음 스레드~"
        if (gThrObjs[thr_idx].cbf == NULL) thr_idx = 0; // 만약 스레드리스트의 마지막인 콜백함수가 NULL인 스레드에 도달할 경우 -> 다시 처음 스레드부터!
}

// adc의 콜백함수 정의부
static void adc_callback(void *arg)
{
        printf("adc value = %d\r\n", *(uint16_t *)arg); // printf문이기에 처리속도는 매우 느리다(라이브러리호출해야 하므로)
}


// io.c 파일 HAL_IncTick() 함수에서 호출
// HAL_IncTick() 함수는 systick irq handler에서 호출됨(인터럽트 서비스 루틴임)
// 1ms 마다 호출됨
// 폴링에 포함되어있는 thread들의 count를 올리고 주기가 되었다면 플래그를 참값으로 함
// 이는 다른 polling이외의 thread들을 실행하기 때문에 CPU의 시간주기인 TICK(1ms)단위로 
// count를 능동적으로 높여주기 위함이다.
void polling_update(void)
{
        for (int i=0; gThrObjs[i].cbf != NULL; i++) {
                gThrObjs[i].count++; // 1ms지났다~
                gThrObjs[i].count %= gThrObjs[i].period; // 1ms마다 스레드들 주기체크
                if (gThrObjs[i].count == 0) gThrObjs[i].flag = true; // 해당 스레드 주기가 됬다?->해당 스레드 작업준비완료!
        }
}

/*
 * app.c
 *
 *  Created on: Apr 11, 2024
 *      Author: iot00
 */
#include <stdio.h>
#include "io.h"
#include "polling.h"
#include "button.h"
#include "app.h"
#include "uart.h"

/*
///////상위계층 함수////////
app.c는 가장 상위의 함수로 하위의 구현된 함수들을 이용하여 어플리케이션을 
구현하는 계층의 함수라고 볼수 있다. 
해당 함수에서는 3가지 방식의 구동방식을 각각의 스레드를 작업하는 형식으로 구성되있다.
1. ADC(Analog to Digital) 기능 -> Polling방식
- 가장 원시적인 방식으로 CPU가 지속적으로 해당 스레드의 Data 레지스터를 읽어
입력이 들어왔는지 확인하는 방법이다.
- CPU가 직접 해당 작업을 주기적으로 확인해야하기 때문에 인터럽트 방식에 비해서 응답속도도 느리고
지속적인 문맥교환(Context Change)으로 전체적인 처리속도를 늦춘다.

2. Button(GPIO) 기능 -> 인터럽트 방식
- Polling이 지속적인 CPU할당이 필요했다면 Interrupt는 인터럽트 벡터 테이블을 이용한
능동적인 CPU할당으로 Polling보다는 응답,처리속도가 빠르고 안정적이다.
- Interrupt 벡터 테이블에 우선순위를 주어 능동적인 순차적 인터럽트 처리가 가능하며,
설령 인터럽트가 Maskable한 구역(임계구역등)을 두었다할지라도 Nonmaskable 인터럽트 설정이 가능하다.

3. Uart(통신) 기능 -> DMA 방식
- Uart의 수신은 CPU가 인터럽트로 처리하기도(1Byte마다 발생하기에 이는 비효율적 이게된다.)그렇다고
폴링으로 처리하기도 애매하다. 따라서 CPU가 지속적으로 인터럽트를 받기보다는 DMA컨트롤러가 처리를 하되
절반 전송과 전체 전송시에만 CPU에 인터럽트를 보내는 방식인 DMA방식을 채택한다.
- DMA(Direct Memorry Access)는 원래 지속적인 CPU의 작업부담을 덜어주기위해서
각 CPU이외의 Peripheral 노드들이 직접 DMA컨트롤러를 통해 빠르게 접근할 수 있도록 
하는 기능이다. 
- STM32에서 살펴보면 DMA는 비슷하게 구동하는데, DMA컨트롤러에 할당된 스레드(작업)은
보통은 Periphral - DMA - Ram 방식으로 처리되다가 절반처리(전송),전부처리시에만 CPU에게
"야 나 이 정도 작업을 끝냈어!"하며 인터럽트를 보내게 된다.

*/
// app_init선언부
void app_init(void);

void app(void)
{
        printf("System Started.....!\r\n");
// 작업을 수행하기전에 모든 Periphral들의 입력및 상태를 초기화해준다!(반드시 필요!)
        app_init();
// 해당 프로그램은 Super Single Loop로 구현된 프로그램으로 지속적으로 아래의
// 스레드들이 발생되도록 구현되어있다.

	
        while (1) {
                // 이곳에 adc가 없는 이유는 polling의 하단으로 포함되어 숨었기 때문

                polling_thread(NULL);
                button_thread(NULL);
                uart_thread(NULL);
        }
}

// app 작업들을 초기화 해주는 app_init정의부
void app_init(void)
{
        io_exti_init();
        polling_init();
        button_init();
        uart_init();
}

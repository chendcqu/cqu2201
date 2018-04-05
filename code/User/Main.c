/****************************************Copyright (c)****************************************************
**                                ������6lowpan���߿���ƽ̨
**                                  һ��Ϊ��ԴӦ�ö���
**
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:           Main.c
** Last modified Date:  2012-06-21
** Last Version:        v1.0
** Description:         contiki����������ģ��
**
**--------------------------------------------------------------------------------------------------------
** Created By:          �κ���
** Created date:        2012-06-21
** Version:             v1.0
** Descriptions:        The original version ��ʼ�汾
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*********************************************************************************************************/
#include "includes.h"

/*********************************************************************************************************
**  ȫ�ֱ�������
*********************************************************************************************************/
#define DEBUG 0
#if DEBUG
#include "runtime/uartstdio.h"
#define PRINTF(...)   uart_printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

/*********************************************************************************************************
**  �ⲿ��������
*********************************************************************************************************/
PROCESS_NAME(udp_client_process);
#define PAN_ID        0x1420//0x2007  //  PAN_ID��
#define PAN_L        (PAN_ID & 0x00FF)       //  PAN_ID��8λ
#define PAN_H        ((PAN_ID & 0xFF00) >> 8)        //  PAN_ID��8λ
#define ADD_SRC        0xAABB
#define ADD_SRC_L    (ADD_SRC & 0x00FF)
#define ADD_SRC_H    ((ADD_SRC & 0xFF00) >> 8)
#define ADD_DEST         0x0203
#define ADD_DEST_L    (ADD_DEST & 0x00FF)
#define ADD_DEST_H  ((ADD_DEST & 0xFF00) >> 8)

uint8_t ieee_addr_64[8]={0,0,0,0,0,0,0,0};

static uint8_t txbuf[23] = { 0x61, 0x88, 0, PAN_L, PAN_H,0xff, 0xff, ADD_SRC_L, ADD_SRC_H,
0, 'e', 'l', 'l', 'o', ' ', '8', '0', '2', '.', '1', '5', '.', '5'};

/*********************************************************************************************************
**  �ڲ���������
*********************************************************************************************************/
PROCESS(led_process, "Blink1");
PROCESS_THREAD(led_process, ev, data)
{
  PROCESS_BEGIN();
  sys_led_toggle(1);

  while(1)
  {
    static struct etimer et;

    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    NETSTACK_RADIO.send(txbuf,23);
    //sys_led_toggle(1);
    PRINTF("The moteid is %d \r\n",get_moteid());
  }
   PROCESS_END();
}

/*********************************************************************************************************
**  ���������߳�
*********************************************************************************************************/
AUTOSTART_PROCESSES(&led_process);

/*********************************************************************************************************
**  ����ϵͳ��Ҫ�Ķ���
*********************************************************************************************************/
PROCINIT(NULL);


/*********************************************************************************************************
** Function name:       main
** Descriptions:        ��������ģ�壬һ�㲻Ҫ�������
** input parameters:    ��
** output parameters:   ��
** Returned value:      0
** Created by:          �κ���
** Created Date:        2012-06-21
*********************************************************************************************************/
int main (void)
{
    // ��ʼ��ϵͳ����
    sys_services_init();

    // ��ʼ����������
    __GtSpiBusDriver.spi_bus_init(&__GtSpiBusDriver);

    // ��ʼ������̨
    uart_stdio_init(115200);
    moteid_init();

    PRINTF("The Contiki System Start!\r\n");

    clock_init();
    rtimer_init();
    process_init();

    random_init(0);

    procinit_init();
    process_start(&etimer_process, NULL);
    ctimer_init();
    // ��ʼ������Э��ջ
    //contiki_net_init();
    userinit();
    energest_init();
    ENERGEST_ON(ENERGEST_TYPE_CPU);

    autostart_start(autostart_processes);
   // watchdog_init();
   // watchdog_start();
    process_start(&udp_client_process, NULL);
    for(;;)
    {
        do
        {
            // ι��
            // watchdog_periodic();
        } while(process_run() > 0);
      // contiki_net_handle();
        //contiki_power_manager();

        // Stop processor clock
         //asm("wfi"::);
    }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

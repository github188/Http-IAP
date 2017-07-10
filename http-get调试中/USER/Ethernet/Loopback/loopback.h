
#ifndef  __LOOPBACK_H_
#define  __LOOPBACK_H_

#include "stm32f4xx.h"	

#define DATA_BUF_SIZE  		 2048//w5500����16k�Ľ��ջ��棬��������ÿ��socket�ֵ�2k����2048byte

#define RECV_MAX_DATALEN   2048//ÿ��socketƽ������2K����������

#define CLIENT_MAXNUM      4//����ͻ�������


//������Ҫ���͵�������
#define		SEND_DATA_LEN				8+1			//����:4��ADֵ+2����������ֵ(us)+1��ͨ����ʶ
//#define		AD_SAMPLIING_NUM		500			//ad����Ƶ��
#define		CLIENT_NUM					1				//�ͻ�������


//server
extern volatile uint8_t tcp_server_recvbuf[];   	//�������ݻ���
extern volatile uint16_t tcp_server_recv_len;		//���ͻ��泤��
extern volatile uint8_t tcp_server_sendbuf[];   	//�������ݻ���	
extern volatile uint16_t tcp_server_send_len;		//���ͻ��泤��
extern volatile uint8_t tcp_server_sta[CLIENT_MAXNUM];//�ͻ���״̬












int32_t loopback_tcps(uint8_t, uint8_t*, uint16_t);
int32_t loopback_udps(uint8_t, uint8_t*, uint16_t);
int32_t loopback_server(uint8_t, uint8_t*, uint16_t);
int32_t loopback_client(uint8_t sn, uint8_t* buf,uint8_t *ip, uint16_t port);




#endif   // _WIZCHIP_LOOPBACK_H_

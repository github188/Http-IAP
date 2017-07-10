
#include "loopback.h"
#include "../../Ethernet/socket.h"
//add
#include "package.h"
#include "lcd.h"
#include "string.h"
#include "delay.h"

extern unsigned char httpreq[200];
//
extern uint16_t server_port[ CLIENT_MAXNUM];
extern wiz_NetInfo gWIZNETINFO;
//uint8_t serverip[4] = {192, 168, 1, 111}; ///< Source IP Address
//uint8_t serverip[4] = {192, 168, 1, 182}; ///< Source IP Address
uint8_t serverip[4] = {115, 159, 180, 213}; ///< Source IP Address

uint16_t localport = 6000;

//server
volatile uint8_t tcp_server_recvbuf[1024]={0};   	//�������ݻ���
volatile uint16_t tcp_server_recv_len;		//���ͻ��泤��
volatile uint8_t tcp_server_sendbuf[1024]={0};   	//�������ݻ���
volatile uint16_t tcp_server_send_len;		//���ͻ��泤��
volatile uint8_t tcp_server_sta[CLIENT_MAXNUM];				//�ͻ���״̬

unsigned char tempused = 0;
//TCP server backup
//int32_t ret;
int32_t loopback_server(uint8_t sn, uint8_t* buf, uint16_t port)
{
    int32_t ret;
    switch(tempused = getSn_SR(sn))	//��ȡsocket��״̬
    {
        //socket��ʼ�����
    case SOCK_INIT:
        connect(sn, serverip, port);	//����TCP����˳���
        break;
        //socket���ӽ���
    case SOCK_ESTABLISHED:
        //״̬��Ϊ���Ӻ��ж������жϱ�־�Ƿ�����֤�����ӳɹ���������жϱ�־λ<д��λ>
        //�����һ�����ӽ������Ͽ���ִֻ��һ��
        if(getSn_IR(sn) & Sn_IR_CON)
        {
            setSn_IR(sn, Sn_IR_CON); //д��λ������жϱ�־λ
            tcp_server_sta[sn] |= 1 << 7;		//��־���ӳɹ�
        }
        ret = getSn_RX_RSR(0);										// len=Socket0���ջ������ѽ��պͱ�������ݴ�С
        if(ret > 0)
        {
					delay_ms(50);//�ȴ����ݽ�����ϣ��������http��Ӧ����ȱʧ
					if((tcp_server_sta[sn]&(1<<6))==0)//û��δ�������ݣ������������
            {
								ret = getSn_RX_RSR(0);				
								tcp_server_recv_len=ret;
                recv(0, (uint8_t*)tcp_server_recvbuf, tcp_server_recv_len);										// W5500�������Է����������ݣ���ͨ��SPI���͸�MCU
                tcp_server_sta[sn] |= 1 << 6; //��ʾ�յ��ͻ�������
            }
        }
				if(tcp_server_sta[sn]&(1<<5))//��������Ҫ����
				{
						//���������������
						ret=send(sn,(uint8_t*)tcp_server_sendbuf,tcp_server_send_len);
						if( ret<tcp_server_send_len)
						{
								//���ʹ���
								break;
						}
						tcp_server_sta[sn]&=~(1<<5);//������
				}
        break;
        //socket�ȴ��ر�״̬
    case SOCK_CLOSE_WAIT:
        close(sn);																// �ر�Socket0
        tcp_server_sta[sn] &= ~(1 << 7);	//��־û������
        break;
        //socket�ر�
    case SOCK_CLOSED:
        tcp_server_sta[sn] &= ~(1 << 7);	//��־û������
        socket(0, Sn_MR_TCP, localport, Sn_MR_ND);		// ��Socket0��������ΪTCP����ʱģʽ����һ�����ض˿�
        break;
    }
    return 0;
}






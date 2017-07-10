
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
volatile uint8_t tcp_server_recvbuf[1024]={0};   	//发送数据缓存
volatile uint16_t tcp_server_recv_len;		//发送缓存长度
volatile uint8_t tcp_server_sendbuf[1024]={0};   	//发送数据缓存
volatile uint16_t tcp_server_send_len;		//发送缓存长度
volatile uint8_t tcp_server_sta[CLIENT_MAXNUM];				//客户端状态

unsigned char tempused = 0;
//TCP server backup
//int32_t ret;
int32_t loopback_server(uint8_t sn, uint8_t* buf, uint16_t port)
{
    int32_t ret;
    switch(tempused = getSn_SR(sn))	//获取socket的状态
    {
        //socket初始化完成
    case SOCK_INIT:
        connect(sn, serverip, port);	//连接TCP服务端程序
        break;
        //socket连接建立
    case SOCK_ESTABLISHED:
        //状态变为连接后，判断连接中断标志是否置起，证明连接成功！并清除中断标志位<写该位>
        //这里从一次连接建立到断开，只执行一次
        if(getSn_IR(sn) & Sn_IR_CON)
        {
            setSn_IR(sn, Sn_IR_CON); //写该位，清除中断标志位
            tcp_server_sta[sn] |= 1 << 7;		//标志连接成功
        }
        ret = getSn_RX_RSR(0);										// len=Socket0接收缓存中已接收和保存的数据大小
        if(ret > 0)
        {
					delay_ms(50);//等待数据接收完毕，否则出现http响应数据缺失
					if((tcp_server_sta[sn]&(1<<6))==0)//没有未处理数据，则接收新数据
            {
								ret = getSn_RX_RSR(0);				
								tcp_server_recv_len=ret;
                recv(0, (uint8_t*)tcp_server_recvbuf, tcp_server_recv_len);										// W5500接收来自服务器的数据，并通过SPI发送给MCU
                tcp_server_sta[sn] |= 1 << 6; //表示收到客户端数据
            }
        }
				if(tcp_server_sta[sn]&(1<<5))//有数据需要发送
				{
						//想服务器发送数据
						ret=send(sn,(uint8_t*)tcp_server_sendbuf,tcp_server_send_len);
						if( ret<tcp_server_send_len)
						{
								//发送错误
								break;
						}
						tcp_server_sta[sn]&=~(1<<5);//清除标记
				}
        break;
        //socket等待关闭状态
    case SOCK_CLOSE_WAIT:
        close(sn);																// 关闭Socket0
        tcp_server_sta[sn] &= ~(1 << 7);	//标志没有连接
        break;
        //socket关闭
    case SOCK_CLOSED:
        tcp_server_sta[sn] &= ~(1 << 7);	//标志没有连接
        socket(0, Sn_MR_TCP, localport, Sn_MR_ND);		// 打开Socket0，并配置为TCP无延时模式，打开一个本地端口
        break;
    }
    return 0;
}






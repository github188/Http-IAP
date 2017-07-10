#ifndef __FUN_H__
#define	__FUN_H__

#include <stdint.h>
#include "string.h"//���ڶ�ʱ��Ƶ�ʲ���
#include "loopback.h"//���ڶ�ʱ��Ƶ�ʲ���
#include "delay.h"//���ڶ�ʱ��Ƶ�ʲ���
#include "CRC.h"//���ڶ�ʱ��Ƶ�ʲ���


//����ģʽ
#define		DEBUG		1

//ÿ��http�����ȡ���ֽ���
#define		BYTESPERREQ		1000

//bin�ļ�info��Ϣ�ֽ���
#define		BINFILESIZE		10


//http��Ӧ�ȴ�ʱ�䣬��λ����
#define		HTTPRESPWAITTIME		50	//100ms

//http������󳤶�
#define		HTTPSENDLENVMAX		500

//http������󳤶�
#define		HTTPRECLENVMAX		500

extern int HttpReqGetData(char* data, uint16_t start,uint16_t end);
extern int HttpDataResUnpack(char* buf1, uint16_t* statr, uint16_t* end, unsigned char* data);
extern int HttpReqGetInfo(char* data);
extern void ibyte2cbyte(char* Des, unsigned char* Res, uint16_t len);
extern void cbyte2ibyte(unsigned char* Des, char* Res, uint16_t len);
extern void ibyte2ibyte(unsigned char* Des, unsigned char* Res, uint16_t len);


extern void HttpDataSend();

extern unsigned int UpdateFileDataGet(unsigned char* UpdateBin);

extern void HttpSendData(char* send_buf,uint16_t WaitTime);


extern unsigned char httpreq[];

extern int HttpInfoResUnpack(char* buf1,unsigned char* data);






#endif // !__FUN_H__

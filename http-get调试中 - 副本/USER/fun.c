

#include "fun.h"//���ڶ�ʱ��Ƶ�ʲ���
#include "lcd.h"//���ڶ�ʱ��Ƶ�ʲ���
#include "stmflash.h"//���ڶ�ʱ��Ƶ�ʲ���



/*
*���ܣ�����http���󣬶�ȡ����
*���룺data����������������
start������������ʼ�ֽ�
end���������ݽ����ֽ�
*���أ�0:�ɹ�
-1:ʧ��
*/
int HttpReqGetData(char* data,uint16_t start, uint16_t end)
{
	char istart[100] = "";
	char iend[100] = "";
	char temp[100] = "";
	int i = 0, j = 0;
	char buf1[200] = {
		"GET /HFS%20TEMP/update.bin HTTP/1.1\r\n"
		"Connection: Keep-Alive\r\n"
	};
	char buf2[100] = "Range: bytes=";
	char *p = strstr(buf1, buf2);
	//
	i = 0;
	do
	{
		temp[i++] = (start % 10) + '0';
		start /= 10;
	} while (start > 0);
	for (j = 0; j < strlen(temp); j++)
	{
		istart[j] = temp[i - j - 1];
	}
	//
	i = 0;
	do
	{
		temp[i++] = (end % 10) + '0';
		end /= 10;
	} while (end>0);
	for (j = 0; j < strlen(temp); j++)
	{
		iend[j] = temp[i - j - 1];
	}
	//
	/*printf("\n %d %s \n", strlen(istart), istart);
	printf("\n %d %s \n", strlen(iend), iend);*/
	//
	strcat(buf2, istart);
	strcat(buf2, "-");
	strcat(buf2, iend);
	strcat(buf2, "\r\n");
	//
	strcat(buf2, "\r\n");
	strcat(buf1, buf2);
	//
	strcpy(data, buf1);
	//printf("\n %d %s \n", strlen(data), data);

	return 0;
}


/*
*���ܣ�����http���󣬶�ȡ��Ϣ
*���룺data����������������
start������������ʼ�ֽ�
end���������ݽ����ֽ�
*���أ�0:�ɹ�
-1:ʧ��
update.txt:
byte[0-1] version
byte[2-7] filesize 0-1Mbyte
byte[8-9] crc
*/
int HttpReqGetInfo(char* data)
{
	char buf1[200] = {
		"GET /HFS%20TEMP/info.bin HTTP/1.1\r\n"
		"Connection: Keep-Alive\r\n"
		"\r\n"
	};
	strcpy(data, buf1);

#if DEBUG
	//printf("\n %d %s \n", strlen(data), data);
	//while (true);
#endif

	return 0;
}




/*
*���ܣ���http��Ӧ����ȡ����Ч����
*���룺buf1-��Ӧ����
start����Ӧ������ʼ�ֽ�
end����Ӧ���ݽ����ֽ�
data����Ӧ����
*���أ�0:ok
-1:���Ȳ���
-2:��λʧ��
*/
int HttpDataResUnpack(char* buf1, uint16_t* start, uint16_t* end, unsigned char* data)
{
	int ret_sta = 0;
	char sstart[200] = "";
	char send[200] = "";//��س�ʼ����ֵ
	//char data[200] = "";
	int istart = 0, iend = 0;
	int i = 0, jj = 0;
	const char buf2[] = { "Content-Range: bytes " };
	const char buf3[] = "\r\n\r\n";

	char tt;
	//��λ
	char *p = strstr(buf1, buf2);
	char *pp = p;
	if (pp != NULL)//find success
	{
		//��λ
		pp += strlen(buf2);
		//find out char
		i = 0;
		while ((tt = *(pp++)) != '-')
		{
			sstart[i++] = tt;
		}
		i = 0;
		while ((tt = *(pp++)) != '/')
		{
			send[i++] = tt;
		}
		//char to int
		for (int j = 0; j < strlen(sstart); j++)
		{
			istart *= 10;
			istart += sstart[j] - '0';
		}*start = istart;
		for (int j = 0; j < strlen(send); j++)
		{
			iend *= 10;
			iend += send[j] - '0';
		}*end = iend;
		//��λ
		p = strstr(buf1, buf3);
		pp = p;
		pp += strlen(buf3);
		//
		i = 0;
		for (i = 0; i < (iend - istart + 1); i++)
		{
			data[i] = *(pp);
			//
			if (*(pp)<0)
			{
				data[i] += 256;
			}
			pp++;
		}

#if DEBUG
//		if (strlen(data) != (istart - iend))	return -1;
#endif // DEBUG

#if DEBUG
		//printf("\n %d %s \n", i, data);
#endif
		//
	}
	else
	{
		return -2;
	}


	return ret_sta;
}


//
//
//
int HttpInfoResUnpack(char* buf1,unsigned char* data)
{
	int ret_sta = 0;
	//char data[200] = "";
	int istart = 0, iend = 0;
	int i = 0, jj = 0;
	const char buf3[] = "\r\n\r\n";
	char *p; 
	char *pp;
	char tt;
	//��λ
	p = strstr(buf1, buf3);
	pp = p;
	pp += strlen(buf3);
	//
	i = 0;
	for (i = 0; i < 10; i++)
	{
		data[i] = *(pp);
		//
		if (*(pp)<0)
		{
			data[i] += 256;
		}
		pp++;
	}

	return ret_sta;
}



void ibyte2cbyte(char* Des, unsigned char* Res, uint16_t len)
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		Des[i] = Res[i];
	}
}
//
void cbyte2ibyte(unsigned char* Des, char* Res, uint16_t len)
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		Des[i] = Res[i];
		//char to unsigned intʱ��ע���������
		if (Res[i]<0)
		{
			Des[i] += 256;
		}
		//printf("\n %d \n", Des[i]);
	}
	//while (true);
}

//
void ibyte2ibyte(unsigned char* Des, unsigned char* Res, uint16_t len)
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		Des[i] = Res[i];
	}
}


extern uint8_t infoint[];
extern uint8_t tempint[];
extern uint8_t tempint2[];

//
void HttpSendData(char* send_buf,uint16_t WaitTime)
{
	tcp_server_send_len = strlen((const char*)send_buf);
	memcpy((uint8_t*)tcp_server_sendbuf,send_buf,tcp_server_send_len);
	tcp_server_sta[0] |= (1<<5);
	do
	{
			loopback_server( 0,NULL,502);//as tcp server
			delay_ms( 100);
			WaitTime--;
	} while(((tcp_server_sta[0]&(1<<6))==0) && (WaitTime != 0));
  tcp_server_sta[0]&=(~(1<<6));
}


extern u32 WriteAddr;//����app������ʼ��ַ
//
unsigned int UpdateFileDataGet(unsigned char* UpdateBin)
{
	uint8_t data[BYTESPERREQ];
	uint16_t istart = 0, iend = 0;
	int i = 0, ii, k;//��ʱϵ��;
	int ret = 0;
	uint16_t WaitTime = 10;
	char send_buf[HTTPSENDLENVMAX];
	uint8_t UpdateInfo[100];
	//
	uint16_t req_times = 0, req_size = 0;
	//
	unsigned int FileSize = 0, FileCRC = 0;

	//
	unsigned int offset1;
	unsigned int offset2;

	//
	unsigned int HttpRecvLen;

	//�������ڽ���һ��Update.bin�ļ���ȡ
	/******************************************************************/
	/*****************************info part*******************************/
	HttpReqGetInfo(send_buf);//��ȡ�ļ���Ϣ���汾����С��
	WaitTime = HTTPRESPWAITTIME;
	//send data to HFS
	HttpSendData(send_buf,WaitTime);
	//
	if (WaitTime > 0)//reqponse ok
	{
		HttpRecvLen = tcp_server_recv_len;
		ret = HttpInfoResUnpack((char*)tcp_server_recvbuf, data);
		ibyte2ibyte(UpdateInfo, data, BINFILESIZE);
		k = 1;//��ʱϵ��
		for (ii = 0; ii < 6; ii++)//convert filesize
		{
			FileSize += UpdateInfo[7 - ii] * k;
			k *= 256;
		}
		FileCRC = UpdateInfo[8] * 256 + UpdateInfo[9];
	}
	else
	{
		//response error
		//i--;//retry
	}
	/******************************************************************/
	/******************************************************************/


	/******************************************************************/
	/*****************************data part*******************************/
	//�����req_times	req_size
	//int FileSize = 20;//�ļ���С(bytes)��Ӧ��update.txt�л�ȡ
	//�����ļ���С��ÿ�ν������޼�������������
	req_times = ((FileSize - 1) / BYTESPERREQ) + 1;
	for (i = 0; i < req_times; i++)
	{
		WaitTime = HTTPRESPWAITTIME;
		offset1 = i*BYTESPERREQ;
		offset2 = (i + 1)*BYTESPERREQ - 1;
		if ((offset2 + 1)>FileSize)
		{
			offset2 = FileSize - 1;
		}
#if DEBUG
		//printf("\n %d  %d \n", offset1, offset2);
#endif 

		ret = HttpReqGetData(send_buf, offset1, offset2);
		//send data to HFS
		HttpSendData(send_buf,WaitTime);
		//
		if (WaitTime>0)//reqponse ok
		{
			//int to char
			HttpRecvLen = tcp_server_recv_len;
			ret = HttpDataResUnpack((char*)tcp_server_recvbuf, &istart, &iend, data);
			STMFLASH_Write(WriteAddr+istart,(u8*)data,iend-istart+1);
			LCD_ShowNum(50,200,istart,6,16);
			LCD_ShowNum(200,200,iend,6,16);

//			ibyte2ibyte(UpdateBin + offset1, data, offset2 - offset1 + 1);
			
			//printf("\n %d %d \n", offset1,offset2);

#if DEBUG
			for (ii = 0; ii <offset2 + 1; ii++)
			{
//				printf("\n %d \n", UpdateBin[ii]);
			}
#endif // DEBUG
		}
		else
		{
			//response error
			//i--;//retry
		}

	}
	//
	i= CRC16(UpdateBin, FileSize);
	if (FileCRC != i)//A2F3
	{
		//�ļ�У�������
		return -1;
	}
	else
	{
		return 0;
;
	}
	//
	//WriteBin2Flash();
	//SoftWareReset();
	//normally,should`t be run to here

	/******************************************************************/
	/******************************************************************/



}












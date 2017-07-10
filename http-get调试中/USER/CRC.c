#include "CRC.h"

/*******************************************************************************
* Function Name  : crc16
* Description	 : Configures the different system clocks.
* Input 		 : *puchMsg(���ݵ�ַ)    usDataLen
* Return		 : 2 byte   
*******************************************************************************/
uint16_t CRC16(volatile uint8_t *puchMsg, uint16_t usDataLen)
{
	uint8_t uchCRCHi = 0xFF ; /* ��CRC�ֽڳ�ʼ�� */
	uint8_t uchCRCLo = 0xFF ; /* ��CRC �ֽڳ�ʼ�� */
	uint32_t uIndex ; /* CRCѭ���е����� */
	while (usDataLen--) /* ������Ϣ������ */
	{
		uIndex = uchCRCLo ^ *puchMsg++;
		uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
		uchCRCHi = auchCRCLo[uIndex];
	}
   return (uchCRCHi << 8 | uchCRCLo) ;
}

uint16_t slaveReturnCRC(uint8_t slaveAddr, uint8_t functionCode, uint8_t bytesCnt,  uint8_t *puchMsg, uint16_t usDataLen)
{
	uint8_t uchCRCHi = 0xFF ; /* ��CRC�ֽڳ�ʼ�� */
	uint8_t uchCRCLo = 0xFF ; /* ��CRC �ֽڳ�ʼ�� */
	uint32_t uIndex ; /* CRCѭ���е����� */
	uint16_t i = 0;

	uIndex = uchCRCLo ^ slaveAddr;
	uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
	uchCRCHi = auchCRCLo[uIndex];

	uIndex = uchCRCLo ^ functionCode;
	uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
	uchCRCHi = auchCRCLo[uIndex];

	uIndex = uchCRCLo ^ bytesCnt;
	uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
	uchCRCHi = auchCRCLo[uIndex];
	
	while (usDataLen--) /* ������Ϣ������ */
	{
		uIndex = uchCRCLo ^ *(puchMsg + i + 1);//��С������
		uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
		uchCRCHi = auchCRCLo[uIndex];
		usDataLen--;
		uIndex = uchCRCLo ^ *(puchMsg + i );
		uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
		uchCRCHi = auchCRCLo[uIndex];
		i = i + 2;

	}
   return (uchCRCHi << 8 | uchCRCLo) ;
}

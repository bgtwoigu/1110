#ifndef	__SD_Card_H__
#define	__SD_Card_H__


/******************************************************************************* 
* ���ļ�ΪSPI����SD���ĵײ������ļ�
* ����SPIģ�鼰���IO�ĳ�ʼ����SPI��дSD����дָ������ݵȣ�
* Wangliang Write 2010--6-26
*******************************************************************************/ 

#include "customer.h"
#include "comdef.h"

#include "msm.h"
#include "clk.h"        /* clk_busy_wait()  */
#include "hw.h"         /* MSM version info */

#ifdef FEATURE_PDA
#include "biostew.h"
#endif /* #ifdef FEATURE_PDA */

#ifdef FEATURE_SDCC_WLAN_CONFIG_API
#include "pm.h"
#endif

/******************************** Basic command set **************************/
/* Reset cards to idle state */
#define CMD0 0
/* Read the OCR (MMC mode, do not use for SD cards) */
#define CMD1 1
/* Card sends the CSD */
#define CMD9 9
/* Card sends CID */
#define CMD10 10
/* Stop a multiple block (stream) read/write operation */
#define CMD12 12
/* Get the addressed card's status register */
#define CMD13 13
/***************************** Block read commands **************************/
/* Set the block length */
#define CMD16 16
/* Read a single block */
#define CMD17 17
/* Read multiple blocks until a CMD12 */
#define CMD18 18
/***************************** Block write commands *************************/
/* Write a block of the size selected with CMD16 */
#define CMD24 24
/* Multiple block write until a CMD12 */
#define CMD25 25
/* Program the programmable bits of the CSD */
#define CMD27 27
/***************************** Write protection *****************************/
/* Set the write protection bit of the addressed group */
#define CMD28 28
/* Clear the write protection bit of the addressed group */
#define CMD29 29
/* Ask the card for the status of the write protection bits */
#define CMD30 30
/***************************** Erase commands *******************************/
/* Set the address of the first write block to be erased */
#define CMD32 32
/* Set the address of the last write block to be erased */
#define CMD33 33
/* Erase the selected write blocks */
#define CMD38 38
/***************************** Lock Card commands ***************************/
/* Commands from 42 to 54, not defined here */
/***************************** Application-specific commands ****************/
/* Flag that the next command is application-specific */
#define CMD55 55
/* General purpose I/O for application-specific commands */
#define CMD56 56
/* Read the OCR (SPI mode only) */
#define CMD58 58
/* Turn CRC on or off */
#define CMD59 59
/***************************** Application-specific commands ***************/
/* Get the SD card's status */
#define ACMD13 13
/* Get the number of written write blocks (Minus errors ) */
#define ACMD22 22
/* Set the number of write blocks to be pre-erased before writing */
#define ACMD23 23
/* Get the card's OCR (SD mode) */
#define ACMD41 41
/* Connect or disconnect the 50kOhm internal pull-up on CD/DAT[3] */
#define ACMD42 42
/* Get the SD configuration register */
#define ACMD51 42

#define SD_BLOCKSIZE 512
#define SD_BLOCKSIZE_NBITS 9

typedef enum
{
	SD_TYPE_V1,
	SD_TYPE_MMC,
	SD_TYPE_V2HC,
	SD_TYPE_V2
}SD_TYPE;

#define SD_CLK_PIN GPIO_OUTPUT_24
#define SD_CMD_PIN GPIO_OUTPUT_29
#define SD_DATA_PIN GPIO_INPUT_25
#define SD_SEL_PIN  GPIO_OUTPUT_28
/*******************************************************************************
* Function Name : SD_WriteByte
* Description    : SPIдһ���ֽڣ�������ɺ󷵻ر���ͨѶ��ȡ�����ݣ�
* Input          : u8 TxData �����͵���
* Output         : None
* Return         : none
*******************************************************************************/
void SD_WriteByte(uint8 value);

/*******************************************************************************
* Function Name : SPI_ReadWriteByte
* Description    : SPI��һ���ֽڣ�������ɺ󷵻ر���ͨѶ��ȡ�����ݣ�
* Input          : none
* Output         : None
* Return         : u8 RxData �յ�����
*******************************************************************************/
uint8 SD_ReadByte(void);


//****************************************************************************
//Routine for set the cs of the MMC/SD-Card
void SD_SetCS(boolean csid);
//****************************************************************************

//****************************************************************************
//Routine for Delay
void SD_Delay(uint32 uiCSId,uint32 uiNum);
//****************************************************************************

/*******************************************************************************
* Function Name : SD_SendCommand_NoDeassert
* Description    : ��SD������һ������(�����ǲ�ʧ��Ƭѡ�����к������ݴ�����
* Input          : uint8 cmd   ���� 
*                  uint32 arg �������
*                  uint8 crc   crcУ��ֵ
* Output         : None
* Return         : uint8 r1 SD�����ص���Ӧ
*******************************************************************************/
uint8 SD_SendCommand_NoDeassert(uint8 cmd, uint32 arg, uint8 crc);

/*******************************************************************************
* Function Name : SD_SendCommand
* Description    : ��SD������һ������
* Input          : uint8 cmd   ���� 
*                  uint32 arg �������
*                  uint8 crc   crcУ��ֵ
* Output         : None
* Return         : uint8 r1 SD�����ص���Ӧ
*******************************************************************************/
uint8 SD_SendCommand(uint8 cmd, uint32 arg, uint8 crc);

/*******************************************************************************
* Function Name : SD_Init
* Description    : ��ʼ��SD��
* Input          : None
* Output         : None
* Return         : uint8 
*                  0��NO_ERR
*                  1��TIME_OUT
*                  99��NO_CARD
*******************************************************************************/
uint8 SD_Init(void);

/*******************************************************************************
* Function Name : SD_WriteSingleBlock
* Description    : д��SD����һ��block
* Input          : uint32 sector ������ַ��sectorֵ���������ַ�� 
*                  uint8 *buffer ���ݴ洢��ַ����С����512byte�� 
* Output         : None
* Return         : uint8 r1 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
uint8 SD_WriteSingleBlock(uint32 sector, const uint8 *data);

/*******************************************************************************
* Function Name : SD_ReadSingleBlock
* Description    : ��SD����һ��block
* Input          : uint32 sector ȡ��ַ��sectorֵ���������ַ�� 
*                  uint8 *buffer ���ݴ洢��ַ����С����512byte�� 
* Output         : None
* Return         : uint8 r1 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
uint8 SD_ReadSingleBlock(uint32 sector, uint8 *buffer);

/*******************************************************************************
* Function Name : SD_ReceiveData
* Description    : ��SD���ж���ָ�����ȵ����ݣ������ڸ���λ��
* Input          : uint8 *data(��Ŷ������ݵ��ڴ�>len)
*                  uint16 len(���ݳ��ȣ�
*                  uint8 release(������ɺ��Ƿ��ͷ�����CS�ø� 0�����ͷ� 1���ͷţ�
* Output         : None
* Return         : uint8 
*                  0��NO_ERR
*                  other��������Ϣ
*******************************************************************************/
uint8 SD_ReceiveData(uint8 *data, uint16 len, uint8 release);

/*******************************************************************************
* Function Name : SD_WaitReady
* Description    : �ȴ�SD��Ready
* Input          : None
* Output         : None
* Return         : uint8 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
uint8 SD_WaitReady(void);

/*******************************************************************************
* Function Name : SD_Initialize
* Description    : SD����ʼ��
* Input          : None
* Output         : None
* Return         : uint8 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
uint32 SD_Initialize(void);

/*******************************************************************************
* Function Name : SD_ReadMultiBlock
* Description    : ��SD���Ķ��block
* Input          : uint32 sector ȡ��ַ��sectorֵ���������ַ�� 
*                  uint8 *buffer ���ݴ洢��ַ����С����512byte��
*                  uint8 count ������count��block
* Output         : None
* Return         : uint8 r1 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
uint8 SD_ReadMultiBlock(uint32 sector, uint8 *buffer, uint8 count);

/*******************************************************************************
* Function Name : SD_WriteMultiBlock
* Description    : д��SD����N��block
* Input          : uint32 sector ������ַ��sectorֵ���������ַ�� 
*                  uint8 *buffer ���ݴ洢��ַ����С����512byte��
*                  uint8 count д���block��Ŀ
* Output         : None
* Return         : u8 r1 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
uint8 SD_WriteMultiBlock(uint32 sector, const uint8 *data, uint8 count);
#endif
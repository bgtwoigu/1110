#include "target.h"

#ifdef FEATURE_SDCC_VOTE_AGAINST_SLEEP
#include "sleep.h"
#endif

#if defined (T_MSM6275) || defined (T_MSM6280)
#error code not present
#else
#include "tlmm.h"
#endif

#include "pm.h"
#include CLKRGM_H
#include "sd_card.h"


#include "assert.h"

#ifdef FEATURE_DRV_SD_SPI_MODE
/*******************************************************************************
* Function Name : SD_WriteByte
* Description    : SPIдһ���ֽڣ�������ɺ󷵻ر���ͨѶ��ȡ�����ݣ�
* Input          : u8 TxData �����͵���
* Output         : None
* Return         : none
*******************************************************************************/
void SD_WriteByte(uint8 value)
{
	uint8 i; 

	//Software SPI
	for (i=0; i<8; i++) 
	{  
		gpio_out(SD_CLK_PIN,0); //set Clock Impuls low
		
		//write a byte
		if (((value >> (7-i)) & 0x01)==0x01)
		{
			gpio_out(SD_CMD_PIN,1); //Send bit by bit(MSB First)
		}
		else
		{
			gpio_out(SD_CMD_PIN,0);		 
		}

		gpio_out(SD_CLK_PIN,1); //set Clock Impuls High	 
	}//write a byte
	
	gpio_out(GPIO_OUTPUT_29,1);	//set Output High 
}

/*******************************************************************************
* Function Name : SPI_ReadWriteByte
* Description    : SPI��һ���ֽڣ�������ɺ󷵻ر���ͨѶ��ȡ�����ݣ�
* Input          : none
* Output         : None
* Return         : u8 RxData �յ�����
*******************************************************************************/
uint8 SD_ReadByte(void)
{
	uint8 temp=0;
	uint8 i;

	//Software SPI
	for (i=0; i<8; i++) //MSB First
	{
		gpio_out(SD_CLK_PIN,0); //Clock Impuls (Low)
		temp = (temp << 1) + gpio_in(SD_DATA_PIN); //read mmc data out pin 
		gpio_out(SD_CLK_PIN,1);//set Clock Impuls High	
	}
	return (temp);
}


//****************************************************************************
//Routine for set the cs of the MMC/SD-Card
void SD_SetCS(boolean csid)
{
	if ( csid == TRUE)	
	{
		gpio_out(SD_SEL_PIN,1);
	}
	else
	{
		gpio_out(SD_SEL_PIN,0);
	}
}
//****************************************************************************

//****************************************************************************
//Routine for Delay
void SD_Delay(uint32 uiCSId,uint32 uiNum)
{
	uint8 i;
    /* Null for now */
    for (i = 0; i < uiNum; i++)
    {
        /* Clock out an idle byte (0xFF) */
        SD_WriteByte(0xFF);
    }
}
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
uint8 SD_SendCommand_NoDeassert(uint8 cmd, uint32 arg, uint8 crc)
{
	uint8 r1;
    uint8 Retry = 0;

    //
    SD_WriteByte(0xFF);
    //Ƭѡ���õͣ�ѡ��SD��
    SD_SetCS(FALSE);

    //����
    SD_WriteByte(cmd | 0x40);                         //�ֱ�д������
    SD_WriteByte(arg >> 24);
    SD_WriteByte(arg >> 16);
    SD_WriteByte(arg >> 8);
    SD_WriteByte(arg);
    SD_WriteByte(crc);
    
    //�ȴ���Ӧ����ʱ�˳�
    while((r1 = SD_ReadByte())==0xFF)
    {
        Retry++;
        if(Retry > 200)
        {
            break;
        }
    }

    for ( Retry=0; Retry<20; Retry++);
    
    //������Ӧֵ
    return r1;	
}

/*******************************************************************************
* Function Name : SD_SendCommand
* Description    : ��SD������һ������
* Input          : uint8 cmd   ���� 
*                  uint32 arg �������
*                  uint8 crc   crcУ��ֵ
* Output         : None
* Return         : uint8 r1 SD�����ص���Ӧ
*******************************************************************************/
uint8 SD_SendCommand(uint8 cmd, uint32 arg, uint8 crc)
{
	uint8 r1;
    uint8 Retry = 0;

    //????????
    SD_WriteByte(0xff);
    //Ƭѡ���õͣ�ѡ��SD��
    SD_SetCS(FALSE);

    //����
    SD_WriteByte(cmd | 0x40);                         //�ֱ�д������
    SD_WriteByte(arg >> 24);
    SD_WriteByte(arg >> 16);
    SD_WriteByte(arg >> 8);
    SD_WriteByte(arg);
    SD_WriteByte(crc);
    
    //�ȴ���Ӧ����ʱ�˳�
    while((r1 = SD_ReadByte())==0xFF)
    {
        Retry++;
        if(Retry > 200)
        {
            break;
        }
    }
   

    //�ر�Ƭѡ
    SD_SetCS(TRUE);
    //�������϶�������8��ʱ�ӣ���SD�����ʣ�µĹ���
    SD_WriteByte(0xFF);

    //����״ֵ̬
    return r1;
}

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
uint8 SD_Init(void)
{
	uint16 i;      // ����ѭ������
    uint8 r1;      // ���SD���ķ���ֵ
    uint16 retry; // �������г�ʱ����
    uint8 buff[6];
	
    // ����ʱ���ȴ�SD���ϵ����
    for(i=0;i<0xf00;)i++;

    //�Ȳ���>74�����壬��SD���Լ���ʼ�����
    for(i=0;i<16;i++)
    {
        SD_WriteByte(0xFF);
    }

    //-----------------SD����λ��idle��ʼ-----------------
    //ѭ����������CMD0��ֱ��SD������0x01,����IDLE״̬
    //��ʱ��ֱ���˳�
    retry = 0;
    do
    {
        //����CMD0����SD������IDLE״̬
        r1 = SD_SendCommand(CMD0, 0, 0x95);
        retry++;
    }while((r1 != 0x01) && (retry<200));
    //����ѭ���󣬼��ԭ�򣺳�ʼ���ɹ���or ���Գ�ʱ��
    if(retry==200)
    {
        return r1;   //��ʱ����1
    }
    //-----------------SD����λ��idle����-----------------

    //��ȡ��Ƭ��SD�汾��Ϣ
    r1 = SD_SendCommand_NoDeassert(8, 0x1aa, 0x87);

    //�����Ƭ�汾��Ϣ��v1.0�汾�ģ���r1=0x05����������³�ʼ��
    if(r1 == 0x05)
    {
        //���ÿ�����ΪSDV1.0����������⵽ΪMMC�������޸�ΪMMC
        sdcc_pdata.card_type = SDCC_CARD_SD;

        //�����V1.0����CMD8ָ���û�к�������
        //Ƭѡ�øߣ�������������
        SD_SetCS(TRUE);
        //�෢8��CLK����SD������������
        SD_WriteByte(0xFF);

        //-----------------SD����MMC����ʼ����ʼ-----------------

        //������ʼ��ָ��CMD55+ACMD41
        // �����Ӧ��˵����SD�����ҳ�ʼ�����
        // û�л�Ӧ��˵����MMC�������������Ӧ��ʼ��
        retry = 0;
        do
        {
            //�ȷ�CMD55��Ӧ����0x01���������
            r1 = SD_SendCommand(CMD55, 0, 0);
            if(r1 != 0x01)
            {
                return r1; 
            }
            //�õ���ȷ��Ӧ�󣬷�ACMD41��Ӧ�õ�����ֵ0x00����������200��
            r1 = SD_SendCommand(ACMD41, 0, 0);
            retry++;
        }while((r1!=0x00) && (retry<400));
        // �ж��ǳ�ʱ���ǵõ���ȷ��Ӧ
        // ���л�Ӧ����SD����û�л�Ӧ����MMC��
        
        //----------MMC�������ʼ��������ʼ------------
        if(retry==400)
        {
            retry = 0;
            //����MMC����ʼ�����û�в��ԣ�
            do
            {
                r1 = SD_SendCommand(1, 0, 0);
                retry++;
            }while((r1!=0x00)&& (retry<400));
            if(retry==400)
            {
                return r1;   //MMC����ʼ����ʱ
            }
            //д�뿨����
            sdcc_pdata.card_type = SDCC_CARD_MMC;
        }
        //----------MMC�������ʼ����������------------
        
        SD_WriteByte(0xFF);
        
        //��ֹCRCУ��
        
		r1 = SD_SendCommand(CMD59, 0, 0x01);
        if(r1 != 0x00)
        {
            return r1; //������󣬷���r1
        }
           
        //����Sector Size
        r1 = SD_SendCommand(CMD16, 512, 0xff);
        if(r1 != 0x00)
        {
            return r1; //������󣬷���r1
        }
        //-----------------SD����MMC����ʼ������-----------------

    }
    //������V2.0���ĳ�ʼ��
    //������Ҫ��ȡOCR���ݣ��ж���SD2.0����SD2.0HC��
    else if(r1 == 0x01)
    {
        //V2.0�Ŀ���CMD8�����ᴫ��4�ֽڵ����ݣ�Ҫ�����ٽ���������
        buff[0] = SD_ReadByte(); //should be 0x00
        buff[1] = SD_ReadByte(); //should be 0x00
        buff[2] = SD_ReadByte(); //should be 0x01
        buff[3] = SD_ReadByte(); //should be 0xAA
     
        SD_SetCS(TRUE);
        //the next 8 clocks
        SD_WriteByte(0xFF);
        
        //�жϸÿ��Ƿ�֧��2.7V-3.6V�ĵ�ѹ��Χ
        if(buff[2]==0x01 && buff[3]==0xAA)
        {
            //֧�ֵ�ѹ��Χ�����Բ���
            retry = 0;
            //������ʼ��ָ��CMD55+ACMD41
       		do
       		{
       			r1 = SD_SendCommand(CMD55, 0, 0);
        		if(r1!=0x01)
        		{
         			return r1;
        	}
        	r1 = SD_SendCommand(ACMD41, 0x40000000, 0);
            if(retry>200)   
            {
            	return r1; //��ʱ�򷵻�r1״̬
            }
            }while(r1!=0);
          
            //��ʼ��ָ�����ɣ���������ȡOCR��Ϣ

            //-----------����SD2.0���汾��ʼ-----------
            r1 = SD_SendCommand_NoDeassert(CMD58, 0, 0);
            if(r1!=0x00)
            {
                return r1; //�������û�з�����ȷӦ��ֱ���˳�������Ӧ��
            }
            //��OCRָ����󣬽�������4�ֽڵ�OCR��Ϣ
            buff[0] = SD_ReadByte();
            buff[1] = SD_ReadByte();
            buff[2] = SD_ReadByte();
            buff[3] = SD_ReadByte();

            //OCR������ɣ�Ƭѡ�ø�
            SD_SetCS(TRUE);
            SD_WriteByte(0xFF);

            //�����յ���OCR�е�bit30λ��CCS����ȷ����ΪSD2.0����SDHC
            //���CCS=1��SDHC   CCS=0��SD2.0
            if(buff[0]&0x40)    //���CCS
            {
                sdcc_pdata.card_type = SDCC_CARD_SDHC;
            }
            //-----------����SD2.0���汾����-----------
        }

    }
    
    return r1;
}
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
uint8 SD_WriteSingleBlock(uint32 sector, const uint8 *data)
{
	uint8 r1;
    uint16 i;
    uint16 retry;

    //����Ϊ����ģʽ

    //�������SDHC����������sector��ַ������ת����byte��ַ
    //if(SD_Type!=SD_TYPE_V2HC)
    {
        sector = sector<<SD_BLOCKSIZE_NBITS;
    }

    r1 = SD_SendCommand(CMD24, sector, 0x00);
    if(r1 != 0x00)
    {
        return r1; //Ӧ����ȷ��ֱ�ӷ���
    }
    
    //��ʼ׼�����ݴ���
    SD_SetCS(0);
    //�ȷ�3�������ݣ��ȴ�SD��׼����
    SD_WriteByte(0xff);
    SD_WriteByte(0xff);
    SD_WriteByte(0xff);
    //����ʼ����0xFE
    SD_WriteByte(0xFE);

    //��һ��sector������
    for(i=0;i<512;i++)
    {
        SD_WriteByte(*data++);
    }
    //��2��Byte��dummy CRC
    SD_WriteByte(0xff);
    SD_WriteByte(0xff);
    
    //�ȴ�SD��Ӧ��
    r1 = SD_ReadByte();
    if((r1&0x1F)!=0x05)
    {
        SD_SetCS(TRUE);
        return r1;
    }
    
    //�ȴ��������
    retry = 0;
    while(!SD_ReadByte())
    {
        retry++;
        if(retry>0xfffe)        //�����ʱ��д��û����ɣ������˳�
        {
            SD_SetCS(TRUE);
            return 1;           //д�볬ʱ����1
        }
    }

    //д����ɣ�Ƭѡ��1
    SD_SetCS(TRUE);
    SD_WriteByte(0xff);

    return 0;
}

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
uint8 SD_ReadSingleBlock(uint32 sector, uint8 *buffer)
{
	uint8 r1;

    //�������SDHC����sector��ַת��byte��ַ
    sector = sector<<SD_BLOCKSIZE_NBITS;

	r1 = SD_SendCommand(CMD17, sector, 0);//������

	if(r1 != 0x00)
    {
        return r1;
    }
    
    r1 = SD_ReceiveData(buffer, SD_BLOCKSIZE, 1);
    if(r1 != 0)
    {
        return r1;   //�����ݳ���
    }
    else
    {
        return 0;
    }
}
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
uint8 SD_ReceiveData(uint8 *data, uint16 len, uint8 release)
{
	uint16 retry;
    uint8 r1;

    // ����һ�δ���
    SD_SetCS(0);
    //�ȴ�SD������������ʼ����0xFE
    //SD_WriteByte(0xff);
    retry = 0;
    do
    {
        
        r1 = SD_ReadByte();
        retry++;
        if(retry>0x2000) //2000�εȴ���û��Ӧ���˳�����
        {
            SD_SetCS(TRUE);
            return 1;
        }
    }while(r1 != 0xFE);
    //while((SPI_ReadByte())!=0xfe); 
    //��ʼ��������
    while(len--)
    {
        *data = SD_ReadByte();
        data++;
    }
    //������2��αCRC��dummy CRC��
    SD_WriteByte(0xFF);
    SD_WriteByte(0xFF);
    //�����ͷ����ߣ���CS�ø�
    if(release == 1)
    {
        //�������
        SD_SetCS(TRUE);
        SD_WriteByte(0xFF);
    }

    return 0;
}

/*******************************************************************************
* Function Name : SD_WaitReady
* Description    : �ȴ�SD��Ready
* Input          : None
* Output         : None
* Return         : uint8 
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
uint8 SD_WaitReady(void)
{
	uint8 r1;
    uint16 retry;
    retry = 0;
    do
    {
        r1 = SD_ReadByte();
        if(retry==0xfffe)
        {
            return 1;
        }
    }while(r1!=0xFF);

    return 0;
}

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
uint8 SD_ReadMultiBlock(uint32 sector, uint8 *buffer, uint8 count)
{
	uint8 r1;

    //�������SDHC����sector��ַת��byte��ַ
    sector = sector<<SD_BLOCKSIZE_NBITS;
    //SD_WaitReady();
    //�����������
	r1 = SD_SendCommand(CMD18, sector, 0);//������
	if(r1 != 0x00)
    {
        return r1;
    }
    //��ʼ��������
    do
    {
        if(SD_ReceiveData(buffer, SD_BLOCKSIZE, 0) != 0x00)
        {
            break;
        }
        buffer += 512;
    } while(--count);

    //ȫ��������ϣ�����ֹͣ����
    SD_SendCommand(CMD12, 0, 0);
    //�ͷ�����
    SD_SetCS(TRUE);
    SD_WriteByte(0xFF);
    
    if(count != 0)
    {
        return count;   //���û�д��꣬����ʣ�����
    }
    else
    {
        return 0;
    }
}

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
uint8 SD_WriteMultiBlock(uint32 sector, const uint8 *data, uint8 count)
{
	uint8 r1;
    uint16 i;

    //�������SDHC����������sector��ַ������ת����byte��ַ
    //if(SD_Type != SD_TYPE_V2HC)
    {
        sector = sector<<SD_BLOCKSIZE_NBITS;
    }
    
    //�����д��ָ��
    r1 = SD_SendCommand(CMD25, sector, 0x00);
    if(r1 != 0x00)
    {
        return r1; //Ӧ����ȷ��ֱ�ӷ���
    }
    
    //��ʼ׼�����ݴ���
    SD_SetCS(FALSE);
    //�ȷ�3�������ݣ��ȴ�SD��׼����
    SD_WriteByte(0xff);
    SD_WriteByte(0xff);

    //--------������N��sectorд���ѭ������
    do
    {
        //����ʼ����0xFC �����Ƕ��д��
        SD_WriteByte(0xFC);
    
        //��һ��sector������
        for(i=0;i<512;i++)
        {
            SD_WriteByte(*data++);
        }
        //��2��Byte��dummy CRC
        SD_WriteByte(0xff);
        SD_WriteByte(0xff);
        
        //�ȴ�SD��Ӧ��
        r1 = SD_ReadByte();
        if((r1&0x1F)!=0x05)
        {
            SD_SetCS(TRUE);    //���Ӧ��Ϊ��������������ֱ���˳�
            return r1;
        }

        //�ȴ�SD��д�����
        if(SD_WaitReady()==1)
        {
            SD_SetCS(TRUE);    //�ȴ�SD��д����ɳ�ʱ��ֱ���˳�����
            return 1;
        }

        //��sector���ݴ������
    }while(--count);
    
    //��������������0xFD
    SD_WriteByte(0xFD);
    r1 = SD_ReadByte();
    if(r1==0x00)
    {
        count = 0xfe;
    }

    if(SD_WaitReady())
    {
        while(1)
        {
        }
    }
    
    //д����ɣ�Ƭѡ��1
    SD_SetCS(TRUE);
    SD_WriteByte(0xff);

    return count;   //����countֵ�����д����count=0������count=1
}

uint32 SD_Initialize(void)
{
	#define SD_MAXTEST_TIMES 5 //changed by pengyu.li to 5 times.
	uint8         i = 0;
	uint32 rtStatus = 0; 
	
	
	//try to find sd card in N times.
	while(i++ < SD_MAXTEST_TIMES)
	{    
		//power down and power on once again.
		sdcc_mdelay(30);
		
		//initialize the sd card.
	    rtStatus = SD_Init();
	    if( 0 == rtStatus) 
	    {
	    	break; //successful to find sd card.
	    }
	    else
	    {
	    	ERR("sd card error status is %ld", rtStatus,0,0);
	    }
	  
	}
	if(i >= SD_MAXTEST_TIMES)
		ERR("not find tflash in many times",0,0,0);//added by lipengyu for n times init of sd card.
	return 0;
}

/*******************************************************************************
* Function Name : SD_GetCID
* Description    : ��ȡSD����CID��Ϣ��������������Ϣ
* Input          : uint8 *cid_data(���CID���ڴ棬����16Byte��
* Output         : None
* Return         : uint8 
*                  0��NO_ERR
*                  1��TIME_OUT
*                  other��������Ϣ
*******************************************************************************/
uint8 SD_GetCID(uint8 *cid_data)
{
	uint8 r1;

    //��CMD10�����CID
    r1 = SD_SendCommand(CMD10, 0, 0xFF);
    if(r1 != 0x00)
    {
        return r1; //û������ȷӦ�����˳�������
    }
    //����16���ֽڵ�����
    SD_ReceiveData(cid_data, 16, 1);

    return 0;
}

/*******************************************************************************
* Function Name : SD_GetCSD
* Description    : ��ȡSD����CSD��Ϣ�������������ٶ���Ϣ
* Input          : uint8 *cid_data(���CID���ڴ棬����16Byte��
* Output         : None
* Return         : uint8 
*                  0��NO_ERR
*                  1��TIME_OUT
*                  other��������Ϣ
*******************************************************************************/
uint8 SD_GetCSD(uint8 *csd_data)
{
	uint8 r1;

    //��CMD9�����CSD
    r1 = SD_SendCommand(CMD9, 0, 0xFF);
    if(r1 != 0x00)
    {
        return r1; //û������ȷӦ�����˳�������
    }
    //����16���ֽڵ�����
    SD_ReceiveData(csd_data, 16, 1);

    return 0;
}

/*******************************************************************************
* Function Name : SD_GetCapacity
* Description    : ��ȡSD��������
* Input          : None
* Output         : None
* Return         : uint32 capacity 
*                   0�� ȡ�������� 
*******************************************************************************/
uint32 SD_GetCapacity(void)
{
	uint8 rsp[16];
	sdcc_csd_type    csd;

    //ȡCSD��Ϣ������ڼ��������0
    if(SD_GetCSD(rsp)!=0)
    {
        return 0;
    }
       
    SD_DecodeCsd((uint32*)rsp, &csd);

	switch (sdcc_pdata.card_type)
	{
		case SDCC_CARD_SD:
		case SDCC_CARD_SDHC:
		{
			if (csd.csd_structure == 1)
			{
				/* csd_structure = 1 means CSD Version 2.0 for */
				/* High Capacity SD memory card */
				SD_GetHcMemoryInfo(csd.read_bl_len, csd.c_size);
			}
			else
			{
				/* csd_structure = 0 means CSD Version 1.0 for */
				/* Standard Capacity SD memory card */
				SD_GetStdMemoryInfo(csd.read_bl_len, csd.c_size_mult,csd.c_size);
			}
		}
		break;

		case SDCC_CARD_MMCHC:
		{
			/* defer to sdcc_config_mmc_modes_segment as EXT_CSD register */
			/* is read there.  SEC_COUNT must be used to calculate the high */
			/* density card size. */
		}
		break;

		case SDCC_CARD_MMC:
		default:
		{
			/* get the standard memory info anyway */
			SD_GetStdMemoryInfo(csd.read_bl_len, csd.c_size_mult,csd.c_size);
		}
		break;
	}
    return csd.c_size;
}

/******************************************************************************
* Name: SD_DecodeCsd
*
* Description:
*    This function is to parse out the CSD data.
*
* Arguments:
*       data  pointer to data read in from response registers.
*       csd   pointer to the stucture to save the csd data to.
*
******************************************************************************/
void SD_DecodeCsd(const uint32   *data,sdcc_csd_type  *csd)
{
	uint32   value;

	/*----------------------------------------------------------------------*/

	if(!csd)
		return;

	/* In resp[0] */
	value = *data;
	csd->csd_structure      = (uint8)(value >> 30);

	switch (sdcc_pdata.card_type)
	{
		case SDCC_CARD_SD:
		case SDCC_CARD_SDHC:
		{
			if (csd->csd_structure == 1)
			{
				/* CSD Version 2.0: CSD_STRUCTURE = 1 */
				SD_DecodeHcCcsd(data, csd);
			}
			else
			{
				/* CSD Version 1.0: CSD_STRUCTURE = 0 */
				SD_DecodeStdCsd(data, csd);
			}
		}
		break;

		case SDCC_CARD_MMC:
		case SDCC_CARD_MMCHC:
		default:
		{
			SD_DecodeStdCsd(data, csd);
			/* System Specification version is for MMC only. */
			sdcc_pdata.mem.spec_vers = csd->spec_vers;
		}
		break;
	}

	return;
}

/******************************************************************************
* Name: SD_DecodeHcCcsd
*
* Description:
*    This function is to read the passed in data of the High Capacity
*    SD memory card (CSD version 2.0) and assign the proper bit values
*    to the fields in csd structure.

*
* Arguments:
*       data             pointer to data read in from response registers.
*       csd              pointer to the stucture to save the csd data to.
*
* Returns:
*    None
*
******************************************************************************/
void SD_DecodeHcCcsd(const uint32 *data,sdcc_csd_type *csd)
{
	uint32    value;
	uint32    tmp;

	if (NULL == csd)
	{
		return;
	}

	/* In resp[0] */
	value                   = *data;
	/* taac: data read access-time is fixed to 0Eh (means 1ms) --> csd[119:112]*/
	csd->taac               = SDCC_CSD_TAAC;
	/* nsac: data read access-time in CLK cycles is fixed to 00h --> csd[111:104] */
	csd->nsac               = SDCC_CSD_NSAC;
	/* tran_speed: max. data transfer rate --> csd[103:96] */
	csd->tran_speed         = (uint8)(value & 0x000000FF);

	/* read_bl_len: max. read data block length is fixed to */
	/* 9h (means 512 Bytes) --> csd[83:80] */
	value                   = *(++data);
	csd->read_bl_len        = SDCC_CSD_READ_BL_LEN;

	/* c_size: device size --> csd[69:48] */
	/* This field is expanded to 22 bits and can indicate up to 2TBytes. */
	tmp         = value & 0x0000003F;
	value       = *(++data);
	csd->c_size = (uint32)((tmp << 16) | ((value & 0xFFFF0000) >> 16));
}

/******************************************************************************
* Name: SD_DecodeStdCsd
*
* Description:
*    This function is to read the passed in data of the Standard Capacity
*    SD memory card (CSD version 1.0) and assign the proper bit values
*    to the fields in csd structure.
*
* Arguments:
*       data             pointer to data read in from response registers.
*       csd              pointer to the stucture to save the csd data to.
*
* Returns:
*    None
*
******************************************************************************/
void SD_DecodeStdCsd(const uint32 *data,sdcc_csd_type  *csd)
{
	uint32    value;
	uint32    tmp;

	if (NULL == csd)
	{
		return;
	}

	/* In resp[0] */
	value                   = *data;
	/* For MMC only: spec_vers: system specification version --> csd[125:122] */
	csd->spec_vers          = (uint8)((value & 0x3C000000) >> 26);
	/* taac: data read access-time-1 --> csd[119:112] */
	csd->taac               = (uint8)((value & 0x00FF0000) >> 16);
	/* nsac: data read access-time-2 in CLK cycles --> csd[111:104] */
	csd->nsac               = (uint8)((value & 0x0000FF00) >> 8);
	/* tran_speed: max. data transfer rate --> csd[103:96] */
	csd->tran_speed         = (uint8)(value & 0x000000FF);

	/* read_bl_len: max. read data block length --> csd[83:80] */
	value                   = *(++data);
	csd->read_bl_len        = (uint8)((value & 0x000F0000) >> 16);

	/* c_size: device size --> csd[73:62] */
	tmp         = value & 0x000003FF;
	value       = *(++data);
	csd->c_size = (uint32)((tmp << 2) | ((value & 0xC0000000) >> 30));

	/* c_size_mult: device size multiplier --> csd[49:47] */
	csd->c_size_mult        = (uint16)((value & 0x00038000) >> 15);
}

/******************************************************************************
* Name: SD_GetStdMemoryInfo
*
* Description:
*    This function is to calculate the block length and memory capacity
*    of the Standard Capacity SD memory card.
*
* Arguments:
*       block_len        data block length
*       c_size_mult      device size multiplier
*       c_size           device size
*
* Returns:
*    None
*
******************************************************************************/
void SD_GetStdMemoryInfo(uint32 block_len,uint8 c_size_mult,uint32 c_size)
{
	uint32    card_size;

	card_size     = 1 << (c_size_mult + 2);
	card_size    *= (c_size + 1); 

	sdcc_pdata.mem.block_len = 1UL << block_len; /* in Bytes */

	/* Since the block length can be in diffrent size
	* 512, 1024, 2048 etc..Calculate the card size in the absolute 
	* block length of multiple 512 (i.e., SDCC_DEFAULT_BLK_LENGTH_SIZE)
	*/
	sdcc_pdata.mem.card_size = card_size*(sdcc_pdata.mem.block_len/SDCC_DEFAULT_BLK_LENGTH_SIZE);
}

/******************************************************************************
* Name: SD_GetHcMemoryInfo
*
* Description:
*    This function is to calculate the block length and memory capacity
*    of the High Capacity SD memory card.
*
* Arguments:
*       block_len        data block length
*       c_size           device size
*
* Returns:
*    None
*
******************************************************************************/
void SD_GetHcMemoryInfo(uint32 block_len,uint32 c_size)
{
	sdcc_pdata.mem.block_len = 1UL << block_len; /* in Bytes */

	/* The user data area capacity is calculated from the c_size as follows: */
	/*    memory capacity = (c_size+1)*512K byte    */
	/* where 512 is the block len of the High Capacity SD memory card and */
	/*       1K byte = 1024 bytes */
	/* Calculate the card size in the absolute block length of muliple */
	/* 512 (i.e., SDCC_DEFAULT_BLK_LENGTH_SIZE) */
	sdcc_pdata.mem.card_size = (c_size + 1)*((sdcc_pdata.mem.block_len*1024)/SDCC_DEFAULT_BLK_LENGTH_SIZE);
}
#endif
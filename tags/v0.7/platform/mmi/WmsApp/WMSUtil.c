/*==============================================================================
//           W M S   A P P L E T  M O D U L E
// �ļ�:
//        WMSUtil.c
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//        
// �ļ�˵��:
//        ����Ϣģ�鹤�ߺ�������
//                 
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼:
ʱ       ��       �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------    -----------------------------------------------

==============================================================================*/



/*==============================================================================
                             
                             ���ļ��������ⲿ�ļ�
                             
==============================================================================*/
#include "WmsApp_priv.h"
#include "WMSUtil.h"
#ifdef FEATURE_CONTACT_APP
#include "ContApp.h"
#endif
#include "CallApp.h"
#include "wms.h"
#ifdef FEATURE_CARRIER_TAIWAN_APBW
#include "wmsmsg.h"
#include "wmstscdma.h"
#endif
#include "recentcalls.h"
#include "mobile.h"
/*==============================================================================
                                 
                                 ����ȫ�ֱ�������
                                 
==============================================================================*/
// �ɻص�����ʹ�õľ�̬����ָ��
static WmsApp * gpWMSApp = NULL;

/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/
#define BCD2INT(x)  (((((x) & 0xf0) >> 4) * 10) + ((x) & 0x0f))


//Add By zzg 2010_08_03 for frenduo
#define FRENDUO_SMS_DESTNUM	 "551"				// "15323435013"
#define FRENDUO_SMS_BUFF     "status"
//Add End
/*==============================================================================

                                    ��������

==============================================================================*/
#ifdef FEATURE_CONTACT_APP
// ���ڴ�ͨ�Ų�ȡ����Ļص�����
static boolean GetNumCallback(sSelectFieldListNode* pNode);

// ���ڴ�ͨ�Ų�ȡ��ϵ����Ϣ�����뵽��ǰ�༭����Ϣ�еĻص�����
static boolean InsertNumCallback(sSelectFieldListNode* pNode);
#endif

// ���ڴ�ͨ����ʷ��¼ȡ��ϵ����Ϣ�����뵽��ǰ�༭����Ϣ�еĻص�����
static boolean GetNumCallbackFromRecentCall(sSelectRecordListNode* pNode);

/*==============================================================================

                                    ��������
                                 
==============================================================================*/
/*==============================================================================
����: 
    OEMWMS_GetTimeFromWmsTimestamp
       
˵��: 
    ��������������ʱ�����ʽת��Ϊ�롣
       
����: 
    pTime [in]: wms_timestamp_s_type �ṹ����ָ��
    
����ֵ:
    ת�����ʱ��ֵ��
       
��ע:
    
==============================================================================*/
static uint32 OEMWMS_GetTimeFromWmsTimestamp(wms_timestamp_s_type *pTime)
{
#ifndef WIN32
    uint8 tmp;
    clk_julian_type jt;
    
    MEMSET(&jt, 0, sizeof(jt));
    
    tmp = BCD2INT(pTime->year);
    
    // If the year is above 95 it we assume it is in 20 th century(1900+ year
    // otherwise we assume it is in 21 century(2000+year)
    jt.year = (tmp <= 95) ? (2000 + tmp) : (1900 + tmp);
    
    jt.month = BCD2INT(pTime->month);
    jt.day = BCD2INT(pTime->day);
    jt.hour = BCD2INT(pTime->hour);
    jt.minute = BCD2INT(pTime->minute);
    jt.second = BCD2INT(pTime->second);
    
    return clk_julian_to_secs(&jt);
#else
	return 0;
#endif
}


/*==============================================================================
����: 
    WMSUtil_SecsToDateString

˵��: 
    �������ʾ��ʱ��ת��Ϊ�ַ�����ʽ��

����: 
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    dwTime [in]: �Լ����ʱ�䡣
    pBuf [out]: ���ڴ��ʱ���ַ����Ļ�������
    nSize [in]: pBuf �Ŀռ��С(��λ:�ֽ�)��

����ֵ: 
    none

��ע: 

==============================================================================*/
void WMSUtil_SecsToDateString(WmsApp* pMe,uint32 dwTime, AECHAR *pBuf, int nSize)
{
    JulianType      jt;
    AECHAR          wstrFmt[32];
    uint16          wHour,len;
    AECHAR          *wstrBuf = NULL;
    byte            btTimeFmt = 0;
#ifdef FEATURE_TIME_DATA_SETTING
    byte            btDateFmt = 0;
#endif /* FEATURE_TIME_DATA_SETTING */    
    
    if ((NULL == pMe) || (NULL == pBuf) || (0 == nSize))
    {
        return;
    }
    
    wstrBuf = (AECHAR *) sys_malloc(128);
    if (NULL == wstrBuf)
    {
        MSG_ERROR("memory allocation failed !",0,0,0);
        return;
    }
    
    (void) ICONFIG_GetItem(pMe->m_pConfig,
                           CFGI_TIME_FORMAT,
                           &btTimeFmt,
                           sizeof(btTimeFmt));
    
    GETJULIANDATE(dwTime, &jt);
    MEMSET(wstrFmt, 0, sizeof(wstrFmt));
    if (btTimeFmt == OEMNV_TIMEFORM_AMPM)
    {
        wHour = jt.wHour > 12 ? (jt.wHour - 12) : jt.wHour;
        if(jt.wHour == 0)
        {
            wHour = 12;
        }

        if(jt.wHour >= 12)
        {
            (void)STRTOWSTR("%02d:%02d PM", wstrFmt, sizeof(wstrFmt));
        }
        else
        {
            (void)STRTOWSTR("%02d:%02d AM", wstrFmt, sizeof(wstrFmt));
        }
    }
    else
    {
        (void)STRTOWSTR("%02d:%02d", wstrFmt, sizeof(wstrFmt));
        wHour = jt.wHour;
    }
    
    // ��ʽ��ʱ���ַ���
    WSPRINTF(wstrBuf, 128, wstrFmt, wHour, jt.wMinute); 
             
    // �ӿո�
    len = WSTRLEN(wstrBuf);
    wstrBuf[len] = (AECHAR)' ';
    len++;
    wstrBuf[len] = 0;
    
    MEMSET(wstrFmt, 0, sizeof(wstrFmt));
    len = WSTRLEN(wstrBuf);
#ifdef FEATURE_TIME_DATA_SETTING
    (void) ICONFIG_GetItem(pMe->m_pConfig,
                           CFGI_DATE_FORMAT,
                           &btDateFmt,
                           sizeof(btDateFmt));

    switch (btDateFmt)
    {
        case OEMNV_DATEFORM_MDY:
            (void)STRTOWSTR("%02d/%02d/%04d", wstrFmt, sizeof(wstrFmt));
            WSPRINTF(&wstrBuf[len], (128-len*sizeof(AECHAR)), 
                 wstrFmt, jt.wMonth, jt.wDay, jt.wYear); 
            break;
            
        case OEMNV_DATEFORM_DMY:
            (void)STRTOWSTR("%02d/%02d/%04d", wstrFmt, sizeof(wstrFmt));
            WSPRINTF(&wstrBuf[len], (128-len*sizeof(AECHAR)), 
                 wstrFmt, jt.wDay, jt.wMonth, jt.wYear); 
            break;
            
        case OEMNV_DATEFORM_YMD:
        default:
            (void)STRTOWSTR("%04d/%02d/%02d", wstrFmt, sizeof(wstrFmt));
            WSPRINTF(&wstrBuf[len], (128-len*sizeof(AECHAR)), 
                 wstrFmt, jt.wYear, jt.wMonth, jt.wDay); 
            break;
    }
    
#else
    (void)STRTOWSTR("%04d/%02d/%02d", wstrFmt, sizeof(wstrFmt));
    WSPRINTF(&wstrBuf[len], (128-len*sizeof(AECHAR)), 
         wstrFmt, jt.wYear, jt.wMonth, jt.wDay); 
#endif /* FEATURE_TIME_DATA_SETTING */
    
        
    len = WSTRLEN(wstrBuf);
    if (nSize > (len * sizeof(AECHAR)))
    {// �������ռ��㹻
        MEMSET(pBuf, 0, nSize);
        (void) WSTRCPY(pBuf, wstrBuf);
    }
    
    SYS_FREEIF(wstrBuf);
}

/*==============================================================================
����: 
    WMSUtil_HaveNoneASCIIChar

˵��: 
    �жϿ��ַ������Ƿ��з�ASCII�ַ���

����: 
    pWstr [in]: ָ����ַ�����
    nPostion [out]: ��ASCII�ַ���һ�γ��ֵ�λ�á�

����ֵ: 
    �����з�ASCII�ַ�������TRUE������FALSE��

��ע: 

==============================================================================*/
boolean WMSUtil_HaveNoneASCIIChar ( AECHAR * pWstr, int * pPostion)
{
#ifdef FEATURE_CARRIER_ISRAEL_PELEPHONE
    // ������ɫ����Ӫ�̶̹�ʹ�� WMS_ENCODING_LATIN_HEBREW ����
    return FALSE;
#else
    int i ,len;
    AECHAR *pWstrTep = pWstr;

    if (NULL == pWstrTep)
    {
        return FALSE;
    }
    
    len = WSTRLEN(pWstrTep);
    if (len <= 0)
    {
        return FALSE;
    }
#if defined(FEATURE_SMS_GSM7BIT)
    for (i=0; ((i < len) && ((*pWstr) < UCS2_TO_GSM_TABLE_SIZE)); i++,pWstr++)
#else
    for (i=0; ((i < len) && ((*pWstr) < 128)); i++,pWstr++)
#endif    
    {
        //ERR("i='%c',=%d",*pWstr,*pWstr,0);//Azlun
        ;
    }
    
    if (i < len)
    {
        if (NULL != pPostion)
        {
            *pPostion = i;
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif /* FEATURE_CARRIER_ISRAEL_PELEPHONE */
}

/*==============================================================================
����: 
    WMSUtil_CalculateMessagesCount

˵��: 
    �жϷ��͸����ַ�����Ҫ�Ķ���������

����: 
    pWstr [in]: ָ����ַ�����
    pencoding [in/out]: ���ڷ��ص�ǰʹ�õı��뷽ʽ��
    pnMaxcharsOneitem [in/out]: ���ڷ��ص�������������ɵ��ַ���������ָ�롣

����ֵ: 
    ���͸����ַ�����Ҫ�Ķ���������

��ע: 

==============================================================================*/
int WMSUtil_CalculateMessagesCount(AECHAR *pWstr, 
                                   int *pencoding,
                                   int *pnMaxcharsOneitem)
{
    int nItems = 1;// ����Ϣ���ֺ�õ���������
    int nMaxItemChars = 0;     // ÿ������ַ���
    int nLen = 0;
    wms_user_data_encoding_e_type      encoding;
    int nMaxChars_UNICODE = MAX_UNICODMSG_PAYLOAD;
    int nMaxChars_ASCII = MAX_ASCIIMSG_PAYLOAD;
    
    if ((NULL == pWstr) || *pWstr == 0)
    {
        if (NULL != pencoding)
        {
            *pencoding = WMS_ENCODING_ASCII;
        }
        
        if (NULL != pnMaxcharsOneitem)
        {
            *pnMaxcharsOneitem = nMaxChars_ASCII;
        }
        return nItems;
    }
    
    nLen = WSTRLEN(pWstr);
    
    // ȷ����Ϣ���뷽ʽ���軮�ֵ�����
    if (WMSUtil_HaveNoneASCIIChar(pWstr, NULL))
    {
        encoding = WMS_ENCODING_UNICODE;
    }
    else
    {
#if defined (FEATURE_CARRIER_ANGOLA_MOVICEL)
        byte  mo_encoding=0;
        
        (void) OEM_GetConfig(CFGI_SMS_MO_ENCODING,
                             &mo_encoding,
                             sizeof(mo_encoding));
        if (mo_encoding == OEMNV_SMS_MO_ENCODING_7BIT )
        {
            encoding = WMS_ENCODING_ASCII;
        }
        else
        {
            encoding = WMS_ENCODING_LATIN;
        }
#elif defined(FEATURE_CARRIER_ISRAEL_PELEPHONE)        
        encoding = WMS_ENCODING_LATIN_HEBREW;
#else //FEATURE_CARRIER_ANGOLA_MOVICEL
        encoding = WMS_ENCODING_ASCII;
#endif //FEATURE_CARRIER_ANGOLA_MOVICEL
    }

    // ȷ����Ϣ���뷽ʽ���軮�ֵ�����
    if (WMS_ENCODING_UNICODE == encoding)
    {        
        if (nLen<=nMaxChars_UNICODE)
        {
            nMaxItemChars = nMaxChars_UNICODE;
        }
        else
        {
#ifdef FEATURE_SMS_UDH
            nMaxItemChars = (nMaxChars_UNICODE-3);
#else
            nMaxItemChars = nMaxChars_UNICODE;
#endif            
        }
    }
    else if (WMS_ENCODING_ASCII == encoding)
    {        
        if (nLen<=nMaxChars_ASCII)
        {
            nMaxItemChars = nMaxChars_ASCII;
        }
        else
        {
#ifdef FEATURE_SMS_UDH
            nMaxItemChars = (nMaxChars_ASCII-7);
#else
            nMaxItemChars = nMaxChars_ASCII;
#endif            
        }
    }
    else
    {        
#ifdef FEATURE_SMS_UDH
        if (nLen <= MAX_OCTETMSG_PAYLOAD)
        {
            nMaxItemChars = MAX_OCTETMSG_PAYLOAD;
        }
        else
        {
            nMaxItemChars = MAX_OCTETMSG_PAYLOAD-6;
        }
#else
        nMaxItemChars = MAX_OCTETMSG_PAYLOAD;
#endif            
    }
    
    nItems = nLen / nMaxItemChars;
    if (nLen % nMaxItemChars != 0)
    {
        nItems++;
    }
    
    if (NULL != pencoding)
    {
        *pencoding = encoding;
    }
    
    if (NULL != pnMaxcharsOneitem)
    {
        *pnMaxcharsOneitem = nMaxItemChars;
    }
    
    return nItems;
}

/*==============================================================================
����: 
    WMSUtil_GetContactName

˵��: 
    ���ݵ绰����ӵ绰���в�ѯ������

����: 
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    pwszNum [in]: �����ַ�����
    pwszName [in/out]: ���ڷ����������ַ���Buf��
    wBufLen [in]: pwszName ������ɵ��ַ���(AECHAR)

����ֵ: 
    none

��ע: 

==============================================================================*/
void WMSUtil_GetContactName(WmsApp* pMe,  
                            const AECHAR *pwszNum, 
                            AECHAR *pwszName,
                            uint16 wBufLen)
{
#ifdef FEATURE_CONTACT_APP
    IContApp * pContApp = NULL;
    ContInfo  CInfo;
    
    if (SUCCESS == ISHELL_CreateInstance(pMe->m_pShell,
        AEECLSID_APP_CONTACT,
        (void**)&pContApp))
    {
        if (pContApp == NULL)
        {
            return;
        }
        
        MEMSET(&CInfo, 0, sizeof(ContInfo));
        CInfo.pName = pwszName;
        CInfo.wNameLen = wBufLen;
        
        (void)ICONTAPP_NumberLookup(pContApp, (AECHAR *)pwszNum, &CInfo);
                    
        (void)ICONTAPP_Release(pContApp);
    } 
#endif       
}

/*==============================================================================
����: 
    WMSUtil_AVKType2AECHAR(AVKType key)

˵��: 
    ������������ʵ��ַ�ת������Ӧ�Ŀ��ַ���

����: 
    key [in]: AVKType��

����ֵ: 
    AECHAR
       
��ע: 

==============================================================================*/
AECHAR WMSUtil_AVKType2AECHAR(AVKType key)
{
    AECHAR  wChar;
    
    switch (key)
    {
        case AVK_STAR:
            wChar = (AECHAR)'*';
            break;

        case AVK_POUND:
            wChar = (AECHAR)'#';
            break;
            
        default:
            if (key < AVK_0 || key > AVK_9) 
            {
                wChar = (AECHAR)'\0';
            }
            else
            {
                wChar = (AECHAR)('0' + (char)(key - AVK_0));
            }
            break;
    }
 
    return wChar;
}

/*==============================================================================
����:
    WMSMessageStruct_Reset

˵��:
    ��������������Ϣ�ṹ����Ա����Ϊȱʡֵ��

����:
    pMsg [in/out]: WMSMessageStruct ���ݽṹָ�롣

����ֵ:
    none

��ע:
        
==============================================================================*/
void WMSMessageStruct_Reset(WMSMessageStruct * pMsg) 
{
    if (NULL == pMsg) 
    {
        return;
    }
    
    // ���ͷŶ�̬����� buffer
    if (NULL != pMsg->m_szMessage)
    {
        sys_free(pMsg->m_szMessage);
        pMsg->m_szMessage = NULL;
    }
    
    // ȫ����������
    MEMSET(pMsg, 0, sizeof(WMSMessageStruct));
}
//add by yangdecai 2010-08-16
/*==============================================================================
����:
    WMSMessageStruct_Free

˵��:
    ��������������Ϣ�ṹ����Ա����Ϊȱʡֵ��

����:
    pMsg [in/out]: WMSMessageStruct_Free ���ݽṹָ�롣

����ֵ:
    none

��ע:
        
==============================================================================*/
void WMSMessageStruct_Free(WmsApp * pMe)
{
	int i;
	for(i = 0;i<LONGSMS_MAX_PACKAGES;i++)
	{
		WMSMessageStruct_Reset(pMe->m_CurMsgNodesMS[i]);
	}
}

static void OEMWMS_ConvertFromUnicode(wms_client_bd_s_type *bd_data)
{
    uint8 i;
    uint16  *pTep;
    
    if (bd_data != NULL)
    {
        bd_data->user_data.data_len = bd_data->user_data.data_len/2;
        pTep = (uint16 *)bd_data->user_data.data;
        
        for (i=0; i<bd_data->user_data.data_len; i++,pTep++)
        {
            *pTep = NTOHS(*pTep);
        }
    }
}

/*==============================================================================
����:
    WmsApp_ConvertClientMsgToMS

˵��:
    ������ wms_client_message_s_type ��Ϣ�������ڲ��ṹ WMSMessageStruct ��

����:
    pClMsg [in]: wms_client_message_s_type ���ݽṹָ�롣
    pOutMs [in/out]: WMSMessageStruct ���ݽṹָ�롣

����ֵ:
    none

��ע:
        
==============================================================================*/
void WmsApp_ConvertClientMsgToMS(wms_client_message_s_type *pClMsg,
                                 WMSMessageStruct *pOutMs)
{
    wms_client_ts_data_s_type   *cl_data = NULL;    // ������ڴ�
    wms_client_bd_s_type        *info_data = NULL;  // ������ڴ�
    uint8                       *buf = NULL;        // ������ڴ�
    wms_cdma_user_data_s_type   *puserdata = NULL;
    int32                       nSize;
    int                         nlen = 0, i;
    
    if ((NULL == pClMsg) || (NULL == pOutMs))
    {
        return;
    }
    if (pClMsg->msg_hdr.message_mode != WMS_MESSAGE_MODE_CDMA)
    {
        return;
    }
    
    // �ṹ��������
    WMSMessageStruct_Reset(pOutMs);
    
    // �ȷ��亯��Ҫ�õ��� buffer
    cl_data = sys_malloc(sizeof(wms_client_ts_data_s_type));
    if (cl_data == NULL)
    {
        MSG_ERROR("memory allocation failed !",0,0,0);
        goto WmsApp_ConvertClientMsgToMSErr;
    }
    (void)MEMSET(cl_data, 0, sizeof(wms_client_ts_data_s_type));
    
    buf = sys_malloc(64);
    if (buf == NULL)
    {
        MSG_ERROR("memory allocation failed !",0,0,0);
        goto WmsApp_ConvertClientMsgToMSErr;
    }
    
    info_data = sys_malloc(sizeof(wms_client_bd_s_type));
    if (info_data == NULL)
    {
        MSG_ERROR("memory allocation failed !",0,0,0);
        goto WmsApp_ConvertClientMsgToMSErr;
    }
    (void)MEMSET(info_data, 0, sizeof(wms_client_bd_s_type));
    
    // ������ʼ
    pOutMs->msg_tag = pClMsg->msg_hdr.tag;
    
    if (pOutMs->msg_tag == WMS_TAG_MO_TEMPLATE) 
    {// CDMA ģ����Ϣ��ݸ���Ϣ
        wms_cdma_template_s_type *pClTemp = &(pClMsg->u.cdma_template);
        
        if ((pClTemp->mask & WMS_CDMA_TEMPLATE_MASK_BEARER_DATA) &&
            (pClTemp->client_bd.mask & WMS_MASK_BD_USER_DATA))
        {
            // copy the user data into the info cache
            switch (pClTemp->client_bd.user_data.encoding)
            {
                case WMS_ENCODING_ASCII:
                case WMS_ENCODING_IA5:
                    puserdata = (wms_cdma_user_data_s_type*)&(pClTemp->client_bd.user_data);
                    nlen = wms_ts_unpack_ascii(puserdata,
                                pClTemp->client_bd.user_data.number_of_digits+1,
                                info_data->user_data.data);
                    info_data->user_data.data_len = nlen;
                    
                    nSize = sizeof(AECHAR)*(nlen + 1);
                    info_data->user_data.data[nlen] = 0;
                    pOutMs->m_szMessage = sys_malloc(nSize);
                    if (NULL == pOutMs->m_szMessage)
                    {
                        MSG_ERROR("memory allocation failed !",0,0,0);
                        goto WmsApp_ConvertClientMsgToMSErr;
                    }
                    (void)MEMSET(pOutMs->m_szMessage, 0, nSize);
                    
                    for (i=0; i<nlen; i++)
                    {
#if defined(FEATURE_SMS_GSM7BIT)
                        pOutMs->m_szMessage[i] = (AECHAR)GsmToUcs2[info_data->user_data.data[i]];
                        
                        if (pOutMs->m_szMessage[i] == 0)
                        {
                            pOutMs->m_szMessage[i] = (AECHAR)0x20;//' '
                        }
#else    
                        pOutMs->m_szMessage[i] = (AECHAR)info_data->user_data.data[i];
#endif            
                    }
                    pOutMs->m_szMessage[i] = 0;
                    break;
                    
                case WMS_ENCODING_UNICODE:
                    info_data->user_data.data_len = pClTemp->client_bd.user_data.data_len;
                    (void)MEMCPY(info_data->user_data.data,
                                 pClTemp->client_bd.user_data.data,
                                 info_data->user_data.data_len);
                    OEMWMS_ConvertFromUnicode(info_data);
                    
                    nlen = info_data->user_data.data_len;
                    nSize = sizeof(AECHAR)*(nlen + 1);
                    pOutMs->m_szMessage = sys_malloc(nSize);
                    if (NULL == pOutMs->m_szMessage)
                    {
                        MSG_ERROR("memory allocation failed !",0,0,0);
                        goto WmsApp_ConvertClientMsgToMSErr;
                    }
                    (void)MEMSET(pOutMs->m_szMessage, 0, nSize);
                    (void)WSTRNCOPYN (pOutMs->m_szMessage,
                                      (nlen+1),
                                      (AECHAR *)(info_data->user_data.data), 
                                      nlen);
                    break;
                    
                // just copy the user data for other encodings
                default:
                    nlen = pClTemp->client_bd.user_data.data_len;
                    
                    for (i=0; i<nlen; i++)
                    {
#if defined(FEATURE_SMS_GSM7BIT)
                        pOutMs->m_szMessage[i] = (AECHAR)pClTemp->client_bd.user_data.data[i];
                        
                        if (pOutMs->m_szMessage[i] == 0)
                        {
                            pOutMs->m_szMessage[i] = (AECHAR)0x20;//' '
                        }
#else    
                        pOutMs->m_szMessage[i] = (AECHAR)pClTemp->client_bd.user_data.data[i];
#endif            
                    }
                    pOutMs->m_szMessage[i] = 0; 
                    break;
            }
            pOutMs->m_epriority = pClTemp->client_bd.priority;
        }
        
        if (pClTemp->mask & WMS_CDMA_TEMPLATE_MASK_DEST_ADDR)
        {
            (void)MEMSET(buf, 0, 64);
            if (pClTemp->address.digit_mode == WMS_DIGIT_MODE_4_BIT)
            {
                (void)wms_ts_dtmf2ascii((byte)pClTemp->address.number_of_digits,
                                (byte*)pClTemp->address.digits, buf);
            }
            else
            {
                nlen = pClTemp->address.number_of_digits;
                (void)MEMCPY(buf, pClTemp->address.digits, nlen);
            }
            nlen = STRLEN((char *)buf);
            
            if (nlen>0)
            {
                nSize = sizeof(pOutMs->m_szNum);
                (void)MEMSET(pOutMs->m_szNum, 0, nSize);
                (void)STRTOWSTR((char *)buf, pOutMs->m_szNum, nSize);
            }
        }
    }
    else
    {// CDMA ��Ϣ
#ifdef FEATURE_SMS_UDH
        uint8 i=0;
#endif 

        // Initialized other to 0
        (void)MEMSET(&cl_data->u.cdma.other, 0, sizeof(wms_other_parm_s_type));

        // decode the raw bearer data
        if (wms_ts_decode(&pClMsg->u.cdma_message.raw_ts, cl_data) != WMS_OK_S)
        {
            MSG_ERROR("Decoding message failed!",0,0,0);
            goto WmsApp_ConvertClientMsgToMSErr;
        }
        
        // ��Ϣ��Ŀ(������֪ͨ����)
        if (cl_data->u.cdma.mask & WMS_MASK_BD_NUM_OF_MSGS)
        {
            pOutMs->m_NumMsgs = cl_data->u.cdma.num_messages;
        }
        
        // ��������ʱ���
        if (cl_data->u.cdma.mask & WMS_MASK_BD_MC_TIME)
        {
            pOutMs->m_dwTime = OEMWMS_GetTimeFromWmsTimestamp(&(cl_data->u.cdma.mc_time));
        }
        
        // ���ȼ�
        if (cl_data->u.cdma.mask & WMS_MASK_BD_PRIORITY)
        {
            pOutMs->m_epriority = cl_data->u.cdma.priority;
        }
        
        // �ܼ�
        if (cl_data->u.cdma.mask & WMS_MASK_BD_PRIORITY)
        {
            pOutMs->m_eprivacy = cl_data->u.cdma.privacy;
        }
        
        // ����ָʾ
        if (cl_data->u.cdma.mask & WMS_MASK_BD_LANGUAGE)
        {
            pOutMs->language = cl_data->u.cdma.language;
        }
    
#ifdef FEATURE_SMS_UDH
        // ������Ϣͷ
        for (i=0; i<cl_data->u.cdma.user_data.num_headers; i++)
        {
            if (cl_data->u.cdma.user_data.headers[i].header_id == WMS_UDH_CONCAT_8)
            {
                pOutMs->seq_num = cl_data->u.cdma.user_data.headers[i].u.concat_8.seq_num;
                pOutMs->total_sm = cl_data->u.cdma.user_data.headers[i].u.concat_8.total_sm;
                pOutMs->msg_ref = cl_data->u.cdma.user_data.headers[i].u.concat_8.msg_ref;
                break;
            }
            else if (cl_data->u.cdma.user_data.headers[i].header_id == WMS_UDH_CONCAT_16)
            {
                pOutMs->seq_num = cl_data->u.cdma.user_data.headers[i].u.concat_16.seq_num;
                pOutMs->total_sm = cl_data->u.cdma.user_data.headers[i].u.concat_16.total_sm;
                pOutMs->msg_ref = cl_data->u.cdma.user_data.headers[i].u.concat_16.msg_ref;
                break;
            }
        }
#endif
        
        // �ȼ����Ϣ���޵�ַ(MT:ʼ����ַ MO:���յ�ַ)
        if ((pClMsg->u.cdma_message.address.number_of_digits) > 0)
        {
            (void)MEMSET(buf, 0, 64);
            if (pClMsg->u.cdma_message.address.digit_mode == WMS_DIGIT_MODE_4_BIT)
            {
                (void)wms_ts_dtmf2ascii((byte)pClMsg->u.cdma_message.address.number_of_digits,
                                        (byte*)pClMsg->u.cdma_message.address.digits, buf);
            }
            else
            {
                nlen = pClMsg->u.cdma_message.address.number_of_digits;
                (void)MEMCPY(buf, pClMsg->u.cdma_message.address.digits, nlen);
            }
            nlen = STRLEN((char *)buf);
            
            if (nlen>0)
            {
                i = 0;
                nSize = sizeof(pOutMs->m_szNum);
                (void)MEMSET(pOutMs->m_szNum, 0, nSize);
                
                if ((nlen>3) &&
                    (pClMsg->msg_hdr.tag == WMS_TAG_MT_READ ||
                     pClMsg->msg_hdr.tag == WMS_TAG_MT_NOT_READ))
                {
                    // ȥ�� "+86"
                    if (STRNCMP((char *)buf, "+86", 3) == 0)
                    {
                        i = 3;
                    }
#ifdef FEATURE_CARRIER_TAIWAN_APBW
                    // ��"886"���"0"
                    else if (STRNCMP((char *)buf, "886", 3) == 0)
                    {
                        i = 2;
                        buf[i] = '0';
                    }
#endif
#ifdef FEATURE_CARRIER_VENEZUELA_MOVILNET
                    // ��"158"���"0416"
                    // ��"199"���"0419"
                    // ��"199"���"0426" guoys modified for movilnet change 2008.10.28
                    else if ((STRNCMP((char *)buf, "158", 3) == 0) ||
                             (STRNCMP((char *)buf, "199", 3) == 0))
                    {
                        int j;
                        
                        buf[nlen+1] = 0;
                        for (j = nlen; j>3; j--)
                        {
                            buf[j] = buf[j-1];
                        }
                        
                        if (buf[2] == '8')
                        {
                            //buf[3] = '6';
                            buf[2] = '1';
                        }
                        else
                        {
                            //buf[3] = '9';
                            buf[2] = '2';
                        }
                        buf[0] = '0';
                        buf[1] = '4';
                        //buf[2] = '1';
                        buf[3] = '6';
                        nlen += 1;
                    }
#endif
                }
                
#ifdef FEATURE_CARRIER_TAIWAN_APBW
                if (pClMsg->msg_hdr.tag == WMS_TAG_MT_READ ||
                    pClMsg->msg_hdr.tag == WMS_TAG_MT_NOT_READ)
                {// 00000000777 ȥ������� 0
                    int j;
                    
                    for (j=i; j<nlen; j++)
                    {
                        if (buf[j] != (AECHAR)'0')
                        {
                            if (STRCMP("777", (char *)&buf[j]) == 0)
                            {
                                i=j;
                            }
                            break;
                        }
                    }
                }
#endif
                
                (void)STRTOWSTR((char *)&buf[i], pOutMs->m_szNum, nSize);
            }
        }
        
        if (cl_data->u.cdma.mask & WMS_MASK_BD_USER_DATA) 
        {
            pOutMs->m_epriority = cl_data->u.cdma.priority;
            
            // copy the user data 
            switch (cl_data->u.cdma.user_data.encoding)
            {
                case WMS_ENCODING_ASCII:
                case WMS_ENCODING_IA5:
                    puserdata = (wms_cdma_user_data_s_type*)&cl_data->u.cdma.user_data;
                    nlen = wms_ts_unpack_ascii(puserdata,
                                cl_data->u.cdma.user_data.number_of_digits+1,
                                info_data->user_data.data);
                    info_data->user_data.data_len = nlen;
                    
                    nlen = info_data->user_data.data_len;
                    nSize = sizeof(AECHAR)*(nlen + 1);
                    info_data->user_data.data[nlen] = 0;
                    pOutMs->m_szMessage = sys_malloc(nSize);
                    if (NULL == pOutMs->m_szMessage)
                    {
                        MSG_ERROR("memory allocation failed !",0,0,0);
                        goto WmsApp_ConvertClientMsgToMSErr;
                    }
                    (void)MEMSET(pOutMs->m_szMessage, 0, nSize);
                    
                    for (i=0; i<nlen; i++)
                    {
#if defined(FEATURE_SMS_GSM7BIT)
                        pOutMs->m_szMessage[i] = (AECHAR)GsmToUcs2[info_data->user_data.data[i]];
                        
                        if (pOutMs->m_szMessage[i] == 0)
                        {
                            pOutMs->m_szMessage[i] = (AECHAR)0x20;//' '
                        }
#else    
                        pOutMs->m_szMessage[i] = (AECHAR)info_data->user_data.data[i];
#endif            
                    }
                    pOutMs->m_szMessage[i] = 0;
                    break;
                    
                case WMS_ENCODING_UNICODE:
                    OEMWMS_ConvertFromUnicode(&cl_data->u.cdma);
                    nlen = cl_data->u.cdma.user_data.data_len;
                    
                    nSize = sizeof(AECHAR)*(nlen + 1);
                    pOutMs->m_szMessage = sys_malloc(nSize);
                    if (NULL == pOutMs->m_szMessage)
                    {
                        MSG_ERROR("memory allocation failed !",0,0,0);
                        goto WmsApp_ConvertClientMsgToMSErr;
                    }
                    (void)MEMSET(pOutMs->m_szMessage, 0, nSize);
                    (void)WSTRNCOPYN (pOutMs->m_szMessage,
                                      (nlen+1),
                                      (AECHAR *)(cl_data->u.cdma.user_data.data), 
                                      nlen);
                    break;
                    
                // just copy the user data for other encodings
                default:
                    nlen = cl_data->u.cdma.user_data.number_of_digits;//xg 2008 08 08modify msg
                    
                    nSize = sizeof(AECHAR)*(nlen + 1);
                    pOutMs->m_szMessage = sys_malloc(nSize);
                    if (NULL == pOutMs->m_szMessage)
                    {
                        MSG_ERROR("memory allocation failed !",0,0,0);
                        goto WmsApp_ConvertClientMsgToMSErr;
                    }
                    for (i=0; i<nlen; i++)
                    {
#ifdef FEATURE_LANG_HEBREW
                        pOutMs->m_szMessage[i] = (AECHAR)HebrewToUcs2[cl_data->u.cdma.user_data.data[i]];
                        
                        if (pOutMs->m_szMessage[i] == 0)
                        {
                            pOutMs->m_szMessage[i] = (AECHAR)0x20;//' '
                        }
#else
                        pOutMs->m_szMessage[i] = (AECHAR)cl_data->u.cdma.user_data.data[i];
#endif                        
                    }
                    pOutMs->m_szMessage[i] = 0; 
                    break;
            }
        }
        
        // Callback number
        if (cl_data->u.cdma.mask & WMS_MASK_BD_CALLBACK)
        {
            (void)MEMSET(buf, 0, 64);
            if (cl_data->u.cdma.callback.digit_mode == WMS_DIGIT_MODE_4_BIT)
            {
                (void)wms_ts_dtmf2ascii((byte)cl_data->u.cdma.callback.number_of_digits,
                                (byte*)cl_data->u.cdma.callback.digits, buf);
            }
            else
            {
                nlen = cl_data->u.cdma.callback.number_of_digits;
                (void)MEMCPY(buf, cl_data->u.cdma.callback.digits, nlen);
            }
            nlen = STRLEN((char *)buf);
            
            if (nlen>0)
            {
                i = 0;
                nSize = sizeof(pOutMs->m_szCallBkNum);
                (void)MEMSET(pOutMs->m_szCallBkNum, 0, nSize);
                
                if ((nlen>3) &&
                    (pClMsg->msg_hdr.tag == WMS_TAG_MT_READ ||
                     pClMsg->msg_hdr.tag == WMS_TAG_MT_NOT_READ))
                {
                    // ȥ�� "+86"
                    if (STRNCMP((char *)buf, "+86", 3) == 0)
                    {
                        i = 3;
                    }
#ifdef FEATURE_CARRIER_TAIWAN_APBW
                    // ��"886"���"0"
                    else if (STRNCMP((char *)buf, "886", 3) == 0)
                    {
                        i = 2;
                        buf[i] = '0';
                    }
#endif
#ifdef FEATURE_CARRIER_VENEZUELA_MOVILNET
                    // ��"158"���"0416"
                    // ��"199"���"0419"
                    // ��"199"���"0426" guoys modified for movilnet change 2008.10.28
                    else if ((STRNCMP((char *)buf, "158", 3) == 0) ||
                             (STRNCMP((char *)buf, "199", 3) == 0))
                    {
                        int j;
                        
                        buf[nlen+1] = 0;
                        for (j = nlen; j>3; j--)
                        {
                            buf[j] = buf[j-1];
                        }
                        
                        if (buf[2] == '8')
                        {
                            //buf[3] = '6';
                            buf[2] = '1';
                        }
                        else
                        {
                            //buf[3] = '9';
                            buf[2] = '2';
                        }
                        buf[0] = '0';
                        buf[1] = '4';
                        //buf[2] = '1';
                        buf[3] = '6';
                        nlen += 1;
                    }
#endif
                }
                
#ifdef FEATURE_CARRIER_TAIWAN_APBW
                if (pClMsg->msg_hdr.tag == WMS_TAG_MT_READ ||
                    pClMsg->msg_hdr.tag == WMS_TAG_MT_NOT_READ)
                {// 00000000777 ȥ������� 0
                    int j;
                    
                    for (j=i; j<nlen; j++)
                    {
                        if (buf[j] != (AECHAR)'0')
                        {
                            if (STRCMP("777", (char *)&buf[j]) == 0)
                            {
                                i=j;
                            }
                            break;
                        }
                    }
                }
#endif
                
                (void)STRTOWSTR((char *)&buf[i], pOutMs->m_szCallBkNum, nSize);
            }
        }
    }
    
    // ���������˳�Ҳ��ִ�к���Ĵ���
WmsApp_ConvertClientMsgToMSErr:
    WMSAPPU_FREE(cl_data)
    WMSAPPU_FREE(buf)
    WMSAPPU_FREE(info_data)
}

/*==============================================================================
����:
    WMSExtApp_CallPhoneNumber

˵��:
    ���������ڷ����ָ������ĺ��С�

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    szVMNumber [in/out]: ���������ַ�����
    bIPCall [in]: TRUE - IP Call; FALSE - Normal Call

����ֵ:
    none

��ע:

==============================================================================*/
void WMSExtApp_CallPhoneNumber(WmsApp * pMe, AECHAR *szVMNumber, boolean bIPCall)
{
    ICallApp  * pUICall = NULL;

    // �������нӿ�
    if (SUCCESS == ISHELL_CreateInstance(pMe->m_pShell,
        AEECLSID_DIALER,
        (void **)&pUICall))
    {
        if (pUICall != NULL)
        {
            // ���ú��нӿں������к���
#ifdef FEATRUE_SET_IP_NUMBER
            if (bIPCall)
            {
                (void)ICallApp_Make_IP_Call_F(pUICall, szVMNumber);
            }
            else
#endif
            {
                (void)ICallApp_CallNumber(pUICall, szVMNumber);
            }
            
            // �ͷź��нӿ�
            (void)ICallApp_Release(pUICall);
        }
    }
}

/*==============================================================================
����:
    WMSExtApp_SaveExtractDetails

˵��:
    ͨ��Contact Applet���绰���뱣�浽ͨ�Ų����ݿ⡣

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    wstrNum [in]: ������ĺ����ַ�����
    wstrURL [in]: ������� URL �ַ�����
    wstrEmail [in]: ������� e-mail ��ַ�ַ�����

����ֵ:
    none

��ע:
    ����Contact Applet��WMS applet������

==============================================================================*/
void WMSExtApp_SaveExtractDetails(WmsApp *pMe, AECHAR  *wstrNum, AECHAR *wstrURL, AECHAR *wstrEmail)
{
 #ifdef FEATURE_CONTACT_APP
    IContApp * pContApp = NULL;
    uint16 wType = SAVE_NUMBER;

    if (SUCCESS == ISHELL_CreateInstance(pMe->m_pShell,
        AEECLSID_APP_CONTACT,
        (void**)&pContApp))
    {
        if (pContApp == NULL)
        {
            return;
        }
        if (NULL != wstrNum)
        {
            wType = SAVE_NUMBER;
            if(pMe->m_wPrevMenuSel == IDS_SAVETOCONTACT)
            {
                wType = ADD_FIELD;
            }
        }
        else if (NULL !=wstrEmail)
        {
            wType = SAVE_EMAIL;
        }
        else if (NULL !=wstrURL)
        {
            wType = SAVE_URL;
        }
        (void)ICONTAPP_SaveNumber(pContApp, wstrNum, wstrEmail, wstrURL,wType);
        (void)ICONTAPP_Release(pContApp);
    }
#endif
}

/*==============================================================================
����:
    WMSExtApp_GetAddresseeFromContactsApp

˵��:
    ���������ڴ�ͨ�Ų���ȡ���롣

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    eType [in]: �ӵ绰��ȡ����ʱ��������

����ֵ:
    �ޡ�

��ע:
    ����Contact Applet��WMS applet������
        
==============================================================================*/
void WMSExtApp_GetAddresseeFromContactsApp(WmsApp *pMe, GetAddr_Type eType)
{
#ifdef FEATURE_CONTACT_APP
    IContApp * pContApp = NULL;
    
    if (SUCCESS == ISHELL_CreateInstance(pMe->m_pShell,
        AEECLSID_APP_CONTACT,
        (void**)&pContApp))
    {
        if (pContApp == NULL)
        {
            return;
        }
        
        gpWMSApp = pMe;
        
        switch (eType)
        {
            case GETADDR_INSERT:
                (void)ICONTAPP_SelectFieldEx(pContApp, (PFNSELECTEDCONTACTCB)InsertNumCallback, MULTI_SELECT_CONTENT);
                break;
                
            case GETADDR_ONEADD:
                (void)ICONTAPP_SelectFieldEx(pContApp, (PFNSELECTEDCONTACTCB)GetNumCallback, SINGLE_SELECT_NUMBER);
                break;
                
            case GETADDR_MULTIADD:
                (void)ICONTAPP_SelectFieldEx(pContApp, (PFNSELECTEDCONTACTCB)GetNumCallback, MULTI_SELECT_NUMBER);
                break;

            case GETADDR_GROUPlIST:
                (void)ICONTAPP_SelectFieldEx(pContApp, (PFNSELECTEDCONTACTCB)GetNumCallback, MULTI_SELECT_GROUPLIST);
                break;
            case GETADDR_GROUPLIST_ONEADD:
                (void)ICONTAPP_SelectFieldEx(pContApp, (PFNSELECTEDCONTACTCB)GetNumCallback, SINGLE_SELECT_GROUPLIST);
                break;
            
            default:
                break;
        }

        (void)ICONTAPP_Release(pContApp);
    } 
#endif       
}

#ifdef FEATURE_CONTACT_APP
/*==============================================================================
����:
    GetNumCallback

˵��:
    ���ڴ�ͨ�Ų�ȡ����Ļص�������

����:
    selRec [in]: ѡ���ļ�¼��
    selItem [in]: ѡ����¼���ֶΡ�

����ֵ:
    TRUE: Contact applet Ӧ�˳���
    FALSE: Contact applet ���˳���

��ע:
    �������޸�WMS����� m_pSendList �е�ĳ����Ա�
        
==============================================================================*/
static boolean GetNumCallback(sSelectFieldListNode* pNode)
{
    int i;
    int32 nMaxItems = 1;
    CMultiSendItemInfo *pItem = NULL;
    sSelectFieldListNode* pTepNode = NULL;
    AECHAR  *wstrName = NULL;
#if defined(FEATURE_CARRIER_THAILAND_HUTCH)
    int nInserted = 0;
#endif
    
    if (NULL == pNode)
    {
        return TRUE;
    }
    
    if (gpWMSApp->m_eMakeAddListMode != MAKEADDLIST_NONE)
    {
        nMaxItems = MAXNUM_MULTISEND - IVector_Size(gpWMSApp->m_pSendList);
#if defined(FEATURE_CARRIER_THAILAND_HUTCH)
        nInserted = IVector_Size(gpWMSApp->m_pSendList);
#endif        
    }
    
    pTepNode = pNode;
    for (i=0; ((i<nMaxItems) && (NULL != pTepNode)); )
    {
        if (pTepNode->SelectFieldInfo.wFieldMask & MASK_ADDRFIELD_NAME)
        {
            wstrName = WSTRDUP((AECHAR *)pTepNode->SelectFieldInfo.m_pAddNewName);
        }
        
        if ((pTepNode->SelectFieldInfo.wFieldMask & MASK_ADDRFIELD_PHONE_GENERIC) &&
            (NULL != pTepNode->SelectFieldInfo.m_pAddNewMobile) &&
            (WSTRLEN((AECHAR *)(pTepNode->SelectFieldInfo.m_pAddNewMobile))>0))
        {
            if (gpWMSApp->m_currState == WMSST_SENDOPTS)
            {// Callback Number Enter:
                (void)WSTRCPY(gpWMSApp->m_msSend.m_szCallBkNum, (AECHAR *)(pTepNode->SelectFieldInfo.m_pAddNewMobile));
                goto GetNumCallback_Exit;
            }
            
            pItem = (CMultiSendItemInfo *)sys_malloc(sizeof(CMultiSendItemInfo));
            
            if (NULL == pItem)
            {// ���������䲻�ɹ�
                goto GetNumCallback_Exit;
            }
            
            if (SUCCESS != IVector_AddElement(gpWMSApp->m_pSendList, pItem))
            {
                sys_free(pItem);
                goto GetNumCallback_Exit;
            }
            (void)WSTRCPY(pItem->m_szTo, (AECHAR *)(pTepNode->SelectFieldInfo.m_pAddNewMobile));
            if (NULL != wstrName)
            {
                (void)WSTRCPY(pItem->m_szName, wstrName);
            }
            i++;
            pItem = NULL;
            if (i>=nMaxItems)
            {
                goto GetNumCallback_Exit;
            }
        }
        
        if ((pTepNode->SelectFieldInfo.wFieldMask & MASK_ADDRFIELD_PHONE_HOME) &&
            (NULL != pTepNode->SelectFieldInfo.m_pAddNewHome) &&
            (WSTRLEN((AECHAR *)(pTepNode->SelectFieldInfo.m_pAddNewHome))>0))
        {
            if (gpWMSApp->m_currState == WMSST_SENDOPTS)
            {// Callback Number Enter:
                (void)WSTRCPY(gpWMSApp->m_msSend.m_szCallBkNum, (AECHAR *)(pTepNode->SelectFieldInfo.m_pAddNewHome));
                goto GetNumCallback_Exit;
            }
            
            pItem = (CMultiSendItemInfo *)sys_malloc(sizeof(CMultiSendItemInfo));
            
            if (NULL == pItem)
            {// ���������䲻�ɹ�
                goto GetNumCallback_Exit;
            }
            
            if (SUCCESS != IVector_AddElement(gpWMSApp->m_pSendList, pItem))
            {
                sys_free(pItem);
                goto GetNumCallback_Exit;
            }
            (void)WSTRCPY(pItem->m_szTo, (AECHAR *)(pTepNode->SelectFieldInfo.m_pAddNewHome));
            if (NULL != wstrName)
            {
                (void)WSTRCPY(pItem->m_szName, wstrName);
            }
            i++;
            pItem = NULL;
            if (i>=nMaxItems)
            {
                goto GetNumCallback_Exit;
            }
        }
        
        if ((pTepNode->SelectFieldInfo.wFieldMask & MASK_ADDRFIELD_PHONE_WORK) &&
            (NULL != pTepNode->SelectFieldInfo.m_pAddNewOffice) &&
            (WSTRLEN((AECHAR *)(pTepNode->SelectFieldInfo.m_pAddNewOffice))>0))
        {
            if (gpWMSApp->m_currState == WMSST_SENDOPTS)
            {// Callback Number Enter:
                (void)WSTRCPY(gpWMSApp->m_msSend.m_szCallBkNum, (AECHAR *)(pTepNode->SelectFieldInfo.m_pAddNewOffice));
                goto GetNumCallback_Exit;
            }
            
            pItem = (CMultiSendItemInfo *)sys_malloc(sizeof(CMultiSendItemInfo));
            
            if (NULL == pItem)
            {// ���������䲻�ɹ�
                goto GetNumCallback_Exit;
            }
            
            if (SUCCESS != IVector_AddElement(gpWMSApp->m_pSendList, pItem))
            {
                sys_free(pItem);
                goto GetNumCallback_Exit;
            }
            (void)WSTRCPY(pItem->m_szTo, (AECHAR *)(pTepNode->SelectFieldInfo.m_pAddNewOffice));
            if (NULL != wstrName)
            {
                (void)WSTRCPY(pItem->m_szName, wstrName);
            }
            i++;
            pItem = NULL;
            if (i>=nMaxItems)
            {
                goto GetNumCallback_Exit;
            }
        }
        
        if ((pTepNode->SelectFieldInfo.wFieldMask & MASK_ADDRFIELD_PHONE_FAX) &&
            (NULL != pTepNode->SelectFieldInfo.m_pAddNewFax) &&
            (WSTRLEN((AECHAR *)(pTepNode->SelectFieldInfo.m_pAddNewFax))>0))
        {
            if (gpWMSApp->m_currState == WMSST_SENDOPTS)
            {// Callback Number Enter:
                (void)WSTRCPY(gpWMSApp->m_msSend.m_szCallBkNum, (AECHAR *)(pTepNode->SelectFieldInfo.m_pAddNewFax));
                goto GetNumCallback_Exit;
            }
            
            pItem = (CMultiSendItemInfo *)sys_malloc(sizeof(CMultiSendItemInfo));
            
            if (NULL == pItem)
            {// ���������䲻�ɹ�
                goto GetNumCallback_Exit;
            }
            
            if (SUCCESS != IVector_AddElement(gpWMSApp->m_pSendList, pItem))
            {
                sys_free(pItem);
                goto GetNumCallback_Exit;
            }
            (void)WSTRCPY(pItem->m_szTo, (AECHAR *)(pTepNode->SelectFieldInfo.m_pAddNewFax));
            if (NULL != wstrName)
            {
                (void)WSTRCPY(pItem->m_szName, wstrName);
            }
            i++;
            pItem = NULL;
            if (i>=nMaxItems)
            {
                goto GetNumCallback_Exit;
            }
        }
        
        pTepNode = pTepNode->pNextNode;
    }
    
GetNumCallback_Exit:
    if (wstrName != NULL)
    {
        SYS_FREEIF(wstrName);
    }
    
    return TRUE;
}


/*==============================================================================
����:
    InsertNumCallback

˵��:
    
    ���ڴ�ͨ�Ų�ȡ���벢���뵽��ǰ�༭����Ϣ�еĻص�������

����:
    selRec [in]: ѡ���ļ�¼��
    selItem [in]: ѡ����¼���ֶΡ�

����ֵ:
    TRUE:  Contact appletӦ�˳���
    FALSE: Contact applet���˳���

��ע:
    �������޸�WMS����� m_pSendList �е�ĳ����Ա�
        
==============================================================================*/
static boolean InsertNumCallback(sSelectFieldListNode* pNode)
{
    AECHAR* wszTep;
    AECHAR* wszPack;
    AECHAR* szMessage = gpWMSApp->m_msSend.m_szMessage;
    int startPos = gpWMSApp->m_dwInsertPos; 
    int  nLen,nCanInsert,nLenPack;
    int  nMax = 0;
    int  nSize = 0;
    
    if ((NULL == pNode) || 
        (NULL == pNode->SelectFieldInfo.m_pAddNewFormatContent) ||
        (WSTRLEN(pNode->SelectFieldInfo.m_pAddNewFormatContent) == 0))
    {
        return TRUE;
    }
    wszPack = pNode->SelectFieldInfo.m_pAddNewFormatContent;
    
    if (startPos < 0)
    {
        startPos = 0;
    }
    
    nLenPack = WSTRLEN(wszPack);
    
    if (NULL == gpWMSApp->m_msSend.m_szMessage)
    {
        gpWMSApp->m_msSend.m_szMessage = (AECHAR *)sys_malloc((nLenPack+1)* sizeof(AECHAR));
        if (NULL != gpWMSApp->m_msSend.m_szMessage)
        {
            (void)WSTRCPY(gpWMSApp->m_msSend.m_szMessage, wszPack);
        }
        return TRUE;
    }
    nLen = WSTRLEN(szMessage);
    
    gpWMSApp->m_bTextFullAlert = TRUE;
    
    if (gpWMSApp->m_eInsertType == INSERT_EDITWMS)
    {
        nMax = gpWMSApp->m_nMOmaxChars;
    }
    else
    {
        nMax = MAX_TEMPLATECHARS;
    }

    // ���㵱ǰ������������ַ���
    if (nLen >= nMax)
    {// ���ܲ���
        nCanInsert = 0;
    }
    else
    {
        if (gpWMSApp->m_eInsertType == INSERT_EDITWMS)
        {
#if defined FEATURE_CARRIER_THAILAND_HUTCH || defined FEATURE_CARRIER_THAILAND_CAT        
            if (AEE_TM_THAI == gpWMSApp->m_CurInputMode || 
                AEE_TM_THAI_R == gpWMSApp->m_CurInputMode ||
                WMSUtil_HaveNoneASCIIChar(szMessage, NULL))
            {
                nCanInsert = MAX_UNICODMSG_PAYLOAD - nLen;
                
                if (nCanInsert < 1)
                {// ���ܲ���
                    nCanInsert = 0;
                }
                else if (nCanInsert > nLenPack)
                {
                    gpWMSApp->m_bTextFullAlert = FALSE;
                }
            }
            else
            {
                int nPos;
                
                if (nLen >= MAX_ASCIIMSG_PAYLOAD)
                {// ���ܲ���
                    nCanInsert = 0;
                }
                else if (WMSUtil_HaveNoneASCIIChar(wszPack, &nPos))
                {
                    if (nLen + nLenPack < MAX_UNICODMSG_PAYLOAD)
                    {// ��ȫ������
                        nCanInsert = nLenPack;
                        gpWMSApp->m_bTextFullAlert = FALSE;
                    }
                    else if (nLen + nLenPack > MAX_UNICODMSG_PAYLOAD)
                    {// ֻ�ܲ��ֲ���
                        // ��Ϣ���������ַ�, ֻ�ܲ���ǰ��ķǿ��ַ�����
                        nCanInsert = (MAX_ASCIIMSG_PAYLOAD - nLen) >= nPos ? 
                                    nPos : (MAX_ASCIIMSG_PAYLOAD - nLen);
                                        
                        if(nCanInsert+nLen < MAX_ASCIIMSG_PAYLOAD)
                        {
                            gpWMSApp->m_bTextFullAlert = FALSE;
                        }               
                    }
                    else
                    {
                        nCanInsert = nLenPack;
                    }
                }
                else
                {
                    nCanInsert = (MAX_ASCIIMSG_PAYLOAD - nLen) >= nLenPack ? 
                                nLenPack : (MAX_ASCIIMSG_PAYLOAD - nLen);
                                    
                    if (nCanInsert+nLen < MAX_ASCIIMSG_PAYLOAD)
                    {
                        gpWMSApp->m_bTextFullAlert = FALSE;
                    }               
                }
            }
#else
            nCanInsert = nMax - nLen;
            
            if (nCanInsert < 1)
            {// ���ܲ���
                nCanInsert = 0;
            }
            else if (nCanInsert > nLenPack)
            {
                gpWMSApp->m_bTextFullAlert = FALSE;
            }
#endif
        }
        else
        {
            nCanInsert = (nMax - nLen) >= nLenPack ? 
                        nLenPack : (nMax - nLen);
        }
                        
        if (nCanInsert+nLen < nMax)
        {
            gpWMSApp->m_bTextFullAlert = FALSE;
        }               
    }
    
    if (nCanInsert <= 0)
    {
        return TRUE;
    }
    
    nSize = (nLen + nCanInsert + 1) * sizeof(AECHAR);
    wszTep = (AECHAR *)sys_malloc(nSize);
    
    if (wszTep != NULL)
    {
        MEMSET(wszTep, 0, nSize);
        (void)WSTRNCOPYN(wszTep, (startPos+1), szMessage, startPos);
        (void)WSTRNCOPYN(&wszTep[startPos], (nCanInsert+1), wszPack, nCanInsert);
        (void)WSTRCAT(wszTep, &szMessage[startPos]);
        
        wszTep[nLen+nCanInsert] = 0;
        gpWMSApp->m_dwInsertPos = startPos + nCanInsert;
        SYS_FREEIF(szMessage);
        gpWMSApp->m_msSend.m_szMessage = wszTep;
    }
    
    return TRUE;
}
#endif       

/*==============================================================================
����:
    WmsApp_StorePrefs
       
˵��:
    ���浱ǰ Applet �������ݡ�
       
����:
    pMe [in]:ָ�� WMS Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    none
       
��ע:
       
==============================================================================*/
void WmsApp_StorePrefs(WmsApp *pMe) 
{
    if (NULL == pMe) 
    {
        return;
    }

    (void)ISHELL_SetPrefs(pMe->m_pShell, 
            AEECLSID_WMSAPP, 
            WMS_CFG_VERSION,
            &pMe->m_cfg, 
            sizeof(WMSAPP_Config_Type));
}

/*==============================================================================
����:
    WmsApp_GetPrefs
       
˵��:
    ��ȡ Applet �������ݡ�
       
����:
    pMe [in]:ָ�� WMS Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    none
       
��ע:
       
==============================================================================*/
void WmsApp_GetPrefs(WmsApp *pMe) 
{
    if (NULL == pMe) 
    {
        return;
    }

    (void)ISHELL_GetPrefs(pMe->m_pShell, 
            AEECLSID_WMSAPP, 
            WMS_CFG_VERSION,
            &pMe->m_cfg, 
            sizeof(WMSAPP_Config_Type));
}

#ifdef FEATURE_SMS_UDH
/*==============================================================================
����:
    WmsApp_GetMsgRef
       
˵��:
    ��ȡ��Ϣ�ο��š�
       
����:
    pMe [in]:ָ�� WMS Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    none
       
��ע:
       
==============================================================================*/
uint8 WmsApp_GetMsgRef(WmsApp *pMe)
{
    uint8      msg_ref = 0;
    
    if (NULL == pMe) 
    {
        return 255;
    }
    
    WmsApp_GetPrefs(pMe);
    msg_ref = pMe->m_cfg.m_msg_ref;
    
    if (msg_ref == 255)
    {
        pMe->m_cfg.m_msg_ref = 1;
    }
    else
    {
        pMe->m_cfg.m_msg_ref++;
    }
    
    WmsApp_StorePrefs(pMe);
    
    return msg_ref;
}
#endif

/*==============================================================================
����:
    WmsApp_GetMsgIDNum
       
˵��:
    ��ȡ��Ϣ ID �š�
       
����:
    pMe [in]:ָ�� WMS Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    none
       
��ע:
       
==============================================================================*/
uint16 WmsApp_GetMsgIDNum(WmsApp *pMe)
{
    uint16      m_msgid = 0;
    
    if (NULL == pMe) 
    {
        return 1;
    }
    
    WmsApp_GetPrefs(pMe);
    m_msgid = pMe->m_cfg.m_msgid;
    
    if (m_msgid == 0xFFFF)
    {
        pMe->m_cfg.m_msgid = 1;
    }
    else
    {
        pMe->m_cfg.m_msgid++;
    }
    
    WmsApp_StorePrefs(pMe);
    
    return m_msgid;
}

/*==============================================================================
����:
    ConvertIntToBcd
       
˵��:
    ת�������� BCD ����
       
����:
    i [in]: ��ת��������
       
����ֵ:
    ת���� BCD ����
       
��ע:
       
==============================================================================*/
uint8 ConvertIntToBcd(uint16 i)
{
    uint8 bcd;
    
    bcd = ((int)(i/10)) << 4;
    bcd |= i % 10;
    
    return bcd;
}

/*==============================================================================
����:
    ConvertMStoMcTime
       
˵��:
    ������Ϊ��λ��ʱ��ת��Ϊ wms_timestamp_s_type ��ʽ��
       
����:
    sec [in]: ʱ��(��λ: ��)
    pmc_time [in]:ָ�� wms_timestamp_s_type ����ṹ��ָ�롣
       
����ֵ:
    none
       
��ע:
       
==============================================================================*/
void ConvertMStoMcTime(uint32 sec, wms_timestamp_s_type *pmc_time)
{
    uint8      year = 0;
    JulianType julian;
    
    if (NULL == pmc_time)
    {
        return;
    }
    GETJULIANDATE(sec, &julian);
    
    if (julian.wYear <= 1999)
    {
        if (julian.wYear <= 1996)
        {
            year = 96;
        }
        else
        {
            year = julian.wYear - 1900;
        }
    }
    else
    {
        if (julian.wYear >= 2095)
        {
            year = 95;
        }
        else
        {
            year = julian.wYear - 2000;
        }
    }

    pmc_time->year     = ConvertIntToBcd(year);
    pmc_time->month    = ConvertIntToBcd(julian.wMonth);
    pmc_time->day      = ConvertIntToBcd(julian.wDay);
    pmc_time->hour     = ConvertIntToBcd(julian.wHour);
    pmc_time->minute   = ConvertIntToBcd(julian.wMinute);
    pmc_time->second   = ConvertIntToBcd(julian.wSecond);
    pmc_time->timezone = 0;
}

/*==============================================================================
����:
    WmsApp_PackPhrase

˵��:
    �������� WMSMessageStruct �ṹ��Ϣ���� wms_client_message_s_type �ṹģ����Ϣ��

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    pMs [in]: WMSMessageStruct ���ݽṹָ�롣
    bOutTemplate [in]: TRUE--��� cdma_template FALSE--��� cdma_message

����ֵ:
    none

��ע:
    �������� buffer ϵ��̬���䣬�ɵ����߸����ͷš�

==============================================================================*/
wms_client_message_s_type *WmsApp_PackPhrase(WmsApp *pMe, 
                                             WMSMessageStruct *pMs,
                                             boolean bOutTemplate)
{
    wms_client_message_s_type   *pCltMsg = NULL;
    wms_cdma_user_data_s_type   *pUserdata = NULL;
    wms_client_ts_data_s_type   *pCltTsData = NULL;
    int32 nSize = 0;
    
    if ((NULL == pMe) || (NULL == pMs) ||(NULL == pMs->m_szMessage))
    {
        return NULL;
    }
    
    // �ȷ�����Ҫ�� buffer
    nSize = sizeof(wms_cdma_user_data_s_type);
    pUserdata = (wms_cdma_user_data_s_type *)sys_malloc(nSize);
    if (NULL == pUserdata)
    {
        ERR("Malloc Failed!",0,0,0);
        return NULL;
    }
    MEMSET(pUserdata, 0, nSize);
    
    nSize = sizeof(wms_client_ts_data_s_type);
    pCltTsData = (wms_client_ts_data_s_type *)sys_malloc(nSize);
    if (NULL == pCltTsData)
    {
        ERR("sys_malloc Failed!",0,0,0);
        SYS_FREEIF(pCltTsData);
        return NULL;
    }
    MEMSET(pCltTsData, 0, nSize);
    
    nSize = sizeof(wms_client_message_s_type);
    pCltMsg = (wms_client_message_s_type *)sys_malloc(nSize);
    if (NULL == pCltMsg)
    {
        SYS_FREEIF(pUserdata);
        SYS_FREEIF(pCltTsData);
        ERR("sys_malloc Failed!",0,0,0);
        return NULL;
    }
    MEMSET(pCltMsg, 0, nSize);
    
    // ת���� wms_cdma_user_data_s_type �ṹ
    pUserdata->encoding = WMS_ENCODING_UNICODE;
    {
        uint16  *pTep = (uint16 *)pUserdata->data;
        int j, nlen;
        
        nlen = WSTRLEN(pMs->m_szMessage);
        pUserdata->number_of_digits = nlen;
        pUserdata->data_len = nlen * sizeof(AECHAR);
        pUserdata->padding_bits = 0;
        
        for (j=0; j<nlen; j++,pTep++)
        {
            *pTep = HTONS(pMe->m_msSend.m_szMessage[j]);
        }
    }
    
    if (bOutTemplate)
    {
        // -------- ��� wms_client_message_s_type �ṹ --------
        pCltMsg->msg_hdr.message_mode = WMS_MESSAGE_MODE_CDMA;
        pCltMsg->msg_hdr.tag = WMS_TAG_MO_TEMPLATE;
        pCltMsg->msg_hdr.mem_store = WMS_MEMORY_STORE_RUIM;
        
        // ��Ϣ�������ⲿ����
        //pCltMsg->msg_hdr.index = 0;
        
        pCltMsg->u.cdma_template.teleservice = WMS_TELESERVICE_CMT_95;
        pCltMsg->u.cdma_template.mask = WMS_CDMA_TEMPLATE_MASK_BEARER_DATA;
        
        pCltMsg->u.cdma_template.client_bd.mask = WMS_MASK_BD_USER_DATA;
        nSize = sizeof(wms_cdma_user_data_s_type);
        MEMCPY(&pCltMsg->u.cdma_template.client_bd.user_data, pUserdata, nSize);
    }
    else
    {
        // -------- ��� wms_client_ts_data_s_type �ṹ --------
        pCltTsData->format = WMS_FORMAT_CDMA;
        pCltTsData->u.cdma.mask = 0;
    
        // ��Ϣ�û����ݲ���
        nSize = sizeof(wms_cdma_user_data_s_type);
        MEMCPY(&pCltTsData->u.cdma.user_data, pUserdata, nSize);
        pCltTsData->u.cdma.mask |= WMS_MASK_BD_USER_DATA;
        
        // ʱ���
        {
            uint32 sec = GETTIMESECONDS();
            
            ConvertMStoMcTime(sec, &pCltTsData->u.cdma.mc_time);
        }
        pCltTsData->u.cdma.mask |= WMS_MASK_BD_MC_TIME;
        
        // -------- ��� wms_client_message_s_type �ṹ --------
        pCltMsg->msg_hdr.message_mode = WMS_MESSAGE_MODE_CDMA;
        pCltMsg->msg_hdr.tag = WMS_TAG_MO_NOT_SENT;
        pCltMsg->msg_hdr.mem_store = WMS_MEMORY_STORE_NV_CDMA;
        // ��Ϣ�������ⲿ����
        //pCltMsg->msg_hdr.index = 0;
        
        pCltMsg->u.cdma_message.is_mo = TRUE;
        pCltMsg->u.cdma_message.teleservice = WMS_TELESERVICE_CMT_95;
    
        // ���յ�ַ
        pCltMsg->u.cdma_message.address.number_of_digits = 0;
        pCltMsg->u.cdma_message.address.digit_mode = WMS_DIGIT_MODE_4_BIT;
        pCltMsg->u.cdma_message.address.number_mode = WMS_NUMBER_MODE_NONE_DATA_NETWORK;
        pCltMsg->u.cdma_message.address.number_type = WMS_NUMBER_UNKNOWN;
        pCltMsg->u.cdma_message.address.number_plan = WMS_NUMBER_PLAN_TELEPHONY;
        
        pCltMsg->u.cdma_message.is_tl_ack_requested = TRUE;
        pCltMsg->u.cdma_message.is_service_present = FALSE;
    
        (void)IWMS_TsEncode(pMe->m_pwms, pCltTsData, &pCltMsg->u.cdma_message.raw_ts);
    }
    
    // ��Ϣ�������
    
    SYS_FREEIF(pCltTsData);
    SYS_FREEIF(pUserdata);
    
    return pCltMsg;
}

/*==============================================================================
����:
    WmsApp_BuildNumList

˵��:
    ������Ϣ��������������

����:
    pMe [in]: ָ�� WMS Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    pmsMsg [in]:ָ�� WMSMessageStruct �ṹ��ָ�롣

����ֵ:
    none

��ע:
    �������ٶ��������Ϣ�ṹ������ǽ��յ�����Ϣ��
==============================================================================*/
void WmsApp_BuildNumList(WmsApp *pMe, WMSMessageStruct *pmsMsg)
{
    AECHAR    *pItem = NULL;
    int  i,nLen, nStartPos, nChars;
    
    if ((NULL == pMe) || (NULL == pmsMsg))
    {
        return;
    }
    
    // ȡ Originate Number
    nLen = WSTRLEN(pmsMsg->m_szNum);
    if (nLen > 0)
    {
        pItem = WSTRDUP(pmsMsg->m_szNum);
        
        if (NULL != pItem)
        {
            if (IVector_AddElement(pMe->m_pSaveNumList, pItem) != SUCCESS)
            {
                SYS_FREEIF(pItem);
                return;
            }
            
            // ע��pItemָ��ĵ�ַ���������ͷ�
            pItem = NULL;
        }
    }
    
    // ȡ CallBack Number
    nLen = WSTRLEN(pmsMsg->m_szCallBkNum);
    if (nLen > 0)
    {
        pItem = WSTRDUP(pmsMsg->m_szCallBkNum);
        
        if (NULL != pItem)
        {
            if (IVector_AddElement(pMe->m_pSaveNumList, pItem) != SUCCESS)
            {
                SYS_FREEIF(pItem);
                return;
            }
            
            // ע��pItemָ��ĵ�ַ���������ͷ�
            pItem = NULL;
        }
    }
    
    nStartPos = -1;
    // ������Ϣ�壬��ȡ����
    if (NULL == pmsMsg->m_szMessage)
    {
        nLen = 0;
    }
    else
    {
        nLen = WSTRLEN(pmsMsg->m_szMessage);
    }
    for (i=0; i<nLen; i++)
    {
        if (pmsMsg->m_szMessage[i] >= (AECHAR)'0' &&
            pmsMsg->m_szMessage[i] <= (AECHAR)'9')
        {// ��ǰ�ַ�Ϊ����
            if (nStartPos < 0)
            {
                nStartPos = i;
            }
            
            // ��Ҫ���жϣ��������λ����Ϣ��β��ʱ��ȡ����
            if (i != nLen-1)
            {
                continue;
            }
            else
            {
                i++;
            }
        }
        
        if (nStartPos < 0)
        {
            continue;
        }
        nChars = i-nStartPos;
        if (nChars < 3 || nChars >= MAX_PH_DIGITS)
        { // �������ִ�����С��3�򲻵���MAX_PH_DIGITS���˺�����Ч
            nStartPos = -1;
            continue;
        }
        
        // ��ȡ��Ч����
        pItem = (AECHAR *)sys_malloc((nChars+1)*sizeof(AECHAR));
        if ((nStartPos > 0) && (pmsMsg->m_szMessage[nStartPos-1]==(AECHAR)'+'))
        {
            nStartPos -= 1;
            nChars++;
        }
        if (NULL != pItem)
        {
            (void)WSTRNCOPYN(pItem, (nChars+1), &(pmsMsg->m_szMessage[nStartPos]), nChars);
            
            if (IVector_AddElement(pMe->m_pSaveNumList, pItem) != SUCCESS)
            {
                SYS_FREEIF(pItem);
                return;
            }
            
            // ע��pItemָ��ĵ�ַ���������ͷ�
            pItem = NULL;
        }
        
        // ������ʼ���
        nStartPos = -1;
    }
}

/*==============================================================================
����:
    WmsApp_BuildMailAddList

˵��:
    ���������ַ��������������ʼ���ַ����

����:
    pMe [in]: ָ�� WMS Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    wstrText [in]: �������ַ���ָ�롣

����ֵ:
    none

��ע:
    �ʼ���ַ������ֻ���� "(),:;<>[\] ֮�����ֵ����0x20��С��0x7F��ASCII �ַ�,
    �ʼ���ַ��󳤶�Ϊ64���ַ���
    ����ͨ<<��ý���ʼ��ն˹��ܹ淶>>
    
    �� WmsApp_BuildNumList �������� m_pSaveNumList �������ô˺���ǰӦ��������
    ��ա�
==============================================================================*/
void WmsApp_BuildMailAddList(WmsApp *pMe, AECHAR *wstrText)
{
    AECHAR    *pItem = NULL;
    AECHAR    wchAt = (AECHAR)'@';
    AECHAR    *wstrAt = NULL;
    AECHAR    *wstrStartPos = NULL;
    AECHAR    *wstrEndPos = NULL;
    AECHAR    *wstrTep = NULL;
    AECHAR    wstrInvaild[]={0x22,0x28,0x29,0x2c,0x3a,0x3b,0x3c,0x3e,0x5b,0x5c,0x5d,0};
    int       nLen, nChars;
    
    if ((NULL == pMe) || (NULL == wstrText))
    {
        return;
    }
    
    nLen = WSTRLEN(wstrText);
    if (nLen == 0)
    {
        return;
    }
    
    wstrAt = WSTRCHR(wstrText, wchAt);
    wstrStartPos = wstrText;
    wstrEndPos = &wstrText[nLen-1];
    while (NULL != wstrAt)
    {
        // ȷ����ַǰ�沿��
        wstrTep = wstrAt-1;
        
        for (; wstrTep>=wstrStartPos; wstrTep--)
        {
            if (((*wstrTep) <= 0x20) || 
                ((*wstrTep) >= 0x7F) || 
                (WSTRCHR(wstrInvaild, (*wstrTep)) != NULL))
            {// ������Ч�ַ�
                break;
            }
        }
        
        if (wstrTep>=wstrStartPos)
        {
            wstrStartPos = wstrTep+1;
        }
        
        // ȷ����ַ���沿��
        wstrTep = wstrAt+1;
        
        for (; wstrTep<=wstrEndPos; wstrTep++)
        {
            if (((*wstrTep) <= 0x20) || 
                ((*wstrTep) >= 0x7F) || 
                ((*wstrTep) == wchAt) || 
                (WSTRCHR(wstrInvaild, (*wstrTep)) != NULL))
            {// ������Ч�ַ�
                break;
            }
        }
        
        if (wstrTep<=wstrEndPos)
        {
            wstrEndPos = wstrTep-1;
        }
        
        if ((wstrStartPos < wstrAt) && 
            (wstrEndPos > wstrAt) &&
            (wstrEndPos-wstrStartPos<=MAX_EMAILADD_LEN))
        {// ��ȡ����Ч�����ʼ���ַ
            nChars = (wstrEndPos - wstrStartPos) + 1;
            
            pItem = (AECHAR *)sys_malloc(((nChars+1)*sizeof(AECHAR)));
            if (NULL != pItem)
            {
                (void)WSTRNCOPYN(pItem, (nChars+1), wstrStartPos, nChars);
                
                if (IVector_AddElement(pMe->m_pSaveNumList, pItem) != SUCCESS)
                {
                    SYS_FREEIF(pItem);
                    return;
                }
                
                //ERR("--%d", WSTRLEN(pItem),0,0);
                
                // ע��pItemָ��ĵ�ַ���������ͷ�
                pItem = NULL;
            }
        }
        
        wstrStartPos = (wstrEndPos+1);
        if (wstrStartPos < &wstrText[nLen-1])
        {
            wstrEndPos = &wstrText[nLen-1];
            wstrAt = WSTRCHR(wstrStartPos, wchAt);
        }
        else
        {
            wstrAt = NULL;// ������ֹ while ѭ��������
        }
    }
}

/*==============================================================================
����:
    WmsApp_BuildUrlList

˵��:
    ���������ַ���������URL����

����:
    pMe [in]: ָ�� WMS Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    wstrText [in]: �������ַ���ָ�롣

����ֵ:
    none

��ע:
    URL������ֻ���� "<> ֮�����ֵ����0x20��С��0x7F��ASCII �ַ�, ����󳤶ȵ���
    256���ַ���
    ����ͨ<<��ý���ʼ��ն˹��ܹ淶>>
==============================================================================*/
void WmsApp_BuildUrlList(WmsApp *pMe, AECHAR *wstrText)
{
    AECHAR    *pItem = NULL;
    AECHAR    *wstrIn = NULL;
    AECHAR    wstrInvaild[]={0x22,0x3c,0x3e,0};
    int       nLen, nCopys, nStartPos, nEndPos;
    int       i;
    
    if ((NULL == pMe) || (NULL == wstrText))
    {
        return;
    }
    
    nLen = WSTRLEN(wstrText);
    if (nLen < 8)
    {
        return;
    }
    
    wstrIn = WSTRDUP(wstrText);
    if (NULL == wstrIn)
    {
        return;
    }
    WSTRLOWER(wstrIn);
    
    nEndPos = nLen-1;
    for (i=nLen-1; i>=6; i--)
    {
        if (wstrText[i] <= 0x20 || 
            wstrText[i] >= 0x7F || 
            WSTRCHR(wstrInvaild, wstrText[i]) != NULL)
        {
            nEndPos = i;
            continue;
        }
        
        if ((wstrIn[i] == (AECHAR)'/') &&
            (wstrIn[i-1] == (AECHAR)'/') &&
            (wstrIn[i-2] == (AECHAR)':') &&
            (wstrIn[i-3] == (AECHAR)'p') &&
            (wstrIn[i-4] == (AECHAR)'t') &&
            (wstrIn[i-5] == (AECHAR)'t') &&
            (wstrIn[i-6] == (AECHAR)'h'))
        {
            nStartPos = i-6;
        }
        else if (i<7)
        {
            break;
        }
        else if ((wstrIn[i] == (AECHAR)'/') &&
                 (wstrIn[i-1] == (AECHAR)'/') &&
                 (wstrIn[i-2] == (AECHAR)':') &&
                 (wstrIn[i-3] == (AECHAR)'s') &&
                 (wstrIn[i-4] == (AECHAR)'p') &&
                 (wstrIn[i-5] == (AECHAR)'t') &&
                 (wstrIn[i-6] == (AECHAR)'t') &&
                 (wstrIn[i-7] == (AECHAR)'h'))
        {
            nStartPos = i-7;
        }
        else
        {
            continue;
        }
        
        nCopys = nEndPos - nStartPos + 1;
        nCopys = (nCopys > MAX_URL_LEN) ? MAX_URL_LEN : nCopys;
            
        pItem = (AECHAR *)sys_malloc(((nCopys+1)*sizeof(AECHAR)));
        if (NULL != pItem)
        {
            (void)WSTRNCOPYN(pItem, (nCopys+1), &wstrText[nStartPos], nCopys);
            
            if (IVector_AddElement(pMe->m_pSaveNumList, pItem) != SUCCESS)
            {
                SYS_FREEIF(pItem);
            }
            else
            {
                ERR("--%d", WSTRLEN(pItem),0,0);
            }
            
            // ע��pItemָ��ĵ�ַ���������ͷ�
            pItem = NULL;
        }
        
        nEndPos = nStartPos;
        i = nStartPos;
    }
    
    SYS_FREEIF(wstrIn);
}

/*==============================================================================
����:
    GetMonthDays

˵��:
    ��������ָ����ָ����������������

����:
    nYear [in]: �ꡣ
    nMonth [in]: �¡�

����ֵ:
    none

��ע:

==============================================================================*/
int GetMonthDays(int nYear, int nMonth)
{
    if (nMonth == 2)
    {
        if ((((nYear)%4)==0) && (((nYear)%100) || (((nYear)%400)==0)))
        {
            return 29;
        }
        else
        {
            return 28;
        }
    }
    
    nMonth = (nMonth > 2) ? nMonth-3 : nMonth+9;
    
    return ((nMonth % 5) & 1) ? 30 : 31;
}

#ifdef FEATURE_CARRIER_TAIWAN_APBW
/*==============================================================================
����: 
    WMSUtil_GetContactName

˵��: 
    ����������̫��Ƶ�ۺ�ɫ�����³ɹ�����ʾ�ַ��������������⡣

����: 
    ruimdata [in/out]: 
        Layout of ruimdata
        0: tag/status 
        1: len 
        2+: data

����ֵ: 
    TRUE:  ������߸��´���Ϣ
    FALSE: �����߲�����´���Ϣ

��ע: 

==============================================================================*/
boolean ChkUpdate(uint8 *ruimdata)
{
    wms_msg_ruim_sms_s_type     *ruim_sms= NULL;
    wms_tl_message_type         *cdma_tl = NULL;
    wms_cdma_message_s_type     *cdma_cl = NULL;
    wms_client_ts_data_s_type   *cl_data = NULL;
    wms_client_bd_s_type        *info_data = NULL;
    wms_cdma_user_data_s_type   *pUserdata = NULL;
    boolean bRet = FALSE;
    int nlen;
    
    if (NULL == ruimdata)
    {
        return bRet;
    }
    
    ruim_sms = (wms_msg_ruim_sms_s_type *)sys_malloc(sizeof(wms_msg_ruim_sms_s_type));
    if (ruim_sms == NULL)
    {
        goto ChkUpdate_Exit;
    }
    cdma_tl = (wms_tl_message_type *)sys_malloc(sizeof(wms_tl_message_type));
    if (cdma_tl == NULL)
    {
        goto ChkUpdate_Exit;
    }
    cdma_cl = (wms_cdma_message_s_type *)sys_malloc(sizeof(wms_cdma_message_s_type));
    if (cdma_cl == NULL)
    {
        goto ChkUpdate_Exit;
    }
    info_data = (wms_client_bd_s_type *)sys_malloc(sizeof(wms_client_bd_s_type));
    if (info_data == NULL)
    {
        goto ChkUpdate_Exit;
    }
    cl_data = (wms_client_ts_data_s_type *)sys_malloc(sizeof(wms_client_ts_data_s_type));
    if (cl_data == NULL)
    {
        goto ChkUpdate_Exit;
    }
    
    ruim_sms->ota.format   = WMS_FORMAT_CDMA;
    ruim_sms->status       = ruimdata[0];
    ruim_sms->ota.data_len = ruimdata[1];
    MEMCPY((void*)ruim_sms->ota.data, ruimdata+2, ruim_sms->ota.data_len);
    if (wms_ts_decode_CDMA_tl(&ruim_sms->ota, cdma_tl, &cdma_cl->raw_ts) != WMS_OK_S)
    {
        goto ChkUpdate_Exit;
    }
    wms_ts_convert_tl2cl(cdma_tl, cdma_cl);
    
    if (wms_ts_decode(&cdma_cl->raw_ts, cl_data) != WMS_OK_S)
    {
        goto ChkUpdate_Exit;
    }
    
    if (cl_data->u.cdma.mask & WMS_MASK_BD_USER_DATA) 
    {
        pUserdata = (wms_cdma_user_data_s_type*)&cl_data->u.cdma.user_data;
        
        switch (cl_data->u.cdma.user_data.encoding)
        {
            case WMS_ENCODING_ASCII:
            case WMS_ENCODING_IA5:
                nlen = wms_ts_unpack_ascii(pUserdata,
                            pUserdata->number_of_digits+1,
                            info_data->user_data.data);
                info_data->user_data.data[nlen+1] = 0;

                if (STRSTR((char *)(info_data->user_data.data), "Succe") != NULL)
                {
                    bRet = TRUE;
                    (void)STRCPY((char *)info_data->user_data.data, "Update Successful");
                    pUserdata->data_len = STRLEN("Update Successful");
                    pUserdata->number_of_digits = wms_ts_pack_ascii((char *)(info_data->user_data.data),
                                                                pUserdata->data,
                                                                &pUserdata->data_len,
                                                                &pUserdata->padding_bits);
                }
                break;
                
            case WMS_ENCODING_UNICODE:
                OEMWMS_ConvertFromUnicode(&cl_data->u.cdma);
                info_data->user_data.data[0] = 0;
                (void)WSTRTOSTR((AECHAR *)(pUserdata->data),
                                (char *)info_data->user_data.data,
                                sizeof(info_data->user_data.data));
                
                if (STRSTR((char *)(info_data->user_data.data), "Succe") != NULL)
                {
                    uint16  *pTep = (uint16 *)pUserdata->data;
                    int i;
                    
                    bRet = TRUE;
                    (void)STRTOWSTR("Update Successful", 
                                    (AECHAR *)(pUserdata->data),
                                    sizeof(pUserdata->data));
                    
                    pUserdata->number_of_digits = STRLEN("Update Successful");
                    for (i=0; i<pUserdata->number_of_digits; i++, pTep++)
                    {
                        *pTep = HTONS(*pTep);
                    }
                    pUserdata->data_len = pUserdata->number_of_digits*sizeof(AECHAR);
                    pUserdata->padding_bits = 0;
                }
                break;
                
            default:
                pUserdata->data[pUserdata->data_len] = 0;
                if (STRSTR((char *)(pUserdata->data), "Succe") != NULL)
                {
                    bRet = TRUE;
                    
                    (void)STRCPY((char *)pUserdata->data, "Update Successful");
                    pUserdata->number_of_digits = STRLEN("Update Successful");
                    pUserdata->data_len = pUserdata->number_of_digits;
                    pUserdata->padding_bits = 0;
                }
                break;
        }
    }
    
    if (bRet)
    {
        if (wms_ts_encode(cl_data, &cdma_cl->raw_ts) == WMS_OK_S)
        {
            wms_ts_convert_cl2tl(cdma_cl, cdma_tl);
            
            if (wms_ts_encode_CDMA_tl(cdma_tl, &cdma_cl->raw_ts, &ruim_sms->ota) == WMS_OK_S)
            {
                ruimdata[1] = ruim_sms->ota.data_len;
                MEMCPY(ruimdata+2, (void*)ruim_sms->ota.data, ruim_sms->ota.data_len);
            }
            else
            {
                bRet = FALSE;
            }
        }
        else
        {
            bRet = FALSE;
        }
    }
    
ChkUpdate_Exit:    
    SYS_FREEIF(ruim_sms);
    SYS_FREEIF(cdma_tl);
    SYS_FREEIF(cdma_cl);
    SYS_FREEIF(info_data);
    SYS_FREEIF(cl_data);
    
    return bRet;
}
#endif

/*==============================================================================
����:
    WmsApp_SetMaxSize

˵��:
    �����������������

����:
    pIText, ��ǰ��text control��

����ֵ:
    ��ǰ�ı��ؼ��������������ַ�����
��ע:

==============================================================================*/
int WmsApp_SetMaxSize(ITextCtl *pIText)
{
    int nMaxchars = WMS_MSG_MAXCHARS;
    
    if (!pIText)
    {
        return 0;
    }
        
#if defined FEATURE_CARRIER_THAILAND_HUTCH || defined FEATURE_CARRIER_THAILAND_CAT        
    {
        AECHAR *pwstrText;
        int nLen = 0;
        int nInputMode;
        wms_user_data_encoding_e_type encoding = WMS_ENCODING_ASCII;    
        
        pwstrText = ITEXTCTL_GetTextPtr ( pIText );
        nInputMode = ITEXTCTL_GetInputMode ( pIText, NULL );
        if ( NULL != pwstrText )
        {
            nLen = WSTRLEN ( pwstrText );
        }
        if (WMSUtil_HaveNoneASCIIChar(pwstrText, NULL) )
        {
            encoding = WMS_ENCODING_UNICODE;
        }
        ERR("encoding (%d),nLen (%d),nInputMode (%d)",encoding,nLen,nInputMode);
        // Thai input mode, or Encoding is Unicode, then Mex size is 70
        if (encoding == WMS_ENCODING_ASCII && nLen>MAX_UNICODMSG_PAYLOAD)
        {// ���ڳ��ȳ��� MAX_UNICODMSG_PAYLOAD ���ı���������Ϊ WMS_ENCODING_ASCII
         // ����ضϣ��������Ϊ MAX_ASCIIMSG_PAYLOAD �����ಿ���Խضϣ����轫����
         // ��������������UNICODE �����뷨; �����ж���Ϣ
            nMaxchars = MAX_ASCIIMSG_PAYLOAD;
        }
        else if (AEE_TM_THAI == nInputMode || AEE_TM_THAI_R == nInputMode || WMS_ENCODING_UNICODE == encoding) 
        {
            nMaxchars = MAX_UNICODMSG_PAYLOAD;
        }
        else
        {
            nMaxchars = MAX_ASCIIMSG_PAYLOAD;
        }
        ERR("WmsApp_SetMaxSize......... %d",nMaxchars,0,0);
    }
#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH || defined FEATURE_CARRIER_THAILAND_CAT

#if defined(FEATURE_CARRIER_ANGOLA_MOVICEL)
    {
        byte  mo_encoding=0;
        
        (void) OEM_GetConfig(CFGI_SMS_MO_ENCODING,
                             &mo_encoding,
                             sizeof(mo_encoding));
        if (mo_encoding == OEMNV_SMS_MO_ENCODING_7BIT )
        {
            nMaxchars = MAX_ASCIIMSG_PAYLOAD;
        }
        else
        {
            nMaxchars = MAX_OCTETMSG_PAYLOAD;
        }
    }
#endif //FEATURE_CARRIER_ANGOLA_MOVICEL

    ITEXTCTL_SetMaxSize(pIText, nMaxchars);
    
    return nMaxchars;
}

#ifdef FEATURE_CARRIER_ISRAEL_PELEPHONE
// �����ַ�(�س���)
#define   INSERT_AECHAR         0x000A 

// �����ַ���
#define   AECHARS_STEP          16 

// �����ַ�ʱ�������ַ�(�ո�)
#define   CONDTION_AECHAR       0x0020 

extern void RbmpBiDiReOrder_WMS(AECHAR *wstrIn, 
                                AECHAR *wstrOut, 
                                int32 nInLen,
                                int32 ABaseLevel, 
                                int32 AReOrderOption);

/*==============================================================================
����:
    wmsutil_formatmessage

˵��:
    Ӧ��ɫ��Ҫ���ʽ�������Ķ����ı���

����:
    pclt_msg [in/out]: wms_client_message_s_type ��ָ�롣

����ֵ:
    none
    
��ע:

==============================================================================*/
void wmsutil_formatmessage(wms_client_message_s_type *pclt_msg)
{
    WMSMessageStruct *pMs = NULL;
    AECHAR           *wtrBuf = NULL;
    AECHAR           *wtrP = NULL;
    AECHAR           *wtrP1 = NULL;
    AECHAR           wstrTep[AECHARS_STEP+4] = {0,};
    AECHAR           wTep;
    int32            nSize = 0;
    int nLen, i, j, nBufLen, nTepLen, nPos, nBufMax;
    wms_client_ts_data_s_type *pclt_ts = NULL;
    wms_cdma_user_data_s_type *puserdata = NULL;
    int              nOpt; // 0-�޲��� 1-���� 2-�滻 ����ֵ-�޶���
    
    if ((NULL == pclt_msg) || AECHARS_STEP < 10)
    {
        return;
    }
    
    pMs = (WMSMessageStruct *)sys_malloc(sizeof(WMSMessageStruct));
    if (NULL == pMs)
    {
        return;
    }
    
    WmsApp_ConvertClientMsgToMS(pclt_msg, pMs);
    if (NULL == pMs->m_szMessage)
    {
        SYS_FREEIF(pMs);
        return;
    }
    
    nLen = WSTRLEN(pMs->m_szMessage);
    if (nLen == 0)
    {
        goto FORMATMESSAGE_EXIT;
    }
    nBufLen = 3*(nLen+1)+3;
    nBufMax = nBufLen;
    nSize = nBufLen*sizeof(AECHAR);
    wtrBuf = (AECHAR *)sys_malloc(nSize);
    
    if (NULL == wtrBuf)
    {
        goto FORMATMESSAGE_EXIT;
    }
    MEMSET(wtrBuf, 0, nSize);
    
    pclt_ts = (wms_client_ts_data_s_type *)sys_malloc(sizeof(wms_client_ts_data_s_type));
    if (NULL == pclt_ts)
    {
        goto FORMATMESSAGE_EXIT;
    }
    MEMSET(pclt_ts, 0, sizeof(wms_client_ts_data_s_type));
    if (WMS_OK_S != wms_ts_decode(&pclt_msg->u.cdma_message.raw_ts, pclt_ts))
    {
        goto FORMATMESSAGE_EXIT;
    }
    
    puserdata = &pclt_ts->u.cdma.user_data;
    
    nPos = 0;
    j = 0;
    do
    {
        nOpt = 0;
        i = nPos + AECHARS_STEP;
        MEMSET(wstrTep, 0, sizeof(wstrTep));
        if (i < nLen)
        {
            wTep = pMs->m_szMessage[i];
            if (wTep != CONDTION_AECHAR)
            {// �ǿո񣬻��ݿ��ܷ��ҵ������ַ�
                wTep = pMs->m_szMessage[i];
                pMs->m_szMessage[i] = 0;
                wtrP = WSTRRCHR(&pMs->m_szMessage[nPos], CONDTION_AECHAR);
                pMs->m_szMessage[i] = wTep;
                
                nOpt = 1;// ����
                if ((NULL != wtrP) && (wtrP != &pMs->m_szMessage[nPos]))
                {
                    for (wtrP1 = &pMs->m_szMessage[nPos]; wtrP1 < wtrP; wtrP1++)
                    {// ����ַ wtrP ǰ�Ƿ�ȫΪ CONDTION_AECHAR
                        if (*wtrP1 != CONDTION_AECHAR)
                        {
                            break;
                        }
                    }
                    
                    if (wtrP1 != wtrP)
                    {
                        i = nPos + (wtrP - &pMs->m_szMessage[nPos]);
                        nOpt = 2;// �滻
                    }
                }
            }
            else
            {
                nOpt = 2;// �滻
            }

            wTep = pMs->m_szMessage[i];
            pMs->m_szMessage[i] = 0;
            (void)WSTRCPY(wstrTep, &pMs->m_szMessage[nPos]);
            pMs->m_szMessage[i] = wTep;
        }
        else
        {// ���һ���ֲ���Ҫ����
            (void)WSTRCPY(wstrTep, &pMs->m_szMessage[nPos]);
            i = nLen;
        }
        
        nTepLen = WSTRLEN(wstrTep);
        if (nTepLen > 0)
        {
            RbmpBiDiReOrder_WMS(wstrTep, &wtrBuf[j], WSTRLEN(wstrTep), 1, 0);
            j = WSTRLEN(wtrBuf);
            
            if (nOpt != 0)
            {
                wtrBuf[j] = INSERT_AECHAR;
                j++;
            }
            nBufLen = nBufMax-j;
        }
        
        nPos = i+1;
    } while (nPos < nLen);
    
    // ���´����Ϣ
    puserdata->encoding = WMS_ENCODING_LATIN_HEBREW;
    {
        byte *pTep = (byte *)puserdata->data;
        
        puserdata->number_of_digits = j;
        puserdata->data_len = j;
        puserdata->padding_bits = 0;
        
        for (i=0; i<j; i++,pTep++)
        {
            if (wtrBuf[i] == 0x203E)
            {
                *pTep = 0xAF;
            }
            else if (wtrBuf[i] == 0x00D7)
            {
                *pTep = 0xAA;
            }
            else
            {
                wTep = Ucs2ToHebrew[wtrBuf[i]&0x00FF];
                
                *pTep = (wTep == NON_HEB ? 0x20 : (byte)wTep);
            }
        }
    }
    (void) wms_ts_encode(pclt_ts, &pclt_msg->u.cdma_message.raw_ts);
    
FORMATMESSAGE_EXIT:
    if (NULL != pMs)
    {
        SYS_FREEIF(pMs->m_szMessage);
        SYS_FREEIF(pMs);
    }
    SYS_FREEIF(pclt_ts);
    SYS_FREEIF(wtrBuf);
}

/*==============================================================================
����:
    wmsutil_decodemessagefromview2logic

˵��:
    ����Ϣ�ɿ���˳��תΪ�߼�˳��

����:
    pclt_msg [in/out]: wms_client_message_s_type ��ָ�롣

����ֵ:
    none
    
��ע:
    �������Ǻ��� wmsutil_formatmessage ���������

==============================================================================*/
void wmsutil_decodemessagefromview2logic(wms_client_message_s_type *pclt_msg)
{
    WMSMessageStruct *pMs = NULL;
    AECHAR           *wtrBuf = NULL;
    AECHAR           *wtrP = NULL;
    AECHAR           wTep;
    int32            nSize = 0;
    int nLen, i, j, nBufLen, nPos, nBufMax;
    wms_client_ts_data_s_type *pclt_ts = NULL;
    wms_cdma_user_data_s_type *puserdata = NULL;
    
    if (NULL == pclt_msg)
    {
        return;
    }
    
    pMs = (WMSMessageStruct *)sys_malloc(sizeof(WMSMessageStruct));
    if (NULL == pMs)
    {
        return;
    }
    
    WmsApp_ConvertClientMsgToMS(pclt_msg, pMs);
    if (NULL == pMs->m_szMessage)
    {
        SYS_FREEIF(pMs);
        return;
    }
    
    nLen = WSTRLEN(pMs->m_szMessage);
    nBufLen = 3*(nLen+1)+3;
    nBufMax = nBufLen;
    nSize = nBufLen*sizeof(AECHAR);
    wtrBuf = (AECHAR *)sys_malloc(nSize);
    if (NULL == wtrBuf)
    {
        goto DECODEMESSAGEFROMVIEW2LOGIC_EXIT;
    }
    MEMSET(wtrBuf, 0, nSize);
    
    pclt_ts = (wms_client_ts_data_s_type *)sys_malloc(sizeof(wms_client_ts_data_s_type));
    if (NULL == pclt_ts)
    {
        goto DECODEMESSAGEFROMVIEW2LOGIC_EXIT;
    }
    MEMSET(pclt_ts, 0, sizeof(wms_client_ts_data_s_type));
    if (WMS_OK_S != wms_ts_decode(&pclt_msg->u.cdma_message.raw_ts, pclt_ts))
    {
        goto DECODEMESSAGEFROMVIEW2LOGIC_EXIT;
    }
    puserdata = &pclt_ts->u.cdma.user_data;
    
    nPos = 0;
    j = 0;
    do
    {
        wtrP = WSTRCHR(&pMs->m_szMessage[nPos], INSERT_AECHAR);
        
        if (wtrP == NULL)
        {
            RbmpBiDiReOrder_WMS(&pMs->m_szMessage[nPos], &wtrBuf[j], WSTRLEN(&pMs->m_szMessage[nPos]), 0, 1);
            nPos = nLen;
        }
        else
        {
            if (wtrP == &pMs->m_szMessage[nPos])
            {
                wtrBuf[j] = CONDTION_AECHAR;
                j++;
                nPos++;
                nBufLen--;
            }
            else
            {
                wTep = *wtrP;
                *wtrP = 0;
                RbmpBiDiReOrder_WMS(&pMs->m_szMessage[nPos], &wtrBuf[j], WSTRLEN(&pMs->m_szMessage[nPos]), 0, 1);
                j = WSTRLEN(wtrBuf);
                wtrBuf[j] = CONDTION_AECHAR;
                j++;
                wtrBuf[j] = 0;
                nBufLen = nBufMax - j;
                *wtrP = wTep;
                nPos =  (wtrP + 1) - pMs->m_szMessage;
            }
        }
    } while (nPos < nLen);
        
    
    // ���´����Ϣ
    puserdata->encoding = WMS_ENCODING_LATIN_HEBREW;
    {
        byte *pTep = (byte *)puserdata->data;
        
        j = WSTRLEN(wtrBuf);
        puserdata->number_of_digits = j;
        puserdata->data_len = j;
        puserdata->padding_bits = 0;
        
        for (i=0; i<j; i++,pTep++)
        {
            if (wtrBuf[i] == 0x203E)
            {
                *pTep = 0xAF;
            }
            else if (wtrBuf[i] == 0x00D7)
            {
                *pTep = 0xAA;
            }
            else
            {
                wTep = Ucs2ToHebrew[wtrBuf[i]&0x00FF];
                
                *pTep = (wTep == NON_HEB ? 0x20 : (byte)wTep);
            }
        }
    }
    (void) wms_ts_encode(pclt_ts, &pclt_msg->u.cdma_message.raw_ts);
    
DECODEMESSAGEFROMVIEW2LOGIC_EXIT:
    if (NULL != pMs)
    {
        SYS_FREEIF(pMs->m_szMessage);
        SYS_FREEIF(pMs);
    }
    SYS_FREEIF(pclt_ts);
    SYS_FREEIF(wtrBuf);
}
#endif
/*==============================================================================
����:
    GetNumCallbackFromRecentCall

˵��:
    ���ڴ�ͨ����ʷ��¼ȡ����Ļص�������

����:
    pNode [in]: ѡ����¼���ֶΡ�

����ֵ:
    TRUE: recentcalls applet Ӧ�˳���
    FALSE: recentcalls applet ���˳���

��ע:
    �������޸�WMS����� m_pSendList �е�ĳ����Ա�
        
==============================================================================*/
static boolean GetNumCallbackFromRecentCall(sSelectRecordListNode* pNode)
{
    int32    nMaxItems = 0;
    CMultiSendItemInfo *pItem = NULL;
    sSelectRecordListNode *pSelectRecordNode = pNode;
    
    if(NULL == pNode)
    {
        return TRUE;
    }
    //if (gpWMSApp->m_eMakeAddListMode != MAKEADDLIST_NONE)
    //{
    nMaxItems = MAXNUM_MULTISEND - IVector_Size(gpWMSApp->m_pSendList);
    //}


    if(nMaxItems <= 0)
    {
        return TRUE;
    }

    while(NULL != pSelectRecordNode)
    {
        if(NULL != pSelectRecordNode->SelectRecord.number && (WSTRLEN((AECHAR *)pSelectRecordNode->SelectRecord.number)>0))
        {
            
            if (gpWMSApp->m_currState == WMSST_SENDOPTS)
            {// Callback Number Enter:
                (void)WSTRCPY(gpWMSApp->m_msSend.m_szCallBkNum, (AECHAR *)(pSelectRecordNode->SelectRecord.number));
                return TRUE;
            }/**/
            
            pItem = (CMultiSendItemInfo *)sys_malloc(sizeof(CMultiSendItemInfo));
            
            if (NULL == pItem)
            {// ���������䲻�ɹ�
                return TRUE;
            }

            if (SUCCESS != IVector_AddElement(gpWMSApp->m_pSendList, pItem))
            {
                sys_free(pItem);
                return TRUE;
            }
            (void)WSTRCPY(pItem->m_szTo, (AECHAR *)(pSelectRecordNode->SelectRecord.number));
            if(WSTRLEN(pSelectRecordNode->SelectRecord.name)>0 && NULL != pSelectRecordNode->SelectRecord.name)
            {
                (void)WSTRCPY(pItem->m_szName, (AECHAR *)(pSelectRecordNode->SelectRecord.name));
            }
            else
            {
                (void)WSTRCPY(pItem->m_szName, (AECHAR *)(pSelectRecordNode->SelectRecord.number));
            }
            pItem = NULL;
        }
        
        nMaxItems--;
        if(nMaxItems <= 0)
        {
            break;
        }

        pSelectRecordNode = pSelectRecordNode->pNextNode;
    }

    return TRUE;

}
/*==============================================================================
����:
    WMSExtApp_GetNumFromRecentCalls

˵��:
    ���������ڴ����ͨ����¼ȡ���롣

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    �ޡ�

��ע:
    ����RecentCalls Applet��WMS applet������
        
==============================================================================*/
void WMSExtApp_GetNumFromRecentCalls(WmsApp *pMe, GetAddr_Type eType)
{
    IRecentCalls   *pRecentCall = NULL;

    if (SUCCESS == ISHELL_CreateInstance(pMe->m_pShell,
        AEECLSID_APP_RECENTCALL,
        (void**)&pRecentCall))
    {
        if (pRecentCall == NULL)
        {
            return;
        }
        
        gpWMSApp = pMe;
        switch(eType)
        {
            case GETADDR_MULTIADD:
                IRecentCalls_CallList(pRecentCall, GetNumCallbackFromRecentCall, SELECT_MULTI_NUMBER);
                break;

            case GETADDR_ONEADD:
                IRecentCalls_CallList(pRecentCall, GetNumCallbackFromRecentCall, SELECT_ONE_NUMBER);
                break;

            default:
                break;
        }
        IRecentCalls_Release(pRecentCall);

    } 
}

#ifdef FEATURE_POWERUP_REGISTER_CHINAUNICOM
/*==============================================================================
������
    ISO_13818_CRC32

˵����
    �������ڼ����ַ����� CRC32 У���롣

������
       
����ֵ��
    TRUE: ��ʼ�����
    FALSE: ��ʼ��δ���

��ע:
       
==============================================================================*/
uint32 ISO_13818_CRC32(char *pdata, int nlen)
{
    uint32 *CRCTable=NULL;
    uint32 crc, i, j;
    uint32 poly = 0xEDB88320L;
    char *pTep;
    //uint8 p[] = {0,1,2,4,5,7,8,10,11,12,16,22,23,26};
    
    if ((NULL == pdata) || (nlen == 0))
    {
        return 0;
    }
    
    
    CRCTable = (uint32 *)sys_malloc(256*sizeof(uint32));
    if (NULL == CRCTable)
    {
        return 0;
    }
    
    /*
    // poly �����㷨�����Ϊ 0xEDB88320L
    poly = 0L;
    for (i = 0; i < sizeof(p)/sizeof(uint8); i++)
    {
        poly |= 1L << (31 - p[i]);
    }
    */

    // ����CRC32��
    for(i=0; i<256; i++)
    {
        crc = i;
        
        for (j=0; j<8; j++)
        {
            crc = (crc & 1) ? (poly ^ (crc >> 1)) : (crc >> 1);
        }
        
        CRCTable[i] = crc;
    }

    // ��������ֽ����� CRC32
    pTep = pdata;
    crc = 0xFFFFFFFFL;
    i = nlen;
    while(i > 0)
    {
        crc = CRCTable[(crc ^ (*pTep)) & 0xFF] ^ (crc >> 8);
        pTep++;
        i--;
    }
    
    SYS_FREEIF(CRCTable);
    
    return crc^0xFFFFFFFFL;
}

/*==============================================================================
������
    GetRegisterInfo(char *szRegInfo, int nSize)

˵����
    ����������ȡ��ע����Ϣ��

������
    szRegInfo [in]��ָ�����ڷ���ע����Ϣ��charָ�롣
    nSize [in]��    �������Ĵ�С��(��λ��byte) 

����ֵ��
    >=0:    �ɹ�,����Ϊ��Ϣռ�õ��ֽ���
    <0:    ʧ��

��ע��
    nSize ����С�� 140 
==============================================================================*/
int GetRegisterInfo(char *szRegInfo, int nSize)
{
    int  nlen;
    AEEMobileInfo     mi;
    uint32 dwCRC32;
    
    if (NULL == szRegInfo  || nSize<140)
    {
        return -1;
    }
        
    MEMSET(szRegInfo, 0, nSize);
    // Э��汾--ʹ��ESN
    szRegInfo[0] = 1;
        
    //  ��������--�ƶ�̨�Զ�ע����Ϣ
    szRegInfo[1] = 3;
    
    // ���λ
    szRegInfo[3] = 0;
    
    // ��ʽ�����ݣ����͡��ֻ�ESN��IMSI���롢��ǰ����汾
    //OEM_GetConfig (CFGI_MOBILEINFO, &mi, sizeof(AEEMobileInfo));
    GetMobileInfo(&mi);
    SPRINTF(&szRegInfo[4], "<a1><b1>%s</b1><b2>%08x</b2><b3>%s</b3><b4>%s</b4></a1>",
            OEMNV_DEFAULT_BANNER, mi.dwESN, mi.szMobileID, ver_modelversion);
    
    nlen = STRLEN(&szRegInfo[4]);
    
    // ���ݳ���
    szRegInfo[2] = nlen;
    
    nlen = nlen+4;
    // ���� CRC32 У����
    
    dwCRC32 = ISO_13818_CRC32(szRegInfo, nlen);
    SPRINTF(&szRegInfo[nlen], "%08x", dwCRC32);
        
    return (nlen+8);
}

wms_client_message_s_type *GetRegisterMsg()
{
    char  *pBuf=NULL;
    int   nMsgSize = 0;
    int   nSize;
    wms_cdma_user_data_s_type    *pUserdata = NULL;
    wms_client_message_s_type    *pCltMsg = NULL;

    
    nSize = sizeof(char)*150;
    pBuf = (char *)sys_malloc(nSize);
    if (NULL == pBuf)
    {
        goto GETREGISTERMSG_EXIT;
    }
    nMsgSize = GetRegisterInfo(pBuf, 150);
    if (nMsgSize<0)
    {
        goto GETREGISTERMSG_EXIT;
    }
    
    nSize = sizeof(wms_cdma_user_data_s_type);
    pUserdata = (wms_cdma_user_data_s_type *)sys_malloc(nSize);
    if (NULL == pUserdata)
    {
        goto GETREGISTERMSG_EXIT;
    }
    MEMSET(pUserdata, 0, nSize);
    pUserdata->encoding = WMS_ENCODING_OCTET;
    pUserdata->number_of_digits = nMsgSize;
    pUserdata->data_len = nMsgSize;
    pUserdata->padding_bits = 0;
    MEMCPY(pUserdata->data, pBuf, nMsgSize);
    
    pCltMsg = GetMOClientMsg(REG_SERVERNUM, pUserdata, TRUE);
    
GETREGISTERMSG_EXIT:
    SYS_FREEIF(pBuf);
    SYS_FREEIF(pUserdata);
    
    return pCltMsg;
}
#endif

#ifdef FEATURE_SEAMLESS_SMS
static char mintable[10] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
extern nv_stat_enum_type ui_get_nv(
  nv_items_enum_type item,        /* which item */
  nv_item_type *data_ptr          /* pointer to space for item */
);

int GetSeamlessSMSInfo(char *szInfoEx, int nSize)
{
    int len = 0;
    AEEMobileInfo     mi;
    JulianType        julian;
    nv_item_type      nvi;
    char* txt;         // destination text
    word temp;         // working buffer
    dword value;       // to store value read from nv
	int i=0;
	char szInfo[150] = {0};
	int count = 0;

	MSG_FATAL("GetSeamlessSMSInfo!!!",0,0,0);
    //if (NULL == szInfo)
    //{
    //    return -1;
    //}
    
    GetMobileInfo(&mi);
    GETJULIANDATE(0, &julian);
    
    // ��ʽ�����ݣ�Ʒ�ơ����͡��ֻ�ESN��MDN��MIN����ǰ����汾��ʱ��
    STRCPY(szInfo, "IV;C500;");
    len = STRLEN(szInfo);
    SPRINTF(&szInfo[len],"%x", mi.dwESN);
    len += STRLEN(&szInfo[len]);
    szInfo[len]=';';
    len++;
        	
    //MDN����Ҫ
    szInfo[len]=';';
    len++;
    
    // GetMIN
    // read MIN2
    txt = &szInfo[len];
    nvi.min2.nam = mi.nCurrNAM;
    if( ui_get_nv(NV_MIN2_I, &nvi) != NV_DONE_S)
    {
    	MSG_FATAL("Get NV_MIN2_I Failed!!!",0,0,0);
		return 0;
    }
    else
    {
    	MSG_FATAL("Get NV_MIN2_I : 0x%x",nvi.min2.min2[1],0,0);
    	value = nvi.min2.min2[1];
    }

    *txt++ = mintable[ (value/100) %10];
    value %= 100;
    *txt++ = mintable[ value/10 ];
    *txt++ = mintable[ value%10 ];

    // read MIN1
    nvi.min1.nam = mi.nCurrNAM;
	if( ui_get_nv(NV_MIN1_I, &nvi) != NV_DONE_S)
	{
		MSG_FATAL("Get NV_MIN1_I Failed!!!",0,0,0);
		return 0;
 	}
 	else
 	{
 		MSG_FATAL("Get NV_MIN1_I : 0x%x",nvi.min1.min1[1],0,0);
 		value = nvi.min1.min1[1];
    }

    temp = (word) (value>>14 );
    *txt++ = mintable[ (temp/100) %10];
    temp %= 100;
    *txt++ = mintable[ temp/10 ];
    *txt++ = mintable[ temp%10 ];
    value &= 0x3FFFL;                /* get bottom 14 bits */
    /* next digit is top 4 bits */
    temp = (word) (( value >> 10 ) & 0xF );
    *txt++ = (char) ( ( ( temp == 10 ) ? 0 : temp ) + '0' );
    temp = (word) ( value & 0x3FF ); /* get bottom 10 bits */
    *txt++ = mintable[ (temp/100) %10];
    temp %= 100;
    *txt++ = mintable[ temp/10 ];
    *txt++ = mintable[ temp%10 ];
    *txt++ = (char)';';
    *txt = '\0';
    len += STRLEN(&szInfo[len]);

    SPRINTF(&szInfo[len], "%s;", ver_modelversion);
    len += STRLEN(&szInfo[len]);
    SPRINTF(&szInfo[len], "%04d%02d%02d%02d%02d", julian.wYear,julian.wMonth,julian.wDay,julian.wHour,julian.wMinute);
    len += STRLEN(&szInfo[len]);

	for(i=0; i<len; i++)
	{
		SPRINTF(szInfoEx+count,"%x",szInfo[i]);
		count = STRLEN(szInfoEx);
	}

	len = 2*len;
	MSG_FATAL("GetSeamlessSMSInfo End!!!",0,0,0);
    return len;
}

wms_client_message_s_type *GetSeamlessSMS()
{
    char  *pBuf=NULL;
    int   nMsgSize = 0;
    int   nSize;
    wms_cdma_user_data_s_type    *pUserdata = NULL;
    wms_client_message_s_type    *pCltMsg = NULL;


    MSG_FATAL("Send SeamLess SMS!",0,0,0);
    nSize = sizeof(char)*300;
    pBuf = (char *)sys_malloc(nSize);
    if (NULL == pBuf)
    {
        goto GETREGISTERMSG_EXIT;
    }
    nMsgSize = GetSeamlessSMSInfo(pBuf, 300);
    if (nMsgSize<=0)
    {
        goto GETREGISTERMSG_EXIT;
    }
    
    nSize = sizeof(wms_cdma_user_data_s_type);
    pUserdata = (wms_cdma_user_data_s_type *)sys_malloc(nSize);
    if (NULL == pUserdata)
    {
        goto GETREGISTERMSG_EXIT;
    }
    MEMSET(pUserdata, 0, nSize);
    pUserdata->encoding = WMS_ENCODING_OCTET;
    pUserdata->number_of_digits = nMsgSize;
    pUserdata->data_len = nMsgSize;
    pUserdata->padding_bits = 0;
    MEMCPY(pUserdata->data, pBuf, nMsgSize);
    
    pCltMsg = GetMOClientMsg(SEAMLESSSMS_SERVERNUM, pUserdata, FALSE);
    
GETREGISTERMSG_EXIT:
    SYS_FREEIF(pBuf);
    SYS_FREEIF(pUserdata);
    
    return pCltMsg;
}
#endif

/*==============================================================================
����:
    GetMOClientMsg

˵��:
    �������� pszTonum ��pUserdata �ṹ��Ϣ���� wms_client_message_s_type ��
    �� MO ��Ϣ������ϢTagΪδ���ͣ���

����:
    pszTonum [in]: ������Ϣ�ĺ����ַ��
    pUserdata [in]: �ѱ����û����ݡ�

����ֵ:
    none

��ע:
    �������� buffer ϵ��̬���䣬�ɵ����߸����ͷš�

==============================================================================*/
wms_client_message_s_type *GetMOClientMsg(char *pszTonum, wms_cdma_user_data_s_type *pUserdata, boolean bTlAck)
{
    int32 nSize;
    wms_client_ts_data_s_type *pCltTsData = NULL;
    wms_client_message_s_type *pCltMsg = NULL;
    
    if ((NULL == pUserdata) ||
        (NULL == pszTonum) ||
        (STRLEN(pszTonum) < 3) ||
        (STRLEN(pszTonum) > MAX_PH_DIGITS))
    {
        return NULL;
    }
    
    nSize = sizeof(wms_client_ts_data_s_type);
    pCltTsData = (wms_client_ts_data_s_type *)sys_malloc(nSize);
    if (NULL == pCltTsData)
    {
        ERR("sys_malloc Failed!",0,0,0);
        return NULL;
    }
    MEMSET(pCltTsData, 0, nSize);
    
    nSize = sizeof(wms_client_message_s_type);
    pCltMsg = (wms_client_message_s_type *)sys_malloc(nSize);
    if (NULL == pCltMsg)
    {
        SYS_FREEIF(pCltTsData);
        ERR("sys_malloc Failed!",0,0,0);
        return NULL;
    }
    
    // -------- ��� wms_client_ts_data_s_type �ṹ --------
    pCltTsData->format = WMS_FORMAT_CDMA;
    pCltTsData->u.cdma.mask = 0;

    // ��Ϣ ID ����
    pCltTsData->u.cdma.message_id.id_number = 65530;
    pCltTsData->u.cdma.message_id.type = WMS_BD_TYPE_SUBMIT;
    pCltTsData->u.cdma.mask |= WMS_MASK_BD_MSG_ID;

    // ��Ϣ�û����ݲ���
    nSize = sizeof(wms_cdma_user_data_s_type);
    MEMCPY(&pCltTsData->u.cdma.user_data, pUserdata, nSize);
    pCltTsData->u.cdma.mask |= WMS_MASK_BD_USER_DATA;
    
    // ʱ���
    //ConvertMStoMcTime(GETTIMESECONDS(), &pCltTsData->u.cdma.mc_time);
    //pCltTsData->u.cdma.mask |= WMS_MASK_BD_MC_TIME;
    
    // ���ȼ�: 
    pCltTsData->u.cdma.priority = WMS_PRIORITY_URGENT;
    pCltTsData->u.cdma.mask |= WMS_MASK_BD_PRIORITY;
    
    
    // �ظ�ѡ��:
    pCltTsData->u.cdma.reply_option.user_ack_requested = FALSE;
    pCltTsData->u.cdma.reply_option.delivery_ack_requested = FALSE;
    pCltTsData->u.cdma.reply_option.read_ack_requested = FALSE;
    pCltTsData->u.cdma.mask |= WMS_MASK_BD_REPLY_OPTION;
    
    pCltTsData->u.cdma.num_messages = 1;
    
    // -------- ��� wms_client_message_s_type �ṹ --------
    pCltMsg->msg_hdr.message_mode = WMS_MESSAGE_MODE_CDMA;
    pCltMsg->msg_hdr.tag = WMS_TAG_MO_NOT_SENT;
    pCltMsg->msg_hdr.mem_store = WMS_MEMORY_STORE_NONE;
    pCltMsg->msg_hdr.index = WMS_DUMMY_MESSAGE_INDEX;
    
    pCltMsg->u.cdma_message.is_mo = TRUE;
    pCltMsg->u.cdma_message.teleservice = WMS_TELESERVICE_CMT_95;
    
#ifndef WIN32
    // ���յ�ַ
    if (pszTonum[0] == '+')
    {
        pCltMsg->u.cdma_message.address.number_of_digits = wms_ts_ascii2dtmf(&pszTonum[1], pCltMsg->u.cdma_message.address.digits);
    }
    else
    {
        pCltMsg->u.cdma_message.address.number_of_digits = wms_ts_ascii2dtmf(pszTonum, pCltMsg->u.cdma_message.address.digits);
    }
#endif
    pCltMsg->u.cdma_message.address.digit_mode = WMS_DIGIT_MODE_4_BIT;
    pCltMsg->u.cdma_message.address.number_mode = WMS_NUMBER_MODE_NONE_DATA_NETWORK;
    pCltMsg->u.cdma_message.address.number_type = WMS_NUMBER_UNKNOWN;
    pCltMsg->u.cdma_message.address.number_plan = WMS_NUMBER_PLAN_TELEPHONY;
    
    pCltMsg->u.cdma_message.is_tl_ack_requested = bTlAck;
    pCltMsg->u.cdma_message.is_service_present = FALSE;
#ifndef WIN32
    (void)wms_ts_encode(pCltTsData, &pCltMsg->u.cdma_message.raw_ts);
#endif   
    // ��Ϣ�������
    
    SYS_FREEIF(pCltTsData);
    
    return pCltMsg;
}

/*==============================================================================
����:
    CWmsApp_Getspecmsg

˵��:
    ���������ڸ����ض�����Ϣ���ͷ�����Ӧ�� wms_client_message_s_type �� MO ��Ϣ��

����:
    pwstrType [in]: ���ֻ�ȡ���͡�

����ֵ:
    �ޡ�

��ע:
    ����Bufϵ��̬���䣬�����߸����ͷš�
        
==============================================================================*/
wms_client_message_s_type *CWmsApp_Getspecmsg(AECHAR *pwstrType)
{
    char chType = 0;
    
    if (NULL == pwstrType)
    {
        return NULL;
    }
    
    chType = (char) (*pwstrType);
    switch (chType)
    {
#ifdef FEATURE_POWERUP_REGISTER_CHINAUNICOM
        case POWERUP_REGISTER_CHINAUNICOM:
            return GetRegisterMsg();
#endif        
#ifdef FEATURE_SEAMLESS_SMS
        case POWERUP_REGISTER_SEAMLESSSMS:
            return GetSeamlessSMS();
#endif
        default:
            return NULL;
    }
}

//Add By zzg 2010_07_21
wms_client_message_s_type *CWmsApp_GetspecmsgEx(void)
{
    int   nMsgSize = 0;
    int   nSize;
    wms_cdma_user_data_s_type    *pUserdata = NULL;
    wms_client_message_s_type    *pCltMsg = NULL;
   
	nSize = sizeof(FRENDUO_SMS_BUFF);    
	nMsgSize = sizeof(FRENDUO_SMS_BUFF);
	
    if (nMsgSize<=0)
    {
        goto GETREGISTERMSG_EXIT;
    }
    
    nSize = sizeof(wms_cdma_user_data_s_type);
    pUserdata = (wms_cdma_user_data_s_type *)sys_malloc(nSize);
	
    if (NULL == pUserdata)
    {
        goto GETREGISTERMSG_EXIT;
    }
	
    MEMSET(pUserdata, 0, nSize);
    pUserdata->encoding = WMS_ENCODING_OCTET;
    pUserdata->number_of_digits = nMsgSize;
    pUserdata->data_len = nMsgSize;
    pUserdata->padding_bits = 0;
    MEMCPY(pUserdata->data, FRENDUO_SMS_BUFF, nMsgSize);
    
    pCltMsg = GetMOClientMsg(FRENDUO_SMS_DESTNUM, pUserdata, FALSE);
    
GETREGISTERMSG_EXIT:    
    SYS_FREEIF(pUserdata);
    
    return pCltMsg;    
}
//Add End

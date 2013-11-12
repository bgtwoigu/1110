#ifndef WMSAPP_H
#define WMSAPP_H
//*==============================================================================
//           W M S   A P P L E T  M O D U L E
// �ļ�:
//        WMSApp.h
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//        
// �ļ�˵��: 
//        
//==============================================================================*/

/*==============================================================================
�޸���ʷ��¼: 
ʱ       ��       �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------    -----------------------------------------------

==============================================================================*/


/*==============================================================================
                             
                             ���ļ��������ⲿ�ļ�
                             
==============================================================================*/
#include "AEE.h"              // Standard AEE Declarations
#include "AEEShell.h"         // AEE Shell Services

/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/
// һ���������ݰ��� OCTET ���������������ַ���
#ifdef FEATURE_CARRIER_ISRAEL_PELEPHONE
// �޳�����Ļس���
#define MAX_OCTETMSG_PAYLOAD    119
#else
#define MAX_OCTETMSG_PAYLOAD    140
#endif

// һ���������ݰ��� ASCII ���������������ַ���
#define MAX_ASCIIMSG_PAYLOAD    (MAX_OCTETMSG_PAYLOAD*8/7) 

// һ���������ݰ��� UNICODE ���������������ַ���
#define MAX_UNICODMSG_PAYLOAD   (MAX_OCTETMSG_PAYLOAD/2)


// ���ڷ��ͳ����ţ����֧�ַ����ı��ĳ���
#if defined(FEATURE_CARRIER_VENEZUELA_MOVILNET)
#define  WMS_MSG_MAXCHARS       MAX_ASCIIMSG_PAYLOAD
#elif defined(FEATURE_CARRIER_THAILAND_HUTCH) || defined(FEATURE_CARRIER_THAILAND_CAT)
#define  WMS_MSG_MAXCHARS       MAX_ASCIIMSG_PAYLOAD
#elif defined(FEATURE_CARRIER_ISRAEL_PELEPHONE)
#define  WMS_MSG_MAXCHARS       MAX_OCTETMSG_PAYLOAD
#else
#ifdef FEATURE_SUPPORT_ID
#define  WMS_MSG_MAXCHARS       1537
#elif defined(FEATURE_VERSION_C316) //Add by pyuangui 20121226
#define  WMS_MSG_MAXCHARS       918
#else
#define  WMS_MSG_MAXCHARS       1024
#endif
#endif

// ����ģ����ţ����֧���ı��ĳ���
#define  WMS_TEMPMSG_MAXCHARS   MAX_UNICODMSG_PAYLOAD

#define MAX_PH_DIGITS                  WMS_ADDRESS_MAX       // Maximum phone number digits

#define MAX_TITLE_LEN                  WMS_ADDRESS_MAX       // Maximum title AECHARs

#define MAX_EMAILADD_LEN               64       // �����ʼ���ַ���������
#define MAX_URL_LEN                    256      // URL���������

#define WMSAPP_RELATIVE_VALIDITY_IMMEDIATE          246

// ÿ���û�������Ϣģ����������ַ���
#define  MAX_TEMPLATECHARS      70

//ԤԼʧ�ܶ����������
#define  MAX_RSVFAILD        10

#ifdef FEATURE_POWERUP_REGISTER_CHINAUNICOM
// ����ע����Ϣ�ķ���������
#define REG_SERVERNUM  "10659401"

#define  POWERUP_REGISTER_CHINAUNICOM   'R'
#endif
#if defined(FEATURE_VERSION_W317A)||defined(FEATURE_VERSION_C337)||defined(FEATURE_VERSION_C316)||defined(FEATURE_SALESTRACKER) || defined(FEATURE_VERSION_IC241A_MMX)
#define  MOBILE_TRACKER_MSG             'M'
#define SMS_TRACKER_MSG                 'T'
#define SMS_TRACKER_MSG_TWO             'Q'
#define SMS_TRACKER_TOW                 'Y'
#define SMS_TRACKER_THREE               'Z'
#endif

//#if defined (FEATURE_VERSION_C337) || defined(FEATURE_VERSION_IC241A_MMX)
#define  MIZONE_MSG             'Z'
//#endif

#if defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_K212)
#define REG_SERVERNUM       "10659401" //"18126010560"
#define REGHOPE_MSG         'Z'
#endif

#ifdef FEATURE_SEAMLESS_SMS
// ����ע����Ϣ�ķ���������
#define SEAMLESSSMS_SERVERNUM  "02170277582"

#define  POWERUP_REGISTER_SEAMLESSSMS   'S'
#endif
/*==============================================================================
                                 
                                ����������Ͷ���
                                 
==============================================================================*/
//Applet�����Ĳ�ͬ״̬
typedef enum _WMSServiceType 
{
    STARTMETHOD_POWERONSTART,       // ISHELL_StartApplet()
    STARTMETHOD_WMSNORMAL,          // ISHELL_StartApplet()
    STARTMETHOD_SENDTEXTMESSAGE,    // Send message with known address
    STARTMETHOD_WRITENEWMESSAGE,    // write a new message with no address
    STARTMETHOD_SENDSPECMESSAGE,    // Send spec message
    STARTMETHOD_SHOWINBOXLIST,      // Show sms received list
    STARTMETHOD_VIEWVMAIL,          // View Voice mail
    STARTMETHOD_REFRESH             // Refresh UIM SMS
}WMSServiceType;

typedef struct _IWmsApp IWmsApp;

QINTERFACE(IWmsApp)
{
    INHERIT_IApplet(IWmsApp);
    
    // �ⲿ������ŷ���ӿں���
    int (*MessageService)(IWmsApp *p, WMSServiceType eStype, AECHAR *toAddrStr, AECHAR *pwstrText);
    int (*GetLeftSendListNodeNum)(IWmsApp *p, int *leftNum);
    int (*DeleteAllNvCdmaSms)(IWmsApp *p);
};

extern boolean gbWmsVMailNtf;
extern boolean gbWmsSMSNtf;
extern boolean gbWmsLastNtfIsSMS;

/*==============================================================================

                                 ��������

==============================================================================*/
/*==============================================================================
����: 
    IWmsApp_AddRef

˵��: 
    ���ڸ������ü�����Applet������

����: 
    p [in/out]: IWmsApp�ӿڶ���ָ�롣

����ֵ: 
    ��ǰ�� WMS Applet��������

��ע:

==============================================================================*/
#define IWmsApp_AddRef(p)                GET_PVTBL(p,IWmsApp)->AddRef(p)

/*==============================================================================
����: 
    IWmsApp_Release

˵��: 
    BREW�����ͷ���Դ��Applet������

����: 
    p [in/out]: IWmsApp�ӿڶ���ָ�롣

����ֵ: 
    ��ǰ�� WMS Applet��������

��ע:

==============================================================================*/
#define IWmsApp_Release(p)               GET_PVTBL(p,IWmsApp)->Release(p)

/*==============================================================================
����: 
    IWmsApp_HandleEvent

˵��: 
    ��BREW���õ�SMS Applet�¼�������������

����: 
    p [in/out]: IWmsApp�ӿڶ���ָ�롣
    ec [in]:  �¼�����
    wp [in]:  �¼��ض�����
    dw [in]: �¼��ض�����

����ֵ: 
    TRUE: �¼��õ�����
    FALSE: û�д����¼���

��ע:

==============================================================================*/
#define IWmsApp_HandleEvent(p,ec,wp,dw) GET_PVTBL(p,IWmsApp)->HandleEvent(p,  \
    ec,  wp, dw)

/*==============================================================================
�ӿ�: 
    IWMSApp_PoweronStart

˵��: 
    �ṩ���ⲿApplet�������� WmsApp ������ʼ�� Applet �ṹ���ݵ����ֺ�����

����: 
    pi [in]: ָ��IWmsApp�ӿ�t�����ָ�롣

����ֵ: 
    SUCCESS: Applet�����ɹ���
    EFAILED: Applet����ʧ�ܡ�

��ע:
    �ú������ⲿ������ã��ڲ����õ��á�
        
==============================================================================*/
#define IWMSApp_PoweronStart(p)  \
    GET_PVTBL(p,IWmsApp)->MessageService(p, STARTMETHOD_POWERONSTART, NULL, NULL);


/*==============================================================================
�ӿ�: 
    IWMSApp_SendTextMessage

˵��: 
    �ṩ���ⲿApplet���������ı���Ϣ��������������ֺ�����

����: 
    pi [in]: ָ��IWmsApp�ӿ�t�����ָ�롣
    s [in]: (��null��β�Ŀ��ַ���)����Ϣ����Ŀ�ĵء�

����ֵ: 
    SUCCESS: Applet�����ɹ���
    EFAILED: Applet����ʧ�ܡ�

��ע:
    �ú������ⲿ������ã��ڲ����õ��á�
        
==============================================================================*/
#define IWMSApp_SendTextMessage(p,s)  \
    GET_PVTBL(p,IWmsApp)->MessageService(p, STARTMETHOD_SENDTEXTMESSAGE, s, NULL)
    
/*==============================================================================
�ӿ�: 
    IWmsApp_SendTextMessageExt

˵��: 
    �ṩ���ⲿApplet�������͸����ı���Ϣ��������������ֺ�����

����: 
    pi [in]: ָ��IWmsApp�ӿ�t�����ָ�롣
    s [in]: (��null��β�Ŀ��ַ���)����Ϣ����Ŀ�ĵء�
    t [in]: (��null��β�Ŀ��ַ���)����Ϣ�ı���

����ֵ: 
    SUCCESS: Applet�����ɹ���
    EFAILED: Applet����ʧ�ܡ�

��ע:
    �ú������ⲿ������ã��ڲ����õ��á�
        
==============================================================================*/
#define IWmsApp_SendTextMessageExt(p,s,t)  \
    GET_PVTBL(p,IWmsApp)->MessageService(p, STARTMETHOD_SENDTEXTMESSAGE, s, t)
    
/*==============================================================================
�ӿ�: 
    IWmsApp_SendSpecMessage

˵��: 
    �ṩ���ⲿApplet������������������Ϣ�����ֺ�����������Ϣ�����ݡ���װ������
    �ڶ�����ʵ�֡��ⲿӦ�õ��øýӿ������ú�����

����: 
    pi [in]: ָ��IWmsApp�ӿڶ����ָ�롣
    t [in]:  ������Ϣ���͡�

����ֵ: 
    SUCCESS: Applet�����ɹ���
    EFAILED: Applet����ʧ�ܡ�

��ע:
    �ú������ⲿ������ã��ڲ����õ��á�
    ������Ϣ���ͣ�
        
==============================================================================*/
#define IWmsApp_SendSpecMessage(p,t)  \
    GET_PVTBL(p,IWmsApp)->MessageService(p, STARTMETHOD_SENDSPECMESSAGE, NULL, t)




//Add By zzg 2010_07_20
#define IWmsApp_SendSpecMessageEx(p)  \
	GET_PVTBL(p,IWmsApp)->MessageService(p, STARTMETHOD_SENDSPECMESSAGE, NULL, NULL)



/*==============================================================================
�ӿ�: 
    IWmsApp_WriteNewMessage

˵��: 
    �ṩ���ⲿApplet���������ı���Ϣ����ȷ����������ֺ�����

����: 
    pi [in]: ָ��IWmsApp�ӿ�t�����ָ�롣

����ֵ: 
    SUCCESS: Applet�����ɹ���
    EFAILED: Applet����ʧ�ܡ�

��ע:
    �ú������ⲿ������ã��ڲ����õ��á�
        
==============================================================================*/
#define IWmsApp_WriteNewMessage(p)  \
    GET_PVTBL(p,IWmsApp)->MessageService(p, STARTMETHOD_WRITENEWMESSAGE, NULL, NULL)

/*==============================================================================
�ӿ�: 
    IWmsApp_ShowMessageList

˵��: 
    �ṩ���ⲿApplet�����鿴�¶��ŵ����ֺ�����

����: 
    pi [in]: ָ��IWmsApp�ӿ�t�����ָ�롣

����ֵ: 
    SUCCESS: Applet�����ɹ���
    EFAILED: Applet����ʧ�ܡ�

��ע:
    �ú�����BREW���ã��û����õ��á�
        
==============================================================================*/
#define IWmsApp_ShowMessageList(p)  \
    GET_PVTBL(p,IWmsApp)->MessageService(p, STARTMETHOD_SHOWINBOXLIST, NULL, NULL)
    
/*==============================================================================
�ӿ�: 
    IWmsApp_ViewVMail

˵��: 
    �ṩ���ⲿApplet�����鿴�������ŵ����ֺ�����

����: 
    pi [in]: ָ��IWmsApp�ӿ�t�����ָ�롣

����ֵ: 
    SUCCESS: Applet�����ɹ���
    EFAILED: Applet����ʧ�ܡ�

��ע:
    �ú�����BREW���ã��û����õ��á�
        
==============================================================================*/
#define IWmsApp_ViewVMail(p)  \
    GET_PVTBL(p,IWmsApp)->MessageService(p, STARTMETHOD_VIEWVMAIL, NULL, NULL)
    
/*==============================================================================
�ӿ�: 
    IWmsApp_RefreshRUIMSMS

˵��: 
    �ṩ���ⲿApplet����֪ͨ������������UIM����Ϣ�����ֺ�����

����: 
    pi [in]: ָ��IWmsApp�ӿ�t�����ָ�롣

����ֵ: 
    SUCCESS: Applet�����ɹ���
    EFAILED: Applet����ʧ�ܡ�

��ע:
    �ú�����BREW���ã��û����õ��á�
        
==============================================================================*/
#define IWmsApp_RefreshRUIMSMS(p)  \
    GET_PVTBL(p,IWmsApp)->MessageService(p, STARTMETHOD_REFRESH, NULL, NULL)


/*==============================================================================
�ӿ�: 
    IWmsApp_GetLeftSendListNodeNum

˵��: 
    �ṩ���ⲿApplet������ȡsendList��໹����Ӻ���ĸ���

����: 
    pi [in]: ָ��IWmsApp�ӿ�t�����ָ�롣

����ֵ: 
    SUCCESS: Applet�����ɹ���
    EFAILED: Applet����ʧ�ܡ�

��ע:
        
==============================================================================*/
#define IWmsApp_GetLeftSendListNodeNum(p, l)  \
    GET_PVTBL(p,IWmsApp)->GetLeftSendListNodeNum(p, l)

/*==============================================================================
�ӿ�: 
    IWmsApp_DeleteAllNvcdmaSms

˵��: 
    �ṩ���ⲿApplet������ȡsendList��໹����Ӻ���ĸ���

����: 
    pi [in]: ָ��IWmsApp�ӿ�t�����ָ�롣

����ֵ: 
    SUCCESS: �ɹ���
    EFAILED: ʧ�ܡ�

��ע:
        
==============================================================================*/
#define IWmsApp_DeleteAllNvCdmaSms(p)  \
    GET_PVTBL(p,IWmsApp)->DeleteAllNvCdmaSms(p)


#endif // end WMSAPP_H


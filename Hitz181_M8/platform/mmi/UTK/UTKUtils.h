#ifndef _UTKUTILS_H
#define _UTKUTILS_H
/*==============================================================================
// �ļ���
//        UTKUtils.h
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//        
// �ļ�˵����
//        
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��       �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------    -----------------------------------------------
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "AEE.h"
#include "AEEStdLib.h"
#ifndef WIN32
#include "uim.h"
#else
#define UIM_K_FIELD          0x0F      /* In T0; Number of historical bytes */
#define UIM_Y_FIELD_SHIFT       4      /* Number of bits to shift to get the
                                           Y field in the lsb                */
#define UIM_TD_PRESENT       0x80      /* TD byte is present in following
                                           bytes                             */
#define UIM_T_FIELD          0x0F      /* In TD; transmission protocol      */

#define UIM_DEFLT_GUARDTIME_BITS 2     /* This is the default number of
                                           bits used for the guardtime.      */
#define UIM_MINIMUM_T_1_GUARDTIME 1    /* This is the minimum guardtime for
                                          the T=1 protocol */

#define UIM_MAX_CHV_DIGITS      8      /* This is the maximum number of digits */
/* Maximum number of chars in one transaction */
#define UIM_MAX_CHARS         512
#define UIM_MAX_TRANSACTION_SIZE 256
#define UIM_MAX_STREAM_ISO7816_APDU_TRANSACTION_SIZE 261
#define UIM_MAX_TRANS_SIZE_OUT   255
#define UIM_MAX_TRANSACTION_P3   0
#define UIM_MAX_PSO_DATA      256
#define NUM_BYTES_APDU_HDR    5
#endif
/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
#define  REFRESHMASK_UIMALL         0x0001
#define  REFRESHMASK_UIMSMS         0x0002
#define  REFRESHMASK_UIMADD         0x0004
#define  REFRESHMASK_UIMRESET       0x0008

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
typedef struct _command_describe 
{
    byte    describe_id;
    byte    length;
    byte    command_num;
    byte    command_type;
    byte    command_restricttag;
} command_describe;

typedef struct _device_identity 
{
    byte    speciality_id;
    byte    length;
    byte    source_speciality;
    byte    target_speciality;
} device_identity;

typedef struct _text_string 
{
    byte    text_id;
    byte    length;
    byte    encode_schedule; // 4:oect 0:ASCII 7λ���� 8: Unicode ����
    AECHAR  data[256];
} text_string;

/* Response Length - parsed Value field */
typedef struct _response_len
{
    byte   response_id;
    byte   length;  
    byte   min_len_rsp;
    byte   max_len_rsp;
} response_len;

/* Default Text - parsed Value field */
typedef struct _default_text
{
    byte    text_id;
    byte    length;
    byte    encode_schedule; // 4:oect 0:ASCII 7??����?? 8: Unicode ����??
    AECHAR  data[256];
} default_text;

/* Icon Identifier - parsed Value field */
typedef struct _icon_id
{
   byte    icon_id;
   byte    length;    
   byte    qual;
   byte    id;
} icon_id;

typedef struct _alpha_id
{
   byte   alpha_id_id;
   byte   length;
  AECHAR  data[256];
} alpha_id;

typedef struct _tone
{
   byte   tone_id;
   byte   length;
   byte   tone;
} tone;

typedef struct _duration
{
   byte   duration_id;
   byte   length;
   byte   time_unit;
   byte   time_interval;   
} duration;

/* File List - parsed Value field */
typedef struct _file_list
{
   byte   file_list_id;
   byte   length;
   byte   num_files;
   byte   data[UIM_MAX_CHARS];
} file_list;

/* Language - parsed Value field */
typedef struct _lang 
{
  byte   lang_id;
  word   lang;
} lang;

typedef struct _date_time
{
   word  year;
   word  month;
   word  day;
   word  hour;
   word  minute;
   word  second;
   word  zone;
}date_time;

typedef struct _locn_info
{
   // based on CCAT 3GPP2 C.S0035
   word   mcc;
   word   imsi_11_12;
   word   sid;
   word   nid;
   word   base_id; 
   dword  base_lat;
   dword  base_long;
}locn_info;
typedef struct _Display_text 
{
    byte    command_tag;
    byte    length;
    command_describe    cmd_describe;
    device_identity     dev_identity;
    text_string         text;
} Display_text;

typedef struct _Setup_Call 
{
    command_describe    cmd_describe;
    AECHAR              wszAlpha[64];           // �û�֤ʵ�׶�
    AECHAR              wszAlphaSetupCall[64];  // ���н����׶�
    AECHAR              wszDialNum[32];
} Setup_Call;

typedef struct _Get_input
{
    byte                command_tag;
    byte                length;
    command_describe    cmd_describe;
    device_identity     dev_identity;
    text_string         text;
    response_len        response;
    default_text        defaulttext;
    icon_id             icon;    
} Get_input;

typedef struct _Local_Information
{
    byte                command_tag;
    byte                length;
    command_describe    cmd_describe;
    device_identity     dev_identity;
    byte                command_rsp_tag;
    lang                lang;    
    date_time           date_time;
    byte                access_technology;
    locn_info           locn_info;
} Local_Information;

typedef struct _Play_Tone
{
    byte                command_tag;
    byte                length;
    command_describe    cmd_describe;
    device_identity     dev_identity;
    text_string         text;    
    alpha_id            alpha_id;
    tone                tone;
    duration            duration;
    icon_id             icon;        
} Play_Tone;

typedef struct FileListType 
{
    flg         isValid:1;      // 1 = this structure contains valid data
    
    byte        numFiles;      
    byte        numBytes;       // number of bytes of valid data in the fileData field
    byte        fileData[256];  // raw file data information
} FileListType;

typedef struct _RefreshCmdType {
   byte                 cmd_qualifier;
   FileListType         fileList;
} RefreshCmdType;

/*==============================================================================
                                 ��������
==============================================================================*/
/*==============================================================================
����: 
    UTK_parse_proactive_command
       
˵��: 
    ������ֻ����ԭ���������ݵ������������֣�������ԭʼ���ݵ��ļ���ȫ�ֻ������С�
       
����: 
    cmd_data [in]��ָ���������ݵ�ָ�롣
    cmd_len [in]���������ݳ��ȡ�
       
����ֵ:
    ԭ������
       
��ע:
    
    
==============================================================================*/
byte UTK_parse_proactive_command(byte *cmd_data, byte cmd_len);

/*==============================================================================
����: 
    UTK_GetCmddata
       
˵��: 
    ������������ȡָ����������ص����ݵĵ�ַ��
       
����: 
    cmd_type [In]: uim_tk_proactive_cmd_enum_type ��
       
����ֵ:
    byte *
       
��ע:
    ע�⣺�������ݵĵ�һ�ֽڱ�ӦΪ����ʽRUIM �����ǩ��Ϊ����ʹ�ã��ں���
    parse_UTK_proactive_command �н����Ϊ proactive command ֵ����ʵ֪��
    proactive command ֵ������ʽRUIM �����ǩ��û��������ˡ�

==============================================================================*/
byte * UTK_GetCmddata(byte cmd_type);


/*==============================================================================
����: 
    UTK_decodetext
       
˵��: 
    �������������밴���ű��뷽ʽ������ֽ����е� AECHAR �ַ�����
       
����: 
    wszBuf [In/out]: ���ڱ��������ı��Ļ����� ��
    nBufSize [In]: �����������С,��λΪ�ֽ� ��
    pdata [In]: ָ��������ֽ��� ��
    nlen [In]: �������ֽ�����С,��λΪ�ֽ� ��
    encode_schedule [In]: �������ֽ����ı��뷽ʽ ��
    
       
����ֵ:
    none
       
��ע:
    ע�������ֽ����ı��뷽ʽ��ȫ�� uasms_user_data_encoding_e_type ����Ĳ�ͬ��
    0x00: SMSȱʡѹ����ĸ7���ص����ݱ���ƻ�
    0x04: GSMȱʡ��ĸ8�������ݵ����ݱ���ƻ�
    0x08: 16����UNICODE��ĸ�����ݱ���ƻ�
    �������Բ���ʶ��ı��뷽ʽ���� 0x04 ����
==============================================================================*/
void UTK_decodetext(AECHAR *wszBuf, int nBufSize, byte *pdata, int nlen, byte encode_schedule);

/*==============================================================================
����: 
    DecodeDisplayTextData
       
˵��: 
    ����������������Ӧ�� Display Text �����ԭʼ�����ֽ����е��ṹ Display_text �С�
       
����: 
    pdata [In]: ��Ӧ�� Display Text �����ԭʼ�����ֽ�����(BER-TLV �ṹ) ��
    pText [In/out]: ָ�����ڱ����������ݵ� Display_text �ṹ������ ��
    
       
����ֵ:
    none
       
��ע:

==============================================================================*/
void DecodeDisplayTextData(byte *pdata, Display_text *pText);

/*==============================================================================
����: 
    DecodeAlphaString
       
˵��: 
    ���������� Alpha ���ַ�����
       
����: 
    pdata [in]��ָ�����������ֽ����С�
    nLen [in]�����������ֽ����г���,��λ�ֽڡ�
    wstrOut [out]�� ���Buffer
    nBufSize [in]:  ���Buffer��С,��λ AECHAR
       
����ֵ:
    none
       
��ע:
       
==============================================================================*/
void DecodeAlphaString(byte *pdata, int nLen, AECHAR *wstrOut, int nBufSize);

/*==============================================================================
����: 
    UTK_Get_Setup_Call_Buf
       
˵��: 
    ������������ȡ�洢��Ӧ�� Setup Call ����ԭʼ�����ֽ����н�������ݵĽṹ 
    Setup_Call ��������ַ��
       
����: 
    none
       
����ֵ:
    Setup_Call �ṹָ��
       
��ע:
    ��Ϊ Setup Call ���������Ϣ���ģ��ʹ�ã���ʹ��һ��ȫ�־�̬���������档��
    ����ͨ�������������ʡ�
==============================================================================*/
Setup_Call *UTK_Get_Setup_Call_Buf(void);

/*==============================================================================
����: 
    UTK_DecodeSetupCallCmdData
       
˵��: 
    ����������������Ӧ�� Setup Call �����ԭʼ�����ֽ����е��ṹ Setup_Call �С�
       
����: 
    pdata [In]: ��Ӧ�� Setup Call �����ԭʼ�����ֽ�����(BER-TLV �ṹ) ��
    pstCall [in/out]: ָ�����ڱ����������ݵ� Setup_Call �ṹ������ ��
       
����ֵ:
    SUCCESS : �ɹ�
    EFAILED : ʧ��
       
��ע:

==============================================================================*/
int UTK_DecodeSetupCallCmdData(byte *pdata, Setup_Call *pstCall);

/*==============================================================================
����: 
    DecodeGetInputData
       
˵��: 
    ���������������ı���������
       
����:     
       
����ֵ:
    none
       
��ע:
==============================================================================*/
void DecodeGetInputData(byte *pdata, Get_input *pGetInput);

/*==============================================================================
����: 
    DecodePlayToneData
       
˵��: 
    ����������������Ƶ��������
       
����:  
       
����ֵ:
    none
       
��ע:
==============================================================================*/
void DecodePlayToneData(byte *pdata, Play_Tone *pPlayTone);

/*==============================================================================
����: 
    DecodeRefreshData
       
˵��: 
    ����������������Ӧ�� Refresh �����ԭʼ�����ֽ����е��ṹ RefreshCmdType �С�
       
����: 
    pdata [In]: ��Ӧ�� Refresh �����ԭʼ�����ֽ�����(BER-TLV �ṹ) ��
    pCmd [In/out]: ָ�����ڱ����������ݵ� RefreshCmdType �ṹ������ ��
    
       
����ֵ:
    none
       
��ע:

==============================================================================*/
void DecodeRefreshData(byte *pdata, RefreshCmdType *pCmd);

#endif //_UTKUTILS_H

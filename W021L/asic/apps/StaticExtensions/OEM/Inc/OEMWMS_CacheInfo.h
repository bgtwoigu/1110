#ifndef _OEMWMS_CACHEINFO_H
#define    _OEMWMS_CACHEINFO_H
 
/*==============================================================================
//           W M S   C A C A H E I N F O   M O D U L E
// �ļ���
//        OEMWMS_CacheInfo.h
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//        
// �ļ�˵����
//        ����Ϣ Cache Info �ṹ�Ķ��塢������ά������ WMSApp ���ٷ��ʴ洢�Ķ���
//        �á�         
//
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��       �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------    -----------------------------------------------

==============================================================================*/
#include "comdef.h"     // Get DMSS type declarations.
#ifndef WIN32
#include "wmsi.h"
#endif
#include "AEEStdDef.h"

/*==============================================================================
                             
                             ���ļ��������ⲿ�ļ�
                             
==============================================================================*/
 

/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/
#ifdef FEATURE_SUPPORT_ID
#define     LONGSMS_MAX_PACKAGES        10
#else
#define     LONGSMS_MAX_PACKAGES        5
#endif

/*==============================================================================
                                 
                                ����������Ͷ���
                                 
==============================================================================*/
// cache info �ڵ����ݽṹ����
typedef struct  _wms_cache_info_node
{
    struct _wms_cache_info_node *pPrev; // ��һ�ڵ�ָ��
    struct _wms_cache_info_node *pNext; // ��һ�ڵ�ָ��
    
    uint32      dwTime;                 // ��Ϣʱ���
    uint32      message_id;             // ��ϢID
#ifdef FEATURE_SMS_UDH
    uint16      msg_ref;                // ��Ϣ�ο���
    uint8       total_sm;               // ��Ϣ���ݰ�����
    uint8       seq_num;                // ��Ϣ���
    
    // �ܹ��ϲ��ĳ����Ÿ����ݰ��ڵ�ָ���б�
    struct _wms_cache_info_node    **pItems; 
#endif    
    wms_priority_e_type             priority;   // ��Ϣ���ȼ�        
    wms_privacy_e_type              privacy;    // ��Ϣ�ܼ�
    wms_bd_message_type_e_type      msg_type;   // ��Ϣ����
    wms_message_tag_e_type          msg_tag;    // ��Ϣ���
    
    wms_message_index_type          index;      // ��Ϣ�ڴ洢�ռ��ϵ�������
    char                            *pszNum;    // ʼ������պ���
    AECHAR                          *pwszTitle; // ��Ϣ����
    wms_memory_store_e_type         mem_store;  // ��Ϣ�洢λ��
    int                             xuhao;      // ��Ϣ��ţ�������Ϣ�б�˵�
} wms_cache_info_node;

// cache info ˫���������ݽṹ����
typedef struct  _wms_cache_info_list
{
    wms_cache_info_node     *pHead;     // ͷ�ڵ�
    wms_cache_info_node     *pTail;     // β�ڵ�
    wms_cache_info_node     *pCur;      // ��ǰ�ڵ�
    uint16                  nCount;     // �����ڵ����������ڿռ�ͳ��
    uint16                  nNews;      // δ����δ���ͳɹ���ԤԼ����Ϣ���������ڲ˵��б�
    uint16                  nAll;       // ��Ϣ�����������ڲ˵��б�
    uint16                  nOnUIMS;    // λ�ڿ��ϵĽڵ���, ����ԤԼ���ţ��ô˱�����ʾԤԼʧ�ܵķ�֧��
    uint16                  nBranches;  // ��֧����������ʾ��Ϣ����, ����ԤԼ���ţ��ô˱�����ʾԤԼ�еķ�֧��
}wms_cache_info_list;


/*==============================================================================

                                 ��������

==============================================================================*/


/*==============================================================================
����: 
    wms_get_cacheinfolist
       
˵��: 
    ����������ȡ����Ϣ���洢���Ӧ�� cache ����ָ�롣
       
����: 
    ebox [in]: ����Ϣ�洢��
    
����ֵ:
    �ɹ�: ����Ϣ�洢���Ӧ�� cache ����ָ�롣
    ʧ��: NULL
       
��ע:
     
==============================================================================*/
wms_cache_info_list * wms_get_cacheinfolist(wms_box_e_type ebox);

/*==============================================================================
����: 
    OEMWMS_MsgInfoCacheCbExt
       
˵��: 
    Callback function for Message Info Cache. The cached information is used to
    display the message lists
       
����: 
    pMsg [in]: ��Ϣ wms_client_message_s_type �ṹָ�롣
    delcachetag [in]: only used while pMsg->msg_hdr.tag == WMS_TAG_NONE
    
����ֵ:
    none
       
��ע:
    �����ڲ���ڵ�ǰ���ȼ��� cache info ��������������Ӧ�ڵ���ɾ��֮��
    Ȼ����Ϊ�½ڵ���롣pMsg->msg_hdr.tag == WMS_TAG_NONE ��ʾҪɾ��
    Cache �ڵ㣬��ʱҪ�õ� delcachetag
    
==============================================================================*/
void OEMWMS_MsgInfoCacheCbExt(const wms_client_message_s_type  *pMsg,
                              wms_message_tag_e_type    delcachetag);

/*==============================================================================
����: 
    wms_cacheinfolist_enumbegin
       
˵��: 
    cache info ����ö�ٳ�ʼ������
       
����: 
    ebox [in]: ����Ϣ�洢�䡣
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
void wms_cacheinfolist_enumbegin(wms_box_e_type ebox);

/*==============================================================================
����: 
    wms_cacheinfolist_enumtoend
       
˵��: 
    cache info ����ö�ٵ�β�ڵ�
       
����: 
    ebox [in]: ����Ϣ�洢�䡣
    
����ֵ:
    none
       
��ע:
    
==============================================================================*/
void wms_cacheinfolist_enumtoend(wms_box_e_type ebox);

/*==============================================================================
����: 
    wms_cacheinfolist_enumnext
       
˵��: 
    cache info ����ö����һ���ڵ�ĺ���
       
����: 
    ebox [in]: ����Ϣ�洢�䡣
    
����ֵ:
    none
       
��ע:
    ��һ�ε��ô˺���ǰ�����ȵ��� wms_cacheinfolist_enumbegin() ������
==============================================================================*/
wms_cache_info_node *wms_cacheinfolist_enumnext(wms_box_e_type ebox);

/*==============================================================================
����: 
    wms_cacheinfolist_enumprev
       
˵��: 
    cache info ����ö����һ���ڵ�ĺ���
       
����: 
    ebox [in]: ����Ϣ�洢�䡣
    
����ֵ:
    none
       
��ע:
    ��һ�ε��ô˺���ǰ�����ȵ��� wms_cacheinfolist_enumbegin() ������
==============================================================================*/
wms_cache_info_node *wms_cacheinfolist_enumprev(wms_box_e_type ebox);

/*==============================================================================
����: 
    wms_cacheinfolist_enumtoxuhao
       
˵��: 
    cache info ����ö������ָ����ŵĽڵ�ĺ���
       
����: 
    ebox [in]: ����Ϣ�洢�䡣
    xuhao [in]: ָ����š�
    
����ֵ:
    none
       
��ע:
   ���ô˺���ǰ�����ȵ��� wms_cacheinfolist_updatexuhao() ������
==============================================================================*/
void wms_cacheinfolist_enumtoxuhao(wms_box_e_type ebox, int xuhao);

/*==============================================================================
����: 
    wms_cacheinfolist_getcounts
       
˵��: 
    ������������ڵ�������������Ϣ������
       
����: 
    box [in]: ����Ϣ�洢�䡣
    pNew[in]: ���ڷ�������Ϣ��Ŀ�� buffer ָ�롣
    pOnUIM[in]: ���ڷ��ش��ڿ�����Ϣ��Ŀ�� buffer ָ�롣
    pTotal[in]: ���ڷ�����Ϣ����Ŀ�� buffer ָ�롣
    
����ֵ:
    ����ڵ���
       
��ע:

==============================================================================*/
void wms_cacheinfolist_getcounts(wms_box_e_type box, 
                                 uint16 *pNew, 
                                 uint16 *pOnUIM,
                                 uint16 *pTotal);
                                 
/*==============================================================================
����: 
    wms_cacheinfolist_getnode
       
˵��: 
    �������ط��������Ľڵ�ָ�롣
       
����: 
    eBox [in]: ��Ϣ������
    mem_store [in]: ��Ϣ�洢λ��
    index[in]: ��Ϣ�ڴ洢�ռ�������š�
    
����ֵ:
    NULL �� ���������Ľڵ�ָ�롣
       
��ע:

==============================================================================*/
wms_cache_info_node *wms_cacheinfolist_getnode(wms_box_e_type  eBox,
                                               wms_memory_store_e_type mem_store, 
                                               uint16 index);

/*==============================================================================
����: 
    wms_cacheinfolist_updatenodetag
       
˵��: 
    �������ط��������Ľڵ�ָ�롣
       
����: 
    mem_store [in]: ��Ϣ�洢λ��
    index[in]: ��Ϣ�ڴ洢�ռ�������š�
    newtag[in]: ��Ϣ�µı�ʶ

����ֵ:
    TRUE: ���ĳɹ�
    FALSE:����ʧ��
       
��ע:

==============================================================================*/
boolean wms_cacheinfolist_updatenodetag(wms_memory_store_e_type mem_store, 
                                     uint16 index,
                                     wms_message_tag_e_type newtag);
                                     
/*==============================================================================
����: 
    wms_get_cacheinfolist
       
˵��: 
    ���������������������߽ڵ���š�
       
����: 
    ebox [in]: ����Ϣ�洢��
    
����ֵ:
    �ɹ�: ����Ϣ�洢���Ӧ�� cache ����ָ�롣
    ʧ��: NULL
       
��ע:
     
==============================================================================*/
void wms_cacheinfolist_updatexuhao(wms_box_e_type ebox);
#endif // _OEMWMS_CACHEINFO_H

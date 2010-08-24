#ifndef RECENTCALL_H
#define RECENTCALL_H

/*==============================================================================
// �ļ���
//        recentcalls.h
//        2007-11-18 ��ϲ�����汾(Draft Version)
//        ��Ȩ����(c) 2007 Anylook
//        
// �ļ�˵����
//        
// ���ߣ�2007-11-18
// �������ڣ���ϲ��
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
07-11-18         ��ϲ��         ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/


/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/


/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
//#include "aeecalllist.h"
#include "CallInfo.h"
typedef struct _Record{
    AECHAR *number;        // number
    AECHAR *name;           // name
    //boolean  is_number;      // number only or not
    callPIType pi;                // PI
    uint16 category;          // call type
    uint16 counter;            // call counter
    uint32 time_stamp;     // years to seconds
    uint32 duration;          // call duration
    uint16 wIndex;            //Index
}Record;

typedef struct _sSelectRecordListNode sSelectRecordListNode ;

struct _sSelectRecordListNode
{
    sSelectRecordListNode* pPreNode;
    sSelectRecordListNode* pNextNode;
    Record                        SelectRecord;
};

//#include "recentcalls_priv.h"
typedef boolean (*PFNSELECTEDRECENTCALLSCB)(sSelectRecordListNode* pSelectRecordListRoot);

typedef enum _SelectRecordType{
    SELECT_ONE_NUMBER,
    SELECT_MULTI_NUMBER
}SelectRecordType;
/*==============================================================================
                                 ��ӿڶ���
==============================================================================*/
typedef struct _IRecentCalls  IRecentCalls;
QINTERFACE(IRecentCalls)
{
    INHERIT_IApplet(IRecentCalls);
    void (*SendList)(IRecentCalls *p);
    void (*MissedList)(IRecentCalls *p);
    void (*CallList)(IRecentCalls *p, PFNSELECTEDRECENTCALLSCB cb, SelectRecordType t); //modfied by chengxiao 2009.02.19
};


/*==============================================================================
                                 ��ӿں�������
==============================================================================*/
#define IRecentCalls_AddRef(p)                GET_PVTBL(p,IRecentCalls)->AddRef(p)

#define IRecentCalls_Release(p)                GET_PVTBL(p,IRecentCalls)->Release(p)

#define IRecentCalls_HandleEvent(p,ec,wp,dw) \
                                    GET_PVTBL(p,IRecentCalls)->HandleEvent(p, ec, wp, dw)

#define IRecentCalls_SendList(p)    GET_PVTBL(p,IRecentCalls)->SendList(p)

#define IRecentCalls_MissedList(p)  GET_PVTBL(p,IRecentCalls)->MissedList(p)

#define IRecentCalls_CallList(p,cb,t) GET_PVTBL(p,IRecentCalls)->CallList(p,cb,t) //modfied by chengxiao 2009.02.19
#endif
//RECENTCALL_H


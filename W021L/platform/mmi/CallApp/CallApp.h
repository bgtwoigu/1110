#ifndef CALLAPP_H
#define CALLAPP_H

/*==============================================================================
// �ļ���
//        CallApp.h
//        2007-11-01 Gemsea����汾(Draft Version)
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//
// �ļ�˵����
//
// ���ߣ�Gemsea
// �������ڣ�2007-11-01
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��    ���ⵥ��  �޸����ݡ�λ�ü�ԭ��
-----------      ----------  --------  -----------------------------------------------
2007-11-01                  ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "CallInfo.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/


/*==============================================================================
                                 ���Ͷ���
==============================================================================*/

/*==============================================================================
                                 ��ӿڶ���
==============================================================================*/
typedef struct _ICallApp  ICallApp;
 QINTERFACE(ICallApp)
{
    INHERIT_IApplet(ICallApp);
    int (*CallNumber)(ICallApp *p, AECHAR *number);	
    int (*BtCallNumber)(ICallApp *p, AECHAR *number);
    //boolean (*IsRestictCallNumber)(ICallApp *p, AECHAR *n, boolean b);
    //int (*SetStartCallType)(ICallApp *p, start_call_type type, AECHAR *num, callPIType pi, AECHAR *name);
    int (*Make_IP_Call_F)(ICallApp *p, AECHAR *number);
    //int (*Make_Call_F)(ICallApp *p, boolean bCloseAll,AECHAR *number);
    int (*VoiceCallDial_F)(ICallApp *p, AECHAR *number);
#ifdef FEATURE_UTK2
    int (*Make_UTK_Call_F)(ICallApp *p);
#endif
#ifdef FEATURE_EDITABLE_RECORD
    int (*EditRecNumber)(ICallApp *p, AECHAR *number, uint16 call_type);
#endif
};


/*==============================================================================
                                 ��ӿں�������
==============================================================================*/
#define ICallApp_New(ps,mo,pp)               CallApp_New(ps,mo,pp)

#define ICallApp_AddRef(p)                GET_PVTBL(p,ICallApp)->AddRef(p)

#define ICallApp_Release(p)               GET_PVTBL(p,ICallApp)->Release(p)
#define ICallApp_HandleEvent(p,ec,wp,dw)  GET_PVTBL(p,ICallApp)->HandleEvent(p,ec,wp,dw)
//#define ICallApp_SetStartCallType(p,t,nu,pi,na)          GET_PVTBL(p,ICallApp)->SetStartCallType(p,t,nu,pi,na)
#define ICallApp_CallNumber(p,n)          GET_PVTBL(p,ICallApp)->CallNumber(p,n)
#define ICallApp_BTCallNumber(p,n)          GET_PVTBL(p,ICallApp)->BtCallNumber(p,n)
//#define ICallApp_IsRestictCallNumber(p,n,b)  GET_PVTBL(p,ICallApp)->IsRestictCallNumber(p,n,b)
#define ICallApp_Make_IP_Call_F(p,n)  GET_PVTBL(p,ICallApp)->Make_IP_Call_F(p,n)
//#define ICallApp_Make_Call_F(p,t,n)  GET_PVTBL(p,ICallApp)->Make_Call_F(p,t,n)
#define ICallApp_VoiceCallDial_F(p,n)  GET_PVTBL(p,ICallApp)->VoiceCallDial_F(p,n)
//#define ICallApp_Release(p)                GET_PVTBL(p,ICallApp)->Release(p)
#ifdef FEATURE_UTK2
#define ICallApp_Make_UTK_Call_F(p)  GET_PVTBL(p,ICallApp)->Make_UTK_Call_F(p)
#endif /* FEATURE_UTK2 */
#ifdef FEATURE_EDITABLE_RECORD
#define ICallApp_EditRecNumber(p,n,t)    GET_PVTBL(p,ICallApp)->EditRecNumber(p, n, t)
#endif

boolean MakeVoiceCall(IShell *pShell, boolean bCloseAll, AECHAR *number);

#endif//CALLAPP_H

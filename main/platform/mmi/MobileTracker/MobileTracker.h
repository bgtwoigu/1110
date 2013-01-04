/** ----------------------------------------------------------------------------
 * MobileTracker.h
 * 
 * Copyright(c) 2007 Anylook
 * 
 * 
 * History:
 * 
 * when         who            what and why
 * -----------  -----------    -----------------------------
 * 2008.12.29            4 X 3 large icon style menu * list menu
 * 
 * -----------------------------------------------------------------------------
 */

#ifndef MOBILETRACKER_H
#define MOBILETRACKER_H
/*==============================================================================
                                 
                                ����������Ͷ���
                                 
==============================================================================*/
//Applet�����Ĳ�ͬ״̬
typedef enum _MobileTrackerServiceType 
{
    STARTMETHOD_PLAYER,       // ISHELL_StartApplet()
    STARTMETHOD_DATA          // ISHELL_StartApplet()
}MobileTrackerServiceType;

typedef struct _IMobileTracker  IMobileTracker;
 QINTERFACE(IMobileTracker)
{
    INHERIT_IApplet(IMobileTracker);

    // �ⲿ����MAINMENU�Ľӿں���
    int (*MobileTrackerServiceType)(IMobileTracker *p, MobileTrackerServiceType eStype);    
};

#define IMOBILETRACKER_AddRef(p)     GET_PVTBL(p,IMobileTracker)->AddRef(p)
#define IMOBILETRACKER_Release(p)    GET_PVTBL(p,IMobileTracker)->Release(p)
#define IMOBILETRACKER_HandleEvent(p,ec,wp,dw) GET_PVTBL(p,IMobileTracker)->HandleEvent(p, ec, wp, dw)


// �ⲿ���ٽ��뵽���ݷ����б�
#define IMOBILETRACKER_StartData(p)            GET_PVTBL(p,IMobileTracker)->MobileTrackerService(p, STARTMETHOD_DATA);



#endif

/** ----------------------------------------------------------------------------
 * Application.h
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

#ifndef APPLICATION_H
#define APPLICATION_H
/*==============================================================================
                                 
                                ����������Ͷ���
                                 
==============================================================================*/
//Applet�����Ĳ�ͬ״̬
typedef enum _ApplicationServiceType 
{
    STARTMETHOD_PLAYER,       // ISHELL_StartApplet()
    STARTMETHOD_DATA          // ISHELL_StartApplet()
}ApplicationServiceType;

typedef struct _IApplication  IApplication;
 QINTERFACE(IApplication)
{
    INHERIT_IApplet(IApplication);

    // �ⲿ����MAINMENU�Ľӿں���
    int (*ApplicationService)(IApplication *p, ApplicationServiceType eStype);    
};

#define IApplication_AddRef(p)     GET_PVTBL(p,IApplication)->AddRef(p)
#define IApplication_Release(p)    GET_PVTBL(p,IApplication)->Release(p)
#define IApplication_HandleEvent(p,ec,wp,dw) GET_PVTBL(p,IApplication)->HandleEvent(p, ec, wp, dw)


// �ⲿ���ٽ��뵽���ݷ����б�
#define IApplication_StartData(p)            GET_PVTBL(p,IApplication)->ApplicationService(p, STARTMETHOD_DATA);

#endif

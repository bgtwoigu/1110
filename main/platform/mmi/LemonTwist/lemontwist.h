/** ----------------------------------------------------------------------------
 * Lemontwist.h
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

#ifndef LEMONTWIST_H
#define LEMONTWIST_H
/*==============================================================================
                                 
                                ����������Ͷ���
                                 
==============================================================================*/
//Applet�����Ĳ�ͬ״̬
typedef enum _LemontwistServiceType 
{
    STARTMETHOD_PLAYER,       // ISHELL_StartApplet()
    STARTMETHOD_DATA          // ISHELL_StartApplet()
}LemontwistServiceType;

typedef struct _ILemontwist  ILemontwist;
 QINTERFACE(ILemontwist)
{
    INHERIT_IApplet(ILemontwist);

    // �ⲿ����MAINMENU�Ľӿں���
    int (*LemontwistService)(ILemontwist *p, LemontwistServiceType eStype);    
};

#define ILEMONTWIST_AddRef(p)     GET_PVTBL(p,ILemontwist)->AddRef(p)
#define ILEMONTWIST_Release(p)    GET_PVTBL(p,ILemontwist)->Release(p)
#define ILEMONTWIST_HandleEvent(p,ec,wp,dw) GET_PVTBL(p,ILemontwist)->HandleEvent(p, ec, wp, dw)


// �ⲿ���ٽ��뵽���ݷ����б�
#define ILEMONTWIST_StartData(p)            GET_PVTBL(p,ILemontwist)->LemontwistService(p, STARTMETHOD_DATA);



#endif

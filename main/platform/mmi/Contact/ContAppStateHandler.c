/*==============================================================================
// �ļ���
//        ContAppStateHandler.c
//        2004-06-09 jyh����汾(Draft Version)
//        ��Ȩ����(c) 2004 Anylook
//        
// �ļ�˵����
//        
// ���ߣ�2004-06-09
// �������ڣ�jyh
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ    ��      ��   ��   ��     �޸����ݡ�λ�ü�ԭ��
--------      -----------     -----------------------------------------------
04-06-09      jyh             ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "ContApp_priv.h"
#include "ContAppBuildMenu.h"
#include "ContAppCommon.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/

/*lint -e506 -e774 -e737*/
/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
typedef NextFSMAction (*FSMSTATE_HANDLER)(CContApp *pMe);


/*==============================================================================
                                 ��������
==============================================================================*/
// ״̬ STATE_NONE ������
static NextFSMAction Handler_STATE_NONE(CContApp *pMe);

// ״̬ STATE_INIT ������
static NextFSMAction Handler_STATE_INIT(CContApp *pMe);

// ״̬ STATE_LOAD ������
static NextFSMAction Handler_STATE_LOAD(CContApp *pMe);

// ״̬ STATE_LOCKED ������
static NextFSMAction Handler_STATE_LOCKED(CContApp *pMe);

// ״̬ STATE_MAINLIST ������
static NextFSMAction Handler_STATE_MAINLIST(CContApp *pMe);

// ״̬ STATE_MAINMENU ������
static NextFSMAction Handler_STATE_MAINMENU(CContApp *pMe);

// ״̬ STATE_OPTS ������
static NextFSMAction Handler_STATE_OPTS(CContApp *pMe);

// ״̬ STATE_VIEW ������
static NextFSMAction Handler_STATE_VIEW(CContApp *pMe);

// ״̬ STATE_FIND ������
static NextFSMAction Handler_STATE_FIND(CContApp *pMe);

// ״̬ STATE_FIND_LIST ������
static NextFSMAction Handler_STATE_FIND_LIST(CContApp *pMe);

// ״̬ STATE_FIND_OPTS ������
static NextFSMAction Handler_STATE_FIND_OPTS(CContApp *pMe);

// ״̬ STATE_GROUPVIEW ������
static NextFSMAction Handler_STATE_GROUPVIEW(CContApp *pMe);

// ״̬ STATE_GROUPVIEW_LIST ������
static NextFSMAction Handler_STATE_GROUPVIEW_LIST(CContApp *pMe);

// ״̬ STATE_GROUPVIEW_OTPS ������
static NextFSMAction Handler_STATE_GROUPVIEW_OPTS(CContApp *pMe);

// ״̬ STATE_ADDNEW ������
static NextFSMAction Handler_STATE_ADDNEW(CContApp *pMe);

// ״̬ STATE_ADDNEW_INPUT ������
static NextFSMAction Handler_STATE_ADDNEW_INPUT(CContApp *pMe);

// ״̬ STATE_SELECTGROUP ������
static NextFSMAction Handler_STATE_SELECTGROUP(CContApp *pMe);

#ifdef FEATURE_RUIM_PHONEBOOK
// ״̬ STATE_SELECTPOS ������
static NextFSMAction Handler_STATE_SELECTPOS(CContApp *pMe);
#endif// FEATURE_RUIM_PHONEBOOK

// ״̬ STATE_EDIT ������
static NextFSMAction Handler_STATE_EDIT(CContApp *pMe);

// ״̬ STATE_EDIT_INPUT ������
static NextFSMAction Handler_STATE_EDIT_INPUT(CContApp *pMe);

// ״̬ STATE_EDIT_SELECTGROUP ������
static NextFSMAction Handler_STATE_EDIT_SELECTGROUP(CContApp *pMe);

// ״̬ STATE_EDIT_RING ������
//static NextFSMAction Handler_STATE_EDIT_RING(CContApp *pMe);

// ״̬ STATE_DELETE ������
static NextFSMAction Handler_STATE_DELETE(CContApp *pMe);
// ״̬ STATE_SELECT_RECORD ������
static NextFSMAction Handler_STATE_SELECT_RECORD(CContApp *pMe);

#ifdef FEATURE_RUIM_PHONEBOOK
// ״̬ STATE_COPYMOVE ������
static NextFSMAction Handler_STATE_COPYMOVE(CContApp *pMe);

// ״̬ STATE_COPYMOVEALL ������
static NextFSMAction Handler_STATE_COPYMOVEALL(CContApp *pMe);

// ״̬ STATE_COPY ������
static NextFSMAction Handler_STATE_COPY(CContApp *pMe);

// ״̬ STATE_SETTING ������
static NextFSMAction Handler_STATE_SETTING(CContApp *pMe);

// ״̬ STATE_SAVETO ������
static NextFSMAction Handler_STATE_SAVETO(CContApp *pMe);

// ״̬ STATE_VIEWTYPE ������
static NextFSMAction Handler_STATE_VIEWTYPE(CContApp *pMe);
#endif /* FEATURE_RUIM_PHONEBOOK */

// ״̬ STATE_CAPACITY ������
static NextFSMAction Handler_STATE_CAPACITY(CContApp *pMe);

// ״̬ STATE_ONEDIAL ������
static NextFSMAction Handler_STATE_ONEDIAL(CContApp *pMe);

// ״̬ STATE_ONEDIAL ������
static NextFSMAction Handler_STATE_ONEDIAL_OPT(CContApp *pMe);

// ״̬ STATE_ONEDIAL_SET ������
static NextFSMAction Handler_STATE_ONEDIAL_SET(CContApp *pMe);

// ״̬ STATE_ONEDIAL_SET ������
static NextFSMAction Handler_STATE_ONEDIAL_SET_NUMFLDSEL(CContApp *pMe);

// ״̬ STATE_SAVEFLD ������
static NextFSMAction Handler_STATE_SAVEFLD(CContApp *pMe);

// ״̬ STATE_SAVEFLD_LIST ������
static NextFSMAction Handler_STATE_SAVEFLD_LIST(CContApp *pMe);

// ״̬ STATE_SELECT ������
static NextFSMAction Handler_STATE_SELECT(CContApp *pMe);

// ״̬ STATE_SELECTFLD_VIEW ������
static NextFSMAction Handler_STATE_SELECTFLD_VIEW(CContApp *pMe);

// ״̬ STATE_SELECT_RETURN ������
static NextFSMAction Handler_STATE_SELECT_RETURN(CContApp *pMe);

// ״̬ STATE_FINDNUM ������
static NextFSMAction Handler_STATE_FINDNUM(CContApp *pMe);

// ״̬ STATE_FINDNUM_OPTS ������
static NextFSMAction Handler_STATE_FINDNUM_OPTS(CContApp *pMe);

// ״̬ STATE_SMARTCALL ������
static NextFSMAction Handler_STATE_SMARTCALL(CContApp *pMe);

// ״̬ STATE_SMARTCALL_VIEW ������
static NextFSMAction Handler_STATE_SMARTCALL_VIEW(CContApp *pMe);

// ״̬ STATE_SEARCH ������
static NextFSMAction Handler_STATE_SEARCH(CContApp *pMe);

// ״̬ STATE_SEARCH_NAME ������
static NextFSMAction Handler_STATE_SEARCHNAME(CContApp *pMe);

// ״̬ STATE_SUCCESS ������
static NextFSMAction Handler_STATE_SUCCESS(CContApp *pMe);

// ״̬ STATE_MSG ������
static NextFSMAction Handler_STATE_MSG(CContApp *pMe);

// ״̬ STATE_ERROR ������
static NextFSMAction Handler_STATE_ERROR(CContApp *pMe);

// ״̬ STATE_EXIT ������
static NextFSMAction Handler_STATE_EXIT(CContApp *pMe);

// ״̬ STATE_DETAIL ������
static NextFSMAction Handler_STATE_DETAIL(CContApp *pMe);

static NextFSMAction Handler_STATE_COPYING(CContApp *pMe);

// ״̬ STATE_GROUPOPT ������
static NextFSMAction Handler_STATE_GROUPOPT(CContApp *pMe);

// ״̬ STATE_GROUPOPT_EDIT ������
static NextFSMAction Handler_STATE_GROUPOPT_EDIT(CContApp *pMe);

// ״̬ STATE_MANAGEMENT ������
static NextFSMAction Handler_STATE_MANAGEMENT(CContApp *pMe);

// ״̬ STATE_DELETE_SELECT ������
static NextFSMAction Handler_STATE_DELETE_SELECT(CContApp *pMe);

// ״̬ STATE_POPNUMFLD ������
static NextFSMAction Handler_STATE_POPNUMFLD(CContApp *pMe);

// ״̬ STATE_SELECTOPT ������
static NextFSMAction Handler_STATE_SELECTOPT(CContApp *pMe);

// ״̬ STATE_NUMFLDVIEW ������
static NextFSMAction Handler_STATE_NUMFLDVIEW(CContApp *pMe);

// ״̬ STATE_DELETEALL ������
static NextFSMAction Handler_STATE_DELETING(CContApp *pMe);

// ״̬ STATE_DETAIL_MULTI ������
static NextFSMAction Handler_STATE_DETAIL_MULTI(CContApp *pMe);

/*==============================================================================
                                 ȫ������
==============================================================================*/


/*==============================================================================
                                 ���أ���̬������
==============================================================================*/
//ע�⣺������������б���_priv.h�е�FSMState���塰���롱Ҫ���ֶ�Ӧ��
//�򵥵������ǽ�״̬�б��Ƶ�����Ȼ����ǰ�����Handler_��
static const FSMSTATE_HANDLER gFSMStateHandler[] =
{
    Handler_STATE_NONE,		//0x0
    Handler_STATE_INIT,		//0x1
    Handler_STATE_LOAD,		//0x2
    Handler_STATE_LOCKED,	//0x3
    Handler_STATE_MAINLIST,	//0x4
    Handler_STATE_MAINMENU, //0x5
    Handler_STATE_OPTS,		//0x6
    Handler_STATE_VIEW,		//0x7	
    Handler_STATE_FIND,		//0x8
    Handler_STATE_FIND_LIST,	//0X9
    Handler_STATE_FIND_OPTS,	//0X10
    Handler_STATE_GROUPVIEW,	//0X11
    Handler_STATE_GROUPVIEW_LIST,	//0X12
    Handler_STATE_GROUPVIEW_OPTS,	//0X13
    Handler_STATE_ADDNEW,	//0X14
    Handler_STATE_ADDNEW_INPUT, //0x15
    Handler_STATE_SELECTGROUP,	//0X16
#ifdef FEATURE_RUIM_PHONEBOOK
    Handler_STATE_SELECTPOS,	//0X17
#endif// FEATURE_RUIM_PHONEBOOK
    Handler_STATE_EDIT,	//0X18
    Handler_STATE_EDIT_INPUT,	//0X19
    Handler_STATE_EDIT_SELECTGROUP,	//0X20
//    Handler_STATE_EDIT_RING,
    Handler_STATE_DELETE,	//0X21
#ifdef FEATURE_RUIM_PHONEBOOK
    Handler_STATE_COPYMOVE,	//0X22
    Handler_STATE_COPYMOVEALL,	//0X23
    Handler_STATE_COPY,	//0X24
    Handler_STATE_SETTING,	//0X25
    Handler_STATE_SAVETO,	//0X26
    Handler_STATE_VIEWTYPE,	//0X27
#endif /* FEATURE_RUIM_PHONEBOOK*/
    Handler_STATE_CAPACITY,	//0X28
    Handler_STATE_ONEDIAL,	//0X29
    Handler_STATE_ONEDIAL_OPT,	//0X30
    Handler_STATE_ONEDIAL_SET,	//0X31
    Handler_STATE_ONEDIAL_SET_NUMFLDSEL,	//0X32
    Handler_STATE_SAVEFLD,	//0X33
    Handler_STATE_SAVEFLD_LIST,	//0X34
    Handler_STATE_SELECT,	//0X35
    Handler_STATE_SELECTFLD_VIEW,	//0X36
    Handler_STATE_SELECT_RETURN,	//0X37
    Handler_STATE_FINDNUM,	//0X38
    Handler_STATE_FINDNUM_OPTS,//0X39
    Handler_STATE_SMARTCALL,	//0X40
    Handler_STATE_SMARTCALL_VIEW,	//0X41
    Handler_STATE_SEARCH, //0x42
    Handler_STATE_SEARCHNAME, //0x43
    Handler_STATE_DETAIL,//0X44
    Handler_STATE_SUCCESS,	//0X45
    Handler_STATE_MSG,	//0X46
    Handler_STATE_ERROR,//0x47
    Handler_STATE_EXIT,//0x48
    Handler_STATE_COPYING,
    Handler_STATE_GROUPOPT,
    Handler_STATE_GROUPOPT_EDIT,
    Handler_STATE_MANAGEMENT, /* 0X52*/
    Handler_STATE_DELETE_SELECT, /* 0X53 */
    Handler_STATE_POPNUMFLD,     /* 0X54 */
    Handler_STATE_SELECTOPT ,        /*0x55 */
    Handler_STATE_NUMFLDVIEW,        /*0X56 */
    Handler_STATE_DELETING,         /*0X57*/
    Handler_STATE_SELECT_RECORD,     /*0X58*/
    Handler_STATE_DETAIL_MULTI      /* OX59 */
};


/*==============================================================================
                                 ��������
==============================================================================*/
/*==============================================================================
����:
       CContApp_ProcessState
˵��:
       ContApp Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��ContApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction CContApp_ProcessState(CContApp *pMe)
{
    int nSize;

#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif

    nSize = ARRAY_SIZE(gFSMStateHandler);
    if ((int)pMe->m_eCurState < nSize )
    {
        // ���ݵ�ǰ״̬������Ӧ��״̬������
        return  gFSMStateHandler[pMe->m_eCurState](pMe);
    }
    else
    {
#if defined(AEE_STATIC)
        ASSERT_NOT_REACHABLE
#endif
        return NFSMACTION_WAIT;
    }
}// CContApp_ProcessState

/*==============================================================================
������
       Handler_STATE_NONE
˵����
       STATE_NONE ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_NONE(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif

    FARF(STATE, ("STATE_NONE"));
    
    MOVE_TO_STATE(STATE_EXIT);
    return NFSMACTION_CONTINUE;
} // Handler_STATE_NONE

/*==============================================================================
������
       Handler_STATE_INIT
˵����
       STATE_INIT ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_INIT(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    MSG_FATAL("Handler_STATE_INIT Start",0,0,0);

    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NOT_UIMADDRINIT))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_EXIT)
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(STATE_EXIT)
            return NFSMACTION_CONTINUE;
    }   
    
    MOVE_TO_STATE(STATE_LOAD); 
    MSG_FATAL("Handler_STATE_INIT End",0,0,0);
    return NFSMACTION_CONTINUE;
} // Handler_STATE_INIT

/*==============================================================================
������
       Handler_STATE_LOAD
˵����
       STATE_LOAD ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_LOAD(CContApp *pMe)
{
    int     nRet;
    boolean isLocked;
    
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    MSG_FATAL("Handler_STATE_LOAD Start",0,0,0);
  
    if (FALSE == pMe->m_bUnLock)    
    {
        //�Ƿ���ס�绰��
        (void) ICONFIG_GetItem(pMe->m_pConfig, CFGI_PHONEBOOK_LOCK_CHECK,
                                            &isLocked, sizeof(isLocked));

        if (isLocked)
        {
            MOVE_TO_STATE(STATE_LOCKED)
            MSG_FATAL("Handler_STATE_LOAD isLocked",0,0,0);
            return NFSMACTION_CONTINUE;
        }
    }

    nRet = CContApp_GetConfig( pMe, 
                               CONTCFG_VIEWTYPE, 
                               &pMe->m_nViewType, 
                               sizeof(pMe->m_nViewType));
    
    if(nRet != SUCCESS)
    {
        pMe->m_wErrStrID = IDS_ERR_GETCONFIG;
        MOVE_TO_STATE(STATE_ERROR);
        MSG_FATAL("Handler_STATE_LOAD nRet != SUCCESS nRet=%d",nRet,0,0);
        return NFSMACTION_CONTINUE;
    }
    
    //�ж���������
    switch(pMe->m_eStartMethod)
    {
        //if the applet started normally by clicking contacts applet
        case STARTMETHOD_NORMAL:
            MSG_FATAL("Handler_STATE_LOAD STARTMETHOD_NORMAL",0,0,0);
            pMe->m_wMainListSel = MENU_SELECT_NULL;
            pMe->m_eListFrom = LISTFROMNOMAL;
            FREEIF(pMe->m_szAlpha);
            MOVE_TO_STATE(STATE_MAINLIST);
            break;

        case STARTMETHOD_ADDEMAIL:
        case STARTMETHOD_ADDURL:
            MSG_FATAL("Handler_STATE_LOAD STARTMETHOD_ADDURL",0,0,0);
            CContApp_LoadPhoneBook(pMe, CONTCFG_VIEWTYPE_PHONE);
            pMe->m_wMainListSel = MENU_SELECT_NULL;
            MOVE_TO_STATE(STATE_MAINLIST);
            return NFSMACTION_CONTINUE;
            
        case STARTMETHOD_ADDFIELD:
            MSG_FATAL("Handler_STATE_LOAD STARTMETHOD_ADDFIELD",0,0,0);
            MOVE_TO_STATE(STATE_MAINLIST);
            break;
        
        case STARTMETHOD_SAVEFIELD:
            //guoys comment 2008.10.22 for fix save number cant list when max record
           // pMe->m_nViewType = CONTCFG_VIEWTYPE_NONE;
#ifdef FEATURE_RUIM_PHONEBOOK
            if( IsRunAsUIMVersion() )
            {
                MSG_FATAL("Handler_STATE_LOAD IsRunAsUIMVersion()",0,0,0);
                //guoys modified for PR4.2.1 begin
                (void)CContApp_GetConfig(pMe, CONTCFG_SAVETYPE, &pMe->m_nSaveType, sizeof(byte));
                if(pMe->m_nSaveType == CONTCFG_SAVETYPE_SELECT)
                {
                    MOVE_TO_STATE(STATE_SELECTPOS);
                }
                else
                {
                    MOVE_TO_STATE(STATE_ADDNEW);
                }
                //guoys modified for PR4.2.1 end	
            }
            else
            {
                MSG_FATAL("Handler_STATE_LOAD IsRunAsUIMVersion() 2",0,0,0);
                MOVE_TO_STATE(STATE_ADDNEW);
            }
#else            
            {
                MSG_FATAL("Handler_STATE_LOAD IsRunAsUIMVersion() 3",0,0,0);
                MOVE_TO_STATE(STATE_ADDNEW);
            }
#endif// FEATURE_RUIM_PHONEBOOK

            pMe->m_nGroupCat = AEE_ADDR_CAT_OTHER;
            pMe->m_wSelectEdit = IDI_ADDNEW_MENU_NAME;
            pMe->m_nAddnewOrEdit = ADDOREDIT_ADD;
            //MOVE_TO_STATE(STATE_SAVEFLD);
            break;
            
        case STARTMETHOD_FINDNUMBER:
            MSG_FATAL("Handler_STATE_LOAD STARTMETHOD_FINDNUMBER",0,0,0);
            pMe->m_nViewType = CONTCFG_VIEWTYPE_ALL;
            if(SUCCESS != CContApp_LoadByNum(pMe, pMe->m_pFindNum))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            FREEIF(pMe->m_pFindNum);
            MOVE_TO_STATE(STATE_FINDNUM);
            return NFSMACTION_CONTINUE;

        case STARTMETHOD_GROUPLIST:
            MSG_FATAL("Handler_STATE_LOAD STARTMETHOD_GROUPLIST",0,0,0);
            pMe->m_wselGroupId = IDS_GROUP_NONE_STRING;
            MOVE_TO_STATE(STATE_GROUPVIEW);
            break;
            
        case STARTMETHOD_SELECTFIELD:
            //pMe->m_wSelectSmart = CONTAPP_RECID_NULL;
            MSG_FATAL("Handler_STATE_LOAD STARTMETHOD_SELECTFIELD",0,0,0);
            pMe->m_wMainListSel = MENU_SELECT_NULL;
            if(pMe->m_wSelFldType == SINGLE_SELECT_NUMBER)
            {
                CContApp_LoadPhoneBook(pMe, CONTCFG_VIEWTYPE_PHONE);
                //pMe->m_eListFrom = LISTFROMNOMAL;
                FREEIF(pMe->m_szAlpha);
                MOVE_TO_STATE(STATE_MAINLIST);
            }
            else
            {
                MOVE_TO_STATE(STATE_SELECT);
            }
            break;

        case STARTMETHOD_SMARTCALL:
            MSG_FATAL("Handler_STATE_LOAD STARTMETHOD_SMARTCALL",0,0,0);
            pMe->m_nViewType = CONTCFG_VIEWTYPE_ALL;
            if(SUCCESS != CContApp_LoadbySmart(pMe, pMe->m_pFindNum))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            FREEIF(pMe->m_pFindNum);
            MOVE_TO_STATE(STATE_SMARTCALL);
            return NFSMACTION_CONTINUE;

        case STARTMETHOD_MAINMENU:
            MSG_FATAL("Handler_STATE_LOAD STARTMETHOD_MAINMENU",0,0,0);
            pMe->m_wMainMenuSel=IDI_MAINMENU_MENU_DIRECTORTLIST;
            MOVE_TO_STATE(STATE_MAINMENU);
		break;
		
        default:
#if defined(AEE_STATIC)
            MSG_FATAL("Handler_STATE_LOAD default",0,0,0);
            ASSERT_NOT_REACHABLE;
#endif
            break;
    }
    
    // Load contacts
    if(SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType))
    {
        MSG_FATAL("Handler_STATE_LOAD SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType)",0,0,0);
        pMe->m_wErrStrID = IDS_ERR_LOAD;
        MOVE_TO_STATE(STATE_ERROR);
        return NFSMACTION_CONTINUE;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_LOAD

/*==============================================================================
������
       Handler_STATE_LOCKED
˵����
       STATE_LOCKED ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_LOCKED(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    MSG_FATAL("Handler_STATE_LOCKED Start",0,0,0);
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_INPUT))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
        }
        
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            if( TRUE == pMe->m_nDeleteAllCheck)
            {
                MOVE_TO_STATE(STATE_DELETE_SELECT);
            }
            else
            {
                MOVE_TO_STATE(STATE_EXIT);
            }
            break;
        
        case DLGRET_OK:
            if( TRUE == pMe->m_nDeleteAllCheck)
            {
                MOVE_TO_STATE(STATE_DELETING);
            }
            else
            {
                // Get old pass word
                {
                    pMe->m_bUnLock = TRUE;
                    MSG_FATAL("Handler_STATE_LOCKED DLGRET_OK",0,0,0);
                    MOVE_TO_STATE(STATE_LOAD);
                }
            }
            break;
        
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_LOCKED);
            break;
            
        case DLGRET_INPUT_NULL:
            // password can not be invalid
            pMe->m_eMsgType = MESSAGE_WARNNING;
             if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_MSG_PSWERR))
             {
                   MOVE_TO_STATE(STATE_EXIT);
                   return NFSMACTION_CONTINUE;
             }
              return NFSMACTION_WAIT;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    MSG_FATAL("Handler_STATE_LOCKED End",0,0,0);
    return NFSMACTION_CONTINUE;
} // Handler_STATE_LOCKED

/*==============================================================================
������
       Handler_STATE_MAINLIST
˵����
       STATE_MAINLIST ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_MAINLIST(CContApp *pMe)
{

    int nRet = SUCCESS;
    int oneDialIndex;
    
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    MSG_FATAL("STATE_MAINLIST %d",pMe->m_eDlgRet,0,0);
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // ��¼�Ƿ�Ϊ��
            MSG_FATAL("Handler_STATE_MAINLIST DLGRET_CREATE",0,0,0);
            if(IS_ZERO_REC())
            {
                if(pMe->m_ePreState == STATE_OPTS)
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                
                if(STARTMETHOD_ADDFIELD == pMe->m_eStartMethod)
                {
                        // Show dialog message
                        pMe->m_eMsgType = MESSAGE_WARNNING;
                        if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_EMPTY))
                        {
                            MOVE_TO_STATE(STATE_EXIT);
                            return NFSMACTION_CONTINUE;
                        }
                        return NFSMACTION_WAIT;
                }
            }
            
            // Restore the menu select
            //if((pMe->m_wMainListSel > 0) &&(pMe->m_wMainListSel < CONTAPP_RECID_NULL))
            POP_LISTMENU_SEL(pMe->m_wMainListSel);
            
            // Restore the menu index
            POP_LISTMENU_IDX(pMe->m_wMainListIdx);
            
            // Show dialog message
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_LIST))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;

        case DLGRET_YES:
            // check speed dial
            MSG_FATAL("Handler_STATE_MAINLIST DLGRET_YES",0,0,0);
            for(oneDialIndex=CONTCFG_ONEDIAL1; oneDialIndex<=CONTCFG_ONEDIAL8; oneDialIndex++)
            {
                if(SUCCESS != CContApp_GetConfig( pMe,
                                          (ContAppCFG)oneDialIndex,
                                          pMe->m_pOneDialBuf,
                                          sizeof(pMe->m_pOneDialBuf)))
                {
                    continue;
                }

                // ���ڵ�����������Ҫ�޸ĵ�������
                if(WSTRCMP(pMe->m_pAddNewMobile, pMe->m_pOneDialBuf) == 0)
                {
                    (void)CContApp_SetConfig( pMe,
                                  (ContAppCFG)oneDialIndex,
                                  pMe->m_pFldInputBuf,
                                  sizeof(pMe->m_pFldInputBuf));
                }
            }
            
            nRet = CContApp_SaveEditField(pMe, pMe->m_nFldInputID);
            
            if( SUCCESS == nRet)
            {
                FREEIF(pMe->m_pFldInputBuf);
                pMe->m_eSuccessRetState = STATE_EXIT;
            
                MOVE_TO_STATE(STATE_SUCCESS);
            }
            else
            {
                pMe->m_wErrStrID = IDS_ERR_ADDFIELD;
                MOVE_TO_STATE(STATE_ERROR);
            }
            break;
            
        case DLGRET_NO:
            MSG_FATAL("Handler_STATE_MAINLIST DLGRET_NO",0,0,0);
            MOVE_TO_STATE(STATE_MAINLIST);
            break;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_OK:
            // store the menu select
            MSG_FATAL("Handler_STATE_MAINLIST DLGRET_OK",0,0,0);
            PUSH_LISTMENU_SEL(pMe->m_wMainListSel);
            
            // store the menu index
            PUSH_LISTMENU_IDX(pMe->m_wMainListIdx);

            switch(pMe->m_eStartMethod)
            {
                case STARTMETHOD_ADDFIELD:
                {
                    pMe->m_nAddnewOrEdit = ADDOREDIT_EDIT;
                    
                    if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                          pMe->m_wEditCont,
                                                          SELECT_ALL))
                     {
                         pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                         MOVE_TO_STATE(STATE_ERROR);
                         return NFSMACTION_CONTINUE;
                     }
                     
                     CContApp_GetRecByID(pMe, pMe->m_wEditCont); 
                     
                    if(!IS_RUIM_REC(pMe->m_wEditCont))
                    {
                        MOVE_TO_STATE(STATE_NUMFLDVIEW);
                    }
                    else
                    {
                        // ��ӵ����Ͽ϶��ǳ�ͻ��
                        pMe->m_nFldInputID = AEE_ADDRFIELD_PHONE_GENERIC;
                        if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                                                IDS_REPLACE,
                                                                NULL,
                                                                -1))
                        {
                            MOVE_TO_STATE(STATE_EXIT);
                            return NFSMACTION_CONTINUE;
                        }
                        return NFSMACTION_WAIT;
                        
                        //MOVE_TO_STATE(STATE_EDIT);
                    }
                }
                break;

                case STARTMETHOD_ADDEMAIL:
                    MSG_FATAL("Handler_STATE_MAINLIST STARTMETHOD_ADDEMAIL",0,0,0);
                    if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                         pMe->m_wEditCont,
                                                         SELECT_ALL))
                    {
                        pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                        MOVE_TO_STATE(STATE_ERROR);
                        return NFSMACTION_CONTINUE;
                    }
                    
                    CContApp_GetRecByID(pMe, pMe->m_wEditCont); 
                    //FREEIF(pMe->m_pFldInputBuf);
                    //pMe->m_pFldInputBuf = pMe->m_pAddNewEMAIL;
                    //pMe->m_pAddNewEMAIL = NULL;
                    pMe->m_nFldInputID = AEE_ADDRFIELD_EMAIL;
                    FARF(ADDR, ("Add email %S", pMe->m_pFldInputBuf,0,0));
                    //wFldID = AEE_ADDRFIELD_EMAIL;
                    if((pMe->m_wFieldMask | MASK_ADDRFIELD_EMAIL) == pMe->m_wFieldMask)
                    {
                        if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                                                IDS_REPLACE,
                                                                NULL,
                                                                -1))
                        {
                            MOVE_TO_STATE(STATE_EXIT);
                            return NFSMACTION_CONTINUE;
                        }
                        return NFSMACTION_WAIT;
                    }
                    else
                    {

                        nRet = CContApp_SaveEditField(pMe, AEE_ADDRFIELD_EMAIL);
                        if( SUCCESS == nRet)
                        {
                            FREEIF(pMe->m_pFldInputBuf);
                            pMe->m_eSuccessRetState = STATE_EXIT;
                        
                            MOVE_TO_STATE(STATE_SUCCESS);
                        }
                        else
                        {
                            pMe->m_wErrStrID = IDS_ERR_ADDFIELD;
                            MOVE_TO_STATE(STATE_ERROR);
                        }
                    }
                    break;
                    
                case STARTMETHOD_ADDURL:
                    MSG_FATAL("Handler_STATE_MAINLIST STARTMETHOD_ADDURL",0,0,0);
                    if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                         pMe->m_wEditCont,
                                                         SELECT_ALL))
                    {
                        pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                        MOVE_TO_STATE(STATE_ERROR);
                        return NFSMACTION_CONTINUE;
                    }
                    
                    CContApp_GetRecByID(pMe, pMe->m_wEditCont); 
                    //FREEIF(pMe->m_pFldInputBuf);
                    //pMe->m_pFldInputBuf = pMe->m_pAddNewURL;
                    //pMe->m_pAddNewURL= NULL;
                    pMe->m_nFldInputID = AEE_ADDRFIELD_URL;
                    if((pMe->m_wFieldMask | MASK_ADDRFIELD_URL) == pMe->m_wFieldMask)
                    {
                        if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                                                IDS_REPLACE,
                                                                NULL,
                                                                -1))
                        {
                            MOVE_TO_STATE(STATE_EXIT);
                            return NFSMACTION_CONTINUE;
                        }
                        return NFSMACTION_WAIT;
                    }
                    else
                    {

                        nRet = CContApp_SaveEditField(pMe, AEE_ADDRFIELD_URL);
                        
                        if( SUCCESS == nRet)
                        {
                            FREEIF(pMe->m_pFldInputBuf);
                            pMe->m_eSuccessRetState = STATE_EXIT;
                        
                            MOVE_TO_STATE(STATE_SUCCESS);
                        }
                        else
                        {
                            pMe->m_wErrStrID = IDS_ERR_ADDFIELD;
                            MOVE_TO_STATE(STATE_ERROR);
                        }
                    }
                    break;

                case STARTMETHOD_SELECTFIELD:
                    MSG_FATAL("Handler_STATE_MAINLIST STARTMETHOD_SELECTFIELD",0,0,0);
                    if(pMe->m_wSelFldType == SINGLE_SELECT_NUMBER)
                    {
                        MOVE_TO_STATE(STATE_SELECT_RETURN)
                        return NFSMACTION_CONTINUE;
                    }
                    break;
                     
                default:
                {
                    MSG_FATAL("Handler_STATE_MAINLIST default",0,0,0);
                     if(IS_ZERO_REC())
                    {
                        // Show dialog message
                        pMe->m_eMsgType = MESSAGE_WARNNING;
                        if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_EMPTY))
                        {
                            MOVE_TO_STATE(STATE_EXIT);
                            return NFSMACTION_CONTINUE;
                        }
                        return NFSMACTION_WAIT;
                    }          
                    MOVE_TO_STATE(STATE_OPTS);
                }
                return NFSMACTION_CONTINUE;
            }
            break;

        case DLGRET_POPNUMFLD:
            MSG_FATAL("Handler_STATE_MAINLIST DLGRET_POPNUMFLD",0,0,0);
            if(pMe->m_wSelFldType == SINGLE_SELECT_NUMBER && STARTMETHOD_SELECTFIELD == pMe->m_eStartMethod)
            {
                MOVE_TO_STATE(STATE_VIEW)
                return NFSMACTION_CONTINUE;
            }
            break;
        
        case DLGRET_DETAIL:
            MSG_FATAL("Handler_STATE_MAINLIST DLGRET_DETAIL",0,0,0);
            // store the menu select
            PUSH_LISTMENU_SEL(pMe->m_wMainListSel);
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            //pMe->m_eOptsRetState = STATE_OPTS;
            MOVE_TO_STATE(STATE_DETAIL);
            break;  
    
        case DLGRET_CALL:
            MSG_FATAL("Handler_STATE_MAINLIST DLGRET_CALL",0,0,0);
            // store the menu select
            PUSH_LISTMENU_SEL(pMe->m_wMainListSel);
            
            // store the menu index
            PUSH_LISTMENU_IDX(pMe->m_wMainListIdx);
            
#ifdef FEATURE_SAVE_ONLY_NAME                    
            if(0 == CContApp_GetCurrFldNum(pMe, AEE_ADDRFIELD_PHONE_GENERIC))
            {
                // Show dialog message
                pMe->m_eMsgType = MESSAGE_WARNNING;
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NO_NUMBER_TO_MAKECALL))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
#endif

            // Reset select field
            pMe->m_wSelectFld = 0;
            pMe->m_wFiledOptState =  FIELDCALL;
            pMe->m_eOptsRetState = STATE_MAINLIST;
        
            MOVE_TO_STATE(STATE_VIEW);
            return NFSMACTION_CONTINUE;
            
        case DLGRET_VIEW: 
            MSG_FATAL("Handler_STATE_MAINLIST DLGRET_VIEW",0,0,0);
            // store the menu select
            PUSH_LISTMENU_SEL(pMe->m_wMainListSel);
            
            // store the menu index
            PUSH_LISTMENU_IDX(pMe->m_wMainListIdx);
             if(IS_ZERO_REC())
            {
                // Show dialog message
                pMe->m_eMsgType = MESSAGE_WARNNING;
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_EMPTY))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }               
            MOVE_TO_STATE(STATE_VIEW);
         break;

        case DLGRET_ADDNEW:
            MSG_FATAL("Handler_STATE_MAINLIST DLGRET_ADDNEW",0,0,0);
            // store the menu select
            PUSH_LISTMENU_SEL(pMe->m_wMainListSel);
            
            // store the menu index
            PUSH_LISTMENU_IDX(pMe->m_wMainListIdx);
            
        //dialog addnew will be delete, addnew and edit will deal together.
#ifdef FEATURE_RUIM_PHONEBOOK
            if(IsRunAsUIMVersion())
            {
                (void)CContApp_GetConfig(pMe, 
                                          CONTCFG_SAVETYPE,
                                         &pMe->m_nSaveType,
                                          sizeof(byte));
                if (CONTCFG_SAVETYPE_SELECT == pMe->m_nSaveType)
                {
                    MOVE_TO_STATE(STATE_SELECTPOS);
                }
                else
                {
                    MOVE_TO_STATE(STATE_ADDNEW);
                }
            }
            else
            {
                MOVE_TO_STATE(STATE_ADDNEW);
            }
#else            
            {
                MOVE_TO_STATE(STATE_ADDNEW);
            }
#endif// FEATURE_RUIM_PHONEBOOK
            CContApp_FreeBuffer(pMe);
            break;
        
        case DLGRET_CANCELED:
            MSG_FATAL("Handler_STATE_MAINLIST DLGRET_CANCELED",0,0,0);
            pMe->m_wSelectCont = 0;
            (void)CContApp_GetConfig(pMe, CONTCFG_SAVETYPE, &pMe->m_nSaveType, sizeof(byte));
            if (pMe->m_eStartMethod == STARTMETHOD_NORMAL || pMe->m_eStartMethod == STARTMETHOD_ADDFIELD)
            {
                MOVE_TO_STATE(STATE_EXIT);
            }
            else if(pMe->m_wSelFldType == SINGLE_SELECT_NUMBER && STARTMETHOD_SELECTFIELD == pMe->m_eStartMethod
                    || (pMe->m_wSelFldType == SINGLE_SELECT_GROUPLIST && STARTMETHOD_GROUPLIST == pMe->m_eStartMethod))
            {
                MOVE_TO_STATE(STATE_EXIT);
            }
            else
            {
                MOVE_TO_STATE(STATE_MAINMENU);
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_ERR:
            MSG_FATAL("Handler_STATE_MAINLIST DLGRET_ERR",0,0,0);
            // Show dialog message
            pMe->m_wErrStrID = IDS_ERR_ADDFIELD;
            MOVE_TO_STATE(STATE_ERROR);
            break;

        case DLGRET_NUMBER_TOOLONG:
            // Show dialog message
            MSG_FATAL("Handler_STATE_MAINLIST DLGRET_ERR",0,0,0);
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NUMBER_TOOLONG))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
        case DLGRET_MSGBOX_OK:
            MSG_FATAL("Handler_STATE_MAINLIST DLGRET_MSGBOX_OK",0,0,0);
            if(STARTMETHOD_ADDFIELD == pMe->m_eStartMethod)
            {
                MOVE_TO_STATE(STATE_EXIT);
            }
            break;
            
        default:
#if defined(AEE_STATIC)
            MSG_FATAL("Handler_STATE_MAINLIST default",0,0,0);
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_MAINLIST

/*==============================================================================
������
       Handler_STATE_OPTS
˵����
       STATE_OPTS ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_OPTS(CContApp *pMe)
{
    static uint16 wType = 0;
    boolean bFlag = FALSE;  
    
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_OPTS %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            IMenuCtl  *pMenuCtl;
            // show options dialog
            pMe->m_bNotOverwriteDlgRet = FALSE;
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_OPTS))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                                      IDC_OPTS_MENU);
            
            // build options menu
            (void)CContApp_BuildOptsMenu(pMe, pMenuCtl, TRUE);
            
            // Redraw the menu for fix the bug of display
            IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl,0));
            
            // restore the menu select
            POP_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            
            return NFSMACTION_WAIT;
        }
        
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            // reset the menu select
            pMe->m_wOptsStatSel = 0;
            
            if(IS_ZERO_REC())
            {
                MOVE_TO_STATE(STATE_EXIT);
            }
            else
            {
                MOVE_TO_STATE(STATE_MAINLIST);
            }
            break;

        case DLGRET_DETAIL:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            pMe->m_eOptsRetState = STATE_OPTS;
            MOVE_TO_STATE(STATE_DETAIL);
            break;	
			
        case DLGRET_EDIT:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            
            // ���õ�ǰ�༭�ļ�¼Ϊ��ǰѡ�е�
            pMe->m_wEditCont = pMe->m_wSelectCont;
            pMe->m_eOptsRetState = STATE_MAINLIST;
            pMe->m_eEditRetState = pMe->m_eOptsRetState;			
            MOVE_TO_STATE(STATE_EDIT);
            break;			
        
        case DLGRET_DELETE:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            wType = 0;            
            if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                                    IDS_YESNO_DELETEREC,
                                                    NULL,
                                                    -1))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
            //MOVE_TO_STATE(STATE_DELETE);
            break;
            
        case DLGRET_ADDNEW :
        {
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
#ifdef FEATURE_RUIM_PHONEBOOK
            if(IsRunAsUIMVersion())
            {
                (void)CContApp_GetConfig(pMe, 
                                          CONTCFG_SAVETYPE,
                                         &pMe->m_nSaveType,
                                          sizeof(byte));
                if (CONTCFG_SAVETYPE_SELECT == pMe->m_nSaveType)
                {
                    MOVE_TO_STATE(STATE_SELECTPOS);
                }
                else
                {
                    MOVE_TO_STATE(STATE_ADDNEW);
                }
            }
            else
            {
                MOVE_TO_STATE(STATE_ADDNEW);
            }
#else            
            {
                MOVE_TO_STATE(STATE_ADDNEW);
            }
#endif// FEATURE_RUIM_PHONEBOOK
            CContApp_FreeBuffer(pMe);
            break;
        }

#ifdef FEATURE_RUIM_PHONEBOOK
        case DLGRET_COPY:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);

            if (!IS_RUIM_REC(pMe->m_wEditCont) )
            {
                if(IS_MAX_REC(pMe->m_pAddrRUIM))
                {
                    pMe->m_eMsgType = MESSAGE_WARNNING;
                    if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_FULL_UIM))
                    {
                        MOVE_TO_STATE(STATE_EXIT);
                        return NFSMACTION_CONTINUE;
                    }
                    return NFSMACTION_WAIT;
                }
                
                if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                     pMe->m_wEditCont,
                                                     SELECT_ALL))
                {
                    pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                    MOVE_TO_STATE(STATE_ERROR);
                    return NFSMACTION_CONTINUE;
                }
                
                if(!CContApp_CheckNumLen(pMe, pMe->m_wEditCont))
                {
                    // Show dialog message
                    pMe->m_eMsgType = MESSAGE_WARNNING;
                    if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NUMBER_TOOLONG))
                    {
                        MOVE_TO_STATE(STATE_EXIT);
                        return NFSMACTION_CONTINUE;
                    }
                    return NFSMACTION_WAIT;
                }
            }
            else
            {
                if(IS_MAX_REC(pMe->m_pAddrPhone))
                {
                    pMe->m_eMsgType = MESSAGE_WARNNING;
                    if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_FULL_PHONE))
                    {
                        MOVE_TO_STATE(STATE_EXIT);
                        return NFSMACTION_CONTINUE;
                    }
                    return NFSMACTION_WAIT;
                }
            }
            
            wType = 1;           
            if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                                    (SINGLECOPY == pMe->m_nCopyMoveType) ? IDS_YESNO_COPYREC : IDS_YESNO_MOVEREC,
                                                    NULL,
                                                    -1))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;            
            
            //MOVE_TO_STATE(STATE_COPY);
            //break;
#endif// FEATURE_RUIM_PHONEBOOK

        case DLGRET_SEARCHOTHERDIRECTORY:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            
            MOVE_TO_STATE(STATE_MAINLIST);
            break;
            
        case DLGRET_NO:
            //MOVE_TO_STATE(STATE_OPTS);		
            MOVE_TO_STATE(STATE_MAINLIST);
            break;  
            
        case DLGRET_YES:
            if (wType ==0)
            {
                if(SUCCESS == CContApp_DeleteCont(pMe, pMe->m_wEditCont))
                {                    

                    if(NULL != pMe->m_szAlpha)
                    {
                        // �Ƿ������һ��, ɾ�����һ���󷵻�ѡ�е����ڶ�����Ҳ������һ��,��Ҫ��ʱIVector_Size(pMe->m_pAddList)��û�м�
                        if((pMe->m_nCurrSmartIdx + MAX_NUM_MENUPOP) == pMe->m_pSmartBufLen)
                        {
                            pMe->m_nCurrSmartIdx--;
                        }
                    }
                    else
                    {
                        // �Ƿ������һ��, ɾ�����һ���󷵻�ѡ�е����ڶ�����Ҳ������һ��,��Ҫ��ʱIVector_Size(pMe->m_pAddList)��û�м�
                        if((pMe->m_nCurrIdx + MAX_NUM_MENUPOP) == IVector_Size(pMe->m_pAddList))
                        {
                            //pMe->m_nCurrIdx--; 
                            pMe->m_wMainListIdx--;
                        }
                    }
                    
                    pMe->m_bDelOk = TRUE;                    
                    bFlag = TRUE;
                }
                else
                {
                    bFlag = FALSE;
                }
            }
            else if (wType ==1)
            {
                if (IS_RUIM_REC(pMe->m_wEditCont) )
                {
                     if (SUCCESS == CContApp_CopyToPhone(pMe, pMe->m_wEditCont - CONTAPP_RUIM_START_IDX + 1))
                    {
                        bFlag = TRUE;
                    }
                    else
                    {
                        bFlag = FALSE;
                    }
                }
                else
                {
                    if(SUCCESS == CContApp_CopyToRUIM(pMe, pMe->m_wEditCont))
                    {
                        bFlag = TRUE;                    
                    }
                    else
                    {
                        bFlag = FALSE;
                    }                            
                }            
            }
            if (TRUE == bFlag)
            {
                CContApp_Phonebook_Load(pMe);
                pMe->m_eSuccessRetState = STATE_MAINLIST;
                MOVE_TO_STATE(STATE_SUCCESS);
            }
            else
            {
                if (wType == 0)
                {
                    pMe->m_wErrStrID = IDS_ERR_DELETE;
                }
                else
                {
                    if(pMe->m_nCopyMoveType == SINGLECOPY)
                    {
                        pMe->m_wErrStrID = IDS_ERR_COPY;
                    }
                    else
                    {
                        pMe->m_wErrStrID = IDS_ERR_MOVE;
                    }
                }
                MOVE_TO_STATE(STATE_ERROR);
            }
            //MOVE_TO_STATE(STATE_DELETE);		
            break;

        case DLGRET_VIEW:
            MOVE_TO_STATE(STATE_VIEW);
            break;
            
       case DLGRET_ERR:
            MOVE_TO_STATE(STATE_ERROR);
            break;

        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_MAINLIST);
            break;

        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_OPTS


/*==============================================================================
������
       Handler_STATE_MAINMENU
˵����
       STATE_MAINMENU ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_MAINMENU(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_MAINMENU %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        case DLGRET_MSGBOX_OK:
        {
            IMenuCtl  *pMenuCtl;
            
            // show options dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_MAINMENU))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                                      IDC_MAINMENU_MENU);
            
            // build options menu
            
            // Redraw the menu for fix the bug of display
            //IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl,0));
            
            return NFSMACTION_WAIT;
        }

        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            // reset the menu select
            pMe->m_wOptsStatSel = 0;
			
            MOVE_TO_STATE(STATE_EXIT);
            break;
        
        case DLGRET_DIRECTORTLIST:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);

            pMe->m_eListFrom = LISTFROMMAINMENU;
            /*if(IS_ZERO_REC())
            {
                // Show dialog message
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_EMPTY))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }*/
   
            pMe->m_wMainListSel = MENU_SELECT_NULL;
            
            MOVE_TO_STATE(STATE_MAINLIST);
            break;
            
        case DLGRET_SEARCH:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);

            pMe->m_sSearchStr = NULL;
            //MOVE_TO_STATE(STATE_SEARCH);
            MOVE_TO_STATE(STATE_SEARCH_NAME);
            break;

        case DLGRET_ADDNEW:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);

            pMe->m_eListFrom = LISTFROMNONE;
            //dialog addnew will be delete, addnew and edit will deal together.
#ifdef FEATURE_RUIM_PHONEBOOK
            if(IsRunAsUIMVersion())
            {
                (void)CContApp_GetConfig(pMe, 
                                          CONTCFG_SAVETYPE,
                                         &pMe->m_nSaveType,
                                          sizeof(byte));
                if (CONTCFG_SAVETYPE_SELECT == pMe->m_nSaveType)
                {
                    MOVE_TO_STATE(STATE_SELECTPOS);
                }
                else
                {
                    MOVE_TO_STATE(STATE_ADDNEW);
                }
             }
            else
            {
                MOVE_TO_STATE(STATE_ADDNEW);
            }
#else            
            {
                MOVE_TO_STATE(STATE_ADDNEW);
            }
#endif// FEATURE_RUIM_PHONEBOOK
            CContApp_FreeBuffer(pMe);
            break;

        case DLGRET_GROUPVIEW :
        {
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            
            pMe->m_wselGroupId = IDS_GROUP_NONE_STRING;
            MOVE_TO_STATE(STATE_GROUPVIEW);

            break;
        }
        
        case DLGRET_ONEDIAL:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            pMe->m_wSelectOneDial = CONTCFG_ONEDIAL1;
            MOVE_TO_STATE(STATE_ONEDIAL);
            break;

#ifdef FEATURE_RUIM_PHONEBOOK
        if( IsRunAsUIMVersion() )
        {
            case DLGRET_CONTACTS_MANAGEMENT:
                // Save the menu select
                PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
                
                //MOVE_TO_STATE(STATE_COPY);
                MOVE_TO_STATE(STATE_MANAGEMENT);
                break;
        }
#endif// FEATURE_RUIM_PHONEBOOK

        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_OPTS

/*==============================================================================
������
       Handler_STATE_VIEW
˵����
       STATE_VIEW ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_VIEW(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_VIEW %d", pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                 pMe->m_wSelectCont,
                                                 SELECT_ALL))
            {
                pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_VIEW))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            //MOVE_TO_STATE(pMe->m_eOptsRetState);
            if(IS_ZERO_REC())
            {
                MOVE_TO_STATE(STATE_EXIT);
            }
            else if(LISTFROMMAINMENU == pMe->m_eListFrom || LISTFROMNOMAL == pMe->m_eListFrom)
            {
                MOVE_TO_STATE(STATE_MAINLIST);
            }
            else if(LISTFROMSEARCH == pMe->m_eListFrom)
            {
                MOVE_TO_STATE(STATE_FIND_LIST);
            }
            else if(LISTFROMGROUP== pMe->m_eListFrom)
            {
                MOVE_TO_STATE(STATE_GROUPVIEW_LIST);
            }
            else if(LISTFROMONEDIAL == pMe->m_eListFrom)
            {
                MOVE_TO_STATE(STATE_ONEDIAL_SET);
            }
            else if(pMe->m_wSelFldType == SINGLE_SELECT_NUMBER && STARTMETHOD_SELECTFIELD == pMe->m_eStartMethod)
            {
                MOVE_TO_STATE(STATE_MAINLIST);
            }
            else if(pMe->m_wSelFldType == SINGLE_SELECT_GROUPLIST && STARTMETHOD_GROUPLIST == pMe->m_eStartMethod)
            {
                MOVE_TO_STATE(STATE_MAINLIST);
            }
            else
            {
                pMe->m_wErrStrID = IDS_ERR_UNKNOWN;
                MOVE_TO_STATE(STATE_ERROR);
            }
            break;

        case DLGRET_ERR:
            pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
            MOVE_TO_STATE(STATE_ERROR);
            break;

        case DLGRET_SELECT_SINGLE_NUMBER:
            if((pMe->m_wSelFldType == SINGLE_SELECT_NUMBER && STARTMETHOD_SELECTFIELD == pMe->m_eStartMethod)
                || (pMe->m_wSelFldType == SINGLE_SELECT_GROUPLIST && STARTMETHOD_GROUPLIST == pMe->m_eStartMethod))
            {
                MOVE_TO_STATE(STATE_SELECT_RETURN)
                return NFSMACTION_CONTINUE;
            }
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_VIEW

/*==============================================================================
������
       Handler_STATE_FIND
˵����
       STATE_FIND ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_FIND(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_FIND %d", pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_FIND))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_MAINMENU);
            break;
        
        case DLGRET_OK:
            if(SUCCESS != CContApp_LoadByName(pMe, pMe->m_pFldInputBuf))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            FREEIF(pMe->m_pFldInputBuf);
            
            // ��¼�Ƿ�Ϊ��
            if(IS_ZERO_REC())
            {
                // Show dialog message
                pMe->m_eMsgType = MESSAGE_WARNNING;
                if(SUCCESS != CContApp_ShowMsgBox( pMe, IDS_EMPTY))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
            
            pMe->m_wSelectCont = CONTAPP_RECID_NULL;
            pMe->m_nCurrIdx    = 0;
            
            // Go to list menu
            MOVE_TO_STATE(STATE_FIND_LIST);
            break;
            
        case DLGRET_MSGBOX_OK:
            // ReLoad contacts
            if(SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            MOVE_TO_STATE(STATE_FIND);
            break;
            
        case DLGRET_INPUT_NULL:
            // field can not be NULL
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CContApp_ShowMsgBox( pMe, IDS_MSG_INPUTNULL))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_FIND

/*==============================================================================
������
       Handler_STATE_FIND_LIST
˵����
       STATE_FIND_LIST ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_FIND_LIST(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_FIND_LIST %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // ��¼�Ƿ�Ϊ��
            if(IS_ZERO_REC())
            {
                // ReLoad contacts
                if(SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType))
                {
                    pMe->m_wErrStrID = IDS_ERR_LOAD;
                    MOVE_TO_STATE(STATE_ERROR);
                    return NFSMACTION_CONTINUE;
                }
                
                MOVE_TO_STATE(STATE_SEARCH_NAME);
                break;
            }
            // restore the menu select
            POP_LISTMENU_SEL(pMe->m_wMainListSel);
            
            // Restore the menu index
            POP_LISTMENU_IDX(pMe->m_wMainListIdx);
            
            pMe->m_eListFrom = LISTFROMSEARCH;
            // Show dialog message
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_LIST))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_OK:
            // store the menu select
            PUSH_LISTMENU_SEL(pMe->m_wFindListSel);
            
            // store the menu index
            PUSH_LISTMENU_IDX(pMe->m_wFindListIdx);
            
            MOVE_TO_STATE(STATE_FIND_OPTS);
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CALL:
            // store the menu select
            PUSH_LISTMENU_SEL(pMe->m_wFindListSel);
            
            // store the menu index
            PUSH_LISTMENU_IDX(pMe->m_wFindListIdx);
            
            // Load the current fields
            if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                 pMe->m_wSelectCont,
                                                 SINGLE_SELECT_NUMBER))
            {
                pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }

#ifdef FEATURE_SAVE_ONLY_NAME                    
            if(0 == CContApp_GetCurrFldNum(pMe, AEE_ADDRFIELD_PHONE_GENERIC))
            {
                // Show dialog message
                pMe->m_eMsgType = MESSAGE_WARNNING;
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NO_NUMBER_TO_MAKECALL))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
#endif
            {
                // Reset select field
                pMe->m_wSelectFld = 0;
                pMe->m_wFiledOptState = FIELDCALL;
                pMe->m_eOptsRetState = STATE_FIND_LIST;
            
                MOVE_TO_STATE(STATE_VIEW);
            }
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CANCELED:
            // reset the menu select
            pMe->m_wFindListSel = 0;
            
            // reset the menu index
            pMe->m_wFindListIdx = 0;
            
            // ReLoad contacts
            if(SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            MOVE_TO_STATE(STATE_SEARCH_NAME);
            return NFSMACTION_CONTINUE;
        
        case DLGRET_DETAIL:
            // store the menu select
            PUSH_LISTMENU_SEL(pMe->m_wMainListSel);
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            //pMe->m_eOptsRetState = STATE_OPTS;
            MOVE_TO_STATE(STATE_DETAIL);
            break;  

        case DLGRET_MSGBOX_OK:
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_FIND_LIST

/*==============================================================================
������
       Handler_STATE_FIND_OPTS
˵����
       STATE_FIND_OPTS ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_FIND_OPTS(CContApp *pMe)
{
    static uint16 wType = 0;
    boolean bFlag = FALSE;    
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_FIND_OPTS %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            IMenuCtl  *pMenuCtl;
            if(IS_ZERO_REC())
            {
                // ReLoad contacts
                if(SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType))
                {
                    pMe->m_wErrStrID = IDS_ERR_LOAD;
                    MOVE_TO_STATE(STATE_ERROR);
                    return NFSMACTION_CONTINUE;
                }
                
                MOVE_TO_STATE(STATE_FIND);
                break;
            }            
            // show options dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_OPTS))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                                      IDC_OPTS_MENU);
            
            // build options menu
            (void)CContApp_BuildOptsMenu(pMe, pMenuCtl, TRUE);
            
            // restore the menu select
            POP_OPTSMENU_SEL(pMe->m_wFindOptsSel);
            return NFSMACTION_WAIT;
        }
        
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            // Reset the menu select
            pMe->m_wFindOptsSel = 0;
            
            MOVE_TO_STATE(STATE_FIND_LIST);
            break;
            
        case DLGRET_VIEW:
            // store the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wFindOptsSel);
            
            // Reset select field
            pMe->m_wSelectFld = 0;
            //pMe->m_eOptsRetState = STATE_FIND_OPTS;
            MOVE_TO_STATE(STATE_VIEW);
            break;
            
        case DLGRET_DELETE:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            wType = 0;            
            if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                                    IDS_YESNO_DELETEREC,
                                                    NULL,
                                                    -1))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
            //MOVE_TO_STATE(STATE_DELETE);
            break;


        case DLGRET_DETAIL:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            //pMe->m_eOptsRetState = STATE_OPTS;
            pMe->m_eOptsRetState = STATE_FIND_OPTS;
            MOVE_TO_STATE(STATE_DETAIL);
            break;
						
        case DLGRET_EDIT:
            // store the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wFindOptsSel);
            
            // ���õ�ǰ�༭�ļ�¼Ϊ��ǰѡ�е�
            pMe->m_wEditCont = pMe->m_wSelectCont;
            pMe->m_eOptsRetState = STATE_FIND_OPTS;
	        pMe->m_eEditRetState =	pMe->m_eOptsRetState;			
            MOVE_TO_STATE(STATE_EDIT);
            break;

        case DLGRET_ADDNEW :
        {
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
#ifdef FEATURE_RUIM_PHONEBOOK
            if(IsRunAsUIMVersion())
            {
                (void)CContApp_GetConfig(pMe, 
                                          CONTCFG_SAVETYPE,
                                         &pMe->m_nSaveType,
                                          sizeof(byte));
                if (CONTCFG_SAVETYPE_SELECT == pMe->m_nSaveType)
                {
                    MOVE_TO_STATE(STATE_SELECTPOS);
                }
                else
                {
                    MOVE_TO_STATE(STATE_ADDNEW);
                }
            }
            else
            {
                MOVE_TO_STATE(STATE_ADDNEW);
            }
#else            
            {
                MOVE_TO_STATE(STATE_ADDNEW);
            }
#endif// FEATURE_RUIM_PHONEBOOK
            CContApp_FreeBuffer(pMe);
            break;
        }

#ifdef FEATURE_RUIM_PHONEBOOK
        case DLGRET_COPY:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            
            if (!IS_RUIM_REC(pMe->m_wEditCont) )
            {
                if(IS_MAX_REC(pMe->m_pAddrRUIM))
                {
                    pMe->m_eMsgType = MESSAGE_WARNNING;
                    if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_FULL_UIM))
                    {
                        MOVE_TO_STATE(STATE_EXIT);
                        return NFSMACTION_CONTINUE;
                    }
                    return NFSMACTION_WAIT;
                }
                
                if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                     pMe->m_wEditCont,
                                                     SELECT_ALL))
                {
                    pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                    MOVE_TO_STATE(STATE_ERROR);
                    return NFSMACTION_CONTINUE;
                }
                
                if(!CContApp_CheckNumLen(pMe, pMe->m_wEditCont))
                {
                    // Show dialog message
                    pMe->m_eMsgType = MESSAGE_WARNNING;
                    if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NUMBER_TOOLONG))
                    {
                        MOVE_TO_STATE(STATE_EXIT);
                        return NFSMACTION_CONTINUE;
                    }
                    return NFSMACTION_WAIT;
                }
            }
            else
            {
                if(IS_MAX_REC(pMe->m_pAddrPhone))
                {
                    pMe->m_eMsgType = MESSAGE_WARNNING;
                    if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_FULL_PHONE))
                    {
                        MOVE_TO_STATE(STATE_EXIT);
                        return NFSMACTION_CONTINUE;
                    }
                    return NFSMACTION_WAIT;
                }
            }
            
            wType = 1;           
            if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                                    (SINGLECOPY == pMe->m_nCopyMoveType) ? IDS_YESNO_COPYREC : IDS_YESNO_MOVEREC,
                                                    NULL,
                                                    -1))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;            
            
            //MOVE_TO_STATE(STATE_COPY);
            break;
#endif// FEATURE_RUIM_PHONEBOOK

        case DLGRET_NO:
              //MOVE_TO_STATE(STATE_OPTS);        
              MOVE_TO_STATE(STATE_FIND_LIST);
              break;  
              
          case DLGRET_YES:
              if (wType ==0)
              {
                  if(SUCCESS == CContApp_DeleteCont(pMe, pMe->m_wSelectCont))
                  {
                      if(NULL != pMe->m_szAlpha)
                      {
                          // �Ƿ������һ��, ɾ�����һ���󷵻�ѡ�е����ڶ�����Ҳ������һ��,��Ҫ��ʱIVector_Size(pMe->m_pAddList)��û�м�
                          if((pMe->m_nCurrSmartIdx + MAX_NUM_MENUPOP) == pMe->m_pSmartBufLen)
                          {
                              pMe->m_nCurrSmartIdx--;
                          }
                      }
                      else
                      {
                          // �Ƿ������һ��, ɾ�����һ���󷵻�ѡ�е����ڶ�����Ҳ������һ��,��Ҫ��ʱIVector_Size(pMe->m_pAddList)��û�м�
                          if((pMe->m_nCurrIdx + MAX_NUM_MENUPOP) == IVector_Size(pMe->m_pAddList))
                          {
                              //pMe->m_nCurrIdx--; 
                              pMe->m_wMainListIdx--;
                          }
                      }
                      
                      pMe->m_bDelOk = TRUE;           
                      bFlag = TRUE;
                  }
                  else
                  {
                      bFlag = FALSE;
                  }
              }
              else if (wType ==1)
              {
                  if IS_RUIM_REC(pMe->m_wSelectCont)
                  {
                       if (SUCCESS == CContApp_CopyToPhone(pMe, pMe->m_wSelectCont - CONTAPP_RUIM_START_IDX + 1))
                      {
                          bFlag = TRUE;
                      }
                      else
                      {
                          bFlag = FALSE;
                      }
                  }
                  else
                  {
                      if(SUCCESS == CContApp_CopyToRUIM(pMe, pMe->m_wSelectCont))
                      {
                          bFlag = TRUE;                    
                      }
                      else
                      {
                          bFlag = FALSE;
                      }                            
                  }            
              }
              if (TRUE == bFlag)
              {
                  CContApp_Phonebook_Load(pMe);
                  pMe->m_eSuccessRetState = STATE_FIND_LIST;
                  MOVE_TO_STATE(STATE_SUCCESS);
              }
              else
              {
                  if (wType == 0)
                  {
                      pMe->m_wErrStrID = IDS_ERR_DELETE;
                  }
                  else
                  {
                      if(pMe->m_nCopyMoveType == SINGLECOPY)
                      {
                          pMe->m_wErrStrID = IDS_ERR_COPY;
                      }
                      else
                      {
                          pMe->m_wErrStrID = IDS_ERR_MOVE;
                      }
                  }
                  MOVE_TO_STATE(STATE_ERROR);
              }
              //MOVE_TO_STATE(STATE_DELETE);      
              break;
              
        case DLGRET_SEARCHOTHERDIRECTORY:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wFindOptsSel);
            MOVE_TO_STATE(STATE_FIND_LIST);
            break;

        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_FIND_LIST);
            break;
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_FIND_OPTS

/*==============================================================================
������
       Handler_STATE_GROUPVIEW
˵����
       STATE_GROUPVIEW ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_GROUPVIEW(CContApp *pMe)
{

#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_GROUPVIEW %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        { 
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_GROUP))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
        }
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            if(STARTMETHOD_GROUPLIST == pMe->m_eStartMethod)
            {
                MOVE_TO_STATE(STATE_SELECT_RETURN);
            }
            else
            {
                //MOVE_TO_STATE(STATE_SEARCH);            
                MOVE_TO_STATE(STATE_MAINMENU);
            }
            break;

        case DLGRET_VIEW:
            // Load record
            if(SUCCESS != CContApp_LoadByCat(pMe, pMe->m_nGroupCat))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            // ��¼�Ƿ�Ϊ��
            if(IS_ZERO_REC())
            {
                // Show dialog message
                pMe->m_eMsgType = MESSAGE_WARNNING;
                if(SUCCESS != CContApp_ShowMsgBox( pMe, IDS_EMPTY))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
            pMe->m_wSelectCont = CONTAPP_RECID_NULL;
            pMe->m_nCurrIdx    = 0;
            
            // Go to list the menu
            MOVE_TO_STATE(STATE_GROUPVIEW_LIST);
            break;
            
        case DLGRET_OK:
//#ifdef FEATURE_CARRIER_THAILAND_HUTCH               
            if(pMe->m_nGroupCat != AEE_ADDR_CAT_NONE
                && pMe->m_nGroupCat != AEE_ADDR_CAT_OTHER
                && pMe->m_nGroupCat != AEE_ADDR_CAT_UIM
                && STARTMETHOD_GROUPLIST != pMe->m_eStartMethod)
            {
                MOVE_TO_STATE(STATE_GROUPOPT);
            }
            else
            {

//#else //FEATURE_CARRIER_THAILAND_HUTCH 

            // Load record
            if(SUCCESS != CContApp_LoadByCat(pMe, pMe->m_nGroupCat))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            // ��¼�Ƿ�Ϊ��
            if(IS_ZERO_REC())
            {
                // Show dialog message
                pMe->m_eMsgType = MESSAGE_WARNNING;
                if(SUCCESS != CContApp_ShowMsgBox( pMe, IDS_EMPTY))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
            pMe->m_wSelectCont = CONTAPP_RECID_NULL;
            pMe->m_nCurrIdx    = 0;
            
            // Go to list the menu
            MOVE_TO_STATE(STATE_GROUPVIEW_LIST);
//#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  
    /**/
            }
            break;
            
        case DLGRET_MSGBOX_OK:
            // ReLoad contacts
            if(SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            MOVE_TO_STATE(STATE_GROUPVIEW);
            return NFSMACTION_CONTINUE;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_GROUPVIEW

/*==============================================================================
������
       Handler_STATE_GROUPVIEW_LIST
˵����
       STATE_GROUPVIEW_LIST ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_GROUPVIEW_LIST(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_GROUPVIEW_LIST %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // ��¼�Ƿ�Ϊ��
            if(IS_ZERO_REC())
            {
                // reload contacts
                if(SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType))
                {
                    pMe->m_wErrStrID = IDS_ERR_LOAD;
                    MOVE_TO_STATE(STATE_ERROR);
                    return NFSMACTION_CONTINUE;
                }
                // ĳһ��Ⱥ�����һ����¼���༭�ü�¼��Ⱥ�飬�༭�ɹ��󷵻أ�
                //���ղű༭��Ŀ��Ⱥ���ѡ�е��Ǹղű༭�ļ�¼�����ǲ˵��ĵ�һ��
                pMe->m_wMainListSel = MENU_SELECT_NULL;
                MOVE_TO_STATE(STATE_GROUPVIEW);
                break;
            }
            /*
            // restore the menu select
            POP_LISTMENU_SEL(pMe->m_wGroupViewSel);
            
            // restore the menu index
            POP_LISTMENU_IDX(pMe->m_wGroupViewIdx);
            */
            POP_LISTMENU_SEL(pMe->m_wMainListSel);
            
            // Restore the menu index
            POP_LISTMENU_IDX(pMe->m_wMainListIdx);
            if(STARTMETHOD_GROUPLIST == pMe->m_eStartMethod && pMe->m_wSelFldType == MULTI_SELECT_GROUPLIST)
            {
                // Show dialog message
                if(SUCCESS != CContApp_ShowDialog(pMe, IDD_SELECT))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
            }
            else
            {
                pMe->m_eListFrom = LISTFROMGROUP;
                // Show dialog message
                if(SUCCESS != CContApp_ShowDialog(pMe, IDD_LIST))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
            }
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_OK:
            if(STARTMETHOD_GROUPLIST == pMe->m_eStartMethod)
            {
                if(pMe->m_wSelFldType == MULTI_SELECT_GROUPLIST)
                {
                    MOVE_TO_STATE(STATE_SELECT_RETURN);
                    return NFSMACTION_CONTINUE;
                }
                else
                {
                    // single select 
                    MOVE_TO_STATE(STATE_SELECT_RETURN);
                }
                break;
            }
            else
            {
                // store the menu select
                PUSH_LISTMENU_SEL(pMe->m_wGroupViewSel);
                
                // store the menu index
                PUSH_LISTMENU_IDX(pMe->m_wGroupViewIdx);
                
                MOVE_TO_STATE(STATE_GROUPVIEW_OPTS);
                return NFSMACTION_CONTINUE;
            }
        case DLGRET_CALL:
            // store the menu select
            PUSH_LISTMENU_SEL(pMe->m_wGroupViewSel);
            
            // store the menu index
            PUSH_LISTMENU_IDX(pMe->m_wGroupViewIdx);
            
            // Load the current fields
            if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                 pMe->m_wSelectCont,
                                                 SINGLE_SELECT_NUMBER))
            {
                pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
#ifdef FEATURE_SAVE_ONLY_NAME                    
            if(0 == CContApp_GetCurrFldNum(pMe, AEE_ADDRFIELD_PHONE_GENERIC))
            {
                // Show dialog message
                pMe->m_eMsgType = MESSAGE_WARNNING;
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NO_NUMBER_TO_MAKECALL))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
#endif
            {
                // Reset select field
                pMe->m_wSelectFld = 0;
                pMe->m_wFiledOptState = FIELDCALL;
                pMe->m_eOptsRetState = STATE_GROUPVIEW_LIST;
                
                MOVE_TO_STATE(STATE_VIEW);
            }
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CANCELED:
            // reset the menu select
            pMe->m_wGroupViewSel = 0;
            
            // reset the menu index
            pMe->m_wGroupViewIdx = 0;
            
            CContApp_FreeSelectFieldListNode();
            // reload contacts
            if(SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            MOVE_TO_STATE(STATE_GROUPVIEW);
            return NFSMACTION_CONTINUE;

        case DLGRET_SELECT:
            if(STARTMETHOD_GROUPLIST == pMe->m_eStartMethod)
            {
                MOVE_TO_STATE(STATE_SELECTOPT);
            }
            break;

        case DLGRET_POPNUMFLD :
            if(pMe->m_wSelFldType == SINGLE_SELECT_GROUPLIST && STARTMETHOD_GROUPLIST == pMe->m_eStartMethod)
            {
                MOVE_TO_STATE(STATE_VIEW)
            }
            else
            {
                MOVE_TO_STATE(STATE_POPNUMFLD);
            }
            break;

        case DLGRET_DETAIL:
            // store the menu select
            PUSH_LISTMENU_SEL(pMe->m_wMainListSel);
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            pMe->m_eOptsRetState = STATE_OPTS;
            MOVE_TO_STATE(STATE_DETAIL);
            break;
            
        case DLGRET_ERR :
            pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
            MOVE_TO_STATE(STATE_ERROR);       
            break;

        case DLGRET_MSGBOX_OK:
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_GROUPVIEW_LIST

/*==============================================================================
������
       Handler_STATE_GROUPVIEW_OPTS
˵����
       STATE_GROUPVIEW_OPTS ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_GROUPVIEW_OPTS(CContApp *pMe)
{
    static uint16 wType = 0;
    boolean bFlag = FALSE;   
    
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_GROUPVIEW_OPTS %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            IMenuCtl  *pMenuCtl;
            if(IS_ZERO_REC())
            {
                // reload contacts
                if(SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType))
                {
                    pMe->m_wErrStrID = IDS_ERR_LOAD;
                    MOVE_TO_STATE(STATE_ERROR);
                    return NFSMACTION_CONTINUE;
                }
                
                MOVE_TO_STATE(STATE_GROUPVIEW);
                break;
            }            
            // show options dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_OPTS))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                                      IDC_OPTS_MENU);
            
            // build options menu
            (void)CContApp_BuildOptsMenu(pMe, pMenuCtl, TRUE);
            
            // restore the menu select
            POP_OPTSMENU_SEL(pMe->m_wGroupOptsSel);
            return NFSMACTION_WAIT;
        }
        
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            // Reset the menu select
            pMe->m_wGroupOptsSel = 0;
            
            MOVE_TO_STATE(STATE_GROUPVIEW_LIST);
            break;
            
        case DLGRET_VIEW:
            // store the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wGroupOptsSel);
            
            // Reset select field
            pMe->m_wSelectFld = 0;
            pMe->m_eOptsRetState = STATE_GROUPVIEW_OPTS;
            MOVE_TO_STATE(STATE_VIEW);
            break;
            
        case DLGRET_DELETE:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            wType = 0;            
            if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                                    IDS_YESNO_DELETEREC,
                                                    NULL,
                                                    -1))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;

        case DLGRET_DETAIL:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            //pMe->m_eOptsRetState = STATE_OPTS;
            pMe->m_eOptsRetState = STATE_GROUPVIEW_OPTS;
            MOVE_TO_STATE(STATE_DETAIL);
            break;	
            
        case DLGRET_EDIT:
            // store the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wGroupOptsSel);
            
            // ���õ�ǰ�༭�ļ�¼Ϊ��ǰѡ�е�
            pMe->m_wEditCont = pMe->m_wSelectCont;
            pMe->m_eOptsRetState = STATE_GROUPVIEW_OPTS;
	        pMe->m_eEditRetState =	pMe->m_eOptsRetState;			
            MOVE_TO_STATE(STATE_EDIT);
            break;

        case DLGRET_ADDNEW :
        {
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
#ifdef FEATURE_RUIM_PHONEBOOK
            if(IsRunAsUIMVersion())
            {
                (void)CContApp_GetConfig(pMe, 
                                          CONTCFG_SAVETYPE,
                                         &pMe->m_nSaveType,
                                          sizeof(byte));
                if (CONTCFG_SAVETYPE_SELECT == pMe->m_nSaveType)
                {
                    MOVE_TO_STATE(STATE_SELECTPOS);
                }
                else
                {
                    MOVE_TO_STATE(STATE_ADDNEW);
                }
            }
            else
            {
                MOVE_TO_STATE(STATE_ADDNEW);
            }
#else            
            {
                MOVE_TO_STATE(STATE_ADDNEW);
            }
#endif// FEATURE_RUIM_PHONEBOOK
            CContApp_FreeBuffer(pMe);
            break;
        }

#ifdef FEATURE_RUIM_PHONEBOOK
        case DLGRET_COPY:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            
            if (!IS_RUIM_REC(pMe->m_wEditCont) )
            {
                if(IS_MAX_REC(pMe->m_pAddrRUIM))
                {
                    pMe->m_eMsgType = MESSAGE_WARNNING;
                    if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_FULL_UIM))
                    {
                        MOVE_TO_STATE(STATE_EXIT);
                        return NFSMACTION_CONTINUE;
                    }
                    return NFSMACTION_WAIT;
                }
                
                if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                     pMe->m_wEditCont,
                                                     SELECT_ALL))
                {
                    pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                    MOVE_TO_STATE(STATE_ERROR);
                    return NFSMACTION_CONTINUE;
                }
                
                if(!CContApp_CheckNumLen(pMe, pMe->m_wEditCont))
                {
                    // Show dialog message
                    pMe->m_eMsgType = MESSAGE_WARNNING;
                    if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NUMBER_TOOLONG))
                    {
                        MOVE_TO_STATE(STATE_EXIT);
                        return NFSMACTION_CONTINUE;
                    }
                    return NFSMACTION_WAIT;
                }
            }
            else
            {
                if(IS_MAX_REC(pMe->m_pAddrPhone))
                {
                    pMe->m_eMsgType = MESSAGE_WARNNING;
                    if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_FULL_PHONE))
                    {
                        MOVE_TO_STATE(STATE_EXIT);
                        return NFSMACTION_CONTINUE;
                    }
                    return NFSMACTION_WAIT;
                }
            }

            wType = 1;           
            if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                                    (SINGLECOPY == pMe->m_nCopyMoveType) ? IDS_YESNO_COPYREC : IDS_YESNO_MOVEREC,
                                                    NULL,
                                                    -1))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;            
            
#endif// FEATURE_RUIM_PHONEBOOK
            
        case DLGRET_NO:
            //MOVE_TO_STATE(STATE_OPTS);		
            MOVE_TO_STATE(STATE_MAINLIST);
            break;  
            
        case DLGRET_YES:
            if (wType ==0)
            {
                if(SUCCESS == CContApp_DeleteCont(pMe, pMe->m_wSelectCont))
                {
                      if(NULL != pMe->m_szAlpha)
                      {
                          // �Ƿ������һ��, ɾ�����һ���󷵻�ѡ�е����ڶ�����Ҳ������һ��,��Ҫ��ʱIVector_Size(pMe->m_pAddList)��û�м�
                          if((pMe->m_nCurrSmartIdx + MAX_NUM_MENUPOP) == pMe->m_pSmartBufLen)
                          {
                              pMe->m_nCurrSmartIdx--;
                          }
                      }
                      else
                      {
                          // �Ƿ������һ��, ɾ�����һ���󷵻�ѡ�е����ڶ�����Ҳ������һ��,��Ҫ��ʱIVector_Size(pMe->m_pAddList)��û�м�
                          if((pMe->m_nCurrIdx + MAX_NUM_MENUPOP) == IVector_Size(pMe->m_pAddList))
                          {
                              //pMe->m_nCurrIdx--; 
                              pMe->m_wMainListIdx--;
                          }
                      }
                      
                      pMe->m_bDelOk = TRUE;           
                      bFlag = TRUE;
                  }
                else
                {
                    bFlag = FALSE;
                }
            }
            else if (wType ==1)
            {
                if IS_RUIM_REC(pMe->m_wSelectCont)
                {
                     if (SUCCESS == CContApp_CopyToPhone(pMe, pMe->m_wSelectCont - CONTAPP_RUIM_START_IDX + 1))
                    {
                        bFlag = TRUE;
                    }
                    else
                    {
                        bFlag = FALSE;
                    }
                }
                else
                {
                    if(SUCCESS == CContApp_CopyToRUIM(pMe, pMe->m_wSelectCont))
                    {
                        bFlag = TRUE;                    
                    }
                    else
                    {
                        bFlag = FALSE;
                    }                            
                }            
            }
            if (TRUE == bFlag)
            {
                CContApp_Phonebook_Load(pMe);
                pMe->m_eSuccessRetState = STATE_MAINLIST;
                MOVE_TO_STATE(STATE_SUCCESS);
            }
            else
            {
                if (wType == 0)
                {
                    pMe->m_wErrStrID = IDS_ERR_DELETE;
                }
                else
                {
                    if(pMe->m_nCopyMoveType == SINGLECOPY)
                    {
                        pMe->m_wErrStrID = IDS_ERR_COPY;
                    }
                    else
                    {
                        pMe->m_wErrStrID = IDS_ERR_MOVE;
                    }
                }
                MOVE_TO_STATE(STATE_ERROR);
            }
            break;

        case DLGRET_SEARCHOTHERDIRECTORY:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wGroupOptsSel);
            
            MOVE_TO_STATE(STATE_MAINLIST);
            break;

        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_MAINLIST);
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_GROUPVIEW_OPTS

/*==============================================================================
������
       Handler_STATE_ADDNEW_INPUT
˵����
       STATE_ADDNEW_INPUT ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_ADDNEW_INPUT(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_ADDNEW_INPUT %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_INPUTFLD))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_ADDNEW);
            break;
        
        case DLGRET_OK:
             MOVE_TO_STATE(STATE_ADDNEW);
            break;
        
        case DLGRET_MSGBOX_OK:
            //MOVE_TO_STATE(STATE_ADDNEW);
            break;
            
        case DLGRET_INPUT_NULL:
            // field can not be NULL
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_MSG_INPUTNULL))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;

        case DLGRET_NUM_UNAVAILD:
            pMe->m_eMsgType = MESSAGE_WARNNING;
            (void)CContApp_ShowMsgBox(pMe,IDS_MSG_NUM_UNAVAILD);
            return NFSMACTION_WAIT;

        case DLGRET_EMAIL_UNAVAILD:
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_MSG_EMAIL_UNAVAILD))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;            
        
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_EDIT_INPUT

/*==============================================================================
������
       Handler_STATE_SELECTGROUP
˵����
       STATE_SELECTGROUP ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_SELECTGROUP(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_SELECTGROUP %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            IMenuCtl *pMenuCtl;
            
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_GROUP))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                                      IDC_GROUP_MENU);
            
            (void)IMENUCTL_DeleteItem(pMenuCtl, IDS_GROUP_NONE_STRING);

//#ifdef FEATURE_CARRIER_THAILAND_HUTCH    
//            CContApp_BuildGroupMenu(pMe, pMenuCtl);
//#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  
            
            return NFSMACTION_WAIT;
        }
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_ADDNEW);
            break;
            
        case DLGRET_OK:
            MOVE_TO_STATE(STATE_ADDNEW);
            return NFSMACTION_CONTINUE;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_EDIT_SELECTGROUP

/*==============================================================================
������
       Handler_STATE_ADDNEW
˵����
       STATE_ADDNEW ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_ADDNEW(CContApp *pMe)
{
    static byte type = 0;// 1 for phone full, 2 for card full, 3 for check zhe same name
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_ADDNEW %d", pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {            
#ifdef FEATURE_RUIM_PHONEBOOK
            if(IsRunAsUIMVersion())
            {
                if(IS_MAX_REC(pMe->m_pAddrRUIM) && IS_MAX_REC(pMe->m_pAddrPhone))
                {
                    //FREEIF(pMe->m_pAddNewName);
                    //FREEIF(pMe->m_pAddNewMobile);
                    
                    pMe->m_eMsgRetState = CContApp_GetReturnState(pMe);
                    pMe->m_wMsgResID = IDS_MEMORYFULL;
                    MOVE_TO_STATE(STATE_MSG);
                    return NFSMACTION_CONTINUE;
                }
                
                if(pMe->m_nSaveType == CONTCFG_SAVETYPE_RUIM)
                {
                    // Check  capacity
                    if(IS_MAX_REC(pMe->m_pAddrRUIM) && !IS_MAX_REC(pMe->m_pAddrPhone))
                    {

                        if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                                                IDS_UIMFULL_SAVETOPHONE,
                                                                NULL,
                                                                -1))
                        {
                            MOVE_TO_STATE(STATE_EXIT);
                            return NFSMACTION_CONTINUE;
                        }
                        type = 2;
                        return NFSMACTION_WAIT;
                    }
                }
                else // (pMe->m_nSaveType == CONTCFG_SAVETYPE_PHONE)
                {
                    // Check  capacity
                    if(!IS_MAX_REC(pMe->m_pAddrRUIM) && IS_MAX_REC(pMe->m_pAddrPhone))
                    {

                        if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                                                IDS_PHONEFULL_SAVETOCARD,
                                                                NULL,
                                                                -1))
                        {
                            MOVE_TO_STATE(STATE_EXIT);
                            return NFSMACTION_CONTINUE;
                        }
                        type = 1;
                        return NFSMACTION_WAIT;
                    }

                }
            }
            else
#endif// FEATURE_RUIM_PHONEBOOK
            {
                // Check Phone capacity
                if(IS_MAX_REC(pMe->m_pAddrPhone))
                {
                    CContApp_FreeBuffer(pMe);

                    pMe->m_eMsgRetState = CContApp_GetReturnState(pMe);
                    pMe->m_wMsgResID = IDS_MSG_MAX_PHONE;
                    MOVE_TO_STATE(STATE_MSG);
                    return NFSMACTION_CONTINUE;
                }
            }

            if (AEE_ADDRFIELD_NONE != pMe->m_nFldInputID)
            {
                if(3 != type && TRUE != pMe->m_bInputNotComplete)
                {
                    CContApp_SetRecByFLDID(pMe, pMe->m_nFldInputID);
                }
                else
                {
                    type = 0;
                }
            }
            
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_ADDNEW))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
        }

        case DLGRET_YES:
            if(2 == type)
            {
                type = 0;
                pMe->m_nSaveType = CONTCFG_SAVETYPE_PHONE;
            }
            else if(1 == type)
            {
                type = 0;
                pMe->m_nSaveType = CONTCFG_SAVETYPE_RUIM;
            }
            else if(3 == type)
            {
                pMe->m_wSelectEdit = IDI_ADDNEW_MENU_NAME;
            }
            else
            {
                pMe->m_wErrStrID = IDS_ERR_CREATEREC;
                MOVE_TO_STATE(STATE_ERROR);
            }
            
            return NFSMACTION_CONTINUE;
            
        case DLGRET_NO:
            {
#ifdef FEATURE_CHECK_SAME_NAME 
                if(3 == type)
                {
                    if(SUCCESS == CContApp_CreateCont( pMe, pMe->m_nSaveType == CONTCFG_SAVETYPE_RUIM))
                    {
                        pMe->m_nFldInputID = AEE_ADDR_CAT_PERSONAL;
                        pMe->m_wSelectEdit = IDI_ADDNEW_MENU_NAME;
                        
                        FREEIF(pMe->m_pFldInputBuf); 
                        FREEIF(pMe->m_pAddNewName);                
                        FREEIF(pMe->m_pAddNewMobile);
                        if (pMe->m_nSaveType != CONTCFG_SAVETYPE_RUIM)
                        {
                            FREEIF(pMe->m_pAddNewHome);
                            FREEIF(pMe->m_pAddNewOffice);
                            FREEIF(pMe->m_pAddNewFax);
                            FREEIF(pMe->m_pAddNewEMAIL);
                            FREEIF(pMe->m_pAddNewAddress);
                            FREEIF(pMe->m_pAddNewURL);
                            FREEIF(pMe->m_pAddNewRemark);
                            FREEIF(pMe->m_nRingToneID);
                            pMe->m_nGroupCat = AEE_ADDR_CAT_OTHER;
                        }
                        
                        if(SUCCESS != CContApp_LoadAddrFlds(pMe, pMe->m_wEditCont, SELECT_ALL))
                        {
                            if(SUCCESS == CContApp_DeleteCont(pMe, pMe->m_wEditCont))
                            {
                                CContApp_Phonebook_Load(pMe);
                            }
                            pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                            MOVE_TO_STATE(STATE_ERROR);
                            return NFSMACTION_CONTINUE;
                        }
                       pMe->m_eSuccessRetState = CContApp_GetReturnState(pMe);

                       MOVE_TO_STATE(STATE_SUCCESS);
                    }
                    else
                    {
                        pMe->m_wErrStrID = IDS_ERR_CREATEREC;
                        MOVE_TO_STATE(STATE_ERROR);
                    }

                    type=0;
                    return NFSMACTION_CONTINUE;
                }
#endif
                {
                    FSMState retrunState = CContApp_GetReturnState(pMe);
                    
                    MOVE_TO_STATE(retrunState);
                    type = 0;
                }
            }
            return NFSMACTION_CONTINUE;

        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
        {
            FSMState retrunState;
            
            if(CONTCFG_SAVETYPE_PHONE == pMe->m_nSaveType)
            {  
                FREEIF(pMe->m_pAddNewHome);
                FREEIF(pMe->m_pAddNewOffice);
                FREEIF(pMe->m_pAddNewFax);
                FREEIF(pMe->m_pAddNewEMAIL);
                FREEIF(pMe->m_pAddNewAddress);
                FREEIF(pMe->m_pAddNewURL);
                FREEIF(pMe->m_pAddNewRemark);
                FREEIF(pMe->m_nRingToneID);
            }

            FREEIF(pMe->m_pFldInputBuf);
            FREEIF(pMe->m_pAddNewName);

            //��STATE_EDIT״̬�˻�����ʱ��û��free������Ϊ���ܻ�����ѡ��洢λ�ã���Ҫ�����recentcall ����
            CContApp_GetConfig(pMe, CONTCFG_SAVETYPE, &pMe->m_nSaveType, sizeof(pMe->m_nSaveType));
            if(!(STARTMETHOD_SAVEFIELD == pMe->m_eStartMethod && CONTCFG_SAVETYPE_SELECT == pMe->m_nSaveType))
            {
                FREEIF(pMe->m_pAddNewMobile);
            }

            retrunState = CContApp_GetReturnState(pMe);
            MOVE_TO_STATE(retrunState);
            return NFSMACTION_CONTINUE;
        }
        
        case DLGRET_OK:
            {
                boolean bCard = (pMe->m_nSaveType == CONTCFG_SAVETYPE_RUIM);
                if (pMe->m_pAddNewName == NULL)
                {
                    pMe->m_wSelectEdit =IDI_ADDNEW_MENU_NAME;
                    pMe->m_bInputNotComplete = TRUE;
                    pMe->m_eMsgType = MESSAGE_WARNNING;
                    if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NEEDNAME))
                    {
                        MOVE_TO_STATE(STATE_EXIT);
                        return NFSMACTION_CONTINUE;
                    }
                    return NFSMACTION_WAIT;
                }
#ifndef FEATURE_SAVE_ONLY_NAME             
                else if(IS_NO_PHONENUMBER())
                {
                    pMe->m_wSelectEdit =IDI_ADDNEW_MENU_MOBILE;
                    pMe->m_bInputNotComplete = TRUE;
                    pMe->m_eMsgType = MESSAGE_WARNNING;
                    if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NEEDNUMBER))
                    {
                        MOVE_TO_STATE(STATE_EXIT);
                        return NFSMACTION_CONTINUE;
                    }
                    return NFSMACTION_WAIT;
                }
#endif
                else
                {
#ifdef FEATURE_CHECK_SAME_NAME             
                    // �������
                    {
                        boolean sameRecord = FALSE;
                        IAddrBook          *addr;      // IADDRBOOK interface for RUIM

                        addr = pMe->m_pAddrPhone;
                        
#ifdef FEATURE_RUIM_PHONEBOOK
                        if(TRUE == bCard)
                        {
                            addr = pMe->m_pAddrRUIM;
                        }
#endif
                        if(SUCCESS == IADDRBOOK_CheckSameRecord(addr, pMe->m_pAddNewName, &sameRecord) && sameRecord == TRUE)
                        {
                            type = 3;
                             if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                                                    IDS_SAME_NAME,
                                                                    NULL,
                                                                    -1))
                            {
                                MOVE_TO_STATE(STATE_EXIT);
                                return NFSMACTION_CONTINUE;
                            }
                             
                             return NFSMACTION_WAIT;
                        }
                    }
#endif
                    pMe->m_bInputNotComplete = FALSE;
                    if(SUCCESS == CContApp_CreateCont( pMe, bCard))
                    {
                        pMe->m_nFldInputID = AEE_ADDR_CAT_PERSONAL;
                        pMe->m_wSelectEdit = IDI_ADDNEW_MENU_NAME;
                        
                        FREEIF(pMe->m_pFldInputBuf); 
                        FREEIF(pMe->m_pAddNewName);                
                        FREEIF(pMe->m_pAddNewMobile);
                        if (!bCard)
                        {
                            FREEIF(pMe->m_pAddNewHome);
                            FREEIF(pMe->m_pAddNewOffice);
                            FREEIF(pMe->m_pAddNewFax);
                            FREEIF(pMe->m_pAddNewEMAIL);
                            FREEIF(pMe->m_pAddNewAddress);
                            FREEIF(pMe->m_pAddNewURL);
                            FREEIF(pMe->m_pAddNewRemark);
                            FREEIF(pMe->m_nRingToneID);
                            pMe->m_nGroupCat = AEE_ADDR_CAT_OTHER;
                        }
                        
                        if(SUCCESS != CContApp_LoadAddrFlds(pMe, pMe->m_wEditCont, SELECT_ALL))
                        {
                            if(SUCCESS == CContApp_DeleteCont(pMe, pMe->m_wEditCont))
                            {
                                CContApp_Phonebook_Load(pMe);
                            }
                            pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                            MOVE_TO_STATE(STATE_ERROR);
                            return NFSMACTION_CONTINUE;
                        }

                       pMe->m_eSuccessRetState = CContApp_GetReturnState(pMe);

                       MOVE_TO_STATE(STATE_SUCCESS);
                    }
                    else
                    {
                        pMe->m_wErrStrID = IDS_ERR_CREATEREC;
                        MOVE_TO_STATE(STATE_ERROR);
                    }
                }
            }
            break;
        
        case DLGRET_EDIT:  
            
            MOVE_TO_STATE(STATE_ADDNEW_INPUT);
            break;

        // ���������
        case DLGRET_NUM_UNAVAILD:
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_MSG_NUM_UNAVAILD))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;

            break;
            
        case DLGRET_MSGBOX_OK:
            break;

        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_ADDNEW

#ifdef FEATURE_RUIM_PHONEBOOK
/*==============================================================================
������
       Handler_STATE_SELECTPOS
˵����
       STATE_SELECTPOS ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_SELECTPOS(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_SELECTPOS %d", pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            IMenuCtl  *pMenuCtl;
            
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_POSITION))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                                      IDC_POSITION_MENU);
            
            CContApp_BuildSelectPostionMenu(pMe, pMenuCtl);
#if defined FEATURE_CARRIER_THAILAND_HUTCH
            IMENUCTL_SetSel(pMenuCtl, IDS_UIMCARD);
#endif //defined FEATURE_CARRIER_THAILAND_HUTCH 
            //MENU_SET_COMICON(pMenuCtl);
            (void)IMENUCTL_Redraw(pMenuCtl);
            return NFSMACTION_WAIT;
        }
        
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            if (STARTMETHOD_MAINMENU == pMe->m_eStartMethod)
            {
                if(LISTFROMNONE == pMe->m_eListFrom)
                {
                    MOVE_TO_STATE(STATE_MAINMENU);
                }
                else if(LISTFROMMAINMENU== pMe->m_eListFrom)
                {
                    MOVE_TO_STATE(STATE_MAINLIST);
                }
                else if(LISTFROMSEARCH == pMe->m_eListFrom)
                {
                    MOVE_TO_STATE(STATE_FIND_LIST);
                }
                else if(LISTFROMGROUP == pMe->m_eListFrom)
                {
                    MOVE_TO_STATE(STATE_GROUPVIEW_LIST);
                }
                else
                {
                    pMe->m_wErrStrID = IDS_ERR_UNKNOWN;
                    MOVE_TO_STATE(STATE_ERROR);
                }
            }
            else if(STARTMETHOD_SAVEFIELD == pMe->m_eStartMethod || STARTMETHOD_ADDFIELD== pMe->m_eStartMethod)
            {
                //��STATE_EDIT״̬�˻�����ʱ��û��free������Ϊ���ܻ�����ѡ��洢λ�ã���Ҫ�����recentcall ����
                if (pMe->m_pAddNewMobile)
                {
                    FREEIF(pMe->m_pAddNewMobile);
                }
                MOVE_TO_STATE(STATE_EXIT);
            }
            else
            {
                MOVE_TO_STATE(STATE_MAINLIST);
            }
            break;
        
        case DLGRET_OK:
            switch(pMe->m_wPosCmd)
            {
                case IDS_POSITION_PHONE:
                    pMe->m_nSaveType = CONTCFG_SAVETYPE_PHONE;
                    if(STARTMETHOD_ADDFIELD == pMe->m_eStartMethod)
                    {
                        pMe->m_eAddFldType = ADDFLDTOPHONE;
                    }
                    break;
                    
                case IDS_UIMCARD:
                default:
                    pMe->m_nSaveType = CONTCFG_SAVETYPE_RUIM;
                    if(STARTMETHOD_ADDFIELD == pMe->m_eStartMethod)
                    {
                        pMe->m_eAddFldType = ADDFLDTORUIMCARD;
                    }
                    break;
            }

            if(pMe->m_eStartMethod == STARTMETHOD_ADDFIELD)
            {
                CContApp_LoadPhoneBook(pMe, pMe->m_nSaveType);
                if(IS_ZERO_REC())
                {
                    // Show dialog message
                    pMe->m_eMsgType = MESSAGE_WARNNING;
                    if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_EMPTY))
                    {
                        MOVE_TO_STATE(STATE_EXIT);
                        return NFSMACTION_CONTINUE;
                    }
                    return NFSMACTION_WAIT;
                }
                    
                MOVE_TO_STATE(STATE_MAINLIST);
                break;
            }
            
            if ((pMe->m_eStartMethod == STARTMETHOD_SAVEFIELD) && (pMe->m_nSaveType == CONTCFG_SAVETYPE_RUIM))
            {
                if (pMe->m_pAddNewEMAIL)
                {
                    pMe->m_eMsgType = MESSAGE_WARNNING;
                    CContApp_ShowMsgBox( pMe, IDS_SAVE_EMAIL);
                }
                else if (pMe->m_pAddNewURL)
                {
                    pMe->m_eMsgType = MESSAGE_WARNNING;
                    CContApp_ShowMsgBox( pMe, IDS_SAVE_URL);
                }
                else
                {
                    pMe->m_eMsgType = MESSAGE_WARNNING;
                    pMe->m_wSelectEdit = IDI_ADDNEW_MENU_NAME;
                    MOVE_TO_STATE(STATE_ADDNEW);
                }                    
            }
            else
            {
                pMe->m_wSelectEdit = IDI_ADDNEW_MENU_NAME;
                MOVE_TO_STATE(STATE_ADDNEW);
            }
            break;

        case DLGRET_NUMBER_TOOLONG:
            // Show dialog message
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NUMBER_TOOLONG))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;

        case DLGRET_MSGBOX_OK:  
            if(IDS_UIMCARD == pMe->m_wPosCmd
                && STARTMETHOD_SAVEFIELD == pMe->m_eStartMethod
#ifndef WIN32
                && WSTRLEN(pMe->m_pAddNewMobile) > OEMRUIMAddr_GetRUIMMaxNumberSize()
#else
                && WSTRLEN(pMe->m_pFldInputBuf) > MAX_INPUT_NUM
#endif
                )
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
           MOVE_TO_STATE(STATE_SELECTPOS);
           break;   
           
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_SELECTPOS
#endif// FEATURE_RUIM_PHONEBOOK

/*==============================================================================
������
       Handler_STATE_EDIT
˵����
       STATE_EDIT ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_EDIT(CContApp *pMe)
{
    int nRet = SUCCESS;

#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_EDIT %d", pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            //if(pMe->m_nAddnewOrEdit == ADDOREDIT_EDIT)
            //{
                if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                     pMe->m_wEditCont,
                                                     SELECT_ALL))
                {
                    pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                    MOVE_TO_STATE(STATE_ERROR);
                    return NFSMACTION_CONTINUE;
                }
            //}

            if(pMe->m_nAddnewOrEdit == ADDOREDIT_EDIT && pMe->m_ePreState != STATE_EDIT_INPUT
                && pMe->m_nFldInputID != AEE_ADDRFIELD_RINGTONE && FALSE == pMe->m_bInputNotComplete)
            {
                CContApp_FreeBuffer(pMe);
                CContApp_GetRecByID(pMe, pMe->m_wEditCont);    
            }

            //��һ�ε�STATE_EDIT���жϣ����ǵ����޿���״̬��
            //addField from other app
            if(!(STARTMETHOD_ADDFIELD == pMe->m_eStartMethod && (STATE_NUMFLDVIEW == pMe->m_ePreState || STATE_MAINLIST == pMe->m_ePreState))
                || (STARTMETHOD_SAVEFIELD == pMe->m_eStartMethod && (STATE_SELECTOPT == pMe->m_ePreState || STATE_LOAD == pMe->m_ePreState)))
            {
                if (AEE_ADDRFIELD_NONE != pMe->m_nFldInputID  && TRUE != pMe->m_bInputNotComplete)
                {
                    CContApp_SetRecByFLDID(pMe, pMe->m_nFldInputID);
                }
            }
         
      /*      if(pMe->m_ePreState == STATE_EDIT_INPUT)       
            {
                nRet = CContApp_SaveEditField(pMe,pMe->m_nFldInputID);
            }*/
            /*if(pMe->m_nFldInputID == AEE_ADDRFIELD_RINGTONE)
            {
                pMe->m_nFldInputID = AEE_ADDRFIELD_NONE;
                nRet = CContApp_SaveRingField(pMe, pMe->m_nRingToneID);
            }*/

            // Show dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_EDIT))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }

            return NFSMACTION_WAIT;
        }
        
        //Proccess yourself dialog retrn value here
#ifdef FEATURE_GIVEUP_EDIT
        case DLGRET_GIVEUP_EDIT:
            if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                                    IDS_SAVE_EDIT,
                                                    NULL,
                                                    -1))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;


        case DLGRET_NO:
            MOVE_TO_STATE(STATE_EXIT);
            return NFSMACTION_CONTINUE;
#endif
        case DLGRET_CANCELED:
            if(!IS_RUIM_REC(pMe->m_wEditCont))
            {  
                FREEIF(pMe->m_pAddNewHome);
                FREEIF(pMe->m_pAddNewOffice);
                FREEIF(pMe->m_pAddNewFax);
                FREEIF(pMe->m_pAddNewEMAIL);
                FREEIF(pMe->m_pAddNewAddress);
                FREEIF(pMe->m_pAddNewURL);
                FREEIF(pMe->m_pAddNewRemark);
                FREEIF(pMe->m_nRingToneID);
            }

            FREEIF(pMe->m_pFldInputBuf);
            FREEIF(pMe->m_pAddNewName);

            //��STATE_EDIT״̬�˻�����ʱ��û��free������Ϊ���ܻ�����ѡ��洢λ�ã���Ҫ�����recentcall ����
            //callapp��������number
            CContApp_GetConfig(pMe, CONTCFG_SAVETYPE, &pMe->m_nSaveType, sizeof(pMe->m_nSaveType));
            if(!(STARTMETHOD_SAVEFIELD == pMe->m_eStartMethod && CONTCFG_SAVETYPE_SELECT == pMe->m_nSaveType))
            {
                FREEIF(pMe->m_pAddNewMobile);
            }

            {
                FSMState retrunState;
                retrunState = CContApp_GetReturnState(pMe);
                MOVE_TO_STATE(retrunState);
            }
            return NFSMACTION_CONTINUE;

#ifdef FEATURE_GIVEUP_EDIT
        case DLGRET_YES:
#endif
        case DLGRET_OK:
        {
            if (pMe->m_pAddNewName == NULL)
            {
                pMe->m_wSelectEdit =IDI_EDIT_MENU_NAME;
                pMe->m_bInputNotComplete = TRUE;
                pMe->m_eMsgType = MESSAGE_WARNNING; 
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NEEDNAME))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
#ifndef FEATURE_SAVE_ONLY_NAME             
            else if(IS_NO_PHONENUMBER())
            {

                pMe->m_wSelectEdit =IDI_EDIT_MENU_MOBILE;
                pMe->m_bInputNotComplete = TRUE;
                pMe->m_eMsgType = MESSAGE_WARNNING; 
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NEEDNUMBER))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
#endif
            else
            {   
                nRet = CContApp_SaveEditResult(pMe);

                // ����ֶΣ�ֻ�л�����¼�����ߵ�����
               if( SUCCESS == nRet)
                {
                    pMe->m_wSelectEdit = IDI_EDIT_MENU_NAME;
                    pMe->m_nFldInputID = AEE_ADDRFIELD_NONE;
                    CContApp_FreeBuffer(pMe);
                    FREEIF(pMe->m_pFldInputBuf);
                    if(STARTMETHOD_ADDFIELD == pMe->m_eStartMethod || STARTMETHOD_SAVEFIELD== pMe->m_eStartMethod)
                    {
                        pMe->m_eSuccessRetState = STATE_EXIT;
                    }
                    else
                    {
                        pMe->m_eSuccessRetState = CContApp_GetReturnState(pMe);
                        
                        if(STATE_GROUPVIEW_LIST == pMe->m_eSuccessRetState)
                        {
                            CContApp_GetGroupCat(pMe, pMe->m_wselGroupId);

                            CContApp_LoadByCat(pMe, pMe->m_nGroupCat);
                        }
                    }
                    MOVE_TO_STATE(STATE_SUCCESS);
                }
                else if(EMAXFIELD == nRet)
                {
                    pMe->m_wMsgResID = IDS_MSG_MAXFIELDS;
                    pMe->m_eMsgRetState = STATE_EDIT;
                    MOVE_TO_STATE(STATE_MSG);
                }
                else if(EEXIST == nRet)
                {
                    pMe->m_wMsgResID = IDS_MSG_EXIST;
                    pMe->m_eMsgRetState = STATE_EDIT;
                    MOVE_TO_STATE(STATE_MSG);
                }
                else
                {
                    if(pMe->m_wErrStrID != IDS_ERR_UPDATE)
                    {
                        pMe->m_wErrStrID = IDS_ERR_ADDFIELD;
                    }
                    MOVE_TO_STATE(STATE_ERROR);
                }
             }                    
        }
        break;
            
        case DLGRET_EDIT:
            MOVE_TO_STATE(STATE_EDIT_INPUT);
            break;
            
        // ���������
        case DLGRET_NUM_UNAVAILD:
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_MSG_NUM_UNAVAILD))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;

        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_EDIT);
            break;      

        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_EDIT

/*==============================================================================
������
       Handler_STATE_EDIT_INPUT
˵����
       STATE_EDIT_INPUT ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_EDIT_INPUT(CContApp *pMe)
{
#ifdef FEATURE_GIVEUP_EDIT
    int nRet = SUCCESS;
#endif

#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_EDIT_INPUT %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_INPUTFLD))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            pMe->m_nInputMode = EDIT_MENU_MODE;
            MOVE_TO_STATE(STATE_EDIT);
            break;

#ifdef FEATURE_GIVEUP_EDIT
        case DLGRET_GIVEUP_EDIT:
            if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                                    IDS_SAVE_EDIT,
                                                    NULL,
                                                    -1))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;

        case DLGRET_NO:
            MOVE_TO_STATE(STATE_EXIT);
            return NFSMACTION_CONTINUE;

        case DLGRET_YES:
             nRet = CContApp_SaveEditResult(pMe);
            
             // ����ֶΣ�ֻ�л�����¼�����ߵ�����
            if( SUCCESS == nRet)
            {
                 CContApp_FreeBuffer(pMe);
                 FREEIF(pMe->m_pFldInputBuf);
                 MOVE_TO_STATE(STATE_SUCCESS);
             }
             else if(EMAXFIELD == nRet)
             {
                 pMe->m_wMsgResID = IDS_MSG_MAXFIELDS;
                 MOVE_TO_STATE(STATE_MSG);
             }
             else if(EEXIST == nRet)
             {
                 pMe->m_wMsgResID = IDS_MSG_EXIST;
                 MOVE_TO_STATE(STATE_MSG);
             }
             else
             {
                 if(pMe->m_wErrStrID != IDS_ERR_UPDATE)
                 {
                     pMe->m_wErrStrID = IDS_ERR_ADDFIELD;
                 }
                 MOVE_TO_STATE(STATE_ERROR);
             }
             break;
#endif

        case DLGRET_OK:
            pMe->m_nInputMode = EDIT_MENU_MODE;
            MOVE_TO_STATE(STATE_EDIT);
            break;
        
        case DLGRET_MSGBOX_OK:
            //MOVE_TO_STATE(STATE_EDIT);
            break;
            
        case DLGRET_INPUT_NULL:
            // field can not be NULL
            if(AEE_ADDRFIELD_NAME != pMe->m_nFldInputID)
            {
                pMe->m_eMsgType = MESSAGE_WARNNING; 
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_MSG_INPUTNULL))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                
                return NFSMACTION_WAIT;
            }
            break;
   
        case DLGRET_NUM_UNAVAILD:
            pMe->m_eMsgType = MESSAGE_WARNNING; 
            if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_MSG_NUM_UNAVAILD))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;

        case DLGRET_EMAIL_UNAVAILD:
            pMe->m_eMsgType = MESSAGE_WARNNING; 
            if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_MSG_EMAIL_UNAVAILD))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;            
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_EDIT_INPUT

/*==============================================================================
������
       Handler_STATE_EDIT_SELECTGROUP
˵����
       STATE_EDIT_SELECTGROUP ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_EDIT_SELECTGROUP(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_EDIT_SELECTGROUP %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            IMenuCtl *pMenuCtl;
            
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_GROUP))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                                      IDC_GROUP_MENU);
            
            (void)IMENUCTL_DeleteItem(pMenuCtl, IDS_GROUP_NONE_STRING);

#ifdef FEATURE_CARRIER_THAILAND_HUTCH    
            CContApp_BuildGroupMenu(pMe, pMenuCtl);
#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  
    
            return NFSMACTION_WAIT;
        }
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_EDIT);
            break;
            
        case DLGRET_OK:
            // ���¼�¼Ⱥ�飬ֻ�л�����¼�����ߵ�����
            /*if( SUCCESS == CContApp_SetCat( pMe,
                                            pMe->m_wEditCont,
                                            pMe->m_nGroupCat))
            {
                MOVE_TO_STATE(STATE_EDIT);
            }
            else
            {
                pMe->m_wErrStrID = IDS_ERR_UPDATE;
                MOVE_TO_STATE(STATE_ERROR);
            }*/
            
            MOVE_TO_STATE(STATE_EDIT);
            return NFSMACTION_CONTINUE;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_EDIT_SELECTGROUP


/*==============================================================================
������
       Handler_STATE_DELETE
˵����
       STATE_DELETE ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_DELETE(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_DELETE %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // ��¼�Ƿ�Ϊ��
            if(IS_ZERO_REC())
            {
                 MOVE_TO_STATE(STATE_MAINMENU);
                 return NFSMACTION_CONTINUE;
            }
            // Restore the menu select
            POP_LISTMENU_SEL(pMe->m_wMainListSel);
            
            // Restore the menu index
            POP_LISTMENU_IDX(pMe->m_wMainListIdx);
            
            // Show dialog message
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_DELETE))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        case DLGRET_CANCELED:   
            if (pMe->m_eStartMethod == STARTMETHOD_NORMAL)
            {
                MOVE_TO_STATE(STATE_OPTS);
            }
            else
            {
                //MOVE_TO_STATE(STATE_MAINMENU);
                MOVE_TO_STATE(STATE_DELETE_SELECT);
            }
            CContApp_FreeSelectFieldListNode();
            CContApp_Phonebook_Load(pMe);
            //CContApp_LoadPhoneBook(pMe, pMe->m_nViewType);
            break;
        case DLGRET_NO:
            MOVE_TO_STATE(STATE_DELETE);		
            break;            
        case DLGRET_OK:
             if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                                    IDS_YESNO_DELETEREC,
                                                    NULL,
                                                    -1))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
        case DLGRET_YES:
        { 
             MOVE_TO_STATE(STATE_DELETING);
             return NFSMACTION_CONTINUE;             
        }    
        break;

        case DLGRET_NO_SELECTION:
            // Show dialog message
            pMe->m_eMsgType = MESSAGE_WARNNING; 
            if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NO_SELECTION))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
        
        case DLGRET_MSGBOX_OK:
            break;
            
        case DLGRET_SELECT:
            MOVE_TO_STATE(STATE_SELECTOPT);
            return NFSMACTION_CONTINUE;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_DELETE

/*==============================================================================
������
       Handler_STATE_SELECT_RECORD
˵����
       STATE_SELECT_RECORD ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_SELECT_RECORD(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_SELECT_RECORD %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // ��¼�Ƿ�Ϊ��
            if(IS_ZERO_REC())
            {
                 MOVE_TO_STATE(STATE_MAINMENU);
                 return NFSMACTION_CONTINUE;
            }

            POP_LISTMENU_SEL(pMe->m_wMainListSel);
            
            // Restore the menu index
            POP_LISTMENU_IDX(pMe->m_wMainListIdx);
            
            // Show dialog message
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_SELECT_RECORD))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        case DLGRET_CANCELED:   
            MOVE_TO_STATE(STATE_MANAGEMENT);
            CContApp_FreeSelectFieldListNode();
            //CContApp_Phonebook_Load(pMe);
            break;
            
        case DLGRET_NO:
            MOVE_TO_STATE(STATE_DELETE);
            break;     
                        
        case DLGRET_YES:
        {             
             MOVE_TO_STATE(STATE_DELETING);
             return NFSMACTION_CONTINUE;             
        }    
            break;
            
        case DLGRET_SELECT:
            MOVE_TO_STATE(STATE_SELECTOPT);
            return NFSMACTION_CONTINUE;

        case DLGRET_NO_SELECTION:
            // Show dialog message
            pMe->m_eMsgType = MESSAGE_WARNNING; 
            if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NO_SELECTION))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
        
        case DLGRET_MSGBOX_OK:
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_SELECT_RECORD

#ifdef FEATURE_RUIM_PHONEBOOK
/*==============================================================================
������
       Handler_STATE_COPYMOVE
˵����
       STATE_COPYMOVE ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_COPYMOVE(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_COPYMOVE %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            IMenuCtl *pMenuCtl;

            // Show dialog
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_COPYMOVE))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                                      IDC_COPYMOVE_MENU);
   /*         
#ifndef FEATURE_CARRIER_THAILAND_HUTCH  
            (void)IMENUCTL_DeleteItem(pMenuCtl, IDI_COPYMOVE_MENU_MOVE);  
#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  
            */
            //MENU_SET_COMICON(pMenuCtl);
            (void)IMENUCTL_Redraw(pMenuCtl);
            return NFSMACTION_WAIT;
        }
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
//#ifdef FEATURE_CARRIER_THAILAND_HUTCH  
            MOVE_TO_STATE(STATE_MANAGEMENT);
//#else
//            MOVE_TO_STATE(STATE_MANAGEMENT);
//#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  
            break;
            
        case DLGRET_COPY:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);  

            if((pMe->m_nCopyMoveType == COPYMULTIPE_TOPHONE || pMe->m_nCopyMoveType == MOVEMULTIPE_TOPHONE)
                && IS_MAX_REC(pMe->m_pAddrPhone))
            {
                pMe->m_eMsgType = MESSAGE_WARNNING;
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_FULL_PHONE))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
            
            if((pMe->m_nCopyMoveType == COPYMULTIPE_TOUIM || pMe->m_nCopyMoveType == MOVEMULTIPE_TOUIM)
                && IS_MAX_REC(pMe->m_pAddrRUIM))
            {
                pMe->m_eMsgType = MESSAGE_WARNNING;
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_FULL_UIM))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
            
            if(IS_ZERO_REC())
            {
                // Show dialog message
                pMe->m_eMsgType = MESSAGE_WARNNING; 
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_EMPTY))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
            else
            {
                pMe->m_eTotalSelectNum = 0;
                //pMe->m_wSelectSmart = CONTAPP_RECID_NULL;
                pMe->m_wMainListSel = MENU_SELECT_NULL;
                
                FREEIF(pMe->m_szAlpha);
                MOVE_TO_STATE(STATE_COPY);
            }
            break;
            
        case DLGRET_MSGBOX_OK:  
           MOVE_TO_STATE(STATE_COPYMOVE);
           break;   
               
        case DLGRET_ALLCOPYMOVE:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);            
            MOVE_TO_STATE(STATE_COPYMOVEALL);
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_COPYMOVE

/*==============================================================================
������
       Handler_STATE_COPYMOVEALL
˵����
       STATE_COPYMOVEALL ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_COPYMOVEALL(CContApp *pMe)
{

#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_COPYMOVEALL %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            // ��ʾȷ�϶Ի���
            if((pMe->m_nCopyMoveType == COPYMULTIPE_TOUIM)||(pMe->m_nCopyMoveType == COPYMULTIPE_TOPHONE))
            {
                if(SUCCESS != CContApp_ShowYesNoDialog(pMe, IDS_YESNO_COPYREC, NULL, -1))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
            }
			 else if((pMe->m_nCopyMoveType == MOVEMULTIPE_TOUIM)||(pMe->m_nCopyMoveType == MOVEMULTIPE_TOPHONE))
            {
                if(SUCCESS != CContApp_ShowYesNoDialog(pMe, IDS_YESNO_MOVERECORD, NULL, -1))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
            }
            return NFSMACTION_WAIT;
        }
        
        //Proccess yourself dialog retrn value here
        case DLGRET_NO:
            MOVE_TO_STATE(STATE_COPYMOVE);
            break;
            
        case DLGRET_YES:
            MOVE_TO_STATE(STATE_COPYING);	
            break;
            
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_COPYMOVE);
            break;
        
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_COPYMOVE_COPY

/*==============================================================================
������
       Handler_STATE_COPYING
˵����
       STATE_COPYING ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_COPYING(CContApp *pMe)
{
    int nRet = SUCCESS;
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_COPYING %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            if(TRUE == pMe->m_bIsNotComplete)
            {
                pMe->m_eMsgType = MESSAGE_WARNNING; 
                if(SUCCESS != CContApp_ShowMsgBox(pMe, 0))
                {
                    pMe->m_bIsNotComplete = FALSE;
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
            nRet = CContApp_CopyMultipe(pMe);
            
            /*
            switch(pMe->m_nCopyMoveType)
            {
                case COPYALLTOUIM:
                    nRet = CContApp_CopyAllToRUIM(pMe, pMe->m_wCopyStartID);
                    break;
                case COPYALLTOPHONE:
                    nRet = CContApp_CopyAllToPhone(pMe, pMe->m_wCopyStartID);
                    break;
                case MOVEALLTOUIM:
                    nRet = CContApp_CopyAllToRUIM(pMe, pMe->m_wCopyStartID);
                    break;
                case MOVEALLTOPHONE:
                    nRet = CContApp_CopyAllToPhone(pMe, pMe->m_wCopyStartID);
                    break;
                default:
                    break;
            }
*/
            // Proccess the return value
           if(EMAXRECORD == nRet)
            {
                pMe->m_wMsgResID = IDS_MSG_MAXFIELDS;
                pMe->m_eMsgRetState = STATE_COPYMOVE;
                MOVE_TO_STATE(STATE_MSG);
                return NFSMACTION_CONTINUE;       
            }
            else if(ENORECORD == nRet)
            {
                pMe->m_wMsgResID = IDS_MSG_NORECORD;
                pMe->m_eMsgRetState = STATE_COPYMOVE;
                MOVE_TO_STATE(STATE_MSG);    
                return NFSMACTION_CONTINUE;       
            }
            else if(EFAILED == nRet)
            {
                pMe->m_wMsgResID = IDS_MSG_COPYFAILED;
                pMe->m_eMsgRetState = STATE_COPYMOVE;
                MOVE_TO_STATE(STATE_MSG);    
                return NFSMACTION_CONTINUE;        
            }
            
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_COPYING))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
    
        //Proccess yourself dialog retrn value here
        case DLGRET_OK:
            if(pMe->m_nFailNum > 0)
            {
                // �к�������ģ����ܿ���������
                pMe->m_eMsgType = MESSAGE_WARNNING; 
                if(SUCCESS != CContApp_ShowMsgBox(pMe, 0))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
            
            if(pMe->m_wCopyRecCnt > 0)
            {
                // Reload contacts
                if(SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType))
                {
                    pMe->m_wErrStrID = IDS_ERR_LOAD;
                    MOVE_TO_STATE(STATE_ERROR);
                    return NFSMACTION_CONTINUE;
                }
            }           
            pMe->m_eSuccessRetState = STATE_COPYMOVE;
            MOVE_TO_STATE(STATE_SUCCESS);       
            break;
        
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_EXIT);
            return NFSMACTION_CONTINUE;

        case DLGRET_FULL:
            if(PHONE_FULL == pMe->m_eErrType)
            {
                pMe->m_wMsgResID = IDS_FULL_PHONE_DONE;
            }
            else if(UIMCARD_FULL == pMe->m_eErrType)
            {
                pMe->m_wMsgResID = IDS_FULL_UIM_DONE;
            }
            //pMe->m_wMsgResID = IDS_MSG_COPYFAILED;
            //pMe->m_eMsgRetState = STATE_COPYMOVE;
            //MOVE_TO_STATE(STATE_MSG);  
            pMe->m_eMsgType = MESSAGE_WARNNING; 
            if(SUCCESS != CContApp_ShowMsgBox(pMe, 0))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            //return NFSMACTION_CONTINUE;
            
        case DLGRET_ERR:
            pMe->m_wErrStrID = IDS_MARK_ERR;
            MOVE_TO_STATE(STATE_ERROR);
            return NFSMACTION_CONTINUE;
            
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_COPYMOVE);
            break;

        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_COPYMOVE_COPY

/*==============================================================================
������
       Handler_STATE_COPY
˵����
       STATE_COPY ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_COPY(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_COPY %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // ��¼�Ƿ�Ϊ��
            if(IS_ZERO_REC())
            {
                 MOVE_TO_STATE(STATE_MAINMENU);
                 return NFSMACTION_CONTINUE;
            }
            /*
            // Restore the menu select
            POP_LISTMENU_SEL(pMe->m_wDeleteListSel);
            
            // Restore the menu index
            POP_LISTMENU_IDX(pMe->m_wDeleteListIdx);
            */
                
            POP_LISTMENU_SEL(pMe->m_wMainListSel);
            
            // Restore the menu index
            POP_LISTMENU_IDX(pMe->m_wMainListIdx);
            // Show dialog message
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_COPY))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
         
        case DLGRET_CANCELED:
            if (pMe->m_eStartMethod == STARTMETHOD_NORMAL)
            {
                MOVE_TO_STATE(STATE_OPTS);
            }
            else
            {
                MOVE_TO_STATE(STATE_COPYMOVE);
            }
           CContApp_FreeSelectFieldListNode();
           //restore viewType
           CContApp_Phonebook_Load(pMe);
           return NFSMACTION_CONTINUE;
            
        case DLGRET_OK:
        {                       
            int16 resId = 0;
            
            if(COPYMULTIPE == pMe->m_nCopyOrMove)
            {
                resId = IDS_YESNO_COPYREC;
            }
            else
            {
                resId = IDS_YESNO_MOVEREC;
            }
            
            if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                        resId,
                                        NULL,
                                        -1))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
        }
        break;        

    case DLGRET_YES:
        if(pMe->m_eTotalSelectNum == 1 && !CContApp_CheckNumLen(pMe, pCurSelectFieldNode->SelectFieldInfo.wIndex))
        {
            // Show dialog message
            pMe->m_eMsgType = MESSAGE_WARNNING; 
            if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NUMBER_TOOLONG))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
        }
        
        MOVE_TO_STATE(STATE_COPYING);
        CContApp_Phonebook_Load(pMe);
        break;

    case DLGRET_NO:
        break;

    case DLGRET_NO_SELECTION:
        // Show dialog message
        pMe->m_eMsgType = MESSAGE_WARNNING; 
        if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NO_SELECTION))
        {
            MOVE_TO_STATE(STATE_EXIT);
            return NFSMACTION_CONTINUE;
        }
        return NFSMACTION_WAIT;

    case DLGRET_MSGBOX_OK:
        break;
        
    case DLGRET_SELECT:
        MOVE_TO_STATE(STATE_SELECTOPT);
        break;
        
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
}

/*==============================================================================
������
       Handler_STATE_SETTING
˵����
       STATE_SETTING ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_SETTING(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_SETTING %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            IMenuCtl  *pMenuCtl;
            
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_SETTING))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            pMenuCtl = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_SETTING_MENU);            
#ifndef FEATURE_CARRIER_THAILAND_HUTCH   
            (void)IMENUCTL_DeleteItem(pMenuCtl, IDI_SETTING_MENU_COPYMOVE);
            (void)IMENUCTL_DeleteItem(pMenuCtl, IDI_SETTING_MENU_MEMORY);
#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  

            MENU_SET_COMICON(pMenuCtl);
            (void)IMENUCTL_Redraw(pMenuCtl);
            
            return NFSMACTION_WAIT;
        }
        
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_MAINMENU);
            break;

        case DLGRET_SAVETO:
            MOVE_TO_STATE(STATE_SAVETO);
            break;
            
        case DLGRET_VIEWTYPE:
            MOVE_TO_STATE(STATE_VIEWTYPE);
            break;
            
#ifdef FEATURE_CARRIER_THAILAND_HUTCH   
#ifdef FEATURE_RUIM_PHONEBOOK
        case DLGRET_COPY:
            if(IS_ZERO_REC())
            {
                // Show dialog message
                pMe->m_eMsgType = MESSAGE_WARNNING; 
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_EMPTY))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            } 
            
            //MOVE_TO_STATE(STATE_COPY);
            MOVE_TO_STATE(STATE_COPYMOVE);
            break;
#endif// FEATURE_RUIM_PHONEBOOK
            
        case DLGRET_CHECKCAPACITY:
            MOVE_TO_STATE(STATE_CAPACITY);
            break;  

         case DLGRET_MSGBOX_OK:  
            MOVE_TO_STATE(STATE_SETTING);
            break;              
#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_SETTING

/*==============================================================================
������
       Handler_STATE_SAVETO
˵����
       STATE_SAVETO ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_SAVETO(CContApp *pMe)
{
    byte nData_saveto;
    
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_SAVETO %d", pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            IMenuCtl  *pMenuCtl;
            
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_POSITION))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                                      IDC_POSITION_MENU);
            
            (void)IMENUCTL_SetTitle( pMenuCtl,
                                     CONTAPP_RES_FILE_LANG,
                                     IDS_SAVETO,
                                     NULL);
                                     
            CContApp_BuildSaveToMenu(pMe, pMenuCtl);
            
            InitMenuIcons(pMenuCtl);
            
            // Get config
            (void)CContApp_GetConfig( pMe, 
                                      CONTCFG_SAVETYPE, 
                                      &nData_saveto,
                                      sizeof(nData_saveto));
            switch(nData_saveto)
            {
                case CONTCFG_SAVETYPE_SELECT:
                    SetMenuIcon(pMenuCtl, IDS_POSITION_SELECT, TRUE);
                    IMENUCTL_SetSel(pMenuCtl, IDS_POSITION_SELECT);
                    break;
                    
                case CONTCFG_SAVETYPE_PHONE:
                    SetMenuIcon(pMenuCtl, IDS_POSITION_PHONE, TRUE);
                    IMENUCTL_SetSel(pMenuCtl, IDS_POSITION_PHONE);
                    break;
                    
                case CONTCFG_SAVETYPE_RUIM:
                    SetMenuIcon(pMenuCtl, IDS_UIMCARD, TRUE);
                    IMENUCTL_SetSel(pMenuCtl, IDS_UIMCARD);
                    break;
                    
                default:
                    break;
            }
            return NFSMACTION_WAIT;
        }
        
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_MANAGEMENT);
            break;
        
        case DLGRET_OK:
            switch(pMe->m_wPosCmd)
            {
                case IDS_POSITION_PHONE:
                    nData_saveto= CONTCFG_SAVETYPE_PHONE;
                    break;
                     
                case IDS_UIMCARD:
                    nData_saveto = CONTCFG_SAVETYPE_RUIM;
                    break;
                    
                case IDS_POSITION_SELECT:
                default:
                    nData_saveto = CONTCFG_SAVETYPE_SELECT;
                    break;
            }

            // update the current data
             pMe->m_nSaveType = nData_saveto;

            (void)CContApp_SetConfig( pMe, 
                                      CONTCFG_SAVETYPE, 
                                     &nData_saveto,
                                      sizeof(nData_saveto));

            pMe->m_eSuccessRetState = STATE_MANAGEMENT;                     
            MOVE_TO_STATE(STATE_SUCCESS);
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_SAVETO

/*==============================================================================
������
       Handler_STATE_VIEWTYPE
˵����
       STATE_VIEWTYPE ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_VIEWTYPE(CContApp *pMe)
{
    byte nData;
    
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_VIEWTYPE %d", pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            IMenuCtl  *pMenuCtl;
            
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_POSITION))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                                      IDC_POSITION_MENU);
            
            (void)IMENUCTL_SetTitle( pMenuCtl,
                                     CONTAPP_RES_FILE_LANG,
                                     IDS_VIEWTYPE,
                                     NULL);
            CContApp_BuildViewTypeMenu(pMe, pMenuCtl);
            
            InitMenuIcons(pMenuCtl);
            
            // Get config
            (void)CContApp_GetConfig( pMe, 
                                      CONTCFG_VIEWTYPE, 
                                     &nData,
                                      sizeof(nData));
            switch(nData)
            {
                case CONTCFG_VIEWTYPE_ALL:
                    SetMenuIcon(pMenuCtl, IDS_POSITION_ALL, TRUE);
                    IMENUCTL_SetSel(pMenuCtl, IDS_POSITION_ALL);
                    break;
                    
                case CONTCFG_VIEWTYPE_PHONE:
                    SetMenuIcon(pMenuCtl, IDS_POSITION_PHONE, TRUE);
                    IMENUCTL_SetSel(pMenuCtl, IDS_POSITION_PHONE);
                    break;
                    
                case CONTCFG_VIEWTYPE_RUIM:
                    SetMenuIcon(pMenuCtl, IDS_UIMCARD, TRUE);
                    IMENUCTL_SetSel(pMenuCtl, IDS_UIMCARD);
                    break;
                    
                default:
                    break;
            }
            return NFSMACTION_WAIT;
        }
        
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_MANAGEMENT);
            break;
        
        case DLGRET_OK:
            switch(pMe->m_wPosCmd)
            {
                case IDS_POSITION_PHONE:
                    nData = CONTCFG_VIEWTYPE_PHONE;
                    break;
                    
                case IDS_UIMCARD:
                    nData = CONTCFG_VIEWTYPE_RUIM;
                    break;
                    
                case IDS_POSITION_ALL:
                default:
                    nData = CONTCFG_VIEWTYPE_ALL;
                    break;
            }
            
            // update the current data
            pMe->m_nViewType = nData;
            
            // ReLoad contacts
            if(SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            // Save config
            (void)CContApp_SetConfig( pMe, 
                                      CONTCFG_VIEWTYPE,
                                     &nData,
                                      sizeof(nData));
                                      
            pMe->m_eSuccessRetState = STATE_MANAGEMENT;
            MOVE_TO_STATE(STATE_SUCCESS);
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_VIEWTYPE
#endif// FEATURE_RUIM_PHONEBOOK

/*==============================================================================
������
       Handler_STATE_CAPACITY
˵����
       STATE_CAPACITY ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_CAPACITY(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_CAPACITY %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_CAPACITY))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
        
        //Proccess yourself dialog retrn value here
        case DLGRET_OK:
//#ifdef FEATURE_CARRIER_THAILAND_HUTCH                
 //           MOVE_TO_STATE(STATE_MANAGEMENT);
//#else
            MOVE_TO_STATE(STATE_MANAGEMENT);
//#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_CAPACITY

/*==============================================================================
������
       Handler_STATE_ONEDIAL
˵����
       STATE_ONEDIAL ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_ONEDIAL(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_ONEDIAL %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_ONEDIAL))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
        }
        
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            CContApp_Phonebook_Load(pMe);
            
            MOVE_TO_STATE(STATE_MAINMENU);
            break;
        case DLGRET_SET:          
        case DLGRET_OK:
            if( TRUE == CContApp_OnedialExit(pMe) && pMe->m_eDlgRet == DLGRET_OK)
            {
                MOVE_TO_STATE(STATE_ONEDIAL_OPT);
            }
            else
            {
                // ��¼�Ƿ�Ϊ��
                if(IS_ZERO_REC())
                {
                    // Show dialog message
                    pMe->m_eMsgType = MESSAGE_WARNNING; 
                    if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_EMPTY))
                    {
                        MOVE_TO_STATE(STATE_EXIT);
                        return NFSMACTION_CONTINUE;
                    }
                    return NFSMACTION_WAIT;
                }
    
                // Load record, only list the RUIM record
                if(SUCCESS != CContApp_LoadPhoneBook(pMe, CONTCFG_VIEWTYPE_ALL))
                {
                    pMe->m_wErrStrID = IDS_ERR_LOAD;
                    MOVE_TO_STATE(STATE_ERROR);
                    return NFSMACTION_CONTINUE;
                }
                
                pMe->m_wMainListSel = MENU_SELECT_NULL;
                pMe->m_nCurrIdx    = 0;

                MOVE_TO_STATE(STATE_ONEDIAL_SET);

            }
            break;
            
        case DLGRET_MSGBOX_OK:
            // reload contacts
            if(SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            MOVE_TO_STATE(STATE_ONEDIAL);
            break;

        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_ONEDIAL

/*==============================================================================
������
       Handler_STATE_ONEDIAL_OPT
˵����
       STATE_ONEDIAL_OPT ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_ONEDIAL_OPT(CContApp *pMe)
{
    //AECHAR nOneDial[MAX_INPUT_NUM + 1];
    ContAppCFGCache *pCFGCache;
    
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_ONEDIAL_OPT %d",pMe->m_eDlgRet));

    pCFGCache = &pMe->m_sCFGCache;  

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            //IMenuCtl  *pMenuCtl;
            
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_ONEDIALOPT))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
        }
        
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_ONEDIAL);
            break;
            
        case DLGRET_SET:
            // ��¼�Ƿ�Ϊ��
            if(IS_ZERO_REC())
            {
                // Show dialog message
                pMe->m_eMsgType = MESSAGE_WARNNING; 
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_EMPTY))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }

            // Load record, only list the RUIM record
            if(SUCCESS != CContApp_LoadPhoneBook(pMe, CONTCFG_VIEWTYPE_ALL))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            pMe->m_wMainListSel = MENU_SELECT_NULL;
            pMe->m_nCurrIdx    = 0;
            
            MOVE_TO_STATE(STATE_ONEDIAL_SET);
            break;
            
        case DLGRET_CLR:
            // ��ʾȷ�϶Ի���
            if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                                    IDS_YESNO_CLRONEDIAL,
                                                    NULL,
                                                    -1))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
        case DLGRET_YES:
            // Clear the onedial config
            pMe->m_pOneDialBuf[0] = ONEDIAL_NULL;
            MSG_FATAL("pMe->m_pOneDialBuf[0] = ONEDIAL_NULL",0,0,0);
            (void)CContApp_SetConfig( pMe,
                                      (ContAppCFG)pMe->m_wSelectOneDial,
                                      pMe->m_pOneDialBuf,
                                      sizeof(pMe->m_pOneDialBuf));
                                      
            pMe->m_eSuccessRetState = STATE_ONEDIAL;
            MOVE_TO_STATE(STATE_SUCCESS);
            break;
        
        case DLGRET_NO:
            MOVE_TO_STATE(STATE_ONEDIAL);
            break;
        
        case DLGRET_MSGBOX_OK:
            // reload contacts
            if(SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            MOVE_TO_STATE(STATE_ONEDIAL);
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_ONEDIAL_OPT

/*==============================================================================
������
       Handler_STATE_ONEDIAL_SET
˵����
       STATE_ONEDIAL_SET ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_ONEDIAL_SET(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_ONEDIAL_SET %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // Restore the menu select
            POP_LISTMENU_SEL(pMe->m_wMainListSel);
            
            // Restore the menu index
            POP_LISTMENU_IDX(pMe->m_wMainListIdx);

            // Show dialog message
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_LIST))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_CALL:
            // store the menu select
            PUSH_LISTMENU_SEL(pMe->m_wMainListSel);
            
            // store the menu index
            PUSH_LISTMENU_IDX(pMe->m_wMainListIdx);

#ifdef FEATURE_SAVE_ONLY_NAME                    
            if(0 == CContApp_GetCurrFldNum(pMe, AEE_ADDRFIELD_PHONE_GENERIC))
            {
                // Show dialog message
                pMe->m_eMsgType = MESSAGE_WARNNING;
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NO_NUMBER_TO_MAKECALL))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
#endif

            // Reset select field
            pMe->m_wSelectFld = 0;
            pMe->m_wFiledOptState =  FIELDCALL;
            pMe->m_eOptsRetState = STATE_ONEDIAL_SET;
            pMe->m_eListFrom = LISTFROMONEDIAL;
            
            MOVE_TO_STATE(STATE_VIEW);
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OK:
            // store the menu select
            PUSH_LISTMENU_SEL(pMe->m_wOneDialStatSel);
            
            // store the menu index
            PUSH_LISTMENU_IDX(pMe->m_wOneDialStatIdx);
            
            /*// Save config
            (void)STRTOWSTR("123456", pMe->m_pOneDialBuf, sizeof(pMe->m_pOneDialBuf));
            ERR("CContApp_SetConfig",0,0,0);
            (void)CContApp_SetConfig( pMe,
                                      (ContAppCFG)pMe->m_wSelectOneDial,
                                      pMe->m_pOneDialBuf,
                                      sizeof(pMe->m_pOneDialBuf));
            
            // reload contacts
            if(SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            pMe->m_eSuccessRetState = STATE_ONEDIAL;
            MOVE_TO_STATE(STATE_SUCCESS);*/
            
            // Load the current fields
            if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                 pMe->m_wSelectCont,
                                                 SINGLE_SELECT_NUMBER))
            {
                pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }

#ifdef FEATURE_SAVE_ONLY_NAME                    
            if(0 == CContApp_GetCurrFldNum(pMe, AEE_ADDRFIELD_PHONE_GENERIC))
            {
                // Show dialog message
                pMe->m_eMsgType = MESSAGE_WARNNING;
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NO_NUMBER_TO_MAKECALL))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
#endif

            if(1 == CContApp_GetCurrFldNum(pMe, AEE_ADDRFIELD_PHONE_GENERIC))
            {
                //(void)CContApp_MakeCall(pMe, IDX_NUM_RUIM_FLD);
                AEEAddrField *pFld;
                
                pFld = IVector_ElementAt( pMe->m_pFldIv, (uint32)IDX_NUM_RUIM_FLD );
                if(WSTRLEN(pFld->pBuffer) <= 3)
                {
                    // Show dialog message
                    pMe->m_eMsgType = MESSAGE_WARNNING;
                    if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_MSG_SHORTNUMBER))
                    {
                        MOVE_TO_STATE(STATE_EXIT);
                        return NFSMACTION_CONTINUE;
                    }
                    return NFSMACTION_WAIT;
                }
                
                (void)CContApp_SetConfig( pMe,
                                          (ContAppCFG)pMe->m_wSelectOneDial,
                                          (AECHAR *)pFld->pBuffer,
                                          sizeof((AECHAR *)pFld->pBuffer));
                
                pMe->m_eSuccessRetState = STATE_ONEDIAL;
                MOVE_TO_STATE(STATE_SUCCESS);
            }
            else
            {
                MOVE_TO_STATE(STATE_ONEDIAL_SET_NUMFLDSEL);
            }
                
            return NFSMACTION_CONTINUE;
        
        case DLGRET_CANCELED:
            // reset the menu select
            pMe->m_wOneDialStatSel = 0;
            
            // reset the menu index
            pMe->m_wOneDialStatIdx = 0;
            
            // reload contacts
            if(SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            MOVE_TO_STATE(STATE_ONEDIAL);
            return NFSMACTION_CONTINUE;


        case DLGRET_ERR:
            // Show dialog message
            pMe->m_wErrStrID = IDS_ERR_ADDFIELD;
            MOVE_TO_STATE(STATE_ERROR);
            break;

        case DLGRET_MSGBOX_OK:
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_ONEDIAL_SET


/*==============================================================================
������
       Handler_STATE_ONEDIAL_SET_NUMFLDSEL
˵����
       STATE_ONEDIAL_SET_NUMFLDSEL ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_ONEDIAL_SET_NUMFLDSEL(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_ONEDIAL_SET_NUMFLDSEL %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                 pMe->m_wSelectCont,
                                                 SINGLE_SELECT_NUMBER))
            {
                pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_ONEDIAL_SET_NUM_SEL))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
        case DLGRET_ERR:
            pMe->m_wMsgResID = IDS_MSG_SHORTNUMBER;
            pMe->m_eMsgRetState = STATE_ONEDIAL_SET_NUMFLDSEL;
            MOVE_TO_STATE(STATE_MSG);
            return NFSMACTION_CONTINUE;
        
        //Proccess yourself dialog retrn value here
        case DLGRET_CALL:
        case DLGRET_OK:
            // store the menu select
            //PUSH_LISTMENU_SEL(pMe->m_wOneDialStatSel);
            
            // store the menu index
            //PUSH_LISTMENU_IDX(pMe->m_wOneDialStatIdx);
            
            // Save config
            //(void)WSTRCPY(pMe->m_pFldInputBuf, pMe->m_pOneDialBuf);
            DBGPRINTF("pMe->m_pOneDial = %s",pMe->m_pOneDial,0,0);
            (void)CContApp_SetConfig( pMe,
                                      (ContAppCFG)pMe->m_wSelectOneDial,
                                      pMe->m_pOneDial,
                                      sizeof(pMe->m_pOneDial));
            
            // reload contacts
            if(SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            pMe->m_eSuccessRetState = STATE_ONEDIAL;
            MOVE_TO_STATE(STATE_SUCCESS);
            return NFSMACTION_CONTINUE;
        
        case DLGRET_CANCELED:
            // reset the menu select
            pMe->m_wOneDialStatSel = 0;
            
            // reset the menu index
            pMe->m_wOneDialStatIdx = 0;
            
            // reload contacts
            if(SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            MOVE_TO_STATE(STATE_ONEDIAL_SET);
            return NFSMACTION_CONTINUE;
        
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_ONEDIAL_SET_NUMFLDSEL


/*==============================================================================
������
       Handler_STATE_SAVEFLD
˵����
       STATE_SAVEFLD ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_SAVEFLD(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_SAVEFLD %d", pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            if(pMe->m_eFldSaveType == SAVE_TO_RUIM)
            {
#ifdef FEATURE_RUIM_PHONEBOOK
                // Check RUIM capacity
                if(IS_MAX_REC(pMe->m_pAddrRUIM))
                {
                    pMe->m_eMsgRetState = STATE_EXIT;
                    pMe->m_wMsgResID       = IDS_MSG_MAX_RUIM;
                    MOVE_TO_STATE(STATE_MSG);
                    FREEIF(pMe->m_pAddNewName);
                    FREEIF(pMe->m_pAddNewMobile);
                    return NFSMACTION_CONTINUE;
                }
#endif// FEATURE_RUIM_PHONEBOOK
            }
            else if(pMe->m_eFldSaveType == SAVE_NEW)
            {
                // Check Phone capacity
                if(IS_MAX_REC(pMe->m_pAddrPhone))
                {
                    pMe->m_eMsgRetState = STATE_EXIT;
                    pMe->m_wMsgResID       = IDS_MSG_MAX_PHONE;
                    MOVE_TO_STATE(STATE_MSG);
                    FREEIF(pMe->m_pAddNewName);
                    FREEIF(pMe->m_pAddNewMobile);
                    FREEIF(pMe->m_pAddNewHome);
                    FREEIF(pMe->m_pAddNewOffice);
                    FREEIF(pMe->m_pAddNewFax);
                    FREEIF(pMe->m_pAddNewEMAIL);
                    FREEIF(pMe->m_pAddNewAddress);
                    FREEIF(pMe->m_pAddNewURL);
                    FREEIF(pMe->m_pAddNewRemark);
                    
                    return NFSMACTION_CONTINUE;
                }
            }
            else
            {
                MOVE_TO_STATE(STATE_SAVEFLD_LIST);
                return NFSMACTION_CONTINUE;
            }
            
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_ADDNEW))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
        }
        
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_EXIT);
            break;
            
        case DLGRET_OK:
        {
            boolean bCard = (pMe->m_nSaveType == CONTCFG_SAVETYPE_RUIM);
            
            if(SUCCESS == CContApp_CreateCont( pMe, bCard))
            {
                FREEIF(pMe->m_pAddNewName);                
                FREEIF(pMe->m_pAddNewMobile);
                if (!bCard)
                {
                    FREEIF(pMe->m_pAddNewHome);
                    FREEIF(pMe->m_pAddNewOffice);
                    FREEIF(pMe->m_pAddNewFax);
                    FREEIF(pMe->m_pAddNewEMAIL);
                    FREEIF(pMe->m_pAddNewAddress);
                    FREEIF(pMe->m_pAddNewURL);
                    FREEIF(pMe->m_pAddNewRemark);
                }
                pMe->m_eSuccessRetState = STATE_ADDNEW;
                MOVE_TO_STATE(STATE_SUCCESS);
            }
            else
            {
                pMe->m_wErrStrID = IDS_ERR_CREATEREC;
                MOVE_TO_STATE(STATE_ERROR);
            }
            break;
        }

        case DLGRET_EDIT:                     
            FREEIF(pMe->m_pFldInputBuf);
            switch(pMe->m_nFldInputID)
            {
                case AEE_ADDRFIELD_NAME:
                     pMe->m_pFldInputBuf = WSTRDUP(pMe->m_pAddNewName);
                     break;
                     
                case AEE_ADDRFIELD_PHONE_GENERIC:
                     pMe->m_pFldInputBuf = WSTRDUP(pMe->m_pAddNewMobile);
                     break;
                     
                case AEE_ADDRFIELD_PHONE_HOME:
                     pMe->m_pFldInputBuf = WSTRDUP(pMe->m_pAddNewHome);
                     break;
                     
                case AEE_ADDRFIELD_PHONE_WORK:
                     pMe->m_pFldInputBuf = WSTRDUP(pMe->m_pAddNewOffice);
                     break;

                case AEE_ADDRFIELD_PHONE_FAX:
                     pMe->m_pFldInputBuf = WSTRDUP(pMe->m_pAddNewFax);
                     break;
                     
                case AEE_ADDRFIELD_EMAIL:
                     pMe->m_pFldInputBuf = WSTRDUP(pMe->m_pAddNewEMAIL);
                     break;
                     
                case AEE_ADDRFIELD_ADDRESS:
                     pMe->m_pFldInputBuf = WSTRDUP(pMe->m_pAddNewAddress);
                     break;
                     
                case AEE_ADDRFIELD_URL:
                     pMe->m_pFldInputBuf = WSTRDUP(pMe->m_pAddNewURL);
                     break;
                     
                case AEE_ADDRFIELD_NOTES:
                     pMe->m_pFldInputBuf = WSTRDUP(pMe->m_pAddNewRemark);
                     break;
                     
                default:
                    break;
            }

            MOVE_TO_STATE(STATE_ADDNEW_INPUT);
            break;        
        
        case DLGRET_INPUT_NULL:
            pMe->m_eMsgType = MESSAGE_WARNNING; 
            (void)CContApp_ShowMsgBox(pMe, IDS_MSG_INPUTNULL);
            return NFSMACTION_WAIT;
    /*        
        case DLGRET_SAME_REC:
            if(pMe->m_eFldSaveType == SAVE_TO_RUIM)
            {
                (void)CContApp_ShowMsgBox(pMe, IDS_MSG_SAMEREC);
            }
            else
            {
                (void)CContApp_ShowMsgBox(pMe, IDS_MSG_SAMENAME);
            }
            return NFSMACTION_WAIT;
        */
        case DLGRET_NUM_UNAVAILD:
            pMe->m_eMsgType = MESSAGE_WARNNING; 
            (void)CContApp_ShowMsgBox(pMe, IDS_MSG_NUM_UNAVAILD);
            return NFSMACTION_WAIT;
            
        case DLGRET_EMAIL_UNAVAILD:
            pMe->m_eMsgType = MESSAGE_WARNNING; 
            if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_MSG_EMAIL_UNAVAILD))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;            
        
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_SAVEFLD);
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_SAVEFLD

/*==============================================================================
������
       Handler_STATE_SAVEFLD_LIST
˵����
       STATE_SAVEFLD_LIST ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_SAVEFLD_LIST(CContApp *pMe)
{
    int nRet;
    
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_SAVEFLD_LIST %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
                // Load contacts
            if(SUCCESS != CContApp_LoadPhoneBook(pMe, CONTCFG_VIEWTYPE_PHONE))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            // ��¼�Ƿ�Ϊ��
            if(IS_ZERO_REC())
            {
                pMe->m_wMsgResID = IDS_MSG_NORECORD;
                pMe->m_eMsgRetState = STATE_EXIT;
                MOVE_TO_STATE(STATE_MSG);
                return NFSMACTION_CONTINUE;
            }
            
            // Show dialog message
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_LIST))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_CALL:
        case DLGRET_OK:
            nRet = CContApp_MergeField( pMe, 
                                        pMe->m_wSelectCont, 
                                        pMe->m_pAddNewMobile,
                                        pMe->m_nAddNewFldID);
            if(SUCCESS == nRet)
            {
                FREEIF(pMe->m_pAddNewMobile);
                pMe->m_eSuccessRetState = STATE_EXIT;
                MOVE_TO_STATE(STATE_SUCCESS);
            }
            else if(EMAXFIELD == nRet)
            {
                pMe->m_wErrStrID = IDS_MSG_MAXFIELDS;
                MOVE_TO_STATE(STATE_ERROR);
            }
            else if(EEXIST == nRet)
            {
                pMe->m_wErrStrID = IDS_MSG_EXIST;
                MOVE_TO_STATE(STATE_ERROR);
            }
            else
            {
                pMe->m_wErrStrID = IDS_ERR_ADDFIELD;
                MOVE_TO_STATE(STATE_ERROR);
            }
            return NFSMACTION_CONTINUE;
        
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_EXIT);
            return NFSMACTION_CONTINUE;
        
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_SAVEFLD_LIST

/*==============================================================================
������
       Handler_STATE_SELECT
˵����
       STATE_SELECTFLD ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_SELECT(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_SELECTFLD %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // ��¼�Ƿ�Ϊ��
            if(IS_ZERO_REC())
            {
                pMe->m_wMsgResID = IDS_MSG_NORECORD;
                pMe->m_eMsgRetState = STATE_EXIT;
                MOVE_TO_STATE(STATE_MSG);
                return NFSMACTION_CONTINUE;
            }

            pMe->m_pSmartBufLen = 0;//option �˵� �ĵ�����Ҫ m_pSmartBufLen Ϊ����
            // Show dialog
            //pMe->m_wFieldIndex = MASK_ADDRFIELD_PHONE_GENERIC;

            POP_LISTMENU_SEL(pMe->m_wMainListSel);
            
            // Restore the menu index
            POP_LISTMENU_IDX(pMe->m_wMainListIdx);
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_SELECT))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_OK:
        {   
#if defined(FEATURE_WMS_APP)
            if(pMe->m_wSelFldType != MULTI_SELECT_CONTENT)
            {
                CContApp_GetSendListLeftNodeNum(pMe, &pMe->m_nleftNum);

                if(pMe->m_eTotalSelectNum > pMe->m_nleftNum)
                {
                    pMe->m_eMsgType = MESSAGE_WARNNING; 
                    if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_SENTLIST_FULL))
                    {
                        MOVE_TO_STATE(STATE_EXIT);
                        return NFSMACTION_CONTINUE;
                    }
                    return NFSMACTION_WAIT;
                }
            }
#endif
            if((pSelectFieldListRoot == pCurSelectFieldNode)&&(pCurSelectFieldNode == NULL))
            {
                pMe->m_eMsgType = MESSAGE_WARNNING; 
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NO_SELECTION))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
            else
            {
                MOVE_TO_STATE(STATE_SELECT_RETURN);
                return NFSMACTION_CONTINUE;
            }
            break;
        }

        case DLGRET_POPNUMFLD :
            MOVE_TO_STATE(STATE_POPNUMFLD);
            break;

        case DLGRET_ERR :
            pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
            MOVE_TO_STATE(STATE_ERROR);       
            break;

        case DLGRET_SELECT:
            MOVE_TO_STATE(STATE_SELECTOPT);
            break;
        
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_EXIT);
            return NFSMACTION_CONTINUE;

        case DLGRET_MSGBOX_OK:
            break;

        case DLGRET_SENDLISTFULL:
            pMe->m_eMsgType = MESSAGE_WARNNING; 
            if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_SENTLIST_FULL))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;

        // for insert content into wms
        case DLGRET_SELECT_FLD: 
            MOVE_TO_STATE(STATE_DETAIL_MULTI);
            return NFSMACTION_CONTINUE;

        // insert content into wms prompt if no selection
        case DLGRET_NO_SELECTION:
            pMe->m_eMsgType = MESSAGE_WARNNING; 
            if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NO_SELECTION))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_SELECT

/*==============================================================================
������
       Handler_STATE_SELECTFLD_VIEW
˵����
       STATE_SELECTFLD_VIEW ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_SELECTFLD_VIEW(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_SELECTFLD_VIEW %d", pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                 pMe->m_wSelectCont,
                                                 pMe->m_wSelFldType))
            {
                pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_VIEW))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_SELECT);
            break;
        
        case DLGRET_OK:
            MOVE_TO_STATE(STATE_SELECT_RETURN);
            break;
            
        case DLGRET_ERR:
            pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
            MOVE_TO_STATE(STATE_ERROR);
            break;
        
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_SELECTFLD_VIEW

/*==============================================================================
������
       Handler_STATE_SELECT_RETURN
˵����
       STATE_SELECT_RETURN ״̬������
              
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_SELECT_RETURN(CContApp *pMe)
{
    boolean       bRet = TRUE;
    
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_SELECT_RETURN %d", pMe->m_wSelectFld));

    CContApp_GetSelectFieldListNode(pMe, pSelectFieldListRoot);

    if (pMe->m_wSelFldType == MULTI_SELECT_CONTENT)
    {
        CContApp_FormatSelectFieldContentListNode(pMe, pSelectFieldListRoot);
    }
    
    if(pMe->m_pSelFldCB)
    {
        bRet = pMe->m_pSelFldCB(pSelectFieldListRoot);
    }

    FREEIF(pSelectFieldListRoot->SelectFieldInfo.m_pAddNewFormatContent);
    
    if(bRet)
    {
        MOVE_TO_STATE(STATE_EXIT);
    }
    else
    {
        pMe->m_wErrStrID = IDS_ERR_UNKNOWN;
        MOVE_TO_STATE(STATE_ERROR);
    }
    return NFSMACTION_CONTINUE;
} // Handler_STATE_SELECT_RETURN

/*==============================================================================
������
       Handler_STATE_FINDNUM
˵����
       STATE_FINDNUM ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_FINDNUM(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_FINDNUM %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // ��¼�Ƿ�Ϊ��
            if(IS_ZERO_REC())
            {
                pMe->m_wMsgResID = IDS_MSG_NORECORD;
                pMe->m_eMsgRetState = STATE_EXIT;
                MOVE_TO_STATE(STATE_MSG);
                return NFSMACTION_CONTINUE;
            }
            
            // Show dialog message
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_LIST))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_OK:
            MOVE_TO_STATE(STATE_FINDNUM_OPTS);
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CALL:
            // Load the current fields
            if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                 pMe->m_wSelectCont,
                                                 SINGLE_SELECT_NUMBER))
            {
                pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            // �����ǰ��¼����һ��������ֱ�ӷ������
            if(1 == CContApp_GetCurrFldNum(pMe, AEE_ADDRFIELD_PHONE_GENERIC))
            {
                (void)CContApp_MakeCall(pMe, IDX_NUM_RUIM_FLD);
            }
            else
            {
                // Reset select field
                pMe->m_wSelectFld = 0;
                pMe->m_eOptsRetState = STATE_FINDNUM;
                
                MOVE_TO_STATE(STATE_VIEW);
            }
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_EXIT);
            return NFSMACTION_CONTINUE;


        case DLGRET_ERR:
            // Show dialog message
            pMe->m_wErrStrID = IDS_ERR_ADDFIELD;
            MOVE_TO_STATE(STATE_ERROR);
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_FIND_LIST

/*==============================================================================
������
       Handler_STATE_FINDNUM_OPTS
˵����
       STATE_FINDNUM_OPTS ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_FINDNUM_OPTS(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_FINDNUM_OPTS %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            IMenuCtl  *pMenuCtl;
            
            // show options dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_OPTS))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                                      IDC_OPTS_MENU);
            
            // build options menu
            (void)CContApp_BuildOptsMenu(pMe, pMenuCtl, TRUE);
            
            return NFSMACTION_WAIT;
        }
        
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_FINDNUM);
            break;
            
        case DLGRET_VIEW:
            // Reset select field
            pMe->m_wSelectFld = 0;
            pMe->m_eOptsRetState = STATE_FINDNUM_OPTS;
            MOVE_TO_STATE(STATE_VIEW);
            break;
            
        case DLGRET_DELETE:
            pMe->m_eOptsRetState = STATE_FINDNUM_OPTS;
            MOVE_TO_STATE(STATE_DELETE);
            break;

        case DLGRET_DETAIL:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            pMe->m_eOptsRetState = STATE_FINDNUM_OPTS;
            MOVE_TO_STATE(STATE_DETAIL);
            break;
            
        case DLGRET_EDIT:
            // ���õ�ǰ�༭�ļ�¼Ϊ��ǰѡ�е�
            pMe->m_wEditCont = pMe->m_wSelectCont;
            pMe->m_eOptsRetState = STATE_FINDNUM_OPTS;
            MOVE_TO_STATE(STATE_EDIT);
            break;

#ifdef FEATURE_RUIM_PHONEBOOK
        case DLGRET_COPY:
            pMe->m_eOptsRetState = STATE_FINDNUM_OPTS;
            
            MOVE_TO_STATE(STATE_COPY);
            break;
#endif// FEATURE_RUIM_PHONEBOOK

        case DLGRET_SEARCHOTHERDIRECTORY:
            
            MOVE_TO_STATE(STATE_MAINLIST);
            break;
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_FINDNUM_OPTS

/*==============================================================================
������
       Handler_STATE_SMARTCALL
˵����
       STATE_SMARTCALL ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_SMARTCALL(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_SMARTCALL %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // ��¼�Ƿ�Ϊ��
            if(IS_ZERO_REC())
            {
                pMe->m_wErrStrID = IDS_MSG_FINDNULL;
                MOVE_TO_STATE(STATE_ERROR);
                break;
            }
            
            // Restore the menu select
            POP_LISTMENU_SEL(pMe->m_wMainListSel);
            
            // Restore the menu index
            POP_LISTMENU_IDX(pMe->m_wMainListIdx);
            
            // Show dialog message
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_LIST))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_OK:
        case DLGRET_CALL:
            // store the menu select
            PUSH_LISTMENU_SEL(pMe->m_wMainListSel);
            
            // store the menu index
            PUSH_LISTMENU_IDX(pMe->m_wMainListIdx);
            
            // Load the current fields
            if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                 pMe->m_wSelectCont,
                                                 SINGLE_SELECT_NUMBER))
            {
                pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            // �����ǰ��¼����һ��������ֱ�ӷ������
            if(1 == CContApp_GetCurrFldNum(pMe, AEE_ADDRFIELD_PHONE_GENERIC))
            {
                (void)CContApp_MakeCall(pMe, IDX_NUM_RUIM_FLD);
            }
            else
            {
                // Reset select field
                pMe->m_wSelectFld = 0;
                
                MOVE_TO_STATE(STATE_SMARTCALL_VIEW);
            }
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_EXIT);
            return NFSMACTION_CONTINUE;
        
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_SMARTCALL

/*==============================================================================
������
       Handler_STATE_SMARTCALL_VIEW
˵����
       STATE_SMARTCALL_VIEW ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_SMARTCALL_VIEW(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_SMARTCALL_VIEW %d", pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_VIEW))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_EDIT:
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_SMARTCALL);
            break;
            
        case DLGRET_OK:
            (void)CContApp_MakeCall(pMe, pMe->m_wSelectFld);
            break;
            
        case DLGRET_ERR:
            pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
            MOVE_TO_STATE(STATE_ERROR);
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_VIEW

// ״̬ STATE_SEARCH ������
static NextFSMAction Handler_STATE_SEARCH(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("Handler_STATE_SEARCH  %d", pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            IMenuCtl  *pMenuCtl;
            
            // show options dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_SEARCH))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                                      IDC_SEARCH_MENU);
            
            // build search menu
            (void)CContApp_BuildSearchMenu(pMe, pMenuCtl);
            
            // Redraw the menu for fix the bug of display
            IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl,0));
            
            // restore the menu select
            POP_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            return NFSMACTION_WAIT;
        }
            
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:

            MOVE_TO_STATE(STATE_MAINMENU);
            break;

        case DLGRET_SEARCH_NAME:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            
            MOVE_TO_STATE(STATE_SEARCH_NAME);
              break;
        
        case DLGRET_SEARCH_GROUP:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);

            MOVE_TO_STATE(STATE_GROUPVIEW);
            
            //MOVE_TO_STATE(STATE_SEARCH_GROUP);
            break;

        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
}

/*==============================================================================
������
       Handler_STATE_SEARCHNAME
˵����
       STATE_SEARCHNAME ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_SEARCHNAME(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_SEARCHNAME %d", pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_SEARCHNAME))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_MAINMENU);
            break;
        
        case DLGRET_OK:
            if(SUCCESS != CContApp_LoadByName(pMe, pMe->m_sSearchStr))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
                        
            // ��¼�Ƿ�Ϊ��
            if(IS_ZERO_REC())
            {
                // Show dialog message
                pMe->m_eMsgType = MESSAGE_WARNNING; 
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_EMPTY))
                {
                    MOVE_TO_STATE(STATE_SEARCH_NAME);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
            
            pMe->m_wMainListSel = MENU_SELECT_NULL;
            pMe->m_nCurrIdx    = 0;
            
            // Go to list menu
            FREEIF(pMe->m_szAlpha);
            MOVE_TO_STATE(STATE_FIND_LIST);
            break;
            
        case DLGRET_MSGBOX_OK:
            // ReLoad contacts
            if(SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            MOVE_TO_STATE(STATE_SEARCH_NAME);
            break;
            /*
        case DLGRET_INPUT_NULL:
            // field can not be NULL
            if(SUCCESS != CContApp_ShowMsgBox(pMe,IDS_MSG_INPUTNULL))
            {
                MOVE_TO_STATE(STATE_SEARCH_NAME);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            */
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_SEARCHNAME

/*==============================================================================
������
       Handler_STATE_SUCCESS
˵����
       STATE_SUCCESS ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_SUCCESS(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_SUCCESS %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            
            // Show dialog
            pMe->m_eMsgType = MESSAGE_INFORMATIVE; 
            if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_SUCCESS))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_MSGBOX_OK:
#ifdef FEATURE_GIVEUP_EDIT
            if(TRUE == pMe->m_bIsEndKey)
            {
                MOVE_TO_STATE(STATE_EXIT);
            }
            else
#endif
            {
                MOVE_TO_STATE(pMe->m_eSuccessRetState);
            }
            return NFSMACTION_CONTINUE;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_SUCCESS

/*==============================================================================
������
       Handler_STATE_MSG
˵����
       STATE_MSG ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_MSG(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_MSG %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // Show dialog
            pMe->m_eMsgType = MESSAGE_WARNNING; 
            if(SUCCESS != CContApp_ShowMsgBox(pMe, pMe->m_wMsgResID))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_MSGBOX_OK:
#ifdef FEATURE_GIVEUP_EDIT
            if(TRUE == pMe->m_bIsEndKey)
            {
                MOVE_TO_STATE(STATE_EXIT);
            }
            else
#endif
            {
                MOVE_TO_STATE(pMe->m_eMsgRetState);
            }   
            return NFSMACTION_CONTINUE;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_MSG

/*==============================================================================
������
       Handler_STATE_ERROR
˵����
       STATE_ERROR ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_ERROR(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_ERROR %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // Show dialog
            pMe->m_eMsgType = MESSAGE_ERR; 
            if(SUCCESS != CContApp_ShowMsgBox(pMe, pMe->m_wErrStrID))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_MSGBOX_OK:
#ifdef FEATURE_GIVEUP_EDIT
            if(TRUE == pMe->m_bIsEndKey)
            {
                MOVE_TO_STATE(STATE_EXIT);
            }
            else
#endif
            {
                MOVE_TO_STATE(STATE_EXIT);
            }
            return NFSMACTION_CONTINUE;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_ERROR

/*==============================================================================
������
       Handler_STATE_EXIT
˵����
       STATE_EXIT ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_EXIT(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_EXIT"));

#ifdef FEATURE_GIVEUP_EDIT
    if(TRUE == pMe->m_bIsEndKey)
    {
        (void) ISHELL_CloseApplet(pMe->m_pShell,TRUE);
    }
    else
#endif
    {
        (void) ISHELL_CloseApplet(pMe->m_pShell,FALSE);
    }
    return NFSMACTION_WAIT;
} // Handler_STATE_EXIT

/*==============================================================================
������
       Handler_STATE_DETAIL
˵����
       STATE_DETAIL ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_DETAIL(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_DETAIL %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                 pMe->m_wSelectCont,
                                                 SELECT_ALL))
            {
                pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_DETAIL))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
        
        //Proccess yourself dialog retrn value here
        case DLGRET_OK:
            FREEIF(pMe->m_pAddNewName);
            MOVE_TO_STATE(pMe->m_eOptsRetState);
            break;

        case DLGRET_CANCELED:
            // reset the menu select
            pMe->m_wOptsStatSel = 0;
            
            if(IS_ZERO_REC())
            {
                MOVE_TO_STATE(STATE_EXIT);
            }
            else if(LISTFROMSEARCH == pMe->m_eListFrom)
            {
                MOVE_TO_STATE(STATE_FIND_LIST);
            }
            else if(LISTFROMGROUP== pMe->m_eListFrom)
            {
                MOVE_TO_STATE(STATE_GROUPVIEW_LIST);
            }
            else
            {
                MOVE_TO_STATE(STATE_MAINLIST);
            }
            CContApp_FreeBuffer(pMe);
            FREEIF(pMe->m_nRingToneID);
            return NFSMACTION_CONTINUE;
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_DETAIL

/*==============================================================================
������
       Handler_STATE_DETAIL_MULTI
˵����
       STATE_DETAIL_MULTI ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_DETAIL_MULTI(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_DETAIL_MULTI %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                 pMe->m_wSelectCont,
                                                 SELECT_ALL))
            {
                pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_DETAIL_MULTI))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
        
        //Proccess yourself dialog retrn value here
        case DLGRET_OK:
            MOVE_TO_STATE(STATE_SELECT);
            break;

        //wuquan.tang 20081009 add start
        case DLGRET_CANCELED:
            // reset the menu select
            MOVE_TO_STATE(STATE_SELECT);
            break;
        //wuquan.tang 20081009 add end
        
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_DETAIL_MULTI

/*==============================================================================
������
       Handler_STATE_GROUPOPT
˵����
       STATE_GROUPOPT ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_GROUPOPT(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_GROUPOPT %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog(pMe, IDD_GROUPOPT))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }        
            return NFSMACTION_WAIT;
            
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_GROUPVIEW);
            break;
            
        case DLGRET_VIEW:
            // Load record
            if(SUCCESS != CContApp_LoadByCat(pMe, pMe->m_nGroupCat))
            {
                pMe->m_wErrStrID = IDS_ERR_LOAD;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            
            // ��¼�Ƿ�Ϊ��
            if(IS_ZERO_REC())
            {
                // Show dialog message
                pMe->m_eMsgType = MESSAGE_WARNNING; 
                if(SUCCESS != CContApp_ShowMsgBox( pMe, IDS_EMPTY))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
            pMe->m_wSelectCont = CONTAPP_RECID_NULL;
            pMe->m_nCurrIdx    = 0;
            
            // Go to list the menu
            MOVE_TO_STATE(STATE_GROUPVIEW_LIST);            
            break;
            
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_GROUPVIEW);
            break;
            
        case DLGRET_EDIT:
            MOVE_TO_STATE(STATE_GROUPOPT_EDIT);   
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_COPYMOVE_COPY


/*==============================================================================
������
       Handler_STATE_GROUPOPT_EDIT
˵����
       STATE_GROUPOPT_EDIT ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_GROUPOPT_EDIT(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_GROUPOPT_EDIT %d",pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // Show dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_GROUPOPT_EDIT))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_GROUPVIEW);
            break;

        case DLGRET_OK:
            MOVE_TO_STATE(STATE_GROUPVIEW);        
            break;
        
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_GROUPOPT_EDIT);
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_EDIT_INPUT

// ״̬ STATE_MANAGEMENT ������
static NextFSMAction Handler_STATE_MANAGEMENT(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("Handler_STATE_MANAGEMENT  %d", pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            IMenuCtl  *pMenuCtl;
            
            // show options dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_MANAGEMENT))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                                      IDC_MENU_CONTACTS_MANAGEMENT);
            
            // Redraw the menu for fix the bug of display
            IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl,0));
            
            // restore the menu select
            POP_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            return NFSMACTION_WAIT;
        }
            
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:

            MOVE_TO_STATE(STATE_MAINMENU);
            break;

        case DLGRET_COPYORMOVE:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            /*if(IS_ZERO_REC())
            {
                pMe->m_eMsgRetState = STATE_MANAGEMENT;
                // Show dialog message
                pMe->m_eMsgType = MESSAGE_WARNNING; 
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_EMPTY))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }*/

            //MOVE_TO_STATE(STATE_COPY);
            MOVE_TO_STATE(STATE_COPYMOVE);

           break;

        case DLGRET_DELETE_SELECT:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);

            MOVE_TO_STATE(STATE_DELETE_SELECT);
            
            //MOVE_TO_STATE(STATE_SEARCH_GROUP);
            break;

        case DLGRET_SAVETO:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);

            MOVE_TO_STATE(STATE_SAVETO);
            break;
        case DLGRET_VIEWTYPE:
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            MOVE_TO_STATE(STATE_VIEWTYPE);
            break;

        case DLGRET_CHECKCAPACITY:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);

            MOVE_TO_STATE(STATE_CAPACITY);
            break;
            
#if defined(FEATURE_SUPPORT_BT_APP) && defined(FEATURE_SUPPORT_VCARD)
        case DLGRET_SEND_BY_BT:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);

            MOVE_TO_STATE(STATE_SELECT_RECORD);
            break;
#endif

        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(pMe->m_eMsgRetState);
            break;

        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
}

// ״̬ STATE_DELETE_SELECT ������
static NextFSMAction Handler_STATE_DELETE_SELECT(CContApp *pMe)
{
    //int16 totalSize = 0;
   // int16 wRecID;

#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("STATE_DELETE_SELECT  %d", pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            IMenuCtl  *pMenuCtl;
            
            // show options dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_DELETE_SELECT))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            pMenuCtl = (IMenuCtl*)IDIALOG_GetControl( pMe->m_pActiveDlg,
                                                      IDC_MENU_DELETE_SELECT);
             // Redraw the menu for fix the bug of display
            IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl,0));
            
            // restore the menu select
            POP_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            return NFSMACTION_WAIT;
        }
            
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:

            MOVE_TO_STATE(STATE_MANAGEMENT);
            break;

        case DLGRET_PHONE_MEMORY:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);

            if(IS_ZERO_REC())
            {
                // Show dialog message
                pMe->m_eMsgType = MESSAGE_WARNNING; 
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_EMPTY))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }            
            
            pMe->m_nDeleteType = DELETE_MULTIPE;
            pMe->m_eTotalSelectNum = 0;
            //pMe->m_wSelectSmart = MENU_SELECT_NULL;
            FREEIF(pMe->m_szAlpha);
            pMe->m_wMainListSel = MENU_SELECT_NULL;
            MOVE_TO_STATE(STATE_DELETE);
            break;

        case DLGRET_UIM_CARD:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);
            if(IS_ZERO_REC())
            {
                // Show dialog message
                pMe->m_eMsgType = MESSAGE_WARNNING; 
                if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_EMPTY))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }         

            pMe->m_nDeleteType = DELETE_MULTIPE;
            pMe->m_eTotalSelectNum = 0;
            //pMe->m_wSelectSmart = MENU_SELECT_NULL;
            FREEIF(pMe->m_szAlpha);
            pMe->m_wMainListSel = MENU_SELECT_NULL;
            MOVE_TO_STATE(STATE_DELETE);
            
            //MOVE_TO_STATE(STATE_SEARCH_GROUP);
            break;

        case DLGRET_DELETE_ALL:
            // Save the menu select
            PUSH_OPTSMENU_SEL(pMe->m_wOptsStatSel);

            pMe->m_nDeleteType = DELETE_ALL;
            
            if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                                    IDS_YESNO_DELETEREC,
                                                    NULL,
                                                    -1))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_WAIT;
            }
            //pMe->m_nDeleteAllCheck = TRUE;
            //MOVE_TO_STATE(STATE_LOCKED);
            return NFSMACTION_WAIT;

        case DLGRET_NO:
            //CContApp_Phonebook_Load(pMe);
            MOVE_TO_STATE(STATE_DELETE_SELECT);
            break;       

        case DLGRET_YES:
        {//�绰����oem����ˣ����ﻹû����Ҫ���޸�
            if(pMe->m_nDeleteType == DELETE_ALL)
            {
                pMe->m_eTotalSelectNum = 0;
                pMe->m_nDeleteAllCheck = TRUE;
                MOVE_TO_STATE(STATE_LOCKED);               
                break;
            }   
        }   

        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_DELETE_SELECT);
            break;

        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
}


/*==============================================================================
������
       Handler_STATE_POPNUMFLD
˵����
       STATE_POPNUMFLD ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_POPNUMFLD(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("Handler_STATE_POPNUMFLD  %d", pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            //IMenuCtl  *pMenuCtl;

            if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                 pMe->m_wSelectCont,
                                                 SELECT_ALL))
            {
                pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
                        
            // show options dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_POPNUMFLD))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
        }

        case DLGRET_OK :
            MOVE_TO_STATE(STATE_SELECT);
            break;
            
        case DLGRET_ERR :
            pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
            MOVE_TO_STATE(STATE_ERROR);
            break;
            
        case DLGRET_SENDLISTFULL:
            pMe->m_eMsgType = MESSAGE_WARNNING; 
            if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_SENTLIST_FULL))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;

        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_SELECT);
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
}
#if 0
/*==============================================================================
������
       Handler_STATE_VIEWNUMFLD
˵����
       STATE_POPNUMFLD ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_VIEWNUMFLD(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("Handler_STATE_POPNUMFLD  %d", pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            //IMenuCtl  *pMenuCtl;

            if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                 pMe->m_wSelectCont,
                                                 SELECT_ALL))
            {
                pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
                        
            // show options dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_VIEW_NUMFLD))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }

            return NFSMACTION_WAIT;
        }
            
        //Proccess yourself dialog retrn value here
        /*case DLGRET_CANCELED:

            MOVE_TO_STATE(STATE_MAINMENU);
            break;
        */    
        case DLGRET_OK :
            MOVE_TO_STATE(STATE_SELECT);
            break;

        case DLGRET_ERR :
            pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
            MOVE_TO_STATE(STATE_ERROR);
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
}
#endif
/*==============================================================================
������
       Handler_STATE_SELECTOPT
˵����
       STATE_SELECTOPT ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_SELECTOPT(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("Handler_STATE_SELECTOPT  %d", pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {            
            // show options dialog
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_SELECTOPT))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }

            return NFSMACTION_WAIT;
        }
            
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            MOVE_TO_STATE(pMe->m_ePreState);
            /*
            if(SMART_STATE_IDD_COPY == pMe->m_nSmartStateType)
            {
                MOVE_TO_STATE(STATE_COPY);
            }
            else if(SMART_STATE_IDD_DELETE== pMe->m_nSmartStateType)
            {
                MOVE_TO_STATE(STATE_DELETE);
            }
            else
            {
                MOVE_TO_STATE(STATE_SELECT);
            }*/
            break;

        case DLGRET_OK:
            {
                int16 resId = 0;
                
                if(SMART_STATE_IDD_COPY == pMe->m_nSmartStateType)
                {
                    if(COPYMULTIPE == pMe->m_nCopyOrMove)
                    {
                        resId = IDS_YESNO_COPYREC;
                    }
                    else
                    {
                        resId = IDS_YESNO_MOVEREC;
                    }
                }
                else if(SMART_STATE_IDD_DELETE == pMe->m_nSmartStateType)
                {
                    resId = IDS_YESNO_DELETEREC;
                }
                else if(SMART_STATE_IDD_SELECT == pMe->m_nSmartStateType)
                {
#if defined(FEATURE_WMS_APP)
                    if(pMe->m_wSelFldType != MULTI_SELECT_CONTENT)
                    {
                        CContApp_GetSendListLeftNodeNum(pMe, &pMe->m_nleftNum);
                        if(pMe->m_eTotalSelectNum > pMe->m_nleftNum)
                        {
                            pMe->m_eMsgType = MESSAGE_WARNNING; 
                            if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_SENTLIST_FULL))
                            {
                                MOVE_TO_STATE(STATE_EXIT);
                                return NFSMACTION_CONTINUE;
                            }
                            return NFSMACTION_WAIT;
                        }
                    }
#endif
                    MOVE_TO_STATE(STATE_SELECT_RETURN);
                    return NFSMACTION_CONTINUE;
                }

                if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                                        resId,
                                                        NULL,
                                                        -1))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                                 
                return NFSMACTION_WAIT;
            }
            break;

        case DLGRET_NO:
            if(SMART_STATE_IDD_DELETE == pMe->m_nSmartStateType)
            {
                //CContApp_FreeSelectFieldListNode();
                MOVE_TO_STATE(STATE_DELETE);
            }
            else if(SMART_STATE_IDD_COPY == pMe->m_nSmartStateType)
            {
                MOVE_TO_STATE(STATE_COPY);
            }
            else
            {
                MOVE_TO_STATE(STATE_ERROR);       
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_YES:
            if(SMART_STATE_IDD_DELETE == pMe->m_nSmartStateType)
            {
                //CContApp_FreeSelectFieldListNode();
                MOVE_TO_STATE(STATE_DELETING);
            }
            else if(SMART_STATE_IDD_COPY == pMe->m_nSmartStateType)
            {
                if(pMe->m_eTotalSelectNum == 1 && !CContApp_CheckNumLen(pMe, pCurSelectFieldNode->SelectFieldInfo.wIndex))
                {
                    // Show dialog message
                    pMe->m_eMsgType = MESSAGE_WARNNING; 
                    if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NUMBER_TOOLONG))
                    {
                        MOVE_TO_STATE(STATE_EXIT);
                        return NFSMACTION_CONTINUE;
                    }
                    return NFSMACTION_WAIT;
                }
                MOVE_TO_STATE(STATE_COPYING);
            }
            else
            {
                MOVE_TO_STATE(STATE_ERROR);       
            }
            return NFSMACTION_CONTINUE;
            
        case DLGRET_ERR :
            MOVE_TO_STATE(STATE_ERROR);       
            break;

        //more than one num field
        case DLGRET_POPNUMFLD :
            MOVE_TO_STATE(STATE_POPNUMFLD);
            break;

        //only one num field
        case DLGRET_MARKCHG :
            MOVE_TO_STATE(pMe->m_ePreState);
            break;
            
        case DLGRET_NO_SELECTION:
            pMe->m_eMsgType = MESSAGE_WARNNING; 
            if(SUCCESS != CContApp_ShowMsgBox(pMe, IDS_NO_SELECTION))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;

        // for insert content into wms
        case DLGRET_SELECT_FLD: 
            MOVE_TO_STATE(STATE_DETAIL_MULTI);
            return NFSMACTION_CONTINUE;

        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(pMe->m_ePreState);
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
}

/*==============================================================================
������
       Handler_STATE_NUMFLDVIEW
˵����
       STATE_NUMFLDVIEW ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_NUMFLDVIEW(CContApp *pMe)
{
    
    int nRet = SUCCESS;
    AEEAddrFieldID wFldID = {0};
    
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif

    FARF(STATE, ("Handler_STATE_NUMFLDVIEW %d", pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            if(SUCCESS != CContApp_LoadAddrFlds( pMe, 
                                                 pMe->m_wEditCont,
                                                 SELECT_ALL))
            {
                pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
                MOVE_TO_STATE(STATE_ERROR);
                return NFSMACTION_CONTINUE;
            }
            /*
            CContApp_FreeBuffer(pMe);
            CContApp_GetRecByID(pMe, pMe->m_wEditCont);    
            */
            if(SUCCESS != CContApp_ShowDialog( pMe, IDD_NUMFLDVIEW))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            //MOVE_TO_STATE(pMe->m_eOptsRetState);
            if(IS_ZERO_REC())
            {
                MOVE_TO_STATE(STATE_EXIT);
            }
            else
            {
                MOVE_TO_STATE(STATE_MAINLIST);
            }

            break;

        case DLGRET_OK:
        
            if((pMe->m_wFieldMask | pMe->m_wNewNumFldMask) ==  pMe->m_wFieldMask)
            {
                 if(SUCCESS != CContApp_ShowYesNoDialog( pMe, 
                                                        IDS_REPLACE,
                                                        NULL,
                                                        -1))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
            else
            {
                switch(pMe->m_wNewNumFldMask)
                {
                    case MASK_ADDRFIELD_PHONE_GENERIC:
                        wFldID = AEE_ADDRFIELD_PHONE_GENERIC;
                        break;

                    case MASK_ADDRFIELD_PHONE_HOME:
                        wFldID = AEE_ADDRFIELD_PHONE_HOME;
                        break;

                    case MASK_ADDRFIELD_PHONE_WORK:
                        wFldID = AEE_ADDRFIELD_PHONE_WORK;
                        break;

                    case MASK_ADDRFIELD_PHONE_FAX:
                        wFldID = AEE_ADDRFIELD_PHONE_FAX;
                        break;

                    default:
                        pMe->m_wErrStrID = IDS_ERR_UNKNOWN;
                        MOVE_TO_STATE(STATE_ERROR);
                        break;
                }
                
                nRet = CContApp_SaveEditField(pMe, wFldID);
        
                if( SUCCESS == nRet)
                {
                    FREEIF(pMe->m_pFldInputBuf);
                    pMe->m_eSuccessRetState = STATE_EXIT;

                    MOVE_TO_STATE(STATE_SUCCESS);
                }
                else
                {
                    pMe->m_wErrStrID = IDS_ERR_ADDFIELD;
                    MOVE_TO_STATE(STATE_ERROR);
                }
            }
            break;

        case DLGRET_YES:
        {
            int oneDialIndex;
            switch(pMe->m_wNewNumFldMask)
            {
                case MASK_ADDRFIELD_PHONE_GENERIC:
                    wFldID = AEE_ADDRFIELD_PHONE_GENERIC;
                    (void)WSTRCPY(pMe->m_tmpBuf,pMe->m_pAddNewMobile);
                    break;

                case MASK_ADDRFIELD_PHONE_HOME:
                    wFldID = AEE_ADDRFIELD_PHONE_HOME;
                    (void)WSTRCPY(pMe->m_tmpBuf,pMe->m_pAddNewHome);
                    break;

                case MASK_ADDRFIELD_PHONE_WORK:
                    wFldID = AEE_ADDRFIELD_PHONE_WORK;
                    (void)WSTRCPY(pMe->m_tmpBuf,pMe->m_pAddNewOffice);
                    break;

                case MASK_ADDRFIELD_PHONE_FAX:
                    wFldID = AEE_ADDRFIELD_PHONE_FAX;
                    (void)WSTRCPY(pMe->m_tmpBuf,pMe->m_pAddNewFax);
                    break;

                default:
                    pMe->m_wErrStrID = IDS_ERR_UNKNOWN;
                    MOVE_TO_STATE(STATE_ERROR);
                    break;
            }

            for(oneDialIndex=CONTCFG_ONEDIAL1; oneDialIndex<=CONTCFG_ONEDIAL8; oneDialIndex++)
            {
                if(SUCCESS != CContApp_GetConfig( pMe,
                                          (ContAppCFG)oneDialIndex,
                                          pMe->m_pOneDialBuf,
                                          sizeof(pMe->m_pOneDialBuf)))
                {
                    continue;
                }

                // ���ڵ�����������Ҫ�޸ĵ�������
                if(WSTRCMP(pMe->m_tmpBuf, pMe->m_pOneDialBuf) == 0)
                {
                    (void)CContApp_SetConfig( pMe,
                                  (ContAppCFG)oneDialIndex,
                                  pMe->m_pFldInputBuf,
                                  sizeof(pMe->m_pFldInputBuf));
                }
            }
            
            nRet = CContApp_SaveEditField(pMe, wFldID);
            
            if( SUCCESS == nRet)
            {
                FREEIF(pMe->m_pFldInputBuf);
                pMe->m_eSuccessRetState = STATE_EXIT;
            
                MOVE_TO_STATE(STATE_SUCCESS);
            }
            else
            {
                pMe->m_wErrStrID = IDS_ERR_ADDFIELD;
                MOVE_TO_STATE(STATE_ERROR);
            }
        }
            break;
            
        case DLGRET_NO:
            MOVE_TO_STATE(STATE_NUMFLDVIEW);
            break;

            
        case DLGRET_ERR:
            pMe->m_wErrStrID = IDS_ERR_LOADFIELDS;
            MOVE_TO_STATE(STATE_ERROR);
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // Handler_STATE_VIEW

// ״̬ STATE_DELETING ������
static NextFSMAction Handler_STATE_DELETING(CContApp *pMe)
{
    int nRet = SUCCESS;
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    FARF(STATE, ("Handler_STATE_DELETING  %d", pMe->m_eDlgRet));
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            if(TRUE == pMe->m_bIsNotComplete)
            {
                pMe->m_bIsNotComplete = FALSE;
                pMe->m_eMsgType = MESSAGE_WARNNING; 
                if(SUCCESS != CContApp_ShowMsgBox(pMe, 0))
                {
                    MOVE_TO_STATE(STATE_EXIT);
                    return NFSMACTION_CONTINUE;
                }
                return NFSMACTION_WAIT;
            }
            if(DELETE_ALL == pMe->m_nDeleteType)
            {
                //CContApp_Phonebook_Load(pMe);
                //CContApp_LoadPhoneBook(pMe, CONTCFG_VIEWTYPE_ALL);
                CContApp_MarkAll(pMe);                
            }

           CContApp_DeleteMultipe(pMe);

           // Proccess the return value

           if(ENORECORD == nRet)
           {
               pMe->m_wMsgResID = IDS_MSG_NORECORD;
               pMe->m_eMsgRetState = STATE_DELETE_SELECT;
               MOVE_TO_STATE(STATE_MSG);    
               return NFSMACTION_CONTINUE;       
           }
           else if(EFAILED == nRet)
           {
               pMe->m_wMsgResID = IDS_MSG_DELETEFAILED;
               pMe->m_eMsgRetState = STATE_DELETE_SELECT;
               MOVE_TO_STATE(STATE_MSG);    
               return NFSMACTION_CONTINUE;        
           }
           
           // Show dialog
           if(SUCCESS != CContApp_ShowDialog(pMe, IDD_DELETING))
           {
               MOVE_TO_STATE(STATE_EXIT);
               return NFSMACTION_CONTINUE;
           }
            return NFSMACTION_WAIT;
        }
            
        //Proccess yourself dialog retrn value here
        case DLGRET_OK:
            if(pMe->m_wDeleteRecCnt > 0)
            {
                // Reload contacts
                if(SUCCESS != CContApp_LoadPhoneBook(pMe, pMe->m_nViewType))
                {
                    pMe->m_wErrStrID = IDS_ERR_LOAD;
                    MOVE_TO_STATE(STATE_ERROR);
                    return NFSMACTION_CONTINUE;
                }
            }           
            pMe->m_eSuccessRetState = STATE_DELETE_SELECT;
            MOVE_TO_STATE(STATE_SUCCESS);       
            break;
        
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_EXIT);
            return NFSMACTION_CONTINUE;
            
        case DLGRET_ERR:
            MOVE_TO_STATE(STATE_ERROR);
            return NFSMACTION_CONTINUE;

        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_DELETE_SELECT);
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
}//Handler_STATE_DELETEALL
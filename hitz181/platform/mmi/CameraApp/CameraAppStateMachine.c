/*==============================================================================
//           CameraApp   A P P L E T  M O D U L E
// �ļ�:
//        CameraAppStateMachine.c
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//        
// �ļ�˵����
//        ��������ģ��״̬������������ļ�
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
#include "CameraApp_priv.h" 
/*==============================================================================                             
                                 �궨��ͳ���                                
==============================================================================*/



/*==============================================================================
                                 ��������
==============================================================================*/


/*==============================================================================
                                 ��������                               
==============================================================================*/
//״̬ STATE_NULL ������
static NextFSMAction CameraApp_StateNullHandle(CCameraApp *pMe);

//״̬ STATE_CMAINMENU ������
static NextFSMAction CameraApp_StateMainMenuHandle(CCameraApp *pMe);

//״̬STATE_CPRIVEW ������
static NextFSMAction CameraApp_StatePreviewHandle(CCameraApp *pMe);

//״̬STATE_CCAMERACFG ������
static NextFSMAction CameraApp_StateCameraCFGHandle(CCameraApp *pMe);

//״̬ STATE_CPIC ������
static NextFSMAction CameraApp_StatePicHandle(CCameraApp *pMe);

//״̬ STATE_CPOPMSG ������
static NextFSMAction CameraApp_StatePopMSGHandle(CCameraApp *pMe);

//״̬ STATE_EXIT ������
static NextFSMAction CameraApp_StateExitHandle(CCameraApp *pMe);


/*==============================================================================
                                 ȫ������
==============================================================================*/

/*==============================================================================
                                 ���أ���̬������
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/
/*==============================================================================
����:
       CameraApp_ProcessState
˵��:
       CameraApp Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��CameraApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction CameraApp_ProcessState(CCameraApp *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;

    if(NULL == pMe)
    {
        return retVal;
    }

    //���ݵ�ǰ״̬������Ӧ��״̬������
    switch(pMe->m_eCurState)
    {
        case STATE_NULL:
            retVal = CameraApp_StateNullHandle(pMe);
            break;
            
        case STATE_CMAINMENU:
            retVal = CameraApp_StateMainMenuHandle(pMe);
            break;
            
        case STATE_CPREVIEW:
            retVal = CameraApp_StatePreviewHandle(pMe);
            break;
      
        case STATE_CCAMERACFG:
            retVal = CameraApp_StateCameraCFGHandle(pMe);
            break;

        case STATE_CPIC:
            retVal = CameraApp_StatePicHandle(pMe);
            break;
            
        case STATE_CPOPMSG:
            retVal = CameraApp_StatePopMSGHandle(pMe);
            break;
                       
        case STATE_EXIT:
      
        default:
            retVal = CameraApp_StateExitHandle(pMe);
            break;       
    }
    return retVal;
}

/*==============================================================================
������
       Camera_StateNullHandle
˵����
       STATE_NULL ״̬������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction CameraApp_StateNullHandle(CCameraApp *pMe)
{
    if(NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(STATE_EXIT)
    return NFSMACTION_CONTINUE;
}

/*==============================================================================
������
       Camera_StateCMainMenuHandle
˵����
       STATE_MAINMENU ״̬������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction CameraApp_StateMainMenuHandle(CCameraApp *pMe)
{
    if(NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE; 
            /*IANNUNCIATOR_SetField(pMe->m_pIAnn, AEECLSID_DISPLAY1, TRUE);
            IANNUNCIATOR_EnableAnnunciatorBar(pMe->m_pIAnn, AEECLSID_DISPLAY1, TRUE);*/
            CameraApp_ShowDialog(pMe, IDD_CMAINMENU);
            return NFSMACTION_WAIT;

        case DLGRET_PREVIEW:           
            MOVE_TO_STATE(STATE_CPREVIEW)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_PICMENU:
            MOVE_TO_STATE(STATE_CPIC)
            return NFSMACTION_CONTINUE;
                       
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_EXIT)
            return NFSMACTION_CONTINUE;

        case DLGRET_POPMSG:
            MOVE_TO_STATE(STATE_CPOPMSG)
            return NFSMACTION_CONTINUE;
                          
        default:
            break;
    }   
    return NFSMACTION_WAIT;
}

/*==============================================================================
������
       CameraApp_StatePreviewHandle
˵����
       STATE_CPREVIEW ״̬������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction CameraApp_StatePreviewHandle(CCameraApp *pMe)
{
    if(NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
  
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            CameraApp_ShowDialog(pMe, IDD_CPREVIEW);
            return NFSMACTION_WAIT;
        
        case DLGRET_PICMENU:
            MOVE_TO_STATE(STATE_CPIC)
            return NFSMACTION_CONTINUE;

        case DLGRET_CAMERACFG:
            MOVE_TO_STATE(STATE_CCAMERACFG)
            return NFSMACTION_CONTINUE;
    
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_CMAINMENU)
            return NFSMACTION_CONTINUE;

        case DLGRET_POPMSG:
            MOVE_TO_STATE(STATE_CPOPMSG)
            return NFSMACTION_CONTINUE;
        
        case DLGRET_EXIT:
            MOVE_TO_STATE(STATE_EXIT)  
            return NFSMACTION_CONTINUE;
                        
        default:
            break;
    }    
    return NFSMACTION_WAIT;
} 

/*==============================================================================
������
       CameraApp_StateCameraCFGHandle
˵����
       STATE_CAMERACFG ״̬������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction CameraApp_StateCameraCFGHandle(CCameraApp *pMe)
{
    if(NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
  
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            CameraApp_ShowDialog(pMe, IDD_CCAMERACFG);
            return NFSMACTION_WAIT;

        case DLGRET_PREVIEW:
            MOVE_TO_STATE(STATE_CPREVIEW)
            return NFSMACTION_CONTINUE;

        case DLGRET_POPMSG:
            MOVE_TO_STATE(STATE_CPOPMSG)
            return NFSMACTION_CONTINUE;    

        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_CPREVIEW)
            return NFSMACTION_CONTINUE;
        
        case DLGRET_EXIT:
            MOVE_TO_STATE(STATE_EXIT)  
            return NFSMACTION_CONTINUE;
                        
        default:
            break;
    }
    return NFSMACTION_WAIT;
} 

/*==============================================================================
������
       CameraApp_StatePicHandle
˵����
       STATE_CPIC ״̬������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction CameraApp_StatePicHandle(CCameraApp *pMe)
{
    if(NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
  
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            CameraApp_ShowDialog(pMe, IDD_CPIC);
            return NFSMACTION_WAIT;

        case DLGRET_PREVIEW:
            MOVE_TO_STATE(STATE_CPREVIEW)
            return NFSMACTION_CONTINUE;

        case DLGRET_POPMSG:
            MOVE_TO_STATE(STATE_CPOPMSG)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_CMAINMENU)
            return NFSMACTION_CONTINUE;
        
        case DLGRET_EXIT:
            MOVE_TO_STATE(STATE_EXIT)  
            return NFSMACTION_CONTINUE;
                        
        default:
            break;
    }    
    return NFSMACTION_WAIT;
} 

static NextFSMAction CameraApp_StatePopMSGHandle(CCameraApp *pMe)
{    
    if(NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // Show dialog
            pMe->m_bNotOverwriteDlgRet = FALSE;
            CameraApp_ShowDialog(pMe, IDD_POPMSG);
            return NFSMACTION_WAIT;

        case DLGRET_MAINMENU:
            MOVE_TO_STATE(STATE_CMAINMENU);
            return NFSMACTION_CONTINUE;

        case DLGRET_PREVIEW:
            MOVE_TO_STATE(STATE_CPREVIEW);
            return NFSMACTION_CONTINUE;
            
        case DLGRET_EXIT:
            MOVE_TO_STATE(STATE_EXIT);
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            if(pMe->m_ePreState == STATE_CPIC)
            {
                pMe->m_bRePreview =  TRUE;
                MOVE_TO_STATE(STATE_CPREVIEW);
            }
            else
            {
                MOVE_TO_STATE(pMe->m_ePreState);
            }
            return NFSMACTION_CONTINUE;
            
        default:
            break;
    }
    
    return NFSMACTION_CONTINUE;
} 

/*==============================================================================
������
       CameraApp_StateExitHandle
˵����
       STATE_EXIT ״̬������
       
������
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction CameraApp_StateExitHandle(CCameraApp *pMe)
{
    if(NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    (void)ISHELL_CloseApplet(pMe->m_pShell, FALSE);

    return NFSMACTION_WAIT;
}

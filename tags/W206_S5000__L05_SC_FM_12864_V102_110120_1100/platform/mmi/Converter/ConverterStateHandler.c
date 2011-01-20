/*==============================================================================
// �ļ���
//        ConverterStateHandler.c
//        2008-10-28 Gemsea����汾(Draft Version)
//        ��Ȩ����(c) 2008 Anylook
//        
// �ļ�˵����
//        
// ���ߣ�Gemsea
// �������ڣ�2008-10-28
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
08-10-28                   Initial version
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "Converter_priv.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/


/*==============================================================================
                                 ���Ͷ���
==============================================================================*/


/*==============================================================================
                                 ��������
==============================================================================*/
    
// ״̬ CONVERTER_NONE ������
static NextFSMAction State_ConverterNoOperationHandler(CConverter *pMe);

// ״̬ CONVERTER_INIT ������
static NextFSMAction State_ConverterInitHandler(CConverter *pMe);

// ״̬ CONVERTER_MAIN ������
static NextFSMAction State_ConverterMainHandler(CConverter *pMe);

// ״̬ CONVERTER_CONVERT ������
static NextFSMAction State_ConverterConvertHandler(CConverter *pMe);

// ״̬ CONVERTER_CHANGECURRENCY ������
static NextFSMAction State_ConverterChangeCurrencyHandler(CConverter *pMe);

// ״̬ CONVERTER_EXIT ������
static NextFSMAction State_ConverterExitHandler(CConverter *pMe);

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
       Converter_ProcessState
˵��:
       Converter Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��Converter Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction Converter_ProcessState(CConverter *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;

    if (NULL == pMe)
    {
        return retVal;
    }
    // ���ݵ�ǰ״̬������Ӧ��״̬������
    switch (pMe->m_eCurState)
    {
        case CONVERTER_NONE:
        {
            retVal = State_ConverterNoOperationHandler( pMe);
        }
        break;
        
        case CONVERTER_INIT:
        {
            retVal = State_ConverterInitHandler( pMe);
        }
        break;
        
        case CONVERTER_MAIN:
        {
            retVal = State_ConverterMainHandler( pMe);
        }
        break;
        
        case CONVERTER_CONVERT:
        {
            retVal = State_ConverterConvertHandler( pMe);
        }
        break;

        case CONVERTER_CHANGECURRENCY:
        {
            retVal = State_ConverterChangeCurrencyHandler( pMe);
        }
        break;

        case CONVERTER_EXIT:
        default:
        {
            retVal = State_ConverterExitHandler( pMe);
        }
        break;
    }
    return retVal;
}

/*==============================================================================
������
       State_ConverterNoOperationHandler
˵����
       Converter_NONE ״̬������

������
       pMe [in]��ָ��Converter Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction State_ConverterNoOperationHandler(CConverter *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(CONVERTER_EXIT)

    return NFSMACTION_CONTINUE;
} 

/*==============================================================================
������
       State_ConverterInitHandler
˵����
       CONVERTER_INIT ״̬������

������
       pMe [in]��ָ��Converter Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction State_ConverterInitHandler(CConverter *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(CONVERTER_MAIN)
    return NFSMACTION_CONTINUE;
} 

/*==============================================================================
������
       State_ConverterMainHandler
˵����
       CONVERTER_MAIN ״̬������
       
������
       pMe [in]��ָ��Converter Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT    ��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction State_ConverterMainHandler(CConverter *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            (void)Converter_ShowDialog(pMe, IDD_CONVERTERMAIN);
            return NFSMACTION_WAIT;
            
        case DLGRET_CURRENCY:
            pMe->m_converterMode = CONVERTER_MODE_CURRENCY;
            MOVE_TO_STATE(CONVERTER_CONVERT)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_LENGTH:
            pMe->m_converterMode = CONVERTER_MODE_LENGTH;
            MOVE_TO_STATE(CONVERTER_CONVERT)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_WEIGHT:
            pMe->m_converterMode = CONVERTER_MODE_WEIGHT;
            MOVE_TO_STATE(CONVERTER_CONVERT)
            return NFSMACTION_CONTINUE;

        case DLGRET_VOLUME:
            pMe->m_converterMode = CONVERTER_MODE_VOLUME;
            MOVE_TO_STATE(CONVERTER_CONVERT)
            return NFSMACTION_CONTINUE;

        case DLGRET_AREA:
            pMe->m_converterMode = CONVERTER_MODE_AREA;
            MOVE_TO_STATE(CONVERTER_CONVERT)
            return NFSMACTION_CONTINUE;

        case DLGRET_TEMPERATURE:
            pMe->m_converterMode = CONVERTER_MODE_TEMPERATURE;
            MOVE_TO_STATE(CONVERTER_CONVERT)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CANCELED:
            MOVE_TO_STATE(CONVERTER_EXIT)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}

/*==============================================================================
������
       State_ConverterConvertHandler
˵����
       CONVERTER_CONVERT ״̬������
       
������
       pMe [in]��ָ��Converter Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT    ��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction State_ConverterConvertHandler(CConverter *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            Converter_ShowDialog(pMe, IDD_CONVERTERCONVERT);
            return NFSMACTION_WAIT;

        case DLGRET_CHANGECURRENCY:
            MOVE_TO_STATE(CONVERTER_CHANGECURRENCY)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CANCELED:
            MOVE_TO_STATE(CONVERTER_MAIN)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }
    return NFSMACTION_WAIT;
}

/*==============================================================================
������
       State_ConverterChangeCurrencyHandler
˵����
       CONVERTER_CHANGECURRENCY ״̬������
       
������
       pMe [in]��ָ��Converter Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT    ��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction State_ConverterChangeCurrencyHandler(CConverter *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            Converter_ShowDialog(pMe, IDD_CHANGECURRENCY);
            return NFSMACTION_WAIT;

        case DLGRET_OK:
            Converter_ShowMsgBox(pMe, IDS_DONE);
            return NFSMACTION_WAIT; 
            
        case DLGRET_ERR:
            Converter_ShowMsgBox(pMe, IDS_SAVE_ZERO);
            return NFSMACTION_WAIT;
   
        case DLGRET_CANCELED:
            MOVE_TO_STATE(CONVERTER_CONVERT)
            return NFSMACTION_CONTINUE;

        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(CONVERTER_CONVERT)
            return NFSMACTION_CONTINUE;

        case DLGRET_MSGBOX_CANCEL:
            return NFSMACTION_CONTINUE;
            
        default:
            break;
    }
    return NFSMACTION_WAIT;
}

/*==============================================================================
������
       State_ConverterExitHandler
˵����
       CONVERTER_EXIT ״̬������
       
������
       pMe [in]��ָ��Converter Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT    ��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction State_ConverterExitHandler(CConverter *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);
    return NFSMACTION_WAIT;
}


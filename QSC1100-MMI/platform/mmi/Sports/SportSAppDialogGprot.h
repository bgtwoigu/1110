#ifndef _SPORTSAPP_DIALOG_GPROT_H_
#define _SPORTSAPP_DIALOG_GPROT_H_
/*********************************************************************************

ģ����       : <SportApplication>
�ļ���       : <SportSAppDialogGprot.h>
����ļ�     : 
�ļ�ʵ�ֹ��� : Sports Application
����         : 
�汾         : 1.0
--------------------------------------------------------------------------------
��ע         : 
--------------------------------------------------------------------------------
�޸ļ�¼ : 

******************************************************************************/
#include "SportSApp_priv.h"


/*=============================================================================
FUNCTION: InitPedoMeter

DESCRIPTION:����������

PARAMETERS:
   *pme:

RETURN VALUE:
   boolean

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern boolean InitPedoMeter(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: Pedometer_Redraw

DESCRIPTION:����Pedometer Screen

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void Pedometer_Redraw(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: PedometerFree

DESCRIPTION:Free Pedometer Control Handler

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void PedometerFree(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: Pedometer_Gettime

DESCRIPTION:������ǰҪˢ�µ�ʱ��

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void Pedometer_Gettime(CSportsMenu *pMe,uint32 nTicks);

/*=============================================================================
FUNCTION: Pedometer_Running

DESCRIPTION: ���ʼ��

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void Pedometer_Running(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: Pedometer_Stop

DESCRIPTION: ���ֹͣ

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void Pedometer_Stop(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: StopWatch_Continue

DESCRIPTION: �����ͣ�����ִ��

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void Pedometer_Resume(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: Pedometer_Reset

DESCRIPTION: �������

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void Pedometer_Reset(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: Pedometer_ResetConfirm

DESCRIPTION: �������ȷ���Ի�

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void Pedometer_ResetConfirm(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: Pedometer_RecordRestConfirm

DESCRIPTION: �������ȷ���Ի�

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void Pedometer_RecordRestConfirm(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: Pedometer_SetPedSpeedData

DESCRIPTION: ����������

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void Pedometer_SetPedSpeedData(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: Pedometer_ClearData

DESCRIPTION: ����������

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void Pedometer_ClearData(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: SportSApp_GetInputMode

DESCRIPTION: ��ȡ����ģʽ

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern SportUnitData SportSApp_GetInputMode(SportUnitData Input_Id);

/*=============================================================================

FUNCTION:SportSApp_GetInputProp

DESCRIPTION:
    ��ö�Ӧ Field �����뷨����
    
PARAMETERS:
    nFldID  : �ֶ�����
    
RETURN VALUE:
    The property of the text control
    
COMMENTS:
    
SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern uint32 SportSApp_GetInputProp(SportUnitData Input_Id);
/*=============================================================================
FUNCTION:  SportSApp_drawAnimation

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportSApp_drawWalkAnimation(CSportsMenu *pMe);

/*=============================================================================
FUNCTION:  SportSApp_drawRunAnimation

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportSApp_drawRunAnimation(CSportsMenu *pMe);

/*=============================================================================
FUNCTION:  SportSApp_ReleaseAnimation

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportSApp_ReleaseAnimation(CSportsMenu *pMe);

/*=============================================================================
FUNCTION:  SportSApp_DisplayArrow

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportSApp_DisplayArrow(CSportsMenu *pMe);

/*=============================================================================
FUNCTION:  SportsApp_ShowEditItem

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportsApp_ShowEditItem(CSportsMenu  *pMe, IMenuCtl  *pMenuCtl, ITextCtl *pText);

/*=============================================================================
FUNCTION:  SportSapp_SaveCurrentItemText

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportSapp_SaveCurrentItemText(CSportsMenu  *pMe, ITextCtl  *pText);
/*=============================================================================
FUNCTION:  SportSApp_SportsPlan

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportSApp_SportsPlan(CSportsMenu *pMe);

/*=============================================================================
FUNCTION:  SportSapp_SetMenuText

DESCRIPTION: 

PARAMETERS:

RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportSapp_SetMenuText(CSportsMenu *pMe,IMenuCtl *pMenu,uint16 nSelMenu);

/*=============================================================================

FUNCTION:SportSApp_BuildGroupList

DESCRIPTION:
    ��ѡ��˵����������
    
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
    bAll    : �Ƿ����ȫ����Ŀ
RETURN VALUE:
    SUCCESS: 
    EFAILED: If the number of current cache is 0
    
COMMENTS:
    
SIDE EFFECTS:
    
SEE ALSO:

=============================================================================*/
extern int SportSApp_BuildGroupList(CSportsMenu  *pMe, IMenuCtl  *groupList);

/*=============================================================================

FUNCTION:SportSApp_BuildGroupList

DESCRIPTION:
    ��ѡ��˵����������
    
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
    bAll    : �Ƿ����ȫ����Ŀ
RETURN VALUE:
    SUCCESS: 
    EFAILED: If the number of current cache is 0
    
COMMENTS:
    
SIDE EFFECTS:
    
SEE ALSO:

=============================================================================*/
extern void SportSapp_ShowGroupSelect(CSportsMenu  *pMe, IMenuCtl  *pMenuCtl, IMenuCtl  *groupList);

/*=============================================================================

FUNCTION:SportSapp_BuildAddMenuMenu

DESCRIPTION:
    ��ѡ��˵����������
    
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
    bAll    : �Ƿ����ȫ����Ŀ
RETURN VALUE:
    SUCCESS: 
    EFAILED: If the number of current cache is 0
    
COMMENTS:

SIDE EFFECTS:
    
SEE ALSO:

=============================================================================*/
extern int CSportSapp_BuildAddMenuMenu(CSportsMenu *pMe, IMenuCtl *pMenuCtl);

#endif


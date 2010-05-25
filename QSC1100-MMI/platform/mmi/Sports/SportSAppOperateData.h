#ifndef _SPORTSAPP_OPERATE_DATA_H_
#define _SPORTSAPP_OPERATE_DATA_H_
/*********************************************************************************

ģ����       : <SportApplication>
�ļ���       : <SportSAppOperateData.h>
����ļ�     : 
�ļ�ʵ�ֹ��� : Sports Application
����         : 
�汾         : 1.0
--------------------------------------------------------------------------------
��ע         : 
--------------------------------------------------------------------------------
�޸ļ�¼ : 

******************************************************************************/

/*=============================================================================
FUNCTION: Sports_SaveCurrentActiveUser

DESCRIPTION:���浱ǰ������û�

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportsApp_SaveCurrentActiveUser(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: Sports_GetCurrentActiveUser

DESCRIPTION:��ȡ��ǰ�û�

PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern ProFileUser SportsApp_GetCurrentActiveUser(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: SportsApp_ClearAllCurrentActiveData

DESCRIPTION:���������û�����Ϣ

PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportsApp_ClearAllCurrentActiveData(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: SportsApp_SaveAllCurrentActiveData

DESCRIPTION:���������û�����Ϣ

PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportsApp_SaveAllCurrentActiveData(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: SportsApp_GetPedometerUserData

DESCRIPTION:���������û�����Ϣ

PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern CSportSAppData SportsApp_GetAllCurrentActiveData(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: SportsApp_GetPedometerData

DESCRIPTION:���������û�����Ϣ

PARAMETERS:
   *pme:

RETURN VALUE:
   CSportSPedoMeterData

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern CSportSPedoMeterData SportsApp_GetPedometerData(CSportsMenu *pMe);


/*=============================================================================
FUNCTION: SportsApp_GetUnitData

DESCRIPTION:���������û�����Ϣ

PARAMETERS:
   *pme:

RETURN VALUE:
   CSportSAppUintData

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern CSportSAppUintData SportsApp_GetUnitData(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: SportsApp_GetAccRecordData

DESCRIPTION:���������û�����Ϣ

PARAMETERS:
   *pme:

RETURN VALUE:
   CSportSPedoMeterData

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern CSportSPedoMeterData SportsApp_GetAccRecordData(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: SportsApp_DisplayLastRecordMenu

DESCRIPTION:��ʾ��Ѽ�¼�˵���

PARAMETERS:
   *pme:

RETURN VALUE:
   CSportSPedoMeterData

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportsApp_DisplayLastRecordMenu(CSportsMenu *pMe,IMenuCtl *pActiveMenu);

/*=============================================================================
FUNCTION: SportsApp_DisplayAccRecordMenu

DESCRIPTION:��ʾ��Ѽ�¼�˵���

PARAMETERS:
   *pme:

RETURN VALUE:
   CSportSPedoMeterData

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportsApp_DisplayAccRecordMenu(CSportsMenu *pMe,IMenuCtl *pActiveMenu);

/*=============================================================================
FUNCTION: SportSApp_FloatToWStr

DESCRIPTION:����������ת���ɿ��ַ�

PARAMETERS:
   *pme:

RETURN VALUE:
   CSportSPedoMeterData

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/

extern boolean SportSApp_FloatToWStr(double v, AECHAR * pdest, int nSize);

/*=============================================================================
FUNCTION: SportsApp_SetPedometerSpeeds

DESCRIPTION:��ʾ��Ѽ�¼�˵���

PARAMETERS:
   *pme:

RETURN VALUE:
   CSportSPedoMeterData

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportsApp_SetPedometerSpeeds(CSportsMenu *pMe,int32 nStep,uint32 active_index);

/*=============================================================================
FUNCTION: SportsApp_SetPedometerDistance

DESCRIPTION:��ʾ��Ѽ�¼�˵���

PARAMETERS:
   *pme:

RETURN VALUE:
   CSportSPedoMeterData

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportsApp_SetPedometerDistance(CSportsMenu *pMe,int32 nStep,uint32 active_index);

/*=============================================================================
FUNCTION: SportsApp_SetPedometerCalorie

DESCRIPTION:��ʾ��Ѽ�¼�˵���

PARAMETERS:
   *pme:

RETURN VALUE:
   CSportSPedoMeterData

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportsApp_SetPedometerCalorie(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: SportsApp_SetPersonalItemName

DESCRIPTION:����Personal ���û�������

PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportsApp_SetPersonalItemName(CSportsMenu *pMe,IMenuCtl *pMenu);

/*=============================================================================
FUNCTION: SportSApp_LoadSportData

DESCRIPTION:Load Sport data From to file System

PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern int SportSApp_LoadSportData(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: SportSApp_SaveSportData

DESCRIPTION:

PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern int SportSApp_SaveSportData(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: SportsApp_ExtraDecimal

DESCRIPTION:Save Sport data to File System

PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern boolean SportsApp_ExtraDecimal(AECHAR *OutWdata,AECHAR *inWData);

/*=============================================================================
FUNCTION: SportSApp_Set_Shake_Disable

DESCRIPTION:�ص�shake 

PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern boolean SportSApp_Set_Shake_Disable(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: SportSApp_Get_Shake_OnOrOff

DESCRIPTION:�ж��Ƿ��Ѿ�����shake

PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern boolean SportSApp_Get_Shake_OnOrOff(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: SportSApp_PlayMusicWithSport

DESCRIPTION:�ж��Ƿ��Ѿ�����shake

PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportSApp_PlayMusicWithSport(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: SportSApp_ClosePlayMusic

DESCRIPTION:��sportֹͣ��ʱ��music ҲҪ�ر�

PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportSApp_ClosePlayMusic(CSportsMenu *pMe);


/*=============================================================================
FUNCTION: SportSApp_PlayMusicOnOrOff

DESCRIPTION:��sportֹͣ��ʱ��music ҲҪ�ر�

PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern boolean SportSApp_GetPlayMusicOnOrOff(void);



/*=============================================================================
FUNCTION: SportSApp_StartFmRadio

DESCRIPTION:
            start fmRadio
PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern boolean SportSApp_StartFmRadio(CSportsMenu *pMe);


/*=============================================================================
FUNCTION: SportSApp_StartFmRadioEx

DESCRIPTION:
            start fmRadio
PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
boolean SportSApp_StartFmRadioEx(CSportsMenu *pMe);
/*=============================================================================
FUNCTION: SportSApp_StartFmRadio

DESCRIPTION:
            start fmRadio
PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern boolean SportSApp_IsRunOnGround(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: SportSApp_StopFmRadio

DESCRIPTION:
            start fmRadio
PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern boolean SportSApp_StopFmRadio(CSportsMenu *pMe);


/*=============================================================================
FUNCTION: SportSapp_SwitchFmAndMusic

DESCRIPTION:
            start fmRadio
PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern boolean SportSapp_SwitchFmAndMusic(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: SportSapp_PauseMusic

DESCRIPTION:
            start fmRadio
PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportSapp_PauseMusic(CSportsMenu *pMe);

/*=============================================================================
FUNCTION: SportSapp_ResumeMusic

DESCRIPTION:
            start fmRadio
PARAMETERS:
   *pme:

RETURN VALUE:
   ProFileUser

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
extern void SportSapp_ResumeMusic(CSportsMenu *pMe);

#endif

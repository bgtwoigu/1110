#ifndef AEEALARMMGR_H
#define AEEALARMMGR_H

/*============================================================================
FILE: AEEAlarmMgr.h

SERVICES:  Alarm Services

GENERAL DESCRIPTION: 
   This file provides definitions for alarm interfaces
   available to application developers.

Copyright (c) 2005,2007-2008 QUALCOMM Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
============================================================================*/
#include "AEE.h"

#define AEEIID_ALARMMGR 0x0102fefd

//
// special IALARMMGR_CancelAlarm() codes for AEEGROUPID_ALARM_MGR
//
#define AEE_ALARMMGR_ALARM_SUSPEND                 0
#define AEE_ALARMMGR_ALARM_RESUME                  1

//******************************************************************************
//
// IAlarmMgr Interface
//
//******************************************************************************
#define INHERIT_IAlarmMgr(iname) \
   INHERIT_IQueryInterface(iname); \
   int   (*SetAlarm)(iname* me, AEECLSID cls, uint32* pnUserCode, uint32 nSecs); \
   int   (*CancelAlarm)(iname* me, AEECLSID cls, uint16 nUserCode); \
   int   (*AlarmsActive)(iname* me, AEECLSID cls); \
   int   (*ListAlarmCodes)(iname* po, AEECLSID cls, uint16* pnUserCodes, uint32* pnCount); \
   int   (*ReserveUserCodes)(iname* po, AEECLSID cls, uint16* pnUserCodes, uint32* pnCount)
   

AEEINTERFACE_DEFINE(IAlarmMgr);

static __inline boolean IALARMMGR_IsCodeReserved(IAlarmMgr* p, uint16 nUserCode) 
{
   int nErr;

   nErr =  AEEGETPVTBL((p),IAlarmMgr)->ListAlarmCodes(p,0,&nUserCode,0);

   if (SUCCESS == nErr)
   {
      return TRUE;
   }

   return FALSE;
}

#define IALARMMGR_AddRef(p)                         AEEGETPVTBL(p,IAlarmMgr)->AddRef(p)
#define IALARMMGR_Release(p)                        AEEGETPVTBL(p,IAlarmMgr)->Release(p)
#define IALARMMGR_QueryInterface(p,i,p2)            AEEGETPVTBL(p,IAlarmMgr)->QueryInterface(p,i,p2)
#define IALARMMGR_SetAlarm(p,cls,uc,nm)             AEEGETPVTBL(p,IAlarmMgr)->SetAlarm(p,cls,uc,nm)
#define IALARMMGR_CancelAlarm(p,cls,uc)             AEEGETPVTBL(p,IAlarmMgr)->CancelAlarm(p,cls,uc)
#define IALARMMGR_SuspendAlarms(p)                  AEEGETPVTBL(p,IAlarmMgr)->CancelAlarm(p,AEEGROUPID_ALARM_MGR,AEE_ALARMMGR_ALARM_SUSPEND)
#define IALARMMGR_ResumeAlarms(p)                   AEEGETPVTBL(p,IAlarmMgr)->CancelAlarm(p,AEEGROUPID_ALARM_MGR,AEE_ALARMMGR_ALARM_RESUME)
#define IALARMMGR_AlarmsActive(p,cls)               AEEGETPVTBL(p,IAlarmMgr)->AlarmsActive(p,cls)

#define IALARMMGR_ListAlarmCodes(p,cls,uc,nm)       AEEGETPVTBL(p,IAlarmMgr)->ListAlarmCodes(p,cls,uc,nm)
#define IALARMMGR_ReserveUserCodes(p,cls,uc,nm)     AEEGETPVTBL(p,IAlarmMgr)->ReserveUserCodes(p,cls,uc,nm)

// helper functions
static __inline uint32 IALARMMGR_SetUserCode(uint16 nUserCode, boolean bUserCodeProvided, boolean bAbsoluteTime)
{
   uint32 nCode = (nUserCode << 16);
   nCode |= (bUserCodeProvided & 0x01);   // set lsb
   nCode |= ((bAbsoluteTime & 0x01) << 1); // set lsb + 1
   
   return nCode;
}

static __inline uint16 IALARMMGR_GetUserCode(uint32 nUserCode)
{
   return (uint16)(nUserCode >> 16);
}


/*=====================================================================
DATA STRUCTURE DOCUMENTATION
=======================================================================


=======================================================================
INTERFACES   DOCUMENTATION
=======================================================================

Interface Name: IAlarmMgr
Description:

The AlarmMgr's alarm functions enable an application to be notified when the current
time reaches a specified value. Unlike timers, which can be active only while your
application is running, you can receive notification that an alarm has expired even
when your application is not running. Alarms are typically used when the time of
notification is in the distant future. For example, a calendar application uses
an alarm to alert the user when a calendar appointment is about to be reached.

For all IAlarmMgr API's when an application needs to do any alarm operation on another class
ID (start alarm, cancel alarm, reserve, list or query alarm user codes), the caller needs to have
AEEGROUPID_ALARM_ALL privilege level. For suspending/resuming the BREW alarm services the 
application needs AEEGROUPID_ALARM_MGR privilege level. For reserving more than 30 user codes
the application needs to have AEEGROUPID_RESERVECODE_LARGE privilege level.

To set an alarm, you call IALARMMGR_SetAlarm() and specify the number of seconds from
the current time at which the alarm notification is to occur or absolute number of seconds
and the BREW ClassID of the application (yours or another) that receives notification when 
the alarm time is reached. The 16-bit alarm user code can be specified by the user or it can be generated by BREW.
If the specified alarm user code is already used by an active alarm, the alarm is canceled 
and a new one started.

At notification time, the IAPPLET_HandleEvent() function of the notified application is 
called with an EVT_ALARM event and the 16-bit alarm code as parameters. The latter parameter 
allows an application to distinguish between more than one simultaneously active alarm. 
If the notified application is not currently running, the AEE Shell creates an instance 
of it to process the notification event, after which it is terminated. The application may 
choose to activate itself if necessary. The alarms are stored in BREW database 
and continuously checked for alarm expirations while the BREW-enabled device is turned on. 
If an alarm�s expiration time passes while the device is turned off, the alarm notification 
is generated the next time the device is turned on.

IALARMMGR_CancelAlarm() cancels a currently active alarm. IALARMMGR_AlarmsActive() checks
whether any of BREW built-in annunciators (alarm clock, countdown timer, or stopwatch)
are currently active for the specified class ID.

IALARMMGR_ListAlarmCodes() lists all the user codes reserved for the application regardless 
of whether or not that alarm is active. The function can also be used to query if specified
alarm user code is reserved or not.

IALARMMGR_ReserveUserCodes() reserves the specified number of alarm codes for the application 
class ID specified without needing the application to actually set alarms. The un-used alarm 
codes for an application are released when it is deleted. For reserving more than 30 user codes
the application needs to have a privilege level of AEEGROUPID_RESERVECODE_LARGE.

Here are the differences between using IAlarmMgt and ISHELL_SetAlarm() to set alarms

1. The API ISHELL_SetAlarm allows to start an alarm for another class ID without any privilege check,
   IALARMMGR_SetAlarm will need a privilege level of AEEGROUPID_ALARM_ALL.
2. There is no check in ISHELL_SetAlarm for a valid class ID on a device to set alarm for. 
   IALARMMGR_SetAlarm will return EBADPARM for an invalid class ID.

See Also:
    ISHELL_SetAlarm

=======================================================================

=======================================================================
   METHOD DOCUMENTATION
=======================================================================
IALARMMGR_SetAlarm()

Description:
This function allows the caller to set a long-term alarm for an applet.
The 16-bit alarm user code is specified in pnUserCode as follows:
The first two bytes (MSB) specify the 16-bit (uint16) user code [in/out]. 
The next 2 bytes carry 16 bit flags. 

The least significant bit specifies if the user code is supplied by the
user or it needs to be generated by BREW, if the bit is set it specifies 
that the user code is given by the user. If the user code is generated by 
BREW it is returned in the first two bytes of pnUserCode. 

The next significant bit specifies if the alarm time specified is relative
or absolute. If the bit is set it means absolute time is specified.

Application can use helper functions IALARMMGR_SetUserCode() and 
IALARMMGR_GetUserCode() to set and extract user code values.

When the alarm expires, the applet is loaded and passed an EVT_ALARM event with the
16-bit nUserCode as the user parameter. If the applet is not active at the time
of the alarm, it is loaded but is not sent an EVT_APP_START event. If the applet
is to be activated, it must call ISHELL_StartApplet(). More than one alarm can be
set for an applet by calling this API more than once.

If the class ID is zero, the function returns the result for the current running app. 
If classID is non-zero and not current app, the caller should have a privilege level 
of AEEGROUPID_ALARM_ALL.

If the class ID specified in cls parameter is not valid on the device, EBADPARM 
is returned.

If user specifies a user code for the alarm and there is a alarm already active with 
that user code, the existing alarm will be cancelled and a new one started.

If an app wants to reserve more than 30 user codes it needs to have a privilege level 
of AEEGROUPID_RESERVECODE_LARGE

Prototype:
int IALARMMGR_SetAlarm
(
IAlarmMgr* me,
AEECLSID cls,
uint32* pnUserCode,
uint32 nSecs
)

Parameters:
me   : Pointer to the IAlarmMgr Interface object.
cls  : Applet class to call when the alarm expires.
pnUserCode  : [in/out]  pointer in which the generated 16-bit alarm user code is returned to/from the caller of API
              in the first 2 most significant bytes. The next 2 bytes are used as 16 bit flags. bit 1 if
              set specifies that user is supplied by user (else generated by BREW), bit 2 if set specifies that
              alarm seconds are for absolute time (else relative from now).
nSecss  : Number of seconds (relative from now or absolute) to set the alarm for.

Return Value:
SUCCESS: if successful.
EBADPARM: invalid parameter.
ENOMEMORY: insufficient memory.
ENORESOURCE: no free user code available
EPRIVLEVEL: user does not have privilege level for the operation.

Comments:
Here are the difference from ISHELL_SetAlarm():

1. The API ISHELL_SetAlarm allows to start an alarm for another class ID without any privilege check,
   IALARMMGR_SetAlarm will need a privilege level of AEEGROUPID_ALARM_ALL.
2. There is no check in ISHELL_SetAlarm for a valid class ID on a device to set alarm for. 
   IALARMMGR_SetAlarm will return EBADPARM for an invalid class ID.

Version:
Introduced BREW Client 3.1.4

See Also:
IALARMMGR_CancelAlarm()

=======================================================================
IALARMMGR_CancelAlarm()

Description:
This function cancels an alarm set through IALARMMGR_SetAlarm()/ISHELL_SetAlarm(). 
The proper class and 16-bit user code must be specified.

If the class ID is zero, the function returns the result for the current running app. 
If classID is non-zero and not current app, the caller should have a privilege level 
of AEEGROUPID_ALARM_ALL.

Prototype:
int IALARMMGR_CancelAlarm
(
IAlarmMgr* me,
AEECLSID cls,
uint16 nUserCode
)

Parameters:
me   :  Pointer to the IAlarmMgr Interface object.
cls  :  Applet class of alarm to cancel.
nUserCode  :  16-bit use code of alarm to cancel.

Return Value:
SUCCESS: if an alarm is found and canceled.
EFAILED: if an alarm cannot be found or canceled.
EPRIVLEVEL: user does not have privilege level for the operation.
EBADPARM: invalid class ID.

Comments:
None

Version:
Introduced BREW Client 3.1.4

See Also:
IALARMMGR_SetAlarm()


=======================================================================

IALARMMGR_SuspendAlarms()

Description:
This function allows the caller to suspend operation of the BREW
alarms subsystem.  This function does not impact applications' ability
to schedule alarms, it merely postpones any alarm notifications
until IALARMMGR_ResumeAlarms() is called, or the phone is reset.

To suspend alarms an application needs to have AEEGROUPID_ALARM_MGR 
privilege level.

Prototype:
int IALARMMGR_SuspendAlarms
(
IAlarmMgr* me
)

Parameters:
me  :  Pointer to the IAlarmMgr Interface object.

Return Value:
SUCCESS: if successful.
EPRIVLEVEL: insufficient privilege (must have AEEGROUPID_ALARM_MGR)
Other: AEEError if appropriate

Comments:
None

Version:
Introduced BREW Client 3.1.4

See Also:
IALARMMGR_ResumeAlarms()
IALARMMGR_SetAlarm()
IALARMMGR_CancelAlarm()

=======================================================================

IALARMMGR_ResumeAlarms()

Description:
This function allows the caller to resume operation of the BREW alarms
subsystem.  Any alarms that have already come due passed will be
scheduled for notification.

To resume alarms an application needs to have AEEGROUPID_ALARM_MGR 
privilege level.

Prototype:
int IALARMMGR_ResumeAlarms
(
IAlarmMgr* me
)

Parameters:
me  :  Pointer to the IAlarmMgr Interface object.

Return Value:
SUCCESS: if successful.
EPRIVLEVEL: insufficient privilege (must have AEEGROUPID_ALARM_MGR)
Other: AEEError if appropriate

Comments:
None

Version:
Introduced BREW Client 3.1.4

See Also:
IALARMMGR_SuspendAlarms()
IALARMMGR_SetAlarm()
IALARMMGR_CancelAlarm()

=======================================================================

IALARMMGR_AlarmsActive()

Description:
This function returns SUCCESS if an alarm is set for the application specified
by the class ID passed to the function. If the class ID is zero, the function returns
the result for the current running app. if classID is non-zero and not current app, 
the caller should have a privilege level of AEEGROUPID_ALARM_ALL.

Prototype:
int IALARMMGR_AlarmsActive
(
IAlarmMgr* me,
AEECLSID   cls
)

Parameters:
me  : Pointer to the IAlarmMgr Interface object.
cls : The class ID whose alarms are checked for being active.

Return Value:
SUCCESS: if there are pending alarms
ENOSUCH: if there are no pending alarms
EPRIVLEVEL: user does not have privilege level for the operation.
EBADPARM: invalid class ID.

Comments:
None

Version:
Introduced BREW Client 3.1.4

See Also:
IALARMMGR_ListAlarmCodes()

=======================================================================
IALARMMGR_ListAlarmCodes()

Description:
This function works in two modes - either to list reserved user codes
(list mode) or to query if the specified user code is reserved (query mode).
In list mode this function lists all the user codes reserved for the application 
regardless of whether or not that alarm is active.

If the class ID is zero, the function returns the result for the current running app. 
If classID is non-zero and not current app, the caller should have a privilege level 
of AEEGROUPID_ALARM_ALL.

Prototype:
int IALARMMGR_ListAlarmCodes
(
IAlarmMgr* me,
AEECLSID cls,
uint16* pnUserCodes,
uint32* pnCount
)

Parameters:
me      : Pointer to the IAlarmMgr Interface object.
cls     : The class whose reserved alarm user codes are queried or are to be listed.
pnUserCode : [in/out]
             Null on IN: pnCount contains number of reserved alarm user codes on out.
             Valid on IN: if pnCount not null - max pnCount number of alarm codes reserved 
             are returned in it.
             Valid on IN: if pnCount null - the alarm user code is used to query if it is
             reserved. Value unchanged on OUT.
             
             Caller is responsible for allocating enough memory to store max pnCount alarm 
             user codes. Caller is responsible for deallocation memory.
             
pnCount : [in/out]
          Null on IN: alarm user code specified in pnUserCode is queried if reserved.
          Valid on IN: specifies the max number of alarm user codes to return. On OUT has
          the total number of reserved user codes if pnUserCode is null.
          
Return Value:
SUCCESS : if there are reserved alarm codes for the application (list mode) or the queried
          alarm user code is reserved (query mode).
ENOSUCH : no alarm codes are reserved for the application (list mode) or the queried alarm
          user code is not reserved (query mode).
EPRIVLEVEL: user does not have privilege level for the operation.

Comments:
None

Version:
Introduced BREW Client 3.1.4

See Also:
IALARMMGR_AlarmsActive()

=======================================================================
IALARMMGR_IsCodeReserved()

Description:
This function checks if the specified user code is reserved by the calling
application.

Prototype:
boolean IALARMMGR_IsCodeReserved
(
IAlarmMgr* me,
uint16 nUserCode
)

Parameters:
me      : Pointer to the IAlarmMgr Interface object.
nUserCode : The alarm user code checked if being reserved.

Return Value:
TRUE : if alarm user code is reserved by the app.
FALSE: if alarm user code is not reserved by the app.

Comments:
None

Version:
Introduced BREW Client 3.1.4

See Also:
IALARMMGR_ListAlarmCodes()

=======================================================================
IALARMMGR_ReserveUserCodes()

Description:
This function reserves the specified number of alarm codes for the application 
class ID specified without needing the application to actually set alarms. The 
un-used alarm codes for an application are released when it is deleted.

If the class ID is zero, the function returns the result for the current running app. 
If classID is non-zero and not current app, the caller should have a privilege level 
of AEEGROUPID_ALARM_ALL.

If an app wants to reserve more than 30 user codes it needs to have a privilege level
of AEEGROUPID_RESERVECODE_LARGE.

Prototype:
int IALARMMGR_ReserveUserCodes
(
IAlarmMgr* me,
AEECLSID cls, 
uint16* pnUserCodes, 
uint32* pnCount
)

Parameters:
me      : Pointer to the IAlarmMgr Interface object.
cls     : The class for which the alarm codes are to be reserved.
pnUserCodes :[out] Contains the generated user codes on function return.
            It is the callers responsibility to allocate memory to contain
            nCount number of user codes. Deallocation is also the callers
            responsibility.
pnCount :[in/out] On IN specifies the number of user codes needed. On
               OUT specifies the number of user codes generated and returned.

Return Value:
SUCCESS : if alarm user code reservation was successful.
EFAILED : if alarm user code reservation failed.
EPRIVLEVEL : user does not have privilege level for the operation.
ENOTALLOWED : app does not have the privilege (AEEGROUPID_RESERVECODE_LARGE) 
             to reserve specified number of user codes.

Comments:
None

Version:
Introduced BREW Client 3.1.4

See Also:
IALARMMGR_AlarmsActive()

=======================================================================
IALARMMGR_AddRef()

Description:
This function increments the reference count of the IAlarmMgr object. This
allows the object to be shared by multiple callers. The object is freed when the
reference count reaches zero.

Prototype:
uint32 IAlarmMgr_AddRef
(
IAlarmMgr* me
)

Parameters:
me      : Pointer to the IAlarmMgr Interface object.

Return Value:
Returns the incremented reference count for the object.

Comments:
None

Version:
Introduced BREW Client 3.1.4

See Also:
IALARMMGR_Release()

=======================================================================
IALARMMGR_Release()

Description:
This function decrements the reference count of IAlarmMgr object. The object is freed from
memory and is no longer valid once the reference count reaches zero.

Prototype:
uint32 IAlarmMgr_Release
(
IAlarmMgr* me
)

Parameters:
me      : Pointer to the IAlarmMgr Interface object.

Return Value:
Returns the decremented reference count (decrement by one) of the object. If the
reference count reaches zero the object is deleted from memory.

Comments:
None

Version:
Introduced BREW Client 3.1.4

See Also:
IALARMMGR_AddRef()

=======================================================================
IALARMMGR_QueryInterface()

Description:
This function is inherited from IQI_QueryInterface()

Prototype:
int IAlarmMgr_QueryInterface
(
IAlarmMgr* me,
AEECLSID cls,
void** ppo
)

Parameters:
me      : Pointer to the IAlarmMgr Interface object.
cls     : class ID of the class queried for.
ppo     : variable in which the pointer of queried object is returned.

Return Value:
SUCCESS: if queried class is supported.
ECLASSNOTSUPPORT: if queried class is not supported.

Comments:
None

Version:
Introduced BREW Client 3.1.4

See Also:
IALARMMGR_AddRef()
IALARMMGR_Release()

=======================================================================

=======================================================================
  HELPER DOCUMENTATION
=======================================================================
IALARMMGR_SetUserCode()

Description:
This inline helper function sets user code values.

Prototype:
uint32 IALARMMGR_SetUserCode
(
uint16 nUserCode, 
boolean bUserCodeProvided, 
boolean bAbsoluteTime
)

Parameters:
nUserCode : 16-bit alarm user code.
bUserCodeProvided : if set specifies that user code is supplied by user,
                    else generated by BREW.
bAbsoluteTime : if set specifies that alarm seconds are for absolute time, 
                else relative from now.

Return Value:
uint32, where the first two bytes (MSB) specify nUserCode, and 
the next 2 bytes carry 16 bit flags. The least significant bit specifies 
bUserCodeProvided, the next significant bit specifies bAbsoluteTime.

Comments:
None

See Also:
IALARMMGR_GetUserCode()


=======================================================================
IALARMMGR_GetUserCode()

Description:
This inline helper function extracts user code values.

Prototype:
uint16 IALARMMGR_GetUserCode 
(
uint32 nUserCode
)

Parameters:
nUserCode : returned code value from IALARMMGR_SetUserCode.

Return Value:
16-bit alarm user code..

Comments:
None

See Also:
IALARMMGR_SetUserCode()


=======================================================================

*/

#endif /* AEEALARMMGR_H */


#ifndef AEEPEN_H
#define AEEPEN_H

/*==============================================================================
// �ļ���
//        AEEPen.h
//        ��Ȩ����(c) 2004 TCL Mobile Communication CO.,LTD.
//        
// �ļ�˵����
//        
// ���ߣ�jyh
// �������ڣ�2004-03-31
// ��ǰ�汾��init
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��       �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
2004-03-31       jyh            Init version.
==============================================================================*/

// Pen event struct that deliver by dwParam of 
// EVT_PEN_UP ,EVT_PEN_DOWN and EVT_PEN_MOVE
//       dwXPos
//        _________________
//dwYPos |Special Event    |
//       |_________________|(ICON)
//       |(0,0)            |
//       |                 |(Display Screen)
//       |                 |
//       |                 |
//       |                 |
//       |                 |
//       |_________________|
//
#include "AEE.h"
#include "AEEText.h"
       
#define AEE_GET_XPOS(dw)      AEE_GET_X(dw)
#define AEE_GET_YPOS(dw)      AEE_GET_Y(dw)
#define AEE_GET_TIMESTAMP(dw) (((PenEvent *)dw)->dwTimeStamp)

#define AEE_TM_ZIMU           (AEE_TM_USER + 1)
#define AEE_TM_FUHAO          (AEE_TM_ZIMU + 1)
#define AEE_TM_HANSTROKE      (AEE_TM_FUHAO + 1)
#define AEE_TM_TSIM           (AEE_TM_HANSTROKE + 1)

#define AEECLSID_FONTUSER1    (AEECLSID_OEM + 0xF09)
#define AEECLSID_FONTUSER2    (AEECLSID_OEM + 0xF0A)

/*
typedef struct _AEEAutoRepeat
{
    uint16    wItem;        // Repeat item
    uint32    dwStart;      // start repeat time
    uint32    dwRate;       // Repeat rate
}AEEAutoRepeat;
*/
#endif //AEEPEN_H


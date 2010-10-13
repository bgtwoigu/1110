#ifndef OEMTPANEL_H
#define OEMTPANEL_H
/*===========================================================================

              Touch Panel BREW OEM layer implement  H E A D E R  

DESCRIPTION
   ����Ϊ����������У׼�ṩ�ӿں�����ͷ�ļ������ļ�����������У׼�Ľӿں�
   ʵ������ĳ����ȡ�
REFERENCES
  ������������ģ������Draft.doc��
  ��ģ��У׼���Draft.doc��
  ������ģ�ͷ���.xls��

                  Copyright (c) 2004 Azlun Xie
                 by T.M.Ltd. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR FILE

   This section contains comments describing changes made to this file.
   Notice that changes are listed in reverse chronological order.
  
when       who     what, where, why
--------   ---     -------------------------------------------------------
12/15/04   xzl     add functions declaration supporting CaliApp.
12/10/04   xzl     ��ʼ�汾��

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "target.h"
#include "customer.h"

#include "touchpanel.h"
#include "touchpanel_cali.h"


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*-------------------------------------------------------------------
Structure prototypes
-------------------------------------------------------------------*/


/*-------------------------------------------------------------------
Pubilc Data 
-------------------------------------------------------------------*/


/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

//ת��UI����ΪӲ�����꣬��ȥ��У׼Ӱ�죬Э��У׼
boolean OEMTPanel_CalculateUIToHW(MTMATRIX_PTR pUIPointBuf,
                                  MTMATRIX_PTR pHWPointBuf,
                                  CalibrateParam *pCParam
                                 );
//ֻȥ��OEM��BREW����ƫ��
boolean OEMTPanel_CalculateUIToOEM(MTMATRIX_PTR pUIPointBuf,
                                   MTMATRIX_PTR pOEMPointBuf
                                  );

//����У׼����
boolean OEMTPanel_CalCalibrateParam(MTMATRIX_PTR pLCDPointBuf,
                                    MTMATRIX_PTR pTSCPointBuf,
                                    CalibrateParam *pCParam
                                   );
//����У׼����
boolean OEMTPanel_SetCalibrateParam(CalibrateParam *pszParam);
//����У׼����
boolean OEMTPanel_SetCalibrateParamEx(CalibrateParam *pszParam);
   
//��õ�ǰ����ʹ�õ�У׼����
boolean OEMTPanel_GetCalibrateParam(CalibrateParam *pdstParam);
//����У׼������NV
boolean OEMTPanel_StoreNVCalibrateParam(CalibrateParam *pszParam);
//��NV��ȡУ׼����
boolean OEMTPanel_RestoreNVCalibrateParam(CalibrateParam *pdstParam);

//��ñ����±ʵ�Driver������¼
void OEMTPanel_CoorGetPendownPoint(matrix_data_type *px,matrix_data_type *py);

void OEMTPanel_CoorCalibration(int x_TSC,int y_TSC,int *ret_x,int *ret_y );

//reset the driver while the driver is hanged.
void OEMTPanel_ResetDriver(void);

#endif //OEMTPANEL_H

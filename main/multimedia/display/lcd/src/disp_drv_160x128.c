/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                   L C D    D I S P L A Y    D R I V E R

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS
  disp_drv_init

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                         INCLUDE FILES FOR MODULE

===========================================================================*/
#include "disp_drv.h"

/*============================================================================

                 CONSTANT AND MACRO DEFINITIONS FOR MODULE

============================================================================*/

#define DISP_DRV_WIDTH             160
#define DISP_DRV_HEIGHT            128

/*============================================================================

                        STRUCTURE TYPES FOR MODULE

============================================================================*/

extern boolean disp_ili9163c_tft177_rot(disp_drv_ic_type *pdispic);

/*============================================================================

                           VARIABLE DECLARATION

============================================================================*/

static disp_ic_install_type     disp_ic_install[] = 
{
    disp_ili9163c_tft177_rot, //Ĭ����������ŵ���󣬲��Ҳ����IC��ID
    NULL
};

#include "disp_drv_generic.h"

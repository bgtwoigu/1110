/*==============================================================================
// �ļ���
//        ContAppBuildMenu.c
//        2007-12-05  ����汾(Draft Version)
//        ��Ȩ����(c) 2007 Anylook
//        
// �ļ�˵����
//        
// ���ߣ�2007-12-05
// �������ڣ�Gemsea
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ    ��      ��   ��   ��     �޸����ݡ�λ�ü�ԭ��
--------      -----------     -----------------------------------------------
07-12-05                   ��ʼ�汾
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


/*==============================================================================
                                 ���Ͷ���
==============================================================================*/

/*lint -e506 -e774 -e737 -e715*/
/*==============================================================================
                                 ��������
==============================================================================*/
/////////////////
//Helper function
//////

/*=============================================================================

FUNCTION:CContApp_BuildOptsMenu

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
int CContApp_BuildOptsMenu(CContApp *pMe, IMenuCtl *pMenuCtl, boolean bAll)
{
    CtlAddItem ai;
    AEERect rc={0};
    AEERect Temprc={0};
    ASSERT(pMe != NULL); 
    
    FARF(ADDR, ("CContApp_BuildOptsMenu bAll%d ", bAll));
    IMENUCTL_SetPopMenuRect(pMenuCtl);
    
    MSG_FATAL("CContApp_BuildOptsMenu",0,0,0);
    
    // Check Parameter
    if(!pMenuCtl)
    {
        return EBADPARM;
    }
    
    // Clear items
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    // Init items
    MEMSET(&ai, 0, sizeof(ai));
    ai.pszResText = CONTAPP_RES_FILE_LANG;
    
    if(bAll)
    {
        if(!IS_ZERO_REC())
        {
            // 1
            if(!pMe->m_boptaleadyView)
            {
	            ai.wText       = IDS_VIEW;
	            ai.wItemID   = IDI_OPTS_MENU_VIEW;
	            
	            if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
	            {
	                FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
	                return EFAILED;
	            }    
			}
            // 2
            ai.wText       = IDS_EDIT;
            ai.wItemID   = IDI_OPTS_MENU_EDIT;
            
            if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
            {
                FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
                return EFAILED;
            }    

            // 3
            ai.wText       = IDS_CALL;
            ai.wItemID   = IDI_OPTS_MENU_CALL;
            
            if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
            {
                FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
                return EFAILED;
            }
				
            // 4
            if(!pMe->m_boptaleadyView)
            {
	            ai.wText       = IDS_ADDNEW;
	            ai.wItemID   = IDI_OPTS_MENU_ADDNEW;
	            
	            if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
	            {
	                FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
	                return EFAILED;
	            }
            }

     /*
#ifdef FEATRUE_SET_IP_NUMBER
            ai.wText       = IDS_IPCALL;
            ai.wItemID   = IDI_OPTS_MENU_IPCALL;
            
            if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
            {
                FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
                return EFAILED;
            } 
#endif
*/
            // 5
#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
            ai.wText       = IDS_SENDSMS;
            ai.wItemID   = IDI_OPTS_MENU_SENDSMS;
            
            if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
            {
                FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
                return EFAILED;
            }     

            // 6
            ai.wText       = IDS_SENDDIRECTORY;
            ai.wItemID   = IDI_OPTS_MENU_SENDDIRECTORY;
            
            if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
            {
                FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
                return EFAILED;
            }     
#endif   

#if defined(FEATURE_SUPPORT_BT_APP) && defined(FEATURE_SUPPORT_VCARD)
//#if	defined(FEATURE_SUPPORT_VCARD)	//Modify by zzg 2011_10_25
            // 7
            ai.wText       = IDS_SEND_VIA_BLUETOOTH;
            ai.wItemID   = IDI_OPTS_MENU_BLUETOOTH;
            
            if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
            {
                FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
                return EFAILED;
            }       
#endif            

#ifdef FEATURE_RUIM_PHONEBOOK
            if(IsRunAsUIMVersion() && pMe->m_bADNRUIMSupport)
            {
                if(IS_RUIM_REC(pMe->m_wSelectCont))
                {
                    ai.wText = IDS_COPY_TOPHONE;
                }
                else
                {
                    ai.wText = IDS_COPY_TOUIMCARD;
                }
                ai.wItemID   = IDI_OPTS_MENU_COPY;
                
                if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
                {
                    FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
                    return EFAILED;
                }    

                //8 / 9
                if(IS_RUIM_REC(pMe->m_wSelectCont))
                {
                    ai.wText = IDS_MOVE_TOPHONE;
                }
                else
                {
                    ai.wText = IDS_MOVE_TOUIMCARD;
                }
                ai.wItemID   = IDI_OPTS_MENU_MOVE;
                
                if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
                {
                    FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
                    return EFAILED;
                }    
            }
#endif

            // 9/10
            ai.wText       = IDS_DELETE;
            ai.wItemID   = IDI_OPTS_MENU_DELETE;
            
            if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
            {
                FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
                return EFAILED;
            }            
        }
    }
    IMENUCTL_SetProperties(pMenuCtl, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY);
    (void)IMENUCTL_Redraw(pMenuCtl);
    return SUCCESS;
}// CContApp_BuildOptsMenu

/*=============================================================================

FUNCTION:CContApp_BuildMainMenuOptsMenu

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
int CContApp_BuildMainMenuMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    CtlAddItem ai;
    
    ASSERT(pMe != NULL); 
    
     // Check Parameter
    if(!pMenuCtl)
    {
        return EBADPARM;
    }
    
    // Clear items
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    // Init items
    MEMSET(&ai, 0, sizeof(ai));
    ai.pszResText = CONTAPP_RES_FILE_LANG;

// 1
	#if defined (FEATURE_VERSION_C337) || defined(FEATURE_VERSION_IC241A_MMX)
	ai.wText       = IDS_PHONEBOOK;
	#else
	ai.wText       = IDS_APPLET;
	#endif
    
    ai.wItemID   = IDI_MAINMENU_MENU_DIRECTORTLIST;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
       FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
       return EFAILED;
    }    

    ai.wText       = IDS_FIND;
    ai.wItemID   = IDI_MAINMENU_MENU_FIND;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
       FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
       return EFAILED;
    }    
// 3
/*
    ai.wText       = IDS_DELETE;
    ai.wItemID   = IDI_MAINMENU_MENU_DELETE;
           
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    }        */
// 4
    ai.wText       = IDS_ADDNEW;
    ai.wItemID   = IDI_MAINMENU_MENU_ADDNEW;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
     }   
// 5


/*
#ifdef FEATURE_RUIM_PHONEBOOK
    if( IsRunAsUIMVersion() )
    {
        ai.wText       = IDS_SETTING;
        ai.wItemID   = IDI_MAINMENU_MENU_SETTING;

        if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
        {
            FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
            return EFAILED;
        }  
 }
#endif *//* FEATURE_RUIM_PHONEBOOK */    
#if defined (FEATURE_VERSION_C337) || defined(FEATURE_VERSION_IC241A_MMX)
	ai.wText       = IDS_CALLER_GROUPS;
#else
    ai.wText       = IDS_INPUT_GROUP;
#endif
    ai.wItemID     = IDI_MAINMENU_MENU_GROUP;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    } 

    ai.wText       = IDS_SPEEDDIAL;
    ai.wItemID     = IDI_MAINMENU_MENU_SPEEDDIAL;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    } 
/*
    ai.wText       = IDS_CHECKCAPACITY;
    ai.wItemID   = IDI_MAINMENU_MENU_CHECKCAPACITY;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
     }  */


//#ifdef FEATURE_RUIM_PHONEBOOK
    //if( IsRunAsUIMVersion() )
    //{
    	#ifdef FEATURE_VERSION_W317A
		ai.wText	= IDS_PHONEBOOK_SETTINGS;
		#else
        ai.wText       = IDS_CONTACTS_MANAGEMENT;
		#endif
		
        ai.wItemID   = IDI_MAINMENU_MENU_CONTACT_MANAGEMENT;
                
        if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
        {
            FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
            return EFAILED;
        }      
#if (defined(FEATURE_VERSION_S1000T) || defined(FEATURE_VERSION_C316))    //add by yangdecai 07-19
		;
#else
#ifdef FEATURE_OEMOMH 
        ai.wText       = IDS_EMERGENCY_CALL;
        ai.wItemID   = IDI_MAINMENU_MENU_EMERGENCY_CALL;
                
        if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
        {
            FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
            return EFAILED;
        }    
#endif
#endif
  //}
//#endif /* FEATURE_RUIM_PHONEBOOK */    
#ifdef FEATURE_VERSION_K212
    (void)IMENUCTL_Redraw(pMenuCtl);
#endif
    return SUCCESS;
}// CContApp_BuildOptsMenu

/*=============================================================================

FUNCTION:CContApp_BuildNumFldMenuMenu

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
int CContApp_BuildNumFldMenuMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    CtlAddItem ai;
    
    ASSERT(pMe != NULL); 
    
     // Check Parameter
    if(!pMenuCtl)
    {
        return EBADPARM;
    }
    
    // Clear items
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    // Init items
    MEMSET(&ai, 0, sizeof(ai));
    ai.pszResText = CONTAPP_RES_FILE_LANG;
    ai.pszResImage = AEE_APPSCOMMONRES_IMAGESFILE;

    ai.wText       = IDS_MOBILE;
    ai.wItemID   = IDI_ADDNEW_MENU_MOBILE;
    ai.wImage    = IDB_PHONE;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
       FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
       return EFAILED;
    }    

    ai.wText       = IDS_HOME;
    ai.wItemID   = IDI_ADDNEW_MENU_HOME;
    ai.wImage    = IDB_HOMENUM;
          
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    }        

    ai.wText       = IDS_OFFICE;
    ai.wItemID   = IDI_ADDNEW_MENU_OFFICE;
    ai.wImage    = IDB_WORKNUM;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
     }   

    ai.wText       = IDS_FAX;
    ai.wItemID   = IDI_ADDNEW_MENU_FAX;
    ai.wImage    = IDB_FAXNUM;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    }

    return SUCCESS;
}

/*=============================================================================

FUNCTION:CContApp_BuildManagementMenu

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
int CContApp_BuildManagementMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    CtlAddItem ai;
    
    ASSERT(pMe != NULL); 
    
     // Check Parameter
    if(!pMenuCtl)
    {
        return EBADPARM;
    }
    
    // Clear items
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    // Init items
    MEMSET(&ai, 0, sizeof(ai));
    ai.pszResText = CONTAPP_RES_FILE_LANG;
    //ai.pszResImage = AEE_APPSCOMMONRES_IMAGESFILE;

#ifdef FEATURE_RUIM_PHONEBOOK
    if( IsRunAsUIMVersion() && pMe->m_bADNRUIMSupport )
    {
        ai.wText       = IDS_MOVE_CONTACTS;
        ai.wItemID   = IDS_MOVE_CONTACTS;
        //ai.wImage    = IDB_PHONE;
                
        if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
        {
           FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
           return EFAILED;
        }    

        ai.wText       = IDS_COPY_CONTACTS;
        ai.wItemID   = IDS_COPY_CONTACTS;
        //ai.wImage    = IDB_HOMENUM;
              
        if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
        {
            FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
            return EFAILED;
        }        
    }
#endif

#if defined(FEATURE_SUPPORT_BT_APP) && defined(FEATURE_SUPPORT_VCARD)
//#if defined(FEATURE_SUPPORT_VCARD) 	//Modify by zzg 2011_10_25
        ai.wText       = IDS_BT_COPYN;
        ai.wItemID   = IDS_BT_COPYN;
        //ai.wImage    = IDB_FAXNUM;
                
        if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
        {
            FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
            return EFAILED;
        }
#endif

    ai.wText       = IDS_DELETE_CONTACTS;
    ai.wItemID   = IDS_DELETE_CONTACTS;
    //ai.wImage    = IDB_WORKNUM;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
     }   

#ifdef FEATURE_RUIM_PHONEBOOK
    if( IsRunAsUIMVersion() && pMe->m_bADNRUIMSupport )
    {
    	#ifdef FEATURE_VERSION_W317A
		ai.wText	= IDS_STORAGE;
        ai.wItemID	= IDS_STORAGE;
		#else
        ai.wText       = IDS_SAVETO;
        ai.wItemID   = IDS_SAVETO;
		#endif
		
        //ai.wImage    = IDB_FAXNUM;
                
        if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
        {
            FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
            return EFAILED;
        }

		#ifdef FEATURE_VERSION_W317A
		ai.wText	= IDS_DISPLAY_OPTION;
        ai.wItemID	= IDS_DISPLAY_OPTION;
		#else
        ai.wText       = IDS_VIEWTYPE;
        ai.wItemID   = IDS_VIEWTYPE;
		#endif
		
        //ai.wImage    = IDB_FAXNUM;
                
        if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
        {
            FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
            return EFAILED;
        }
        
    }
#endif

    ai.wText       = IDS_CONTACTS_MEMORY_STUTAS;
    ai.wItemID   = IDS_CONTACTS_MEMORY_STUTAS;
    //ai.wImage    = IDB_FAXNUM;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    }

    return SUCCESS;
}//CContApp_BuildManagementMenu

/*=============================================================================

FUNCTION:CContApp_BuildDeleteSelectMenu

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
int CContApp_BuildDeleteSelectMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    CtlAddItem ai;
    
    ASSERT(pMe != NULL); 
    
     // Check Parameter
    if(!pMenuCtl)
    {
        return EBADPARM;
    }
    
    // Clear items
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    // Init items
    MEMSET(&ai, 0, sizeof(ai));
    ai.pszResText = CONTAPP_RES_FILE_LANG;
    //ai.pszResImage = AEE_APPSCOMMONRES_IMAGESFILE;

    ai.wText       = IDS_PHONE_CAPACITY;
    ai.wItemID   = IDS_PHONE_CAPACITY;
    //ai.wImage    = IDB_WORKNUM;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    }   

#ifdef FEATURE_RUIM_PHONEBOOK
    if( IsRunAsUIMVersion() && pMe->m_bADNRUIMSupport )
    {
        ai.wText       = IDS_UIM_CARD;
        ai.wItemID   = IDS_UIM_CARD;
        //ai.wImage    = IDB_PHONE;
                
        if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
        {
           FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
           return EFAILED;
        }    
    
    }
#endif

    ai.wText       = IDS_DELETE_ALL;
    ai.wItemID   = IDS_DELETE_ALL;
    //ai.wImage    = IDB_WORKNUM;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    }   

    return SUCCESS;
}//CContApp_BuildManagementMenu

/*=============================================================================

FUNCTION:CContApp_BuildAddMenuMenu

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
int CContApp_BuildAddMenuMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    CtlAddItem ai;
    
    ASSERT(pMe != NULL); 
    
     // Check Parameter
    if(!pMenuCtl)
    {
        return EBADPARM;
    }
    
    // Clear items
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    // Init items
    MEMSET(&ai, 0, sizeof(ai));
    ai.pszResText = CONTAPP_RES_FILE_LANG;
    ai.pszResImage = AEE_APPSCOMMONRES_IMAGESFILE;


    //ai.wText       = IDS_NAME;
    ai.wItemID   = IDI_ADDNEW_MENU_NAME;
    ai.wImage    = IDB_NAME;
    
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
       FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
       return EFAILED;
    }    
	

    //ai.wText       = IDS_MOBILE;
    ai.wItemID   = IDI_ADDNEW_MENU_MOBILE;
    ai.wImage    = IDB_PHONE;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
       FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
       return EFAILED;
    }    

    if(!IsRunAsUIMVersion() || CONTCFG_SAVETYPE_PHONE == pMe->m_nSaveType)
    {
    //ai.wText       = IDS_HOME;
    ai.wItemID   = IDI_ADDNEW_MENU_HOME;
    ai.wImage    = IDB_HOMENUM;
          
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    }        

    //ai.wText       = IDS_OFFICE;
    ai.wItemID   = IDI_ADDNEW_MENU_OFFICE;
    ai.wImage    = IDB_WORKNUM;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
     }   
	
	

    //ai.wText       = IDS_FAX;
    ai.wItemID   = IDI_ADDNEW_MENU_FAX;
    ai.wImage    = IDB_FAXNUM;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    } 

	ai.wItemID	 = IDI_ADDNEW_MENU_SELECTGROUP;
	ai.wImage	 = IDB_GROUP;
				
	if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
	{
		FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
		return EFAILED;
	}	

    ai.wItemID   = IDI_ADDNEW_MENU_EMAIL;
    ai.wImage    = IDB_EMAIL;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
     }  

		ai.wItemID	 = IDI_ADDNEW_MENU_ADDRESS;
		ai.wImage	 = IDB_ADDRESS;
				
		if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
		{
			FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
			return EFAILED;
		 } 

    ai.wItemID   = IDI_ADDNEW_MENU_URL;
    ai.wImage    = IDB_URL;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
     }  

    ai.wItemID   = IDI_ADDNEW_MENU_REMARK;
    ai.wImage    = IDB_NOTES;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
     }  
    ai.wItemID   = IDI_ADDNEW_MENU_SELECTRINGTONE;
    ai.wImage    = IDB_RING;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
     }  
    }
      
    //(void)IMENUCTL_Redraw(pMenuCtl);
    return SUCCESS;
}// CContApp_BuildAddMenuMenu

/*=============================================================================

FUNCTION:CContApp_BuildEditMenuMenu

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
int CContApp_BuildEditMenuMenu(CContApp *pMe, IMenuCtl *pMenuCtl, boolean bAll)
{
    CtlAddItem ai;
    
    ASSERT(pMe != NULL); 

     // Check Parameter
    if(!pMenuCtl)
    {
        return EBADPARM;
    }
    
    // Clear items
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    // Init items
    MEMSET(&ai, 0, sizeof(ai));
    ai.pszResText = CONTAPP_RES_FILE_LANG;
    ai.pszResImage = AEE_APPSCOMMONRES_IMAGESFILE;

    if(bAll)
    {
       //ai.wText       = IDS_NAME;
       ai.wItemID   = IDI_EDIT_MENU_NAME;
       ai.wImage    = IDB_NAME;
       
       if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
       {
          FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
          return EFAILED;
       }    
   
       //ai.wText       = IDS_MOBILE;
       ai.wItemID   = IDI_EDIT_MENU_MOBILE;
       ai.wImage    = IDB_PHONE;
               
       if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
       {
          FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
          return EFAILED;
       }    

       if(!IsRunAsUIMVersion() || !IS_RUIM_REC(pMe->m_wEditCont))
       {
           //ai.wText       = IDS_HOME;
           ai.wItemID   = IDI_EDIT_MENU_HOME;
           ai.wImage    = IDB_HOMENUM;
                 
           if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
           {
               FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
               return EFAILED;
           }        
       
           //ai.wText       = IDS_OFFICE;
           ai.wItemID   = IDI_EDIT_MENU_OFFICE;
           ai.wImage    = IDB_WORKNUM;
                   
           if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
           {
               FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
               return EFAILED;
            }   
		    //ai.wText       = IDS_FAX;
           ai.wItemID   = IDI_EDIT_MENU_FAX;
           ai.wImage    = IDB_FAXNUM;
                   
           if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
           {
               FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
               return EFAILED;
           } 
       
            //ai.wText       = IDS_INPUT_GROUP;
           ai.wItemID   = IDI_EDIT_MENU_GROUP;
           ai.wImage    = IDB_GROUP;
                   
           if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
           {
               FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
               return EFAILED;
            } 
		   
      	 //ai.wText       = IDS_EMAIL;
           ai.wItemID   = IDI_EDIT_MENU_EMAIL;
           ai.wImage    = IDB_EMAIL;
                   
           if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
           {
               FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
               return EFAILED;
            }
           //ai.wText       = IDS_ADDRESS;
           ai.wItemID   = IDI_EDIT_MENU_ADDRESS;
           ai.wImage    = IDB_ADDRESS;
                   
           if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
           {
               FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
               return EFAILED;
            }  

           //ai.wText       = IDS_URL;
           ai.wItemID   = IDI_EDIT_MENU_URL;
           ai.wImage    = IDB_URL;
                   
           if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
           {
               FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
               return EFAILED;
            }  
       
           //ai.wText       = IDS_NOTES;
           ai.wItemID   = IDI_EDIT_MENU_REMARK;
           ai.wImage    = IDB_NOTES;
                   
           if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
           {
               FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
               return EFAILED;
            }  
       
           
           #ifndef FEATURE_VERSION_K212
           //ai.wText       = IDS_RING;
           ai.wItemID   = IDI_EDIT_MENU_RINGTONE;
           ai.wImage    = IDB_RING;
           #endif       
           if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
           {
               FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
               return EFAILED;
            }
       }
    }
    else
    {
    //name 
        if(!(STARTMETHOD_SELECTFIELD == pMe->m_eStartMethod && pMe->m_wSelFldType == MULTI_SELECT_CONTENT))
        {
            if(pMe->m_pAddNewName)
            {
              ai.wText       = IDS_NAME;
              ai.wItemID   = IDI_EDIT_MENU_NAME;
              ai.wImage    = IDB_NAME;
              
              if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
              {
                 FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
                 return EFAILED;
              }


              IMENUCTL_SetItemText(pMenuCtl, IDI_EDIT_MENU_NAME, NULL, 0, pMe->m_pAddNewName);
           }
        }
        
//mobile num
    if(pMe->m_pAddNewMobile)
        {
          ai.wText       = IDS_MOBILE;
          ai.wItemID   = IDI_EDIT_MENU_MOBILE;
          ai.wImage    = IDB_PHONE;
          
          if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
          {
             FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
             return EFAILED;
          }


       	IMENUCTL_SetItemText(pMenuCtl, IDI_EDIT_MENU_MOBILE, NULL, 0, pMe->m_pAddNewMobile);
       }

///////////////////////////////////// IF RUIM CARD 
       if(!IS_RUIM_REC(pMe->m_wEditCont))
       {
           //AECHAR *m_pString = NULL;
           int        len = 0;
           uint16 ResID = 0;
           //AEERingerInfo  ri;
           
//home num         
           if(pMe->m_pAddNewHome)
             {
               ai.wText       = IDS_HOME;
               ai.wItemID   = IDI_EDIT_MENU_HOME;
               ai.wImage    = IDB_HOMENUM;
               
               if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
               {
                  FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
                  return EFAILED;
               }
     
           IMENUCTL_SetItemText(pMenuCtl, IDI_EDIT_MENU_HOME, NULL, 0, pMe->m_pAddNewHome);
            }
           
//office num     
           if(pMe->m_pAddNewOffice)
             {
               ai.wText       = IDS_OFFICE;
               ai.wItemID   = IDI_EDIT_MENU_OFFICE;
               ai.wImage    = IDB_WORKNUM;
               
               if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
               {
                  FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
                  return EFAILED;
               }
     
           IMENUCTL_SetItemText(pMenuCtl, IDI_EDIT_MENU_OFFICE, NULL, 0, pMe->m_pAddNewOffice);
            }
//fax     
           if(pMe->m_pAddNewFax)
             {
               ai.wText       = IDS_FAX;
               ai.wItemID   = IDI_EDIT_MENU_FAX;
               ai.wImage    = IDB_FAXNUM;
               
               if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
               {
                  FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
                  return EFAILED;
               }
     
                IMENUCTL_SetItemText(pMenuCtl, IDI_EDIT_MENU_FAX, NULL, 0, pMe->m_pAddNewFax);
            }

 // insert content into wms don't need group and ringtone
            if(!(STARTMETHOD_SELECTFIELD == pMe->m_eStartMethod && pMe->m_wSelFldType == MULTI_SELECT_CONTENT))
            {
                // group
                AECHAR pGroup[MAX_INPUT_NAME_EN + 1];
                pGroup[0] = ONEDIAL_NULL;
                MSG_FATAL("pGroup[0] = ONEDIAL_NULL",0,0,0);
                switch(pMe->m_nGroupCat)
                {
                    case AEE_ADDR_CAT_VIP:
                        pMe->m_wSelectGroup = CONTCFG_GROUP1;
                        ResID = IDS_GROUP_VIP;
                        break;
                    case AEE_ADDR_CAT_HOME:
                        pMe->m_wSelectGroup = CONTCFG_GROUP2;
                        ResID = IDS_GROUP_HOME;
                        break;
                    case AEE_ADDR_CAT_WORK:
                        pMe->m_wSelectGroup = CONTCFG_GROUP3;
                        ResID = IDS_GROUP_WORK;
                        break;
                    case AEE_ADDR_CAT_FRIEND:
                        pMe->m_wSelectGroup = CONTCFG_GROUP4;
                        ResID = IDS_GROUP_FRIEND;
                        break;
                    case AEE_ADDR_CAT_PERSONAL:
                        pMe->m_wSelectGroup = CONTCFG_GROUP5;                       
                        ResID = IDS_GROUP_PERSONAL;  
                        break;
                    case AEE_ADDR_CAT_BUSINESS:
                        pMe->m_wSelectGroup = CONTCFG_GROUP6;                       
                        ResID = IDS_GROUP_BUSINESS;  
                        break;
                    default:
                        pMe->m_wSelectGroup = 0;                      
                        ResID = IDS_GROUP_NONE_STRING;
                        break;
                }
                
                if(pMe->m_wSelectGroup != 0)
                {
                    if(SUCCESS != CContApp_GetConfig( pMe,
                                                      (ContAppCFG)pMe->m_wSelectGroup,
                                                      pGroup,
                                                      sizeof(pGroup)))
                    {
                        return EFAILED;
                    } 
                }
                
                if(pGroup[0] == ONEDIAL_NULL)
                {                        
                    len = ISHELL_LoadResString ( pMe->m_pShell, 
                                        CONTAPP_RES_FILE_LANG, 
                                        ResID,
                                        pGroup,
                                        sizeof(pGroup)); 
                    if(len <= 0 )
                    {
                        return EFAILED;
                    }                        
                }    

                if(pMe->m_wSelectGroup != 0)
                {
#if defined (FEATURE_VERSION_C337) || defined(FEATURE_VERSION_IC241A_MMX)
				   ai.wText       = IDS_CALLER_GROUPS;
#else
				   ai.wText       = IDS_INPUT_GROUP;
#endif
                   ai.wItemID   = IDI_EDIT_MENU_GROUP;
                   ai.wImage    = IDB_GROUP;
                   
                   if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
                   {
                      FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
                      return EFAILED;
                   }
                   IMENUCTL_SetItemText(pMenuCtl, IDI_EDIT_MENU_GROUP, NULL, 0, pGroup);
                }
//email
			  if(pMe->m_pAddNewEMAIL)
				{
				  ai.wText		 = IDS_EMAIL;
				  ai.wItemID   = IDI_EDIT_MENU_EMAIL;
				  ai.wImage    = IDB_EMAIL;
				  
				  if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
				  {
					 FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
					 return EFAILED;
				  }
			  IMENUCTL_SetItemText(pMenuCtl, IDI_EDIT_MENU_EMAIL, NULL, 0, pMe->m_pAddNewEMAIL);
			   }


//address
           if(pMe->m_pAddNewAddress)
             {
               ai.wText       = IDS_ADDRESS;
               ai.wItemID   = IDI_EDIT_MENU_ADDRESS;
               ai.wImage    = IDB_ADDRESS;
               
               if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
               {
                  FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
                  return EFAILED;
               }
     
           IMENUCTL_SetItemText(pMenuCtl, IDI_EDIT_MENU_ADDRESS, NULL, 0, pMe->m_pAddNewAddress);
            }

		  



//url
           if(pMe->m_pAddNewURL)
             {
               ai.wText       = IDS_URL;
               ai.wItemID   = IDI_EDIT_MENU_URL;
               ai.wImage    = IDB_URL;
               
               if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
               {
                  FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
                  return EFAILED;
               }
     
           IMENUCTL_SetItemText(pMenuCtl, IDI_EDIT_MENU_URL, NULL, 0, pMe->m_pAddNewURL);
            }

//notes     
           if(pMe->m_pAddNewRemark)
           {
               ai.wText       = IDS_NOTES;
               ai.wItemID   = IDI_EDIT_MENU_REMARK;
               ai.wImage    = IDB_NOTES;
               
               if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
               {
                  FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
                  return EFAILED;
               }
     
                IMENUCTL_SetItemText(pMenuCtl, IDI_EDIT_MENU_REMARK, NULL, 0, pMe->m_pAddNewRemark);
           }

            #ifndef FEATURE_VERSION_K212

                // ringtone     
                if (pMe->m_nRingToneID && pMe->m_nRingToneID[0] != (AECHAR)'\0')
                {
                    AECHAR name[128]={0};
                    AECHAR *fileName=NULL;
                    char nametemp[128] = {0};
                    fileName =  WSTRRCHR(pMe->m_nRingToneID, (AECHAR)'/');
                    if(NULL != fileName)
                    {
                        fileName++;
                        WSTRCPY(name, fileName);
                    }
                    else
                    {
                        WSTRCPY(name, pMe->m_nRingToneID);
                    }

					WSTRTOSTR(name,nametemp,128);
					DBGPRINTF("nametemp====%s",nametemp);
					DBGPRINTF("name====%s",name);
					MEMSET(name,0,128);
					UTF8TOWSTR((byte *)nametemp,STRLEN(nametemp),name,128);
					DBGPRINTF("name====%s",name);
                ai.wText       = IDS_RING;
                    ai.wItemID   = IDI_EDIT_MENU_RINGTONE;
                    ai.wImage    = IDB_RING;
                   
                    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
                    {
                      FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
                      return EFAILED;
                    }
                    IMENUCTL_SetItemText(pMenuCtl, IDI_EDIT_MENU_RINGTONE, NULL, 0, name);
                }
				#endif
            }
        }
    }
    //(void)IMENUCTL_Redraw(pMenuCtl);
    return SUCCESS;
}// CContApp_BuildEditMenuMenu



/*=============================================================================

FUNCTION:CContApp_BuildListMenu

DESCRIPTION:
    ���б�˵��������Ŀ
    
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
    
RETURN VALUE:
    SUCCESS: 
    EFAILED: If the number of current cache is 0
    
COMMENTS:
    
SIDE EFFECTS:
    pMe->m_nCurrIdx
    MAX_NUM_MENUPOP
    
SEE ALSO:

=============================================================================*/
int CContApp_BuildListMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    int     i;
    AEEAddCacheInfo *pCache;
    int32   nListItems;
    
    ASSERT(pMe != NULL); 
    
    FARF(ADDR, ("Build List menu %d,  %d ", pMe->m_nCurrIdx, IVector_Size(pMe->m_pAddList)));

	MSG_FATAL("***zzg CContApp_BuildListMenu***", 0, 0, 0);
    
    if (pMenuCtl == NULL)
    {
        return EBADPARM;
    }

	#if defined (FEATURE_VERSION_C337) || defined(FEATURE_VERSION_IC241A_MMX)
	{
		boolean b_defaultcont = FALSE;	  
	    OEM_GetConfig(CFGI_DEFAULTCONT,&b_defaultcont, sizeof(b_defaultcont));

		MSG_FATAL("***zzg CContApp_BuildListMenu CFGI_DEFAULTCONT=%x***", b_defaultcont, 0, 0);

		if (b_defaultcont == FALSE)
		{
			CContApp_CreateDefaultCont(pMe);	
		}		
	}
	#endif
    
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    nListItems = IVector_Size(pMe->m_pAddList);
    
    // Adjust the current index
    if (pMe->m_nCurrIdx < 0)
    {
        // ���� Index
        pMe->m_nCurrIdx += nListItems;
    }
    else if (pMe->m_nCurrIdx >= nListItems)
    {
        pMe->m_nCurrIdx -= nListItems;
    }//else Remain the current value
    
    if(pMe->m_nCurrIdx >(MAX_PHONEBOOK_CAPACITY+MAX_RUIMBOOK_CAPACITY))
    {
    	pMe->m_nCurrIdx = 0;
    }
    // �����ǰ�ļ�¼��ĿС������һ��������˵���Ŀ MAX_NUM_MENUPOP
    // ��ֱ����ӡ����򲿷ֽ������
    if (SUCCESS != CContApp_EnumContInit(pMe, (uint32)pMe->m_nCurrIdx))
    {
        // Add phone record to menu
        for (i=0; i<nListItems; i++)
        {
            pCache = (AEEAddCacheInfo *)IVector_ElementAt(pMe->m_pAddList, (uint32)i);
            (void)CContApp_AddItem(pMe, pMenuCtl, pCache);
        }
        CContApp_RestoreMenuRect(pMe, pMenuCtl);
    }
    else
    {
        uint32   nContIdx;
        
        // ÿ�������� MAX_NUM_MENUPOP ����¼
        for (i=0; i<MAX_NUM_MENUPOP; i++)
        {
            nContIdx = CContApp_EnumContNext(pMe);
            
            FARF(ADDR, ("Cont idx %d curr idx %d",nContIdx, pMe->m_nCurrIdx));
            
            // Read data from the phone
            pCache = IVector_ElementAt(pMe->m_pAddList, nContIdx);
            (void)CContApp_AddItem(pMe, pMenuCtl, pCache);
        }
        
        CContApp_DrawScrollBar(pMe, pMenuCtl);

    }
    
    // Set menu select
    if(pMe->m_wSelectCont != CONTAPP_RECID_NULL)
    {
        IMENUCTL_SetSel(pMenuCtl, pMe->m_wSelectCont);
    }
    else
    {
        IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, 0));
    }
    return SUCCESS;
}// CContApp_BuildListMenu


/*=============================================================================

FUNCTION:CContApp_BuildViewMenu

DESCRIPTION:
    ��View�˵�����Ӽ�¼������Ŀ
    
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
    
RETURN VALUE:
    SUCCESS: 
    EFAILED: If the number of current cache is 0
    
COMMENTS:
    
SIDE EFFECTS:
    
SEE ALSO:

=============================================================================*/
int CContApp_BuildViewMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    int           i;
    uint32        nFldsNum;
    AEEAddrField *pFld;
    CtlAddItem    ai;
    
    ASSERT(pMe != NULL); 
    MSG_FATAL("CContApp_BuildViewMenu start",0,0,0);
    FARF(ADDR, ("Build View menu"));
    
    if(pMenuCtl == NULL)
    {
        return EBADPARM;
    }
    
    nFldsNum = IVector_Size(pMe->m_pFldIv);
    
    // ���������Menu�ؼ�����������Cache��һ��
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    if(nFldsNum == 0)
    {
        FARF(ADDR, ("No fields to List"));
        return EFAILED;
    }
    
    //��ʼ���˵���ʽ
    MEMSET(&ai, 0, sizeof(ai));
    ai.pszResText = CONTAPP_RES_FILE_LANG;
    ai.pszResImage = AEE_APPSCOMMONRES_IMAGESFILE;
    
    for(i=0; i<(int)nFldsNum; i++)
    {
        pFld = IVector_ElementAt( pMe->m_pFldIv, (uint32)i );
        if(NULL == pFld)
        {
            continue;
        }
        if (IS_NUM_FIELD(pFld->fID))
        {
            ai.wItemID = (uint16)i;
            //select menu icon image
            ai.wImage = CContApp_GetFldImgID(pFld->fID);

            ai.pText  = (AECHAR *)pFld->pBuffer;
            ai.wText  = 0;
            
            if(FALSE == IMENUCTL_AddItemEx( pMenuCtl, &ai ))
            {
                FARF(ADDR, ("Failed to add %d item %d",pFld->fID, i));
                return EFAILED;
            }
            else
            {
                FARF(ADDR, ("Field pBuffer is NULL %d", i));
            }   
        }        
    }
    MSG_FATAL("CContApp_BuildViewMenu End",0,0,0);
    return SUCCESS;
}// CContApp_BuildViewMenu


/*=============================================================================

FUNCTION:CContApp_BuildOneDialMenu

DESCRIPTION:
    �� Onedial �˵�����Ӽ�¼������Ŀ
    
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
    
RETURN VALUE:
    SUCCESS: 
    
COMMENTS:
    One dial only support the RUIM record
SIDE EFFECTS:
    
SEE ALSO:

=============================================================================*/
int CContApp_BuildOneDialMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    int           i;
    ContInfo      pContInfo;
    CtlAddItem    ai;
    AECHAR        wszNumName[MAX_INPUT_NAME_TH+4];//����ŵ��ַ�������buf
    AECHAR        wszFmt[4];
    AECHAR        wszNull[MAX_INPUT_NAME_TH] = {0};
    AECHAR        szTmp[MAX_INPUT_NAME_TH] = {0};
    AECHAR space[] = {(AECHAR)' ',(AECHAR)'\0'};

    ASSERT(pMe != NULL); 

    MSG_FATAL("CContApp_BuildOneDialMenu Start",0,0,0);

    if(pMenuCtl == NULL)
    {
        return EBADPARM;
    }

    MEMSET(&pContInfo, 0, sizeof(ContInfo));
    szTmp[0] = 0;
    pContInfo.pName = szTmp;
    pContInfo.wNameLen = MAX_INPUT_NAME_CN+1;

    // ���Menu�ؼ�
    (void)IMENUCTL_DeleteAll(pMenuCtl);

    // ��ʼ���˵���ʽ
    MEMSET(&ai, 0, sizeof(ai));
    ai.pszResText = CONTAPP_RES_FILE_LANG;

    ISHELL_LoadResString(pMe->m_pShell, CONTAPP_RES_FILE_LANG, IDS_ONEDIAL_NULL, wszNull, sizeof(wszNull));

#if defined(FEATURE_VERSION_EC99) || defined (FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_K212_ND)
    for( i = CONTCFG_ONEDIAL1; i <= CONTCFG_ONEDIAL9; i++)	
#else
    for( i = CONTCFG_ONEDIAL2; i <= CONTCFG_ONEDIAL9; i++)	//CONTCFG_ONEDIAL1
#endif    
    {
        if(SUCCESS != CContApp_GetConfig( pMe,
                                          (ContAppCFG)i,
                                          pMe->m_pOneDialBuf,
                                          sizeof(pMe->m_pOneDialBuf)))
        {
            MSG_FATAL("Failed to get %d onedial", i,0,0);
            continue;
        }

#if defined(FEATURE_VERSION_EC99) 
        if ((i == CONTCFG_ONEDIAL5) || (i == CONTCFG_ONEDIAL6))
        {
            continue;
        }
#endif
#if defined (FEATURE_VERSION_K212_20D)
				if ((i == CONTCFG_ONEDIAL6))
				{
					continue;
				}
#endif

        
        // set menu item id
        ai.wItemID = (uint16)i;
        if(pMe->m_pOneDialBuf[0] == ONEDIAL_NULL)
        {
            MSG_FATAL("pMe->m_pOneDialBuf[%d] == ONEDIAL_NULL",i,0,0);
            //ERR("pMe->m_szBuf == NULL", 0, 0, 0);
            ISHELL_LoadResString(pMe->m_pShell, CONTAPP_RES_FILE_LANG, IDS_ONEDIAL_NULL, wszNull, sizeof(wszNull));
            STRTOWSTR("%d.", wszFmt, sizeof(wszFmt));
            WSPRINTF(wszNumName,sizeof(wszNumName),wszFmt,(i-CONTCFG_ONEDIAL2+2)); //CONTCFG_ONEDIAL1
            WSTRCAT(wszNumName, space);
            WSTRLCAT(wszNumName,wszNull,sizeof(wszNumName));
            ai.pText  = wszNumName;
            ai.wText  = 0;
            (void)IMENUCTL_AddItemEx( pMenuCtl, &ai );
        }
        else
        {
            MSG_FATAL("pMe->m_pOneDialBuf[%d] != ONEDIAL_NULL",i,0,0);
            if(SUCCESS != CContApp_GetNameByNum(pMe, 
                                     pMe->m_pOneDialBuf, 
                                     &pContInfo))
            {
                MSG_FATAL("SUCCESS != CContApp_GetNameByNum",0,0,0);
                //���Ѿ������õĺ����ֶα�ɾ������ȡ����������ֱ�����"δ����"
                STRTOWSTR("%d.", wszFmt, sizeof(wszFmt));
                WSPRINTF(wszNumName,sizeof(wszNumName),wszFmt,(i-CONTCFG_ONEDIAL2+2));//CONTCFG_ONEDIAL1
                WSTRCAT(wszNumName, space);
                WSTRLCAT(wszNumName,wszNull,sizeof(wszNumName));
                ai.pText  = wszNumName;
                ai.wText  = 0;
                (void)IMENUCTL_AddItemEx( pMenuCtl, &ai );
				/*	pMe->m_pOneDialBuf[0] = ONEDIAL_NULL;
 					(void)CContApp_SetConfig( pMe,
		 					(ContAppCFG)i,
		 					pMe->m_pOneDialBuf,
		 					sizeof(pMe->m_pOneDialBuf));		*/		
                continue;
            }
            
            if(pContInfo.pName == NULL)
            {
                MSG_FATAL("pContInfo.pName == NULL", 0, 0, 0);
                STRTOWSTR("%d.", wszFmt, sizeof(wszFmt));
                WSPRINTF(wszNumName,sizeof(wszNumName),wszFmt,(i-CONTCFG_ONEDIAL2+2));//CONTCFG_ONEDIAL1
                WSTRCAT(wszNumName, space);
                WSTRLCAT(wszNumName,wszNull,sizeof(wszNumName));
                ai.pText  = wszNumName;
                ai.wText  = 0;
                (void)IMENUCTL_AddItemEx( pMenuCtl, &ai );
            }
            else
            {
                MSG_FATAL("pContInfo.pName != NULL", 0, 0, 0);
                STRTOWSTR("%d.", wszFmt, sizeof(wszFmt));
                WSPRINTF(wszNumName,sizeof(wszNumName),wszFmt,(i-CONTCFG_ONEDIAL2+2));//CONTCFG_ONEDIAL1
                WSTRCAT(wszNumName, space);
                WSTRLCAT(wszNumName,pContInfo.pName,sizeof(wszNumName));
                ai.pText  = wszNumName;
                ai.wText  = 0;
                
                // Add menu item
                (void)IMENUCTL_AddItemEx( pMenuCtl, &ai );
            }
        }
    }

    // Set this menu select
    //IMENUCTL_SetSel(pMenuCtl, pMe->m_wSelectOneDial);
    return SUCCESS;
}

/*=============================================================================

FUNCTION:CContApp_BuildSearchMenu

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
int CContApp_BuildSearchMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    CtlAddItem ai;
    
    ASSERT(pMe != NULL); 
    
     // Check Parameter
    if(!pMenuCtl)
    {
        return EBADPARM;
    }
    
    // Clear items
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    // Init items
    MEMSET(&ai, 0, sizeof(ai));
    ai.pszResText = CONTAPP_RES_FILE_LANG;

    ai.wText       = IDS_SEARCH_NAME;
    ai.wItemID   = IDI_SEARCH_MENU_NAME;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
       FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
       return EFAILED;
    }    

    ai.wText       = IDS_SEARCH_GROUP;
    ai.wItemID   = IDI_SEARCH_MENU_GROUP;
           
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    }        
    
    (void)IMENUCTL_Redraw(pMenuCtl);
    return SUCCESS;
}// CContApp_BuildOptsMenu

/*=============================================================================

FUNCTION:CContApp_ListMenuScroll

DESCRIPTION:
    ��������б�˵�
    
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
    eCode   :
    wParam  :
    
RETURN VALUE:
    None
    
COMMENTS:
    
SIDE EFFECTS:
    pMe->m_nCurrIdx
    
SEE ALSO:

=============================================================================*/
void CContApp_ListMenuScroll( CContApp *pMe, IMenuCtl *pMenuCtl, AEEEvent  eCode, uint16 wParam)
{
    uint16 wLastItem, wFirstItem;
    int32  nListItems;
    
    ASSERT(pMe != NULL);
    
    if (pMenuCtl == NULL)
    {
        return;
    }
    
    nListItems = IVector_Size(pMe->m_pAddList);
    
    switch(eCode)
    {
        case EVT_KEY:
            switch(wParam)
            {
                case AVK_UP:
                    wFirstItem = IMENUCTL_GetItemID(pMenuCtl, 0);
                    
                    // Fix the bug that after delete a record
                    pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                    
                    if (wFirstItem == pMe->m_wSelectCont)//��ǰѡ���ڵ�һ��
                    {
                        if (nListItems <= MAX_NUM_MENUPOP)
                        {
                            IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, (nListItems-1)));
                            MSG_FATAL("nListItems---nListItems=%d---MAX_NUM_MENUPOP=%d",nListItems,MAX_NUM_MENUPOP,0);
                        }
                        else 
                        {
                            if(pMe->m_nCurrIdx == 0)
                            {
                                pMe->m_nCurrIdx = nListItems - MAX_NUM_MENUPOP;
                                MSG_FATAL("nListItems---nListItems=%d---MAX_NUM_MENUPOP=%d",nListItems,MAX_NUM_MENUPOP,0);
                                (void)CContApp_BuildListMenu(pMe, pMenuCtl);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID( pMenuCtl, MAX_NUM_MENUPOP - 1));
                            }
                            else
                            {
                                pMe->m_nCurrIdx--;
                                (void)CContApp_BuildListMenu(pMe, pMenuCtl);
                                MSG_FATAL("nListItems---nListItems=%d---MAX_NUM_MENUPOP=%d",nListItems,MAX_NUM_MENUPOP,0);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID( pMenuCtl, 0));
                            }
                            pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                        }
                        (void)IMENUCTL_Redraw(pMenuCtl);
                    }
                    break;
                    
                case AVK_DOWN:
                    if (nListItems <= MAX_NUM_MENUPOP)
                    {
                        wLastItem  = IMENUCTL_GetItemID(pMenuCtl, (nListItems -1));
                    }
                    else
                    {
                        wLastItem  = IMENUCTL_GetItemID(pMenuCtl, MAX_NUM_MENUPOP -1);
                    }
                    pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                    
                    if (wLastItem == pMe->m_wSelectCont)
                    {
                        if (nListItems <= MAX_NUM_MENUPOP)
                        {
                            IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, 0));
                        }
                        else 
                        {
                            if(pMe->m_nCurrIdx + MAX_NUM_MENUPOP == nListItems)
                            {
                                pMe->m_nCurrIdx = 0;
                                (void)CContApp_BuildListMenu(pMe, pMenuCtl);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, 0));
                            }
                            else
                            {
                                pMe->m_nCurrIdx++;
                                (void)CContApp_BuildListMenu(pMe, pMenuCtl);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, MAX_NUM_MENUPOP -1));
                            }
                            pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                        }
                        (void)IMENUCTL_Redraw(pMenuCtl);
                    }
                    break;
                    
                default:
                    break;
            }
            break;
            
        default:
            break;
    }
}// CContApp_ListMenuScroll


/*=============================================================================

FUNCTION:CContApp_deleteMenuScroll

DESCRIPTION:
    ��������б�˵�
    
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
    eCode   :
    wParam  :
    
RETURN VALUE:
    None
    
COMMENTS:
    
SIDE EFFECTS:
    pMe->m_nCurrIdx
    
SEE ALSO:

=============================================================================*/
void CContApp_DeleteMenuScroll( CContApp *pMe, IMenuCtl *pMenuCtl, AEEEvent  eCode, uint16 wParam)
{
    uint16 wLastItem, wFirstItem;
    int32  nListItems;
    
    // Check parameter
    ASSERT(pMe != NULL);
    
    if(pMenuCtl == NULL)
    {
        return;
    }
    
    nListItems = IVector_Size(pMe->m_pAddList);

    switch(eCode)
    {
        case EVT_KEY:
            switch(wParam)
            {
                case AVK_UP:
                    wFirstItem = IMENUCTL_GetItemID(pMenuCtl, 0);
                    
                    // Fix the bug that after delete a record
                    pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                    
                    if(wFirstItem == pMe->m_wSelectCont)//��ǰѡ���ڵ�һ��
                    {
                        if (nListItems <= MAX_NUM_MENUPOP)
                        {
                            IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID( pMenuCtl, nListItems -1));
                        }
                        else 
                        {
                            if(pMe->m_nCurrIdx == 0)
                            {
                                pMe->m_nCurrIdx = nListItems - MAX_NUM_MENUPOP;
                                (void)CContApp_BuildDeleteMenu(pMe, pMenuCtl);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID( pMenuCtl, MAX_NUM_MENUPOP - 1));
                            }
                            else
                            {
                                pMe->m_nCurrIdx--;
                                (void)CContApp_BuildDeleteMenu(pMe, pMenuCtl);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID( pMenuCtl, 0));
                            }
                            pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                        }
                        (void)IMENUCTL_Redraw(pMenuCtl);
                    }
                    break;
                    
                case AVK_DOWN:
                    if(nListItems <= MAX_NUM_MENUPOP)
                    {
                        wLastItem  = IMENUCTL_GetItemID( pMenuCtl, nListItems -1);
                    }
                    else
                    {
                        wLastItem  = IMENUCTL_GetItemID( pMenuCtl, MAX_NUM_MENUPOP -1);
                    }
                    pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                   
                    if(wLastItem == pMe->m_wSelectCont)
                    {
                        if (nListItems <= MAX_NUM_MENUPOP)
                        {
                            IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, 0));
                        }
                        else
                        {
                            if(pMe->m_nCurrIdx + MAX_NUM_MENUPOP == nListItems)
                            {
                                pMe->m_nCurrIdx = 0;
                                (void)CContApp_BuildDeleteMenu(pMe, pMenuCtl);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, 0));
                            }
                            else
                            {
                                pMe->m_nCurrIdx++;
                                (void)CContApp_BuildDeleteMenu(pMe, pMenuCtl);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, MAX_NUM_MENUPOP -1));
                            }
                            pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                        }
                        (void)IMENUCTL_Redraw(pMenuCtl);
                    }
                    break;
                    
                default:
                    break;
            }
            break;
            
        default:
            break;
    }
}// CContApp_DeleteMenuScroll

/*=============================================================================

FUNCTION:CContApp_Select_record_MenuScroll

DESCRIPTION:
    ��������б�˵�
    
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
    eCode   :
    wParam  :
    
RETURN VALUE:
    None
    
COMMENTS:
    
SIDE EFFECTS:
    pMe->m_nCurrIdx
    
SEE ALSO:

=============================================================================*/
void CContApp_Select_record_MenuScroll( CContApp *pMe, IMenuCtl *pMenuCtl, AEEEvent  eCode, uint16 wParam)
{
    uint16 wLastItem, wFirstItem;
    int32  nListItems;
    
    // Check parameter
    ASSERT(pMe != NULL);
    
    if(pMenuCtl == NULL)
    {
        return;
    }
    
    nListItems = IVector_Size(pMe->m_pAddList);

    switch(eCode)
    {
        case EVT_KEY:
            switch(wParam)
            {
                case AVK_UP:
                    wFirstItem = IMENUCTL_GetItemID(pMenuCtl, 0);
                    
                    // Fix the bug that after delete a record
                    pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                    
                    if(wFirstItem == pMe->m_wSelectCont)//��ǰѡ���ڵ�һ��
                    {
                        if (nListItems <= MAX_NUM_MENUPOP)
                        {
                            IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID( pMenuCtl, nListItems -1));
                        }
                        else
                        {
                            if(pMe->m_nCurrIdx == 0)
                            {
                                pMe->m_nCurrIdx = nListItems - MAX_NUM_MENUPOP;
                                (void)CContApp_BuildDeleteMenu(pMe, pMenuCtl);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID( pMenuCtl, MAX_NUM_MENUPOP - 1));
                            }
                            else
                            {
                                pMe->m_nCurrIdx--;
                                (void)CContApp_BuildDeleteMenu(pMe, pMenuCtl);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID( pMenuCtl, 0));
                            }
                            pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                        }
                        (void)IMENUCTL_Redraw(pMenuCtl);
                    }
                    break;
                    
                case AVK_DOWN:
                    if(nListItems <= MAX_NUM_MENUPOP)
                    {
                        wLastItem  = IMENUCTL_GetItemID( pMenuCtl, nListItems -1);
                    }
                    else
                    {
                        wLastItem  = IMENUCTL_GetItemID( pMenuCtl, MAX_NUM_MENUPOP -1);
                    }
                    pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                   
                    if(wLastItem == pMe->m_wSelectCont)
                    {
                        if (nListItems <= MAX_NUM_MENUPOP)
                        {
                            IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, 0));
                        }
                        else
                            /*
                        {
                            pMe->m_nCurrIdx++;
                            (void)CContApp_BuildDeleteMenu(pMe, pMenuCtl);
                            IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID( pMenuCtl, MAX_NUM_MENUPOP -1));
                        }
                        (void)IMENUCTL_Redraw(pMenuCtl);  
                        */
                        {
                            if(pMe->m_nCurrIdx + MAX_NUM_MENUPOP == nListItems)
                            {
                                pMe->m_nCurrIdx = 0;
                                (void)CContApp_BuildDeleteMenu(pMe, pMenuCtl);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, 0));
                            }
                            else
                            {
                                pMe->m_nCurrIdx++;
                                (void)CContApp_BuildDeleteMenu(pMe, pMenuCtl);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, MAX_NUM_MENUPOP -1));
                            }
                            pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                            //CContApp_DrawScrollBar(pMe, pMenuCtl);
                        }
                        (void)IMENUCTL_Redraw(pMenuCtl);
                    }
                    break;
                    
                default:
                    break;
            }
            break;
            
        default:
            break;
    }
}// CContApp_Select_record_MenuScroll

/*=============================================================================

FUNCTION:CContApp_BuildDeleteMenu

DESCRIPTION:
    ���б�˵��������Ŀ
    
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
    
RETURN VALUE:
    SUCCESS: 
    EFAILED: If the number of current cache is 0
    
COMMENTS:
    
SIDE EFFECTS:
    pMe->m_nCurrIdx
    MAX_NUM_MENUPOP
    
SEE ALSO:

=============================================================================*/
int CContApp_BuildDeleteMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    AEEAddCacheInfo *pCache;
    int i = 0;
    int32 nListItems;
    
    ASSERT(pMe != NULL); 
    
    FARF(ADDR, ("Build Delete Menu"));
    
    if (pMenuCtl == NULL)
    {
        return EBADPARM;
    }
    
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    nListItems = IVector_Size(pMe->m_pAddList);
    
    // Adjust the current index
    if (pMe->m_nCurrIdx < 0)
    {
        // ���� Index
        pMe->m_nCurrIdx += nListItems;
    }
    else if (pMe->m_nCurrIdx >= nListItems)
    {
        pMe->m_nCurrIdx -= nListItems;
    }//else Remain the current value

    // �����ǰ�ļ�¼��ĿС������һ��������˵���Ŀ MAX_NUM_MENUPOP
    // ��ֱ����ӡ����򲿷ֽ������
    if (SUCCESS != CContApp_EnumContInit(pMe, (uint32)pMe->m_nCurrIdx))
    {
        // Add phone record to menu
        for (i=0; i<nListItems; i++)
        {
            pCache = (AEEAddCacheInfo *)IVector_ElementAt(pMe->m_pAddList, (uint32)i);
            (void)CContApp_DeleteItem(pMe, pMenuCtl, pCache);
        }

        CContApp_RestoreMenuRect(pMe, pMenuCtl);
    }
    else
    {
        uint32   nContIdx;
        
        // ÿ�������� MAX_NUM_MENUPOP ����¼
        for (i=0; i<MAX_NUM_MENUPOP; i++)
        {
            nContIdx = CContApp_EnumContNext(pMe);
            
            FARF(ADDR, ("Cont idx %d curr idx %d",nContIdx, pMe->m_nCurrIdx));
            
            // Read data from the phone
            pCache = (AEEAddCacheInfo *)IVector_ElementAt(pMe->m_pAddList, nContIdx);
            (void)CContApp_DeleteItem(pMe, pMenuCtl, pCache);
        }

        CContApp_DrawScrollBar(pMe, pMenuCtl);
    }

    // Set menu select
    /*if (pMe->m_wSelectCont == CONTAPP_RECID_NULL)
    {
        IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, 0));
    }*/
    if(pMe->m_wSelectCont != CONTAPP_RECID_NULL)
    {
        IMENUCTL_SetSel(pMenuCtl, pMe->m_wSelectCont);
    }
    else
    {
        IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, 0));
    }
    
    return SUCCESS;
}// CContApp_BuildDeleteMenu

/*=============================================================================

FUNCTION:CContApp_CopyMenuScroll

DESCRIPTION:
    ��������б�˵�
    
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
    eCode   :
    wParam  :
    
RETURN VALUE:
    None
    
COMMENTS:
    
SIDE EFFECTS:
    pMe->m_nCurrIdx
    
SEE ALSO:

=============================================================================*/
void CContApp_CopyMenuScroll( CContApp *pMe, IMenuCtl *pMenuCtl, AEEEvent  eCode, uint16 wParam)
{
    uint16 wLastItem, wFirstItem;
    int32  nListItems;
    
    // Check parameter
    ASSERT(pMe != NULL);
    
    if(pMenuCtl == NULL)
    {
        return;
    }
    
    nListItems = IVector_Size(pMe->m_pAddList);
    
    switch(eCode)
    {
        case EVT_KEY:
            switch(wParam)
            {
                case AVK_UP:
                    wFirstItem = IMENUCTL_GetItemID(pMenuCtl, 0);
                    
                    // Fix the bug that after delete a record
                    pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                    
                    if(wFirstItem == pMe->m_wSelectCont)//��ǰѡ���ڵ�һ��
                    {
                        if (nListItems <= MAX_NUM_MENUPOP)
                        {
                            IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID( pMenuCtl, nListItems -1));
                        }
                        else
                        {
                            if(pMe->m_nCurrIdx == 0)
                            {
                                pMe->m_nCurrIdx = nListItems - MAX_NUM_MENUPOP;
                                (void)CContApp_BuildCopyMenu(pMe, pMenuCtl);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID( pMenuCtl, MAX_NUM_MENUPOP - 1));
                            }
                            else
                            {
                                pMe->m_nCurrIdx--;
                                (void)CContApp_BuildCopyMenu(pMe, pMenuCtl);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID( pMenuCtl, 0));
                            }
                            pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                        }
                        (void)IMENUCTL_Redraw(pMenuCtl);
                    }
                    break;
                    
                case AVK_DOWN:
                    if(nListItems <= MAX_NUM_MENUPOP)
                    {
                        wLastItem  = IMENUCTL_GetItemID( pMenuCtl, nListItems -1);
                    }
                    else
                    {
                        wLastItem  = IMENUCTL_GetItemID( pMenuCtl, MAX_NUM_MENUPOP -1);
                    }
                    pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                    
                    if(wLastItem == pMe->m_wSelectCont)
                    {
                        if (nListItems <= MAX_NUM_MENUPOP)
                        {
                            IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, 0));
                        }
                        else 
                        {
                            if(pMe->m_nCurrIdx + MAX_NUM_MENUPOP == nListItems)
                            {
                                pMe->m_nCurrIdx = 0;
                                (void)CContApp_BuildCopyMenu(pMe, pMenuCtl);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, 0));
                            }
                            else
                            {
                                pMe->m_nCurrIdx++;
                                (void)CContApp_BuildCopyMenu(pMe, pMenuCtl);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, MAX_NUM_MENUPOP -1));
                            }
                            pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                            //CContApp_DrawScrollBar(pMe, pMenuCtl);
                        }
                        (void)IMENUCTL_Redraw(pMenuCtl);
                    }
                    break;
                    
                default:
                    break;
            }
            break;
            
        default:
            break;
    }
}// CContApp_DeleteMenuScroll

/*=============================================================================

FUNCTION:CContApp_BuildCopyMenu

DESCRIPTION:
    ���б�˵��������Ŀ
    
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
    
RETURN VALUE:
    SUCCESS: 
    EFAILED: If the number of current cache is 0
    
COMMENTS:
    
SIDE EFFECTS:
    pMe->m_nCurrIdx
    MAX_NUM_MENUPOP
    
SEE ALSO:

=============================================================================*/
int CContApp_BuildCopyMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    AEEAddCacheInfo *pCache;
    int i = 0;
    int32 nListItems;
    
    ASSERT(pMe != NULL); 
    
    FARF(ADDR, ("Build Select Menu"));
    
    if (pMenuCtl == NULL)
    {
        return EBADPARM;
    }
    
    nListItems = IVector_Size(pMe->m_pAddList);
    
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    // Adjust the current index
    if (pMe->m_nCurrIdx < 0)
    {
        // ���� Index
        pMe->m_nCurrIdx += nListItems;
    }
    else if (pMe->m_nCurrIdx >= nListItems)
    {
        pMe->m_nCurrIdx -= nListItems;
    }//else Remain the current value

    // �����ǰ�ļ�¼��ĿС������һ��������˵���Ŀ MAX_NUM_MENUPOP
    // ��ֱ����ӡ����򲿷ֽ������
    if(SUCCESS != CContApp_EnumContInit(pMe, (uint32)pMe->m_nCurrIdx))
    {
        // Add phone record to menu
        for (i=0; i<nListItems; i++)
        {
            pCache = (AEEAddCacheInfo *)IVector_ElementAt(pMe->m_pAddList, (uint32)i);
            (void)CContApp_CopyItem(pMe, pMenuCtl, pCache);
        }

        CContApp_RestoreMenuRect(pMe, pMenuCtl);
    }
    else
    {
        uint32   nContIdx;
        
        // ÿ�������� MAX_NUM_MENUPOP ����¼
        for (i=0; i<MAX_NUM_MENUPOP; i++)
        {
            nContIdx = CContApp_EnumContNext(pMe);
            
            FARF(ADDR, ("Cont idx %d curr idx %d",nContIdx, pMe->m_nCurrIdx));
            
            pCache = (AEEAddCacheInfo *)IVector_ElementAt(pMe->m_pAddList, nContIdx);
            (void)CContApp_CopyItem(pMe, pMenuCtl, pCache);
        }
        
        CContApp_DrawScrollBar(pMe, pMenuCtl);
    }
    /*
    // Set menu select
    if (pMe->m_wSelectCont == CONTAPP_RECID_NULL)
    {
        IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, 0));
    }*/
      if(pMe->m_wSelectCont != CONTAPP_RECID_NULL)
    {
        IMENUCTL_SetSel(pMenuCtl, pMe->m_wSelectCont);
    }
    else
    {
        IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, 0));
    }
    
    return SUCCESS;
}// CContApp_BuildCopyMenu

/*=============================================================================

FUNCTION:CContApp_SelectMenuScroll

DESCRIPTION:
    ��������б�˵�
    
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
    eCode   :
    wParam  :
    
RETURN VALUE:
    None
    
COMMENTS:
    
SIDE EFFECTS:
    pMe->m_nCurrIdx
    
SEE ALSO:

=============================================================================*/
void CContApp_SelectMenuScroll( CContApp *pMe, IMenuCtl *pMenuCtl, AEEEvent  eCode, uint16 wParam)
{
    uint16 wLastItem, wFirstItem;
    int32  nListItems = 0;
    
    // Check parameter
    ASSERT(pMe != NULL);
    
    if(pMenuCtl == NULL)
    {
        return;
    }
    
    nListItems = IVector_Size(pMe->m_pAddList);
    
    switch(eCode)
    {
        case EVT_KEY:
            switch(wParam)
            {
                case AVK_UP:
                    wFirstItem = IMENUCTL_GetItemID(pMenuCtl, 0);
                                        
                    // Fix the bug that after delete a record
                    pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                    pMe->m_wKeyType = AEE_ADDR_KEY_UP;
                    
                    if(wFirstItem == pMe->m_wSelectCont)//��ǰѡ���ڵ�һ��
                    {
                        if (nListItems <= MAX_NUM_MENUPOP)
                        {
                            IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID( pMenuCtl, nListItems -1));
                        }
                        else 
                        {
                            if(pMe->m_nCurrIdx == 0)
                            {
                                pMe->m_nCurrIdx = nListItems - MAX_NUM_MENUPOP;
                                (void)CContApp_BuildSelectMenu(pMe, pMenuCtl);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID( pMenuCtl, MAX_NUM_MENUPOP - 1));
                            }
                            else
                            {
                                pMe->m_nCurrIdx--;
                                (void)CContApp_BuildSelectMenu(pMe, pMenuCtl);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID( pMenuCtl, 0));
                            }
                            pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                        }
                        (void)IMENUCTL_Redraw(pMenuCtl);
                    }
                    break;
                    
                case AVK_DOWN:
                    if(nListItems <= MAX_NUM_MENUPOP)
                    {
                        wLastItem  = IMENUCTL_GetItemID( pMenuCtl, nListItems -1);
                    }
                    else
                    {
                        wLastItem  = IMENUCTL_GetItemID( pMenuCtl, MAX_NUM_MENUPOP -1);
                    }
                    pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                    
                    pMe->m_wKeyType = AEE_ADDR_KEY_DOWN;
                    
                    if(wLastItem == pMe->m_wSelectCont)
                    {
                        if (nListItems <= MAX_NUM_MENUPOP)
                        {
                            IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, 0));
                        }
                        else 
                        {
                            if(pMe->m_nCurrIdx + MAX_NUM_MENUPOP == nListItems)
                            {
                                pMe->m_nCurrIdx = 0;
                                (void)CContApp_BuildSelectMenu(pMe, pMenuCtl);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, 0));
                            }
                            else
                            {
                                pMe->m_nCurrIdx++;
                                (void)CContApp_BuildSelectMenu(pMe, pMenuCtl);
                                IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, MAX_NUM_MENUPOP -1));
                            }
                            pMe->m_wSelectCont = IMENUCTL_GetSel(pMenuCtl);
                        }
                        (void)IMENUCTL_Redraw(pMenuCtl);
                    }
                    break;
                    
                default:
                    break;
            }
            break;
            
        default:
            break;
    }
}// CContApp_DeleteMenuScroll


/*=============================================================================

FUNCTION:CContApp_BuildSelectMenu

DESCRIPTION:
    ���б�˵��������Ŀ
    
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
    
RETURN VALUE:
    SUCCESS: 
    EFAILED: If the number of current cache is 0
    
COMMENTS:
    
SIDE EFFECTS:
    pMe->m_nCurrIdx
    MAX_NUM_MENUPOP
    
SEE ALSO:

=============================================================================*/
int CContApp_BuildSelectMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    AEEAddCacheInfo *pCache;
    int             i = 0;
    int32           nListItems;
    
    ASSERT(pMe != NULL); 
    
    FARF(ADDR, ("Build Select Menu"));
    
    if (pMenuCtl == NULL)
    {
        return EBADPARM;
    }
    
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    nListItems = IVector_Size(pMe->m_pAddList);
    
    // Adjust the current index
    if (pMe->m_nCurrIdx < 0)
    {
        // ���� Index
        pMe->m_nCurrIdx += nListItems;
    }
    else if(pMe->m_nCurrIdx >= nListItems)
    {
        pMe->m_nCurrIdx -= nListItems;
    }//else Remain the current value

    // �����ǰ�ļ�¼��ĿС������һ��������˵���Ŀ MAX_NUM_MENUPOP
    // ��ֱ����ӡ����򲿷ֽ������
    if (SUCCESS != CContApp_EnumContInit(pMe, (uint32)pMe->m_nCurrIdx))
    {
        // Add phone record to menu
        for (i=0; i<nListItems; i++)
        {
            pCache = (AEEAddCacheInfo *)IVector_ElementAt(pMe->m_pAddList, (uint32)i);
            (void)CContApp_SelectItem(pMe, pMenuCtl, pCache);
        }
        CContApp_RestoreMenuRect(pMe, pMenuCtl);
    }
    else
    {
        uint32   nContIdx;
        
        // ÿ�������� MAX_NUM_MENUPOP ����¼
        for (i=0; i<MAX_NUM_MENUPOP; i++)
        {
            nContIdx = CContApp_EnumContNext(pMe);
            
            FARF(ADDR, ("Cont idx %d curr idx %d",nContIdx, pMe->m_nCurrIdx));
            
            // Read data from the phone
            pCache = (AEEAddCacheInfo *)IVector_ElementAt(pMe->m_pAddList, nContIdx);
            (void)CContApp_SelectItem(pMe, pMenuCtl, pCache);
        }

        CContApp_DrawScrollBar(pMe, pMenuCtl);
    }
    
    // Set menu select
    if (pMe->m_wSelectCont != CONTAPP_RECID_NULL)
    {
        IMENUCTL_SetSel(pMenuCtl, pMe->m_wSelectCont);
    }
    else
    {
        IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, 0));
    }

    return SUCCESS;
}// CContApp_BuildCopyMenu

/*=============================================================================

FUNCTION:CContApp_BuildOneDialNumFldSelMenu

DESCRIPTION:
    ��OneDial NumSel�˵�����Ӽ�¼������Ŀ
    
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
    
RETURN VALUE:
    SUCCESS: 
    EFAILED: If the number of current cache is 0
    
COMMENTS:
    
SIDE EFFECTS:
    
SEE ALSO:

=============================================================================*/
int CContApp_BuildOneDialNumFldSelMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    int           i;
    uint32        nFldsNum;
    AEEAddrField *pFld;
    CtlAddItem    ai;
    
    ASSERT(pMe != NULL); 
    
    FARF(ADDR, ("Build OneDialNumFldSel menu"));
    
    if(pMenuCtl == NULL)
    {
        return EBADPARM;
    }
    
    nFldsNum = IVector_Size(pMe->m_pFldIv);
    
    // ���������Menu�ؼ�����������Cache��һ��
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    if(nFldsNum == 0)
    {
        FARF(ADDR, ("No fields to List"));
        return EFAILED;
    }

    if(SUCCESS != CContApp_GetConfig( pMe,
                                      (ContAppCFG)pMe->m_wSelectOneDial,
                                      pMe->m_pOneDialBuf,
                                      sizeof(pMe->m_pOneDialBuf)))
    {
        FARF(ADDR, ("Failed to get onedial"));
        return EFAILED;
    }
    
    //��ʼ���˵���ʽ
    MEMSET(&ai, 0, sizeof(ai));
    ai.pszResText = CONTAPP_RES_FILE_LANG;
    ai.pszResImage = AEE_APPSCOMMONRES_IMAGESFILE;
    
    for(i=0; i<(int)nFldsNum; i++)
    {
        pFld = IVector_ElementAt( pMe->m_pFldIv, (uint32)i );
        if(NULL == pFld)
        {
            continue;
        }
        
        ai.wItemID = (uint16)i;
        switch(pFld->fID)
        {
            case AEE_ADDRFIELD_NAME:
				#if 0
                (void)IMENUCTL_SetTitle(pMenuCtl, NULL, 0, pFld->pBuffer); 
				#else
				{
					IANNUNCIATOR_SetFieldTextEx(pMe->m_pIAnn,pFld->pBuffer,FALSE);
				}
				#endif
                break;
            
            case AEE_ADDRFIELD_NOTES:
            case AEE_ADDRFIELD_BIRTHDAY:
            case AEE_ADDRFIELD_PHOTO:
            case AEE_ADDRFIELD_RINGTONE:
                break;
                
            default:
                ai.wImage = CContApp_GetFldImgID(pFld->fID);
                ai.pText  = pFld->pBuffer;
                ai.wText  = 0;
                
                if(ai.pText)
                {
                    if(FALSE == IMENUCTL_AddItemEx( pMenuCtl, &ai ))
                    {
                        FARF(ADDR, ("Failed to add %d item %d",pFld->fID, i));
                        return EFAILED;
                    }
                }
                else
                {
                    FARF(ADDR, ("Field pBuffer is NULL %d", i));
                }
                
                break;
        }
        
        if(IS_NUM_FIELD(pFld->fID))
        {
            if(0 == WSTRCMP(pMe->m_pOneDialBuf, (AECHAR *)pFld->pBuffer))
            {
                ERR("0",0,0,0);
                pMe->m_wSelectFld = i;
            }
            ERR("pMe->m_wSelectFld = %d",pMe->m_wSelectFld,0,0);
        }

    }
    
    // ����Ѿ����������ָ���ѡ��ֵ
    IMENUCTL_SetSel(pMenuCtl, pMe->m_wSelectFld);
    
    // ���¸�ֵ������ɾ���ֶκ���ִ���
    pMe->m_wSelectFld = IMENUCTL_GetSel(pMenuCtl);
    
    return SUCCESS;
}// CContApp_BuildOneDialNumFldSelMenu

/*=============================================================================

FUNCTION:CContApp_BuildSmartMenu

DESCRIPTION:
    ��˵���������ܲ��ҵ�����
    
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
    
RETURN VALUE:
    SUCCESS: 
    EFAILED: If the number of current cache is 0
    
COMMENTS:
    
SIDE EFFECTS:
    
SEE ALSO:

=============================================================================*/
int CContApp_BuildSmartMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    int             i;
    AEEAddCacheInfo *pCache=NULL;
    int             smartmaxpop;
    
    ASSERT(pMe != NULL); 
    
    FARF(ADDR, ("Build Smart menu: %d  %d", pMe->m_nCurrSmartIdx, pMe->m_pSmartBufLen));
    
    if(pMenuCtl == NULL)
    {
        return EBADPARM;
    }
    
    smartmaxpop = MAX_NUM_MENUPOP;

    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    // Adjust the current index
    if(pMe->m_nCurrSmartIdx < 0)
    {
        // ���� Index
        pMe->m_nCurrSmartIdx += (int)pMe->m_pSmartBufLen;
    }
    else if(pMe->m_nCurrSmartIdx >= (int)pMe->m_pSmartBufLen)
    {
        pMe->m_nCurrSmartIdx -= (int)pMe->m_pSmartBufLen;
    }//else Remain the current value
    
    // �����ǰ�ļ�¼��ĿС������һ��������˵���Ŀ MAX_NUM_MENUPOP
    // ��ֱ����ӡ����򲿷ֽ������
    if (pMe->m_pSmartBufLen <= smartmaxpop)
    {
        for (i=0; i<(int)pMe->m_pSmartBufLen; i++)
        {
            pCache = (AEEAddCacheInfo *)IVector_ElementAt(pMe->m_pAddList, pMe->m_pSmartFindBuf[i]);
            (void)CContApp_SmartItems(pMe, pMenuCtl, (AEEAddCacheInfo *)pCache);
        }
        CContApp_RestoreMenuRect(pMe, pMenuCtl);
    }
    else
    {
        uint32   nContIdx = (uint32)pMe->m_nCurrSmartIdx;
        
        // ÿ�������� MAX_NUM_MENUPOP ����¼
        for (i=0; i<smartmaxpop; i++, nContIdx++)
        {
            if (nContIdx >= pMe->m_pSmartBufLen)
            {
                nContIdx -= pMe->m_pSmartBufLen;
            }
            
            pCache = (AEEAddCacheInfo *)IVector_ElementAt(pMe->m_pAddList, pMe->m_pSmartFindBuf[nContIdx]);
            (void)CContApp_SmartItems(pMe, pMenuCtl, (AEEAddCacheInfo *)pCache);
        }
        CContApp_DrawScrollBar(pMe, pMenuCtl);
    }
    
    // Set menu select
    if(pMe->m_wSelectCont != CONTAPP_RECID_NULL)
    {
        IMENUCTL_SetSel(pMenuCtl, pMe->m_wSelectCont);
    }
    else
    {
        pMe->m_wSelectCont = IMENUCTL_GetItemID(pMenuCtl, 0);
        IMENUCTL_SetSel(pMenuCtl, IMENUCTL_GetItemID(pMenuCtl, 0));
    }
    return SUCCESS;
}// CContApp_BuildSmartMenu

/*=============================================================================

FUNCTION:CContApp_BuildSmartStateReturnMenu

DESCRIPTION:
    ���ܲ�����Ϻ���Ҫ���ص��б�˵�
    
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
    
RETURN VALUE:
    SUCCESS: 
    EFAILED: If the number of current cache is 0
    
COMMENTS:
    
SIDE EFFECTS:
    
SEE ALSO:

=============================================================================*/
int CContApp_BuildSmartStateReturnMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{    
    ASSERT(pMe != NULL); 
    
    FARF(ADDR, ("Build SmartStateReturn menu"));
    
    if(pMenuCtl == NULL)
    {
        return EBADPARM;
    }
    
    switch(pMe->m_nSmartStateType)
    {
      case SMART_STATE_IDD_LIST:
          (void)CContApp_BuildListMenu(pMe, pMenuCtl);
          break;
    
      case SMART_STATE_IDD_SELECT:
          (void)CContApp_BuildSelectMenu(pMe, pMenuCtl);
          break;
    
       case SMART_STATE_IDD_COPY:
          (void)CContApp_BuildCopyMenu(pMe, pMenuCtl);
          break;
    
      case SMART_STATE_IDD_SELECT_RECORD:
      case SMART_STATE_IDD_DELETE:
          (void)CContApp_BuildDeleteMenu(pMe, pMenuCtl);
          break; 
          
      default:
          return EBADPARM;         
    }  

    return SUCCESS;
}// CContApp_BuildSmartMenu

/*=============================================================================

FUNCTION:CContApp_BuildGroupMenu

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
int CContApp_BuildGroupMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    int           i=0;
    CtlAddItem    ai;
    AECHAR        pGroup[MAX_INPUT_NAME_EN + 1];
    int           ResID = 0;    
    ASSERT(pMe != NULL); 
    
     // Check Parameter
    if(!pMenuCtl)
    {
        return EBADPARM;
    }   
    
    // Clear items
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    // Init items
    MEMSET(&ai, 0, sizeof(ai));
    ai.pszResText = CONTAPP_RES_FILE_LANG;

    ai.pText = NULL;
    ai.wText       = IDS_GROUP_NONE_STRING;
    ai.wItemID   = IDS_GROUP_NONE_STRING;
           
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
       FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
       //return EFAILED;
    }  

#ifdef FEATURE_RUIM_PHONEBOOK
    if(IsRunAsUIMVersion() && pMe->m_bADNRUIMSupport)
    {
        ai.pText = NULL;
        ai.wText       = IDS_GROUP_UIM;
        ai.wItemID   = IDS_GROUP_UIM;
               
        if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
        {
           FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
           //return EFAILED;
        }
    }
#endif

    for( i = CONTCFG_GROUP1; i <= CONTCFG_GROUP6; i++)
    {
        pGroup[0] = ONEDIAL_NULL;
        MSG_FATAL("pGroup[0] = ONEDIAL_NULL",0,0,0);
        if(SUCCESS != CContApp_GetConfig( pMe,
                                          (ContAppCFG)i,
                                          pGroup,
                                          sizeof(pGroup)))
        {
            FARF(ADDR, ("Failed to get %d group", i));
            continue;
        }
        
        // set menu item id
        switch(i)
        {
            case CONTCFG_GROUP1:
                ai.wItemID = IDS_GROUP_VIP;
                ResID = IDS_GROUP_VIP;
                break;

            case CONTCFG_GROUP2:
                ai.wItemID = IDS_GROUP_HOME;
                ResID = IDS_GROUP_HOME;
                break;

            case CONTCFG_GROUP3:
                ai.wItemID = IDS_GROUP_WORK;
                ResID = IDS_GROUP_WORK;
                break;

            case CONTCFG_GROUP4:
                ai.wItemID = IDS_GROUP_FRIEND;
                ResID = IDS_GROUP_FRIEND;
                break;

            case CONTCFG_GROUP5:
                ai.wItemID = IDS_GROUP_PERSONAL;
                ResID = IDS_GROUP_PERSONAL;
                break;

            case CONTCFG_GROUP6:
                ai.wItemID = IDS_GROUP_BUSINESS;
                ResID = IDS_GROUP_BUSINESS;
                break;                
        }  
             
        if(pGroup[0] == ONEDIAL_NULL)
        {
            ISHELL_LoadResString ( pMe->m_pShell, 
                                CONTAPP_RES_FILE_LANG, 
                                ResID,
                                pGroup,
                                sizeof(pGroup));                  
        }
        
        ai.pText  = pGroup; 
        ai.wText  = 0;               
        
        if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
        {
            FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
            //return EFAILED;
        }           
    }

    //(void)IMENUCTL_Redraw(pMenuCtl);
    return SUCCESS;
}// CContApp_BuildOptsMenu

/*=============================================================================

FUNCTION:CContApp_BuildNunFldMenu

DESCRIPTION:
    ��View�˵�����Ӽ�¼������Ŀ
    
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
    
RETURN VALUE:
    SUCCESS: 
    EFAILED: If the number of current cache is 0
    
COMMENTS:
    
SIDE EFFECTS:
    
SEE ALSO:

=============================================================================*/
int CContApp_BuildNunFldMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    int           i;
    uint32        nFldsNum;
    AEEAddrField *pFld;
    CtlAddItem    ai;
    
    ASSERT(pMe != NULL); 
    
    FARF(ADDR, ("Build View menu"));
    
    if(pMenuCtl == NULL)
    {
        return EBADPARM;
    }
    
    nFldsNum = IVector_Size(pMe->m_pFldIv);
    
    // ���������Menu�ؼ�����������Cache��һ��
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    if(nFldsNum == 0)
    {
        FARF(ADDR, ("No fields to List"));
        return EFAILED;
    }
    
    //��ʼ���˵���ʽ
    MEMSET(&ai, 0, sizeof(ai));
    ai.pszResText = CONTAPP_RES_FILE_LANG;
    ai.pszResImage = AEE_APPSCOMMONRES_IMAGESFILE;
    
    for(i=0; i<(int)nFldsNum; i++)
    {
        pFld = IVector_ElementAt( pMe->m_pFldIv, (uint32)i );
        if(NULL == pFld)
        {
            continue;
        }

        if (IS_NUM_FIELD(pFld->fID))
        {
            ai.wItemID = (uint16)pFld->fID;
            //select menu icon image
            ai.wImage = CContApp_GetFldImgID(pFld->fID);

            ai.pText  = (AECHAR *)pFld->pBuffer;
            ai.wText  = 0;
            
            if(FALSE == IMENUCTL_AddItemEx( pMenuCtl, &ai ))
            {
                FARF(ADDR, ("Failed to add %d item %d",pFld->fID, i));
                return EFAILED;
            }
            else
            {
                FARF(ADDR, ("Field pBuffer is NULL %d", i));
            }   
        }        
    }
    
    return SUCCESS;
}// CContApp_BuildNunFldMenu

/*=============================================================================

FUNCTION:CContApp_BuildSelectOptMenu

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
int CContApp_BuildSelectOptMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    CtlAddItem ai;
    sSelectFieldInfo curSelectFieldInfo;
    
    ASSERT(pMe != NULL); 
    
     // Check Parameter
    if(!pMenuCtl)
    {
        return EBADPARM;
    }
    
    // Clear items
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    // Init items
    MEMSET(&ai, 0, sizeof(ai));
    ai.pszResText = CONTAPP_RES_FILE_LANG;

#ifdef FEATURE_VERSION_W317A
    if(pMe->m_nCopyOrMove == MOVEMULTIPE ||pMe->m_nCopyOrMove == COPYMULTIPE)
    {
        ai.wText       = IDS_COPYMOVE;	
    }
    else
    {
    	ai.wText       = IDS_EXECUTE;
    }
#else
    ai.wText       = IDS_EXECUTE;
#endif
    ai.wItemID   = IDS_EXECUTE;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
       FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
       return EFAILED;
    }    
    
    MEMSET(&curSelectFieldInfo, 0, sizeof(curSelectFieldInfo)); 
    curSelectFieldInfo.wIndex = pMe->m_wSelectCont;
    if(TYPE_INDEX_MATCH == CContApp_FindSelectFieldListNode(pSelectFieldListRoot, &curSelectFieldInfo))
    {
        ai.wText       = IDS_UNMARK;
        ai.wItemID   = IDS_UNMARK;
               
        if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
        {
            FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
            return EFAILED;
        }
    }
    else
    {
        ai.wText       = IDS_MARK;
        ai.wItemID   = IDS_MARK;
               
        if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
        {
            FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
            return EFAILED;
        }
    }


    ai.wText       = IDS_MARKALL;
    ai.wItemID   = IDS_MARKALL;
           
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    }

    ai.wText       = IDS_UNMARKALL;
    ai.wItemID     = IDS_UNMARKALL;
           
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    }
    return SUCCESS;
}// CContApp_BuildOptsMenu

/*=============================================================================

FUNCTION:CContApp_BuildGroupList

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
int CContApp_BuildGroupList(CContApp  *pMe, IMenuCtl  *groupList)
{
    int           i=0;
    CtlAddItem    ai;
    AECHAR        pGroup[MAX_INPUT_NAME_EN + 1];
    uint16        ResID = 0;
    
    if( NULL == groupList)
    {
        
        FARF(ADDR, ("CContApp_BuildGroupList  point is NULL"));
        return EFAILED;
    }

    // Clear items
    (void)IMENUCTL_DeleteAll(groupList);

    // Init items
    MEMSET(&ai, 0, sizeof(ai));
    ai.pszResText = CONTAPP_RES_FILE_LANG;

    ai.pText = NULL;
    ai.wText       = IDS_GROUP_NONE_STRING;
    ai.wItemID   = IDS_GROUP_NONE_STRING;
            
    if(FALSE == IMENUCTL_AddItemEx(groupList, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    }
    
    for( i = CONTCFG_GROUP1; i <= CONTCFG_GROUP6; i++)
    {
        pGroup[0] = ONEDIAL_NULL;
        MSG_FATAL("pGroup[0] = ONEDIAL_NULL",0,0,0);
        if(SUCCESS != CContApp_GetConfig( pMe,
                                          (ContAppCFG)i,
                                          pGroup,
                                          sizeof(pGroup)))
        {
            FARF(ADDR, ("Failed to get %d group", i));
            continue;
        }
        
        // set menu item id
        switch(i)
        {
            case CONTCFG_GROUP1:
                ai.wItemID = IDS_GROUP_VIP;
                ResID = IDS_GROUP_VIP;
                break;

            case CONTCFG_GROUP2:
                ai.wItemID = IDS_GROUP_HOME;
                ResID = IDS_GROUP_HOME;
                break;

            case CONTCFG_GROUP3:
                ai.wItemID = IDS_GROUP_WORK;
                ResID = IDS_GROUP_WORK;
                break;

            case CONTCFG_GROUP4:
                ai.wItemID = IDS_GROUP_FRIEND;
                ResID = IDS_GROUP_FRIEND;
                break;

            case CONTCFG_GROUP5:
                ai.wItemID = IDS_GROUP_PERSONAL;
                ResID = IDS_GROUP_PERSONAL;
                break;

            case CONTCFG_GROUP6:
                ai.wItemID = IDS_GROUP_BUSINESS;
                ResID = IDS_GROUP_BUSINESS;
                break;                
        }  
             
        if(pGroup[0] == ONEDIAL_NULL)
        {
            ISHELL_LoadResString ( pMe->m_pShell, 
                                CONTAPP_RES_FILE_LANG, 
                                ResID,
                                pGroup,
                                sizeof(pGroup));                  
        }
        
            ai.pText  = pGroup; 
            ai.wText  = 0;               
        
        if(FALSE == IMENUCTL_AddItemEx(groupList, &ai))
        {
            FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
            return EFAILED;
        }           
    }

return SUCCESS;
}//CContApp_BuildGroupList

/*=============================================================================

FUNCTION:CContApp_BuildSaveToMenu

DESCRIPTION:
        
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
RETURN VALUE:
    SUCCESS: 
    EFAILED: If the number of current cache is 0
    
COMMENTS:
    
SIDE EFFECTS:
    
SEE ALSO:

=============================================================================*/
int CContApp_BuildSaveToMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    CtlAddItem ai;
    
    ASSERT(pMe != NULL); 
    
     // Check Parameter
    if(!pMenuCtl)
    {
        return EBADPARM;
    }
    
    // Clear items
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    // Init items
    MEMSET(&ai, 0, sizeof(ai));
    ai.pszResText = CONTAPP_RES_FILE_LANG;

    ai.wText       = IDS_POSITION_PHONE;
    ai.wItemID   = IDS_POSITION_PHONE;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
       FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
       return EFAILED;
    }    

    ai.wText       = IDS_UIMCARD;
    ai.wItemID   = IDS_UIMCARD;
           
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    }

#ifdef FEATURE_VERSION_W317A
	ai.wText       = IDS_ALWAYS_ASK;
    ai.wItemID   = IDS_ALWAYS_ASK;
#else
    ai.wText       = IDS_POSITION_SELECT;
    ai.wItemID   = IDS_POSITION_SELECT;
#endif	
           
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    }

    return SUCCESS;
}// CContApp_BuildSaveToMenu


/*=============================================================================

FUNCTION:CContApp_BuildSelectPostionMenu

DESCRIPTION:
        
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
RETURN VALUE:
    SUCCESS: 
    EFAILED: If the number of current cache is 0
    
COMMENTS:
    
SIDE EFFECTS:
    
SEE ALSO:

=============================================================================*/
int CContApp_BuildSelectPostionMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    CtlAddItem ai;
    
    ASSERT(pMe != NULL); 
    
     // Check Parameter
    if(!pMenuCtl)
    {
        return EBADPARM;
    }
    
    // Clear items
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    // Init items
    MEMSET(&ai, 0, sizeof(ai));
    ai.pszResText = CONTAPP_RES_FILE_LANG;

    ai.wText       = IDS_POSITION_PHONE;
    ai.wItemID   = IDS_POSITION_PHONE;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
       FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
       return EFAILED;
    }    

    ai.wText       = IDS_UIMCARD;
    ai.wItemID   = IDS_UIMCARD;
           
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    }

    return SUCCESS;
}// CContApp_BuildSelectPostionMenu


/*=============================================================================

FUNCTION:CContApp_BuildViewTypeMenu

DESCRIPTION:
PARAMETERS:
    pMe     :
    pMenuCtl: ָ��Menu Control��ָ��
RETURN VALUE:
    SUCCESS: 
    EFAILED: If the number of current cache is 0
    
COMMENTS:
    
SIDE EFFECTS:
    
SEE ALSO:

=============================================================================*/
int CContApp_BuildViewTypeMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    CtlAddItem ai;
    
    ASSERT(pMe != NULL); 
    
     // Check Parameter
    if(!pMenuCtl)
    {
        return EBADPARM;
    }
    
    // Clear items
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    // Init items
    MEMSET(&ai, 0, sizeof(ai));
    ai.pszResText = CONTAPP_RES_FILE_LANG;

    ai.wText       = IDS_POSITION_PHONE;
    ai.wItemID   = IDS_POSITION_PHONE;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
       FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
       return EFAILED;
    }    

    ai.wText       = IDS_UIMCARD;
    ai.wItemID   = IDS_UIMCARD;
           
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    }

    ai.wText       = IDS_POSITION_ALL;
    ai.wItemID   = IDS_POSITION_ALL;
           
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    }

    return SUCCESS;
}// CContApp_BuildViewTypeMenu

int CContApp_BuildGroupOptMenu(CContApp *pMe, IMenuCtl *pMenuCtl)
{
    CtlAddItem ai;
    
    ASSERT(pMe != NULL); 
    
     // Check Parameter
    if(!pMenuCtl)
    {
        return EBADPARM;
    }
    
    // Clear items
    (void)IMENUCTL_DeleteAll(pMenuCtl);
    
    // Init items
    MEMSET(&ai, 0, sizeof(ai));
    ai.pszResText = CONTAPP_RES_FILE_LANG;

    ai.wText       = IDS_VIEW;
    ai.wItemID   = IDS_VIEW;
            
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
       FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
       return EFAILED;
    }    

    ai.wText       = IDS_RENAME;
    ai.wItemID   = IDS_RENAME;
           
    if(FALSE == IMENUCTL_AddItemEx(pMenuCtl, &ai))
    {
        FARF(ADDR, ("Failed to Add Opts item %d", ai.wItemID));
        return EFAILED;
    }

    return SUCCESS;

}


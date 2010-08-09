#ifndef CONTAPPBUILDMENU_H
#define CONTAPPBUILDMENU_H

/*==============================================================================
// �ļ���
//        ContAppBuildMenu.h
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
07-12-05                 ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
int CContApp_BuildOptsMenu(CContApp *pMe, IMenuCtl *pMenuCtl, boolean bAll);

int CContApp_BuildMainMenuMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

int CContApp_BuildAddMenuMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

int CContApp_BuildEditMenuMenu(CContApp *pMe, IMenuCtl *pMenuCtl, boolean isAll);

int CContApp_BuildListMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

int CContApp_BuildViewMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

int CContApp_BuildOneDialMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

int CContApp_BuildSearchMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

void CContApp_ListMenuScroll( CContApp *pMe, IMenuCtl *pMenuCtl, AEEEvent  eCode, uint16 wParam);

void CContApp_DeleteMenuScroll( CContApp *pMe, IMenuCtl *pMenuCtl, AEEEvent  eCode, uint16 wParam);

int CContApp_BuildDeleteMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

void CContApp_CopyMenuScroll( CContApp *pMe, IMenuCtl *pMenuCtl, AEEEvent  eCode, uint16 wParam);

int CContApp_BuildCopyMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

void CContApp_SelectMenuScroll( CContApp *pMe, IMenuCtl *pMenuCtl, AEEEvent  eCode, uint16 wParam);

int CContApp_BuildSelectMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

int CContApp_BuildOneDialNumFldSelMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

int CContApp_BuildSmartMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

int CContApp_BuildSmartStateReturnMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

int CContApp_BuildGroupMenu(CContApp * pMe, IMenuCtl * pMenuCtl);

int CContApp_BuildNunFldMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

int CContApp_BuildSelectOptMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

int CContApp_BuildNumFldMenuMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

int CContApp_BuildManagementMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

int CContApp_BuildDeleteSelectMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

int CContApp_BuildGroupList(CContApp  *pMe, IMenuCtl  *groupList);

int CContApp_BuildSaveToMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

int CContApp_BuildSelectPostionMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

int CContApp_BuildViewTypeMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

int CContApp_BuildGroupOptMenu(CContApp *pMe, IMenuCtl *pMenuCtl);

void CContApp_Select_record_MenuScroll( CContApp *pMe, IMenuCtl *pMenuCtl, AEEEvent  eCode, uint16 wParam);
#endif //CONTAPPBUILDMENU_H


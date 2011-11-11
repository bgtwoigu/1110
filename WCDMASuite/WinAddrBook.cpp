#include "StdAfx.h"
#include "winaddrbook.h"
#include "ProgressWnd.h"
#include "dlgmsg.h"
#include "GlobalDataCenter.h"
#include "Util.h"


CWinAddrBook::CWinAddrBook(void):lpWABObject(NULL),
lpAddrBook(NULL), IsInit(false)
{

	//Opens the default WAB file in the system and loads it into the object
	HINSTANCE hinstWAB=NULL;
	HRESULT hr=E_FAIL;
	HKEY keyResult;
	BYTE keyValue[MAX_PATH]={0};
	DWORD dataout=800;

	RegOpenKeyEx(HKEY_LOCAL_MACHINE, WAB_DLL_PATH_KEY, 0, KEY_QUERY_VALUE, &keyResult); //"Software\\Microsoft\\WAB\\DLLPath"
	long result = RegQueryValueEx(keyResult, TEXT(""), 0, 0, keyValue, &dataout);
	RegCloseKey(keyResult);

	//Get the full path of WAB and store in PathToWAB
	
	_tcscpy(PathToWAB, reinterpret_cast<TCHAR*>(keyValue));

	//Now let us load the library
	hModule = LoadLibrary(PathToWAB);

}

CWinAddrBook::~CWinAddrBook(void)
{
	//Free the library we use to get the WAB file
	if(hModule != NULL)
	{
		FreeLibrary(hModule);
	}
	
	IsInit = false;
}

// Initialize the address book and basically open the address book
int CWinAddrBook::InitAddrBook(void)
{
	//After opening the library go ahead and try and get the values into memory
	//maybe implement a memory map file later depending on how big some on the 
	//performance of this class after release, and on some people's systems (i.e. huge address files)
	//
	// For now go ahead and store the information in a STL Vector

		if (hModule!=NULL)
		{
			//We're safe the module was initialzised let's do what we need to do
			ssWABOpen = (fWABOpen)GetProcAddress(hModule, "WABOpen");
			//If not successful throw an error throw the value = 2
			if (ssWABOpen == NULL) return 2;
			//It is successful call it
			HRESULT hr = (ssWABOpen)(&lpAddrBook, &lpWABObject, NULL, 0);
		}
		
		IsInit = true;
		//LoadWABs(true); //Moved here was being weird when called as another function from outsidethe class... hmmmm... go figure?!?

		//Everything was OK
		return 0;
}

// Load email addresses into the vector
void CWinAddrBook::LoadWABs()
{
	LoadAddr(lpAddrBook);

}

void CWinAddrBook::InsertWABs(const CPhonebookPC& itemList)
{
	InsertAddr(lpAddrBook,itemList);
}
// Test mostly for debug purposes so you can step through and ensure that all the information is present for the address book vector
void CWinAddrBook::TestAddrBook(void)
{
	for (int x =0; x < (int)AddrMemBook.size(); x++)
	{
		//Set break here maybe later do a trace command instead
		WABS thisAddr = AddrMemBook[x];
		TRACE(thisAddr.DisplayName/*.c_str()*/); TRACE(TEXT("\r\n"));
		TRACE(thisAddr.EmailAddr/*.c_str()*/); TRACE(TEXT("\r\n"));
		TRACE(thisAddr.homePhone/*.c_str()*/); TRACE(TEXT("\r\n"));
		TRACE(thisAddr.officePhone/*.c_str()*/); TRACE(TEXT("\r\n"));
		TRACE(thisAddr.mobilePhone/*.c_str()*/); TRACE(TEXT("\r\n"));

		
	}
}


void CWinAddrBook::LoadAddr(void* pAB)
{
	ULONG cbEntryID = 0;
	ENTRYID* lpEntryID;

	LPADRBOOK pAddrBook = reinterpret_cast<LPADRBOOK>(pAB);
	HRESULT hr = pAddrBook->GetPAB(&cbEntryID, &lpEntryID);
	if(hr != S_OK)return;

	LPUNKNOWN lpIUnknown = NULL;
	ULONG ulObjType = 0;

	hr = pAddrBook->OpenEntry(
		cbEntryID,
		lpEntryID,
		NULL,
		MAPI_MODIFY,
		&ulObjType,
		&lpIUnknown);

	if(hr == S_OK )
	{
		if(ulObjType == MAPI_ABCONT)
		{
			LoadABContainer(static_cast<IABContainer*>(lpIUnknown)); 
		}
		
		if(lpIUnknown != NULL)
		{
			lpIUnknown->Release();
		}
		
	}
	
	LPWABOBJECT pWABObj = reinterpret_cast<LPWABOBJECT>(lpWABObject);
	pWABObj->FreeBuffer(lpEntryID);
}


void CWinAddrBook::InsertAddr(void* pAB,const CPhonebookPC& itemList)
{
	ULONG cbEntryID = 0;
	ENTRYID* lpEntryID;
	LPABCONT lpContainer =   NULL; //test

	LPADRBOOK pAddrBook = reinterpret_cast<LPADRBOOK>(pAB);
	HRESULT hr = pAddrBook->GetPAB(&cbEntryID, &lpEntryID);
	if(hr != S_OK)return;


	LPUNKNOWN lpIUnknown = NULL;
	ULONG ulObjType = 0;

	hr = pAddrBook->OpenEntry(
		cbEntryID,
		lpEntryID,
		NULL,
		MAPI_MODIFY,
		&ulObjType,
		&lpIUnknown);

	lpContainer = (LPABCONT)lpIUnknown;//test
	//������ʾ
	CProgressWnd wndProgress(AfxGetMainWnd(), LoadStringEx(IDS_SYN_PROGRESS));
	int count = itemList.Size();
	int nFailed = 0;
	int nsuccessed = 0;
	wndProgress.GoModal();
	wndProgress.SetRange(0,count);
	wndProgress.SetText(LoadStringEx(IDS_SYNTO_OE));
	//������������
	for(int i=0;i<count;i++)
	{
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		
		if (wndProgress.Cancelled()) 
		{
			nFailed = count-i;
			GMessageBox(LoadStringEx(IDS_SYN_HASCANCEL));
			break;
		}
		TPhoneBookItemEx item = itemList.GetAt(i);
		const TCHAR* strcompanyDuty = (LPCTSTR )item.strBusiness;	//ְ��
		const TCHAR* strEmail =   (LPCTSTR )item.strEmail;//�ʼ�
		const TCHAR* strCountry = (LPCTSTR )item.strCountry;  //����/����
		const TCHAR* strHomePhone = (LPCTSTR )item.strHomePhone;	//��ͥ�绰
		const TCHAR* strFax = (LPCTSTR )item.strFax;			//����
		const TCHAR* strOffice = (LPCTSTR )item.strOffice;		//��λ	
		const TCHAR* strOfficePhone = (LPCTSTR )item.strOfficePhone;	//�칫�绰
		const TCHAR* strMobilePhone = (LPCTSTR )item.strMobilePhone;//�ƶ��绰
		const TCHAR* strName = (LPCTSTR )item.strName;//����
		
		  
   		
   
		SPropValue   arrProp[9];  
		memset(arrProp,0,sizeof(SPropValue)*9);
   
		arrProp[0].ulPropTag   =   PR_DISPLAY_NAME;  
		arrProp[0].Value.lpszW   =   (LPTSTR)strName; 
			
		arrProp[1].ulPropTag   =   PR_EMAIL_ADDRESS;  
		arrProp[1].Value.lpszW   =   (LPTSTR)strEmail; 
		
		arrProp[2].ulPropTag   =   PR_OFFICE_TELEPHONE_NUMBER;  
		arrProp[2].Value.lpszW   =   (LPTSTR)strOfficePhone;
		arrProp[3].ulPropTag   =   PR_HOME_TELEPHONE_NUMBER;  
		arrProp[3].Value.lpszW   =   (LPTSTR)strHomePhone;
		arrProp[4].ulPropTag   =   PR_MOBILE_TELEPHONE_NUMBER;  
		arrProp[4].Value.lpszW   =   (LPTSTR)strMobilePhone;
		arrProp[5].ulPropTag   =   PR_TITLE;  
		arrProp[5].Value.lpszW   =   (LPTSTR)strcompanyDuty;
		arrProp[6].ulPropTag   =   PR_COUNTRY;  
		arrProp[6].Value.lpszW   =   (LPTSTR)strCountry;
		arrProp[7].ulPropTag   =   PR_BUSINESS_FAX_NUMBER;  
		arrProp[7].Value.lpszW   =   (LPTSTR)strFax;
		arrProp[8].ulPropTag   =   PR_COMPANY_NAME;  
		arrProp[8].Value.lpszW   =   (LPTSTR)strOffice;

		 //��ַ������  
		LPMAPIPROP lpProp = NULL;  
		hr = lpContainer->CreateEntry(0,NULL,0,&lpProp);  

		if(hr == S_OK)  
		{  
			hr   =   lpProp->SetProps(9,arrProp,NULL);  
			ASSERT(hr   ==   S_OK);  
			hr   =   lpProp->SaveChanges(0); 
			if(lpProp != NULL)
			{
				lpProp->Release(); 
			}
			
			nsuccessed += 1;
		}  
		else  
		{  
			LPMAPIERROR   lpError =   NULL;  
			lpContainer->GetLastError(hr,0,&lpError);  

			if(lpError != NULL)  
			{  
				tstring strErr;  
				strErr = TEXT("Error:");  
				if(lpError->lpszError != NULL)  
				strErr += lpError->lpszError;  

				strErr += TEXT("\r\nContent:");  
				if(lpError->lpszComponent   !=   NULL)  
				strErr += lpError->lpszComponent;  

//				MessageBox(NULL,strErr.c_str(),"Add Error",MB_OK); 
				GMessageBox(strErr.c_str(),TEXT("Add Error"));
			} 
			nFailed += 1;
		}
	}

	wndProgress.m_bCancelled = true;
	wndProgress.Hide();
	if(nsuccessed>0 )//
	{
		
		if((nsuccessed + nFailed)<count)
		{
			nFailed = count - nsuccessed;
		}
		//��ɺ����ؽ�����ʾ		
		CString strMsg ;
			CString strA(LoadStringEx(IDS_TRY_SAMESTEP_A));
			CString strB;
			strB.Format(TEXT("%d"), count);
			strA = strA + strB;
			strA += LoadStringEx(IDS_TRY_SAMESTEP_B);
			CString strC;
			strC.Format(TEXT("%d"), nsuccessed);
			strA += strC;
			strA += LoadStringEx(IDS_TRY_SAMESTEP_C);
			CString strD;
			strD.Format(TEXT("%d"), nFailed);
			strA += strD;
			strA += LoadStringEx(IDS_OUTLOOK_INFO_F);
			strMsg = strA;
		//	strMsg.Format("����ͬ�� %d ����ϵ�˼�¼���ɹ�%d��,ʧ��%d����",count,succeeded,failed);
		
		GMessageBox(strMsg,NULL,MB_OK|MB_ICONASTERISK) ;

	}
	else if ( nsuccessed = count )
	{
		GMessageBox(LoadStringEx(IDS_CONTACTS_ISNULL));
	}
	else 
	{
		GMessageBox(LoadStringEx(IDS_SYN_FAIL));
	}
	
	if(lpContainer != NULL)
	{
		lpContainer->Release(); 
	}	 
	
	if(pAddrBook != NULL)
	{
		pAddrBook->Release();
	}
		
}

void CWinAddrBook::LoadABContainer(void* pContainer)
{
	IABContainer* lpABContainer = reinterpret_cast<IABContainer*>(pContainer);
	LPMAPITABLE lpMAPItbl = NULL;
			
	HRESULT hr = lpABContainer->GetContentsTable(MAPI_BEST_ACCESS, &lpMAPItbl);
	if(hr != S_OK)return;

	LoadTable(lpMAPItbl);

	if(lpMAPItbl != NULL)
	{
		lpMAPItbl->Release();
	}
	

}

void CWinAddrBook::LoadTable(void* pTable)
{
	LPMAPITABLE lpMAPItbl = reinterpret_cast<LPMAPITABLE>(pTable);

	ULONG ulRows = 0;
	HRESULT hr = lpMAPItbl->GetRowCount(0, &ulRows);
	if(hr != S_OK)return;

	SRowSet* lpRowSet = NULL;
	hr = lpMAPItbl->QueryRows(ulRows, 0, &lpRowSet);
	if(hr != S_OK)return;
	
	LoadRows(lpRowSet);

	LPWABOBJECT pWABObj = reinterpret_cast<LPWABOBJECT>(lpWABObject);
	pWABObj->FreeBuffer(lpRowSet);
}

void CWinAddrBook::LoadRows(void* pRowset)
{
	SRowSet* lpRowSet = reinterpret_cast<SRowSet*>(pRowset);
	for (ULONG i = 0; i < lpRowSet->cRows; i++)
	{
		SRow* lpRow = &lpRowSet->aRow[i];
		GetRowInfo(lpRow);

		LPWABOBJECT pWABObj = reinterpret_cast<LPWABOBJECT>(lpWABObject);
		pWABObj->FreeBuffer(lpRow);
	}
}

void CWinAddrBook::GetRowInfo(void* pRow)
{
	SRow* lpRow = reinterpret_cast<SRow*>(pRow);

	for (ULONG i = 0; i < lpRow->cValues; ++i)
	{
		SPropValue* lPropVal = &lpRow->lpProps[i];
		if(lPropVal->ulPropTag ==PR_ENTRYID)
		{
			WABS info;
			LoadMailUser(lPropVal, info);
			AddrMemBook.push_back(info);
			break;
		}
	}
}

void CWinAddrBook::LoadMailUser(void* lPropVal, WABS& info)
{
	SPropValue* lProp = reinterpret_cast<SPropValue*>(lPropVal);

	ULONG cb = lProp->Value.bin.cb;
    ENTRYID* pEntryID = (ENTRYID*)lProp->Value.bin.lpb;

	ULONG ulObjType = 0;
	LPUNKNOWN pUnknown = NULL;
	LPADRBOOK lpAdrBook = reinterpret_cast<LPADRBOOK>(lpAddrBook);
	HRESULT hr = lpAdrBook->OpenEntry(
		cb,
		pEntryID,
		NULL,
		0,
		&ulObjType,
		&pUnknown);
	if( hr == S_OK)
	{
		if(MAPI_MAILUSER == ulObjType)
		{
			GetMailUserInfo(static_cast<IMailUser*>(pUnknown), info);
		}
		if(pUnknown != NULL)
		{
			pUnknown->Release();
		}
		
		LPWABOBJECT pWABObj = reinterpret_cast<LPWABOBJECT>(lpWABObject);
		pWABObj->FreeBuffer(pEntryID);
	}
}

void CWinAddrBook::GetMailUserInfo(void* pUser, WABS& info)
{
	IMailUser* pMailUser = reinterpret_cast<IMailUser*>(pUser);

	LPSPropValue pProp = NULL;
    ULONG cb = 0;

	HRESULT hr = pMailUser->GetProps(NULL, NULL, &cb, &pProp);
	if( hr != S_OK)return;
	for(ULONG i=0; i<cb; ++i)
	{
		SPropValue& val = pProp[i];
		switch(val.ulPropTag)
		{
		case PR_EMAIL_ADDRESS_A://e-mail��ַ
			info.EmailAddr = val.Value.lpszA;
			break;
		case PR_DISPLAY_NAME_A://����
			info.DisplayName = val.Value.lpszA;
			break;
		case PR_MOBILE_TELEPHONE_NUMBER_A://�ƶ��绰
			info.mobilePhone = val.Value.lpszA;
			break;
		case PR_HOME_TELEPHONE_NUMBER_A://��ͥ�绰
			info.homePhone = val.Value.lpszA;
			break;
		case PR_OFFICE_TELEPHONE_NUMBER_A://�칫�ҵ绰
			info.officePhone = val.Value.lpszA;
			break;
		case PR_BUSINESS_FAX_NUMBER_A://��˾�������
			info.officeFax = val.Value.lpszA;
			break;
		case PR_COMPANY_NAME_A://��˾��
			info.company = val.Value.lpszA;
			break;
		case PR_COUNTRY_A://��˾���ڹ���
			info.officeCountry = val.Value.lpszA;
			break;
		case PR_DEPARTMENT_NAME_A://���ڲ���
			info.officeDepartment = val.Value.lpszA;
			break;
		case PR_HOME_FAX_NUMBER_A://��ͥ�����
			info.homeFax = val.Value.lpszA;
			break;
		case PR_STREET_ADDRESS_A://��˾���ڽֵ���ַ
			info.officeStreetAddr = val.Value.lpszA;
			break;
		case PR_STATE_OR_PROVINCE_A://��˾����ʡ
			info.officeProvince = val.Value.lpszA;
			break;
		case PR_LOCALITY_A://��˾���ڳ���
			info.officeCity = val.Value.lpszA;
			break;
		case PR_POSTAL_CODE_A://��˾�ʱ�
			info.officePostCode =val.Value.lpszA;
			break;
		case PR_OFFICE_LOCATION_A://�칫��
			info.officebureaux = val.Value.lpszA;
			break;
		case PR_TITLE://ְ��
			info.officeDuty =val.Value.lpszA;
			break;
		case PR_PAGER_TELEPHONE_NUMBER_A://Ѱ��������
			info.officeCallnum = val.Value.lpszA;
			break;
		}
	}

}
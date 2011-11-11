#include "stdafx.h"
#include "SortHeaderCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSortHeaderCtrl::CSortHeaderCtrl()
	: m_iSortColumn( -1 )
	, m_bSortAscending( TRUE )
{
//	R=39;
//	G=133;
//	B=188;//��ʼɫ��RGB 
//	SetHeadBkColor() ;
	bDown = false;
}

CSortHeaderCtrl::~CSortHeaderCtrl()
{
	headText.clear();
}


BEGIN_MESSAGE_MAP(CSortHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CSortHeaderCtrl)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSortHeaderCtrl message handlers

void CSortHeaderCtrl::SetSortArrow( const int iSortColumn, const BOOL bSortAscending )
{
	m_iSortColumn = iSortColumn;
	m_bSortAscending = bSortAscending;

	// change the item to owner drawn.
	HD_ITEM hditem;

	hditem.mask = HDI_FORMAT;
	VERIFY( GetItem( iSortColumn, &hditem ) );
	hditem.fmt |= HDF_OWNERDRAW;
	VERIFY( SetItem( iSortColumn, &hditem ) );
	bDown = true;
	// invalidate the header control so it gets redrawn
	Invalidate();
}

void CSortHeaderCtrl::ClearColTextBuf()
{
	headText.clear();

}

void CSortHeaderCtrl::SetHeadText(CString str)
{
	int i = 0;
	headText.push_back(str);
}

void CSortHeaderCtrl::OnPaint() 
{ 
	CPaintDC dc(this); // device context for painting 
	// TODO: �ڴ˴������Ϣ���������� 
	// ��Ϊ��ͼ��Ϣ���� CHeaderCtrl::OnPaint() 
	// TODO: Add your message handler code here 
	CRect lpRect;
	GetClientRect(lpRect);
	int nItem; 
	nItem = GetItemCount();//�õ��м�����Ԫ 
	CRect tRect;
	//TCHAR * szChar[4] = {_TEXT("����1"),_TEXT("����2"), 
	//_TEXT("������"),_TEXT("������")}; 
	for(int i = 0; i<nItem ;i ++) 
	{ 
		 
		GetItemRect(i,&tRect);//�õ�Item�ĳߴ� 
		int R=64,G=140,B=193;//��ʼɫ��RGB 
	/*	if(bDown)
		{
			R += 10;
			G += 10;
			B += 10;
		}
		*/
	CRect nRect(tRect);//�����ߴ絽�µ������� 
	nRect.left++;//�����ָ��ߵĵط� 
	//�������屳�� 
	for(int j = tRect.top;j<=tRect.bottom;j++) 
	{ 
		nRect.bottom = nRect.top+1; 
		CBrush _brush; 
		_brush.CreateSolidBrush(RGB(R,G,B));//������ˢ 
		dc.FillRect(&nRect,&_brush); //��䱳�� 
		_brush.DeleteObject(); //�ͷŻ�ˢ 
		R+=4;G+=2;B+=1; 
		if(R<0)
			R = 39;
		if(G<0)
			G = 133;
		if(B<0)
			B = 188;
		nRect.top = nRect.bottom; 
	} 
	dc.SetBkMode(TRANSPARENT); 
	tRect.top+=2; 
	CFont nFont ,* nOldFont; 
	dc.SetTextColor(RGB(255,255,255)); 
	nFont.CreateFont(15,0,0,0,0,FALSE,FALSE,0,0,0,0,0,0,_TEXT("Times New Roman"));//�������� 
	nOldFont = dc.SelectObject(&nFont); 
	UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER | DT_END_ELLIPSIS|DT_CENTER ;
	dc.DrawText(headText.at(i),&tRect,uFormat); 
	dc.SelectObject(nOldFont); 
	nFont.DeleteObject(); //�ͷ�����

	} 
	tRect.left = tRect.right+1;
	tRect.right = lpRect.right;
	tRect.bottom=lpRect.bottom;
	tRect.top = lpRect.top;
	int R=64,G=140,B=193;//��ʼɫ��RGB 

	for(int j = lpRect.top;j<=lpRect.bottom;j++) 
	{ 
		tRect.bottom = tRect.top+1; 
		CBrush _brush; 
		_brush.CreateSolidBrush(RGB(R,G,B));//������ˢ 
		dc.FillRect(&tRect,&_brush); //��䱳�� 
		_brush.DeleteObject(); //�ͷŻ�ˢ 
		R+=4;G+=2;B+=1; 

		tRect.top = tRect.bottom; 
	}

	bDown = false;

}
void CSortHeaderCtrl::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{	
	return;
}


void CSortHeaderCtrl::Serialize( CArchive& ar )
{
	if( ar.IsStoring() )
	{
		const int iItemCount = GetItemCount();
		if( iItemCount != -1 )
		{
			ar << iItemCount;

			HD_ITEM hdItem = { 0 };
			hdItem.mask = HDI_WIDTH;

			for( int i = 0; i < iItemCount; i++ )
			{
				VERIFY( GetItem( i, &hdItem ) );
				ar << hdItem.cxy;
			}
		}
	}
	else
	{
		int iItemCount;
		ar >> iItemCount;
		
		if( GetItemCount() != iItemCount )
			TRACE0( "Different number of columns in registry." );
		else
		{
			HD_ITEM hdItem = { 0 };
			hdItem.mask = HDI_WIDTH;

			for( int i = 0; i < iItemCount; i++ )
			{
				ar >> hdItem.cxy;
				VERIFY( SetItem( i, &hdItem ) );
			}
		}
	}
}


void CSortHeaderCtrl::SetHeadBkColor(COLORREF clrBk,COLORREF clrLine)
{
	m_clrHeadColor = clrBk   ;
	m_clrLineColor = clrLine ;
}





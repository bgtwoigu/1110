#ifndef _RENDERING_TYPE_H
#define _RENDERING_TYPE_H
//**************************************************************
//Copyright (C), 2008-2008, AnyLook Co., Ltd.
//File name:    Rendering_Type.h
//Author:       AnyLook
//Version:      1.0
//Date:         2008/07/31
//Description:
//   Rendering Include file
//Others: 
//Function List: 
//<Name>        <desc>
//Simple        
//History: 
//<author>      <time>      <version >  <desc>
// AnyLook    2009/01/09       1.0       build this moudle  
//**************************************************************
//wlh 20090406 add start
typedef enum{
    DIRECTION_NONE = 0,
	DIRECTION_TOP,
	DIRECTION_BOTTOM,
	DIRECTION_LEFT,
    DIRECTION_RIGHT
}FlexDirection;
//wlh 20090406 add end
typedef struct {
	void *pDest;
	void *pSrc1; 
	void *pSrc2;
	int16 wWidthSrc;
	int16 wHeightSrc;
	FlexDirection   iFlexDirection;//wlh 20090406 add for flex direction
}RendDraw;

typedef struct {
	void   *m_pBmp;
	int16   m_wWidth;
	int16   m_wHeight;
	int16   m_nPitch; // ���ֽ��ͣ����Ǹ���BMP�������õĲ���
}BmpType;

typedef struct {
	int16	m_xAngle;		// ��x����ת�Ƕ�
	int16   m_yAngle;		// ��y����ת�Ƕ�
	int16   m_zAngle;		// ��z����ת�Ƕ�
	int16   m_xCenter;		// ������ת��x���ĵ㣬ԴͼƬ����
	int16   m_yCenter;		// ������ת��y���ĵ㣬ԴͼƬ����
	int16   m_z;			// z����ȣ���������
	int16   m_zxAngle;		// z����ƽ����������x��ļн�,(+90,-90)
	int16   m_xMove;		// x����ƽ��λ��
	int16   m_yMove;		// y����ƽ��λ��
}BmpPos;

typedef void (*PFNRENDDRAW)(RendDraw *pMe, int Step, int MaxStep);

extern PFNRENDDRAW Rendering_GetRendDraw16(RendType eType);
extern PFNRENDDRAW Rendering_GetRendDraw18(RendType eType);
extern PFNRENDDRAW Rendering_GetRendDraw24(RendType eType);
extern PFNRENDDRAW Rendering_GetRendDraw16Flex(void);
extern int Rendering_GetRend16FlexDirect(RendType eType);
#endif // _RENDERING_TYPE_H 

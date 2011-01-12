/*------------------------------------------------------------
  Copyright (c) 2007 �����Ƽ��ɷ����޹�˾
  
  �ļ�����:
		HWAPI.h

  �����ˣ�
		lzh@hanwang.com.cn		

  ��������:
		2007��5��23��

  ��������:
		2007��08��08��
		2007��08��13��
		2007��09��19��
		2007��10��26��
		2008��11��18��
  �ؼ���:
		PDA��

�������
002E .      00B1 +-     002B +      002D -
00D7 X      00F7 ��     003D =      0025 %
2030 ��            

���ı��
201C ��           201D ��      3001 ��   3002 ��        
FF0C ��    FF1A ��   FF1B ��   FF01 !
FF1F ��    2026 ��

Chinese Ext Punc 
FF5E ��    2018��   2019  ��   3008 ��
3009 ��    300A��        300B ��    3014 ��
3015 ��    FF08 ��  FF09 ��    FF5B ��
FF5D ��


Eng Punc
002C ,    002E .     0022 "    0027 '
003A :    003B ;     0021 !    003F ?

Ext Punc 
007E ~    005B [     005D ]    0028 (
0029 )    007B {     007D }


Comm Symbol
0023 #    0025 %     0026 &    002A *
002B +    002D -     002F /    003C <
003D =    003E >     0040 @    0024 $
FFE1 ��   FFE5 ��    20AC 

Ext Sym
005C \    005E ^     005F _    007C ح       
0060 `    00A4 ��         FFE0 ��   2116 ��
203B ��   2103 ��    00A7 ��   00B7 ��
3005 


------------------------------------------------------------*/
#ifndef __HWAPI_H
#define __HWAPI_H

#define HANWANG_SIMPLIFIED_CHINESE		1	// �������İ�
#define HANWANG_INTEGRATED_DICT			1	// �Ƿ�����ֵ伯��
#define HANWANG_LITTLEENDIAN			1	// �ֽ��� LittleEndian

#ifndef _WCHAR_T_DEFINED
	typedef unsigned short wchar_t;
	#define _WCHAR_T_DEFINED
#endif

/* -------------------ʶ������------------------------ */
	// �����
	#define HWLANG_Simp_Chinese		0x1000	// ��������
	#define HWLANG_GB2312			0x1001	// GB2312��׼ �����ַ���
/* -------------------��������ʶ���ַ���------------------ */
	// �����
	#define ALC_CHN_STANDARD	0x00000001	// ��׼���֣������ַ�����6721�� ���ַ�����20912�֣�
	#define ALC_CHN_RADICAL		0x00000002	// ����ƫ�ԣ������ַ�����42�֡����ַ�����92�֣�
	#define ALC_CHN_VARIANT		0x00000004	// ����ת��
	#define ALC_CHN_CURSIVE		0x00000008	// �в��鷨�����壩

	#define OUT_COMPATIBILITY	0x00000040	// ǿ������������֣�һ�ֶ��룩

#define ALC_LATIN_LOWERCASE		0x00000100	// Сд������ĸ a-z
#define ALC_LATIN_UPPERCASE		0x00000200	// ��д������ĸ A-Z

#define ALC_NUMERALS			0x00000400	// ���֣�0-9��
#define ALC_OPERATORS			0x00000800	// �����

#define ALC_CHN_COMMON_PUNC		0x00001000	// ���ĳ��ñ��
#define ALC_CHN_EXTEND_PUNC		0x00002000	// ������չ���

#define ALC_ENG_COMMON_PUNC		0x00004000	// Ӣ�ĳ��ñ��
#define ALC_ENG_EXTEND_PUNC		0x00008000	// Ӣ����չ���

#define ALC_COMMON_SYMBOLS		0x00010000	// ���÷���
#define ALC_EXTEND_SYMBOLS		0x00020000	// ��չ����

#define ALC_HIRAGANA			0x00040000	// ����ƽ������83�֣����뷶Χ0x3041-0x3093��
#define ALC_KATAKANA			0x00080000	// ����Ƭ������86�֣����뷶Χ0x30A1-0x30F6��

#define ALC_GESTURE				0x00400000	// �������ƣ��ո� �Ʊ�� ��ɾ �س���


/* ----------------����Զ�ת��-------------------------- */
#define	OUT_INVERT				0x10000000	// ��ʱ��Сд�ߵ����߼�ʱ�������л��������ڲ�ʹ�ã�
#define OUT_UPPERCASE			0x20000000	// �����д��ĸ
#define OUT_LOWERCASE			0x40000000	// ���Сд��ĸ
#define OUT_FULLWIDTH			0x80000000	// ���ȫ���ַ�

#define ALC_LATIN		(ALC_LATIN_LOWERCASE | ALC_LATIN_UPPERCASE)	// ������ĸ
#define ALC_KANA		(ALC_HIRAGANA | ALC_KATAKANA)				// ���ļ���

#define ALC_CHN_PUNC	(ALC_CHN_COMMON_PUNC | ALC_CHN_EXTEND_PUNC)	// ���ı��
#define ALC_ENG_PUNC	(ALC_ENG_COMMON_PUNC | ALC_ENG_EXTEND_PUNC)	// Ӣ�ı��
#define ALC_PUNCTUATION	(ALC_CHN_PUNC | ALC_ENG_PUNC)				// ���

#define ALC_SYMBOLS		(ALC_COMMON_SYMBOLS | ALC_EXTEND_SYMBOLS)	// ����

/* ----------------�������Ʊ��붨��---------------------- */
#define CODE_BACKSPACE		0x0008	// ��ɾ
#define CODE_TAB			0x0009	// Tab
#define CODE_RETURN			0x000D	// �س�
#define CODE_SPACE			0x0020	// �ո�


//---------------��������----------------------------
#define HWRE_MAXCANDNUM			8			// ���ṩ������ѡ�ָ���
#define HWRE_MAXSEGNUM			20			// ����ʶ���£����ָ����

// ������Ҫ�ڴ�ռ�
#define HWRE_CHAR_RAMSIZE		(17*1024)	// ����ʶ����������ռ��С
#define HWRE_MULTICHAR_RAMSIZE	(33*1024)	// ����ʶ����������ռ��С
#define HWRE_PHRASE_RAMSIZE		(60*1024)	// ���Ķ̾�ʶ������Ҫ������ռ��С
#define HWRE_CANDBUFSIZE		1024		// ʶ�����洢����С


//------------�ʼ��ṹ����-------------------
#define STROKEENDMARK		(-1)		// ����ʻ��������ֵ

typedef struct tagTHWPoint
{
	short x;	//�ʼ� X ����
	short y;	//�ʼ� Y ����
} THWPoint;

//------------��д��ṹ����-------------------
typedef struct tagTHWFrame{
	short left;		// ��д�����Ͻ� X ����
	short top;		// ��д�����Ͻ� Y ����
	short width;	// ��д����
	short height;	// ��д��߶�
} THWFrame;

/* -------------------ʶ��ģʽ---------------- */
#define HWMODE_SINGLECHAR		1	// ����ʶ��
#define HWMODE_MULTICHAR		2	// ����ʶ��
#define HWMODE_WORDSTRING		3	// ���Ķ̾�ʶ��


//------------ʶ�����Զ���------------------
typedef struct tagTHWAttribute
{
	unsigned short	wMode;		// ʶ��ģʽ
	unsigned short	wLanguage;	// ʶ������
	unsigned long	dwRange;	// ʶ��Χ
	int				iSlantScope;	// ��б�Ƕȷ�Χ�����Ϊ0����ر���б�ַ�ʶ��
	int				iCandidateNum;	// ��ѡ�ָ���
	unsigned char*	pRam;		// ����ռ䣬Ҫ����ʼ��ַ4�ֽڶ���
	unsigned char*  pRom;		// ʶ���ֵ䡣����ֵ伯�ɣ���ΪNULL��
	unsigned char*	pAdaptWorkSpace;	// ����Ӧѧϰ�ռ�
	unsigned char*	pMacroWorkSpace;	// ͼ�κ궨��洢�ռ�	
	unsigned char*	pSystemDict;	// ����ϵͳ�ʿ� ���� Ӣ��ϵͳ�ʿ⡣
	unsigned char*	pUserDict;		// �����û��ʿ� ���� Ӣ���û��ʿ�
	unsigned char*	pLangModel;		// ����ģ���ֵ䣨�������Ķ̾�ʶ��
	THWFrame*		pFrame;			// ��д�����ΪNULL����Ϊȫ����д
} THWAttribute;

//-------------����ṹ����---------------------------
#define OUTTYPE_MARCO		100	// �����������ֺ�

//-------------������붨��---------------------------
#define HWERR_SUCCESS                    0		// �ɹ�
#define HWERR_INVALID_FUNCTION           (-1)	// ��������
#define HWERR_NOT_ENOUGH_MEMORY          (-2)	// �ڴ治��
#define HWERR_INVALID_DATA               (-3)	// ��Ч������
#define HWERR_DATA_EXIST				 (-4)	// �����Ѿ�����

// ���ʲ��ҷ���ֵ����
#define HWLX_FOUND_NONE			0  // �ַ���û��ƥ���
#define HWLX_FOUND_PREFIX		1  // �ַ���û��ƥ��ʣ��������Ը��ַ�����ʼ�Ĵ�
#define HWLX_FOUND_SAMENESS		2  // �������ַ�����ȫƥ��Ĵ�
#define HWLX_FOUND_MORE			3  // �������ַ���ƥ��Ĵʣ�ͬʱ�����Ը��ַ�����ʼ�Ĵ�

//----------------������������-----------------------
#ifdef __cplusplus
extern "C" {
#endif

// ------------------ ʶ����غ��� -------------------------
/*
���ܣ��ʼ�ʶ��
����˵����
	pTrace���ʼ����ݣ����ʽΪ(x1,y1),(x2,y2),...,(STROKEENDMARK,0),...,(STROKEENDMARK,0),
			(STROKEENDMARK,STROKEENDMARK)������(STROKEENDMARK,0)Ϊ�ʻ�������־��
			(STROKEENDMARK,STROKEENDMARK)Ϊ�ֽ�����־��
	iTraceNum���ʼ����������Ϊ-1�����ʾ���ֽ�����Ǿ����ʼ�������
	pAttr��ʶ�����ԡ�
	pResult��ʶ�������仺������СΪ HWRE_CANDBUFSIZE��	
����ֵ��
	HWERR_SUCCESS���ɹ�
	HWERR_INVALID_FUNCTION����������
	HWERR_INVALID_DATA����Ч������
	> 0������൥��ʶ���´�Ž���Ļ������ռ䲻�㣬��pResult�в���Ž������ֱ�ӷ��طָ����Ŀ
ʶ������Žṹ��
	struct
	{
		unsigned short wType = 1;			// ������ͣ����ڵ���ʶ��
		unsigned short wCandNum;			// ��ѡ�ָ���
		unsigned long pCands[wCandNum];		// ��ѡ�ַ�����UCS4����
	};

	struct
	{
		unsigned short wType = 2;			// ������ͣ����ڶ���ʶ��
		unsigned short wBlockNum;			// �ָ����
		unsigned short wCandNum;			// ��ѡ��Ŀ
		unsigned short wSeperate;			// �ָ�����ǣ�Ԥ����
		unsigned long pCands[wBlockNum][wCandNum];	// ��ѡ�ַ�����UCS4����
	};

	struct
    {
        unsigned short wType = 3;			// ������ͣ��ʺ϶̾�
        unsigned short wCandNum;			// ��ѡ��Ŀ
        wchar_t pCands[HWRE_CANDBUFSIZE / 2 - 2]; // unicode�ַ���
    }

*/
int HWRE_Recognize( THWPoint* pTrace, int iTraceNum, const THWAttribute* pAttr, unsigned short* pResult );

#ifdef __cplusplus
}
#endif

#endif

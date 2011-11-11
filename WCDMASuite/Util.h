#ifndef PCMCIA_UTIL_CE562D0D5A624c3686FF65F98493055C
#define PCMCIA_UTIL_CE562D0D5A624c3686FF65F98493055C

#include <vector>
//#define CN_SUNNYCAT


std::string ToMultiBytes(const std::wstring& str, UINT codepage = 0 );
std::string ToMultiBytes(const wchar_t* str, size_t charcount, UINT codepage = 0 );
std::string BytesToString(BYTE* bytes, size_t size, bool bUnicode);
std::wstring ToUnicode(const std::string& str, UINT codepage = 0 );

CTime TimeFromCDMA(const CString& strCDMA);

void TimeFromCDMA(const CString& strCDMA, CTime& tm);

//��ȡӦ�ó������ڵ�Ŀ¼������Ŀ¼���'\'
CString GetAppPath();

//���ʽ����"��:��"��ʽ���ַ���
CString SecondToTime(int iSecond);

//װ���ַ�����Դ
CString LoadStringEx(UINT id);

//��ʾ�Ի��򣬲���������Ϣ�ַ���
int ShowMessageBox(UINT strid, UINT nType = MB_OK);

//����һ���߳�
HANDLE CreateThreadEx(LPTHREAD_START_ROUTINE func, void* param);


//�����ַ����е��ַ���������Ӣ�Ķ���һ���ַ�
size_t GetCharNum(const CString& str);

//Ѱ�Ҷ�Ӧ���ִ�
bool FindString(const CString& str_info, const CString& str_find);

/**
*	�ַ������͡�
*/
enum EStringType
{
	STR_DOUBLE_BYTE = 0,//˫�ֽ��ַ�
	STR_ASCII,			//���ֽ��ַ�,�ұ��뷶Χ��[0, 127]֮��
	STR_SINGLE_BYTE		//���ֽ��ַ�,�Һ����뷶Χ��[0, 127]֮��
};

int MaxSMSLength(EStringType strType);

//��ȡ�ַ���������
EStringType GetStringType(const CString& str);
EStringType GetStringType(const char* str, size_t strLen);
void TimeFromString(const CString& strCDMA, CTime& tm);
//��ȡ�ַ�����������ɸ��ַ���ɵ��Ӵ�
CString GetLeftString(const CString& str, size_t iCount);

//�ж��Ƿ�ΪUNICODE
bool IsUnicode(const char *p, int len);

void UnicodeToASCII(LPCWSTR SourceStr, DWORD dwWCharCount, LPSTR pASCII, DWORD dwASCIISize);

int ASCIIToUnicode(LPCSTR ASC, DWORD ASCCount,	LPWSTR WCharBuf, DWORD dwWCharSize);

//
//��Uncide�ĸ��ֽ�����ֽڷ�ת
//
void ReverseUnicode(BYTE *pUnicodem, int iUnicodeNum);

//
//	����һ��������
//
HANDLE RunProgram(const CString& strCmdLine, bool bShowWindow);

enum Win32Type{ Win_32s = 1,
		Win_NT3,
		Win_NT4,
		Win_95,
		Win_98,
		Win_ME,
		Win_2000 = 0xF,
		Win_XP,
		Win_2003,
		Win_Vista	};
DWORD OptionsShellType();

//���ݹ���ƥ��
bool MathingNumByRules(const CString& strNum1, const CString& strNum2);


enum ETimeFormat
{
	TIMEF_YMD_HM,
	TIMEF_YMD_HMS,
	TIMEF_YMD,
	TIMEF_HM,
	TIMEF_HMS,
	TIMEF_YM,
};
CString TimeToString(CTime tm, ETimeFormat format);
#endif

#ifndef MY_F_STREAM
#define MY_F_STREAM

#include <fstream>

class MyStream: public std::ifstream
{
public:
	MyStream(const char* filename)
		: std::basic_ifstream<char>(filename)
	{
	}

	/** ��ȡһ�У�����\n��β���߻�����
	*std::ifstream::getline�����治��ʱ������failbit��������һ�ζ�ʧ��
	*����д�������������
	*@return ���ض�ȡ�������е��ֽ���
	*/
	size_t readline(char buf[], size_t buf_size)
	{
		size_t nRead = 0;
		char c;
		while(!eof() && nRead<buf_size)
		{
			read(&c, 1);
			if(fail())
				break;
			if(c=='\n')
			{
				break;
			}
			if(c!='\r')
				buf[nRead++] = c;
		}

		return nRead;
	}
};

#endif
#pragma once
#include <vector>
#include <cassert>
#include <string>
#include <stdexcept>

class BitsArray
{
	std::vector<unsigned char> m_Data;
	size_t m_Size;
	size_t m_Free;
public:
	/** \brief Ĭ�Ϲ��캯�� */
	BitsArray();
	/** \brief ��һ���ֽ����鹹��
	*@param bytes �ֽ������ͷָ��
	*@param bytesCount �ֽ�����ĳ���(�ֽ���)
	*/
	BitsArray(unsigned char* bytes, size_t bytesCount);

	/** \brief ת���ɶ������ַ���*/
	std::string ToBinaryString()const;

	/** \brief ת����16�����ַ���*/
	std::string ToHexString()const;

	/** \brief ������ɱ��ص�β��
	*@param val ����ӵ���ֵ�ֽ�
	*@param pos ����ӱ�������ֵ�ֽ��ֽ��е�λ�ã���ΧΪ0-7
	*@param count ����ӵı�����,��pos��Ӳ��ܳ���8
	*/
	void PushBits(unsigned char val, size_t pos, size_t count);

	/** \brief ��ȡ���ɸ�����
	*@param dest �����ȡ�ı��ص��ֽ�
	*@param destPos ��ȡ�ı������ֽ��еĴ��λ�ã���ΧΪ0-7
	*@param count ��ȡ�ı���������destPos��Ӳ��ܳ���8
	*@param srcPos ��ʼ��ȡ��λ��
	*/
	void GetBits(unsigned char& dest, size_t destPos, size_t count, size_t srcPos);

	/** \brief �������д�ŵı�����*/
	size_t BitsCount()const;

	/** \breif ռ�õ��ֽڳ��� */
	size_t BytesCount()const;

	/** \ռ�õ��ֽ�����ͷָ�� */
	const unsigned char* Bytes()const;

private:
	void Grow();
	static std::string Byte2String(unsigned char byte);
	static char BCD2Char(unsigned char bcd);
};
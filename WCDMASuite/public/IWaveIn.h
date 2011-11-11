#pragma once

namespace NMWave
{
	struct IWaveIn;

	/** \brief Wave��������������ӿڡ�
	*ʵ�ִ˽ӿ��������IWaveIn�����Wave����.
	*/
	struct IWaveInListener
	{
		/** \brief ��Wave�������롣
		*@param sender ����Դ
		*@param data Wave����ͷ��ַ
		*@param dataSize Wave���ݳ���(�ֽ���)
		*/
		virtual void OnDataIn(IWaveIn* sender, const BYTE* data, size_t dataSize) = 0;
	};

	/** \brief ��Ƶ������������Դ�ӿڡ�
	*������һ����Ƶ��������Դ�Ļ�������
	*/
	struct IWaveIn
	{
		/** \brief ������Դ*/
		virtual bool Open() = 0;
		/** \brief ����Դ�Ƿ��Ѿ���*/
		virtual bool IsOpen() = 0;

		/** \brief �ر�����Դ*/
		virtual bool Close() = 0;
		/** \brief ��ʼ����*/
		virtual bool Start() = 0;
		/** \brief ֹͣ����*/
		virtual bool Stop() = 0;
		/** \brief ���һ��������*/
		virtual void AddListener(IWaveInListener& listener) = 0;
		/** \brief ɾ��һ��������*/
		virtual void RemoveListener(IWaveInListener& listener) = 0;
	};
}
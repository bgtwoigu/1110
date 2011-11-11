#pragma once 

#include ".\IWaveIn.h"
#include ".\IWaveOut.h"
#include ".\IWaveMixer.h"
#include ".\IRecorder.h"
#include <string>

#ifdef WAVELIB_EXPORTS
#define WAVELIB_API __declspec(dllexport)
#else
#define WAVELIB_API __declspec(dllimport)
#endif

namespace NMWave
{
	/** \brief ��Ƶ�����ӿڡ�*/
	struct IWaveLib
	{
		/** \brief �õ�һ��PC��������Ƶ����ӿ�*/
		virtual IWaveIn& GetAudioInput() = 0;

		/** \brief �õ�һ�����ڵ���Ƶ����ӿ�*/
		virtual IWaveIn& GetSerialInput() = 0;

		/** \brief �õ�һ��PC��������Ƶ����ӿ�*/
		virtual IWaveOut& GetAudioOutPut() = 0;

		/** \brief �õ�һ�����ڵ���Ƶ����ӿ�*/
		virtual IWaveOut& GetSerialOutput() = 0;

		/** \brief ����һ���������ӿ�
		*@param hwndCallback ���ջ�������Ϣ�Ĵ��ڡ�
		*���������������ı�������øı�ʱ����մ��ڷ���
		*MM_MIXM_LINE_CHANGE �� MM_MIXM_CONTROL_CHANGE ��Ϣ.
		*@return �´����Ķ���,��Ҫʹ��IWaveMixer::Release���ͷ�
		*/
		virtual IWaveMixer* CreateMixer(HWND hwndCallback) = 0;

		/** \brief ����һ���ļ���¼����������Ƶ����¼�Ƴ�wave�ļ���
		*@param fileName �ļ���
		*@return �´����Ķ���,��Ҫʹ��IRecorder::Release���ͷ�
		*/
		virtual IRecorder* CreateRecorder(const std::string& fileName) = 0;

		/** \brief �л���������豸��������*/
		virtual void SwitchDevice() = 0;
	};

	/** \brief ����һ��IWaveLib�ӿڵ�ʵ��
	*@param portName ������Ƶ���ݵĴ��������� COM3
	*@param baud ������Ƶ���ݵĴ��ڲ�����
	*/
	WAVELIB_API	IWaveLib* CreateWaveLib(const std::string& portName, DWORD baud);

	/** \brief �ͷ�һ����CreateWaveLib������IWaveLib�ӿڵ�ʵ��	*/
	WAVELIB_API void ReleaseWaveLib(IWaveLib* pWaveLib);
}

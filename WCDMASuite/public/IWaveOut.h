#pragma once

namespace NMWave
{
	/** \brief ��Ƶ��������ӿ�*/
	struct IWaveOut
	{
		/** \brief �����*/
		virtual bool Open() = 0;

		/** \brief �ر����*/
		virtual bool Close() = 0;

		/** \brief ֹͣ���*/
		virtual bool Stop() = 0;

		/** \brief ��ʼ���*/
		virtual bool Start() = 0;
	};
}
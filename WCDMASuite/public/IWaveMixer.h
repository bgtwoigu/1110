#pragma once

namespace NMWave
{
	struct IWaveMixer
	{
		/**\brief ����Ƿ���˷�����Ϊ����
		*@param bMute [out]��ǰ��˷羲������
		*@return ��ѯ�����Ƿ�ɹ�
		*/
		virtual bool IsMute(bool& bMute) = 0;

		/**\brief ������˷��Ƿ�Ϊ����
		*@param bMute ����˷羲������
		*@return �����Ƿ�ɹ�
		*/
		virtual bool SetMute(bool bMute) = 0;
		/**\brief ��ѯ����������
		*@param vol[out] ��ǰ����
		*@return �����Ƿ�ɹ�
		*/
		virtual bool GetVolume(DWORD& vol) = 0;
		/**\brief ��������������
		*@param vol ������
		*@return �����Ƿ�ɹ�
		*/
		virtual bool SetVolume(DWORD vol) = 0;

		/**\brief ��ѯ������������Χ
		*@param minVol[out] ��С����
		*@param maxVol[out] �������
		*@return �����Ƿ�ɹ�
		*/
		virtual bool GetVolumeRange(DWORD& minVol, DWORD& maxVol) = 0;

		/** \brief �ͷű����� */
		virtual void Release() = 0;
	};
}
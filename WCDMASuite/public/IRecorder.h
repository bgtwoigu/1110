#pragma once
#include ".\IWaveOut.h"
#include ".\IWaveIn.h"

namespace NMWave
{
	struct IRecorder: public IWaveOut, public IWaveInListener
	{
		/** \brief �ͷű�����*/
		virtual void Release() = 0;
	};
};
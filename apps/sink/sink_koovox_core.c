/****************************************************************************
Copyright (C) JoySoft . 2015-2025
Part of KOOVOX 1.0.1

FILE NAME
    sink_koovox_core.c

*/

#ifdef ENABLE_KOOVOX

#include <kalimba.h>
#include <vm.h>
#include <micbias.h>
#include <pio.h>
#include <pio_common.h>
#include <bdaddr.h>

#include "sink_events.h"
#include "sink_statemanager.h"
#include "sink_audio_prompts.h"
#include "sink_tones.h"
#include "sink_pio.h"
#include "sink_callmanager.h"
#include "sink_config.h"
#include "sink_koovox_task.h"
#include "sink_koovox_core.h"
#include "koovox_wechat_handle.h"
#include "sink_koovox_uart.h"
#include "sink_gatt_db.h"


/*************************************************************************
NAME
    KoovoxLog
    
DESCRIPTION
    send log message to mobile phone 
*/
void KoovoxLog(const char* value_data)
{

}



/****************************************************************************
NAME 
  	KoovoxDisableAutomaticSpeechRecognition

DESCRIPTION
 	disable the automatic speech recognition business
 
RETURNS
  	void
*/ 
void KoovoxDisableAutomaticSpeechRecognition(void)
{
	
}

/****************************************************************************
NAME 
  	KoovoxEnableAutomaticSpeechRecognition

DESCRIPTION
 	enable the automatic speech recognition business
 
RETURNS
  	void
*/ 
void KoovoxEnableAutomaticSpeechRecognition(void)
{
	KOOVOX_CORE_DEBUG(("KoovoxEnableAutomaticSpeechRecognition\n"));
	
}

/****************************************************************************
NAME 
  	KoovoxConfirmAsrNumber

DESCRIPTION
 	user confirm the asr number
 
RETURNS
  	void
*/ 
void KoovoxConfirmAsrNumber(void)
{

}

/****************************************************************************
NAME 
  	KoovoxRecordControl

DESCRIPTION
 	start or stop the message record
 
RETURNS
  	void
*/ 
void KoovoxRecordControl(void)
{

}

/****************************************************************************
NAME 
  	KoovoxMuteActiveCall

DESCRIPTION
 	mute on or off the active call
 
RETURNS
  	void
*/ 
void KoovoxMuteActiveCall(void)
{
	KOOVOX_CORE_DEBUG(("KoovoxMuteActiveCall\n"));

    /* If MUTE enabled, generate EventUsrMuteOff, otherwise generate EventUsrMuteOn */
    if(koovox.muteStatus == FALSE)
    {
        MessageSend(&theSink.task, EventUsrMuteOn, 0);
		koovox.muteStatus = TRUE;
	}
    else
    {
        MessageSend(&theSink.task, EventUsrMuteOff, 0);
		koovox.muteStatus = FALSE;
	}
}

/****************************************************************************
NAME 
  	KoovoxPresentIncomingCall

DESCRIPTION
 	present the incoming call
 
RETURNS
  	void
*/ 
void KoovoxPresentIncomingCall(void)
{
	KOOVOX_CORE_DEBUG(("KoovoxPresentIncomingCall\n"));

	if((koovox.presentStatus != PRESET_BUSY)&&(koovox.presentEnable == TRUE))
	{
		koovox.presentStatus = PRESET_BUSY;
		
		/* answer the call */
		MessageSend(&(theSink.task), EventUsrAnswer, 0);
	}
}

/****************************************************************************
NAME 
  	KoovoxHandleEventCvcLoaded

DESCRIPTION
 	handle the Event cvc kalimba loaded
 
RETURNS
  	void
*/ 
void KoovoxHandleEventCvcLoaded(void)
{
	KOOVOX_CORE_DEBUG(("KoovoxHandleEventCvcLoaded\n"));

}


/****************************************************************************
NAME 
  	KoovoxGetBluetoothAdrress

DESCRIPTION
 	get the bluetooth address
 
RETURNS
  	void
*/ 
void KoovoxGetBluetoothAdrress(uint8* addr)
{
	uint8 i = 0;
	BD_ADDR_T bd_addr;
	BdaddrConvertVmToBluestack(&bd_addr, (const bdaddr *)(&theSink.local_bd_addr));
	
	addr[i++] = (bd_addr.nap >> 8) & 0xff;
	addr[i++] = bd_addr.nap & 0xff;
	addr[i++] = bd_addr.uap;
	addr[i++] = (bd_addr.lap >> 16) & 0xff;
	addr[i++] = (bd_addr.lap >> 8) & 0xff;
	addr[i++] = bd_addr.lap & 0xff;
}
 
/****************************************************************************
NAME
    KoovoxAudioPresencePlayEvent
    
DESCRIPTION
    play presece audio to peer listenning

RETURN
	void
*/
void KoovoxUsrEventHandle(MessageId id, Message message)
{
	sinkState state = stateManagerGetState();
	
	KOOVOX_CORE_DEBUG(("KoovoxUsrEventHandle: %x\n", id));
	
	switch(id)
	{
	case EventKoovoxButtonCClick:
	{
		switch(state)
		{
		/* ���ܲ��� */
		case deviceConnected:
			break;
			
		/* ��һ�� */
		case deviceA2DPStreaming:
			MessageSend(&(theSink.task), EventUsrAvrcpSkipForward, 0);
			break;

		/* �����绰 */
		case deviceIncomingCallEstablish:
			MessageSend(&(theSink.task), EventUsrAnswer, 0);
			break;

		/* ȡ�������绰 */
		case deviceOutgoingCallEstablish:
			MessageSend(&(theSink.task), EventUsrCancelEnd, 0);
			break;

		/* �Ҷϵ绰 */
		case deviceActiveCallSCO:
			MessageSend(&(theSink.task), EventUsrCancelEnd, 0);
			break;
			
		default:
			break;
		
		}

		/* confirm the asr number */
	}
	break;
	
	case EventKoovoxButtonCDoubleClick:
	{
		switch(state)
		{
		/* �������ģʽ */
		case deviceConnectable:
		case deviceConnDiscoverable:
			MessageSend(&theSink.task, EventUsrRssiPair, 0);
			break;

		/* ��һ�� */
		case deviceA2DPStreaming:
			MessageSend(&(theSink.task), EventUsrAvrcpSkipBackward, 0);
			break;

		/* �绰���� */
		case deviceIncomingCallEstablish:
			KoovoxPresentIncomingCall();
			break;

		/* ͨ������ */
		case deviceActiveCallSCO:
			KoovoxMuteActiveCall();
			break;

		default:
			break;
		}
	}
	break;
	
	case EventKoovoxButtonCLongClick:
	{
		switch(state)
		{
		/* ����/��ͣ���� */
		case deviceConnected:
		case deviceA2DPStreaming:
			MessageSend(&(theSink.task), EventUsrAvrcpPlayPause, 0);
			break;

		/* �ܽ����� */
		case deviceIncomingCallEstablish:
			MessageSend(&(theSink.task), EventUsrReject, 0);
			break;

		/* �绰¼������ */
		case deviceActiveCallSCO:
			KoovoxRecordControl();
			break;

		default:
			break;
		}
	}
	break;
	
	case EventKoovoxButtonMClick:
	{
		/* ������ؿ��� */
		if(koovox.health == FALSE)
		{
			if(button_req)
			{
				/* ������ʾ��������Ч�����Ժ���� */
			}
			else
			{
				KoovoxFillAndSendUartPacket(START, OBJ_HEART_RATE, 0, 0);
				KoovoxStoreLastCmd(START, OBJ_HEART_RATE);
				button_req = TRUE;
			}
		}
		else
		{
			if(button_req)
			{
				/* ������ʾ��������Ч�����Ժ���� */
			}
			else
			{
				KoovoxFillAndSendUartPacket(STOP, OBJ_HEART_RATE, 0, 0);
				KoovoxStoreLastCmd(STOP, OBJ_HEART_RATE);
				button_req = TRUE;
			}			
		}
	}
	break;
	
	case EventKoovoxButtonMDoubleClick:
	{
		/* ����������� */
		if(state >= deviceConnected)
		{
			MessageSend(&(theSink.task), EventUsrDialStoredNumber, 0);
		}
	}
	break;
	
	case EventKoovoxButtonMPress:
	{
		/* automatic speech recognition enable */
	}
	break;
	
	case EventKoovoxButtonMRelease:
	{
		/* automatic speech recognition disable */
	}
	break;

	case EventKoovoxRepeatSendIndication:
	{
		koovox_send_data_to_wechat();
	}
	break;

	default:
		break;
	}
	
}


/*************************************************************************
NAME
    KoovoxResultI2cTest
    
DESCRIPTION
    
*/
void KoovoxResultI2cTest(uint8* data, uint8 size_data)
{
	uint16 value = 0;
	ConfigRetrieve(CONFIG_ACC_I2C_TEST_RESULT,&value, 1);
	DEBUG(("acc_i2c result:%d pskey_value:%d\n", data[0], value));
	if(value != (uint16)data[0])
		ConfigStore(CONFIG_ACC_I2C_TEST_RESULT, (const void*)data, 1);

	if(data[0])
	{
		button_req = TRUE;
		/*�����Ʋ�����*/
		KoovoxFillAndSendUartPacket(START, OBJ_STEP_COUNT, 0, 0);
		KoovoxStoreLastCmd(START, OBJ_STEP_COUNT);
		KOOVOX_CORE_LOG(("I2C suc\n"));
	}
	else
	{
		KOOVOX_CORE_LOG(("I2C failed\n"));
		if(koovox.repeat_times >= REPEAT_TIMES)
		{
			koovox.repeat_times = 0;
			/* ������ʾ�û����������쳣���������豸 */
			
		}
		else
		{
			button_req = TRUE;
			/*���½���i2c���*/
			KoovoxFillAndSendUartPacket(START, OBJ_I2C_TEST, 0, 0);
			KoovoxStoreLastCmd(START, OBJ_I2C_TEST);
			koovox.repeat_times++;
		}
	}
}

/*************************************************************************
NAME
    KoovoxStoreLastCmd
    
DESCRIPTION
    
*/
void KoovoxStoreLastCmd(uint8 cmd, uint8 obj)
{
	koovox.last_cmd = 0;
	koovox.last_cmd = cmd;
	koovox.last_cmd += (uint16)obj << 8;
}



#else  /* ENABLE_KOOVOX */

#include <csrtypes.h>
static const uint16 dummy_gatt = 0;    

#endif /* ENABLE_KOOVOX */



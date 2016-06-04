
#include <stdio.h>
#include <malloc.h>

#include "amr_if.h"

#include "amrencode.h"
#include "amrdecode.h"
#include "sp_dec.h"


/***************************************************************************
		TYPE DEFINITION DECLARATION
***************************************************************************/

static const int32 IETFFrameSize[16] =
{
    13      // AMR 4.75 Kbps
    , 14        // AMR 5.15 Kbps
    , 16        // AMR 5.90 Kbps
    , 18        // AMR 6.70 Kbps
    , 20        // AMR 7.40 Kbps
    , 21        // AMR 7.95 Kbps
    , 27        // AMR 10.2 Kbps
    , 32        // AMR 12.2 Kbps
    , 6     // GsmAmr comfort noise
    , 7     // Gsm-Efr comfort noise
    , 6     // IS-641 comfort noise
    , 6     // Pdc-Efr comfort noise
    , 1     // future use; 0 length but set to 1 to skip the frame type byte
    , 1     // future use; 0 length but set to 1 to skip the frame type byte
    , 1     // future use; 0 length but set to 1 to skip the frame type byte
    , 1     // AMR Frame No Data
};

typedef struct
{
	int bInitialized;
	void *decoder;
	void *encoder;
	void *sidState;
	AMR_MODE encodeMode;
}AUDGlobalsT;


/***************************************************************************
		FUNCTION DEFINITION
***************************************************************************/

static Frame_Type_3GPP  getFrameTypeLength(void* pFrame, int* pnFrameLength)
{
	Frame_Type_3GPP ft = (Frame_Type_3GPP)((*((unsigned char*)pFrame) >> 3) & 0x0F);
	
	//Narrow Band AMR
	if ( pnFrameLength )
	{
		*pnFrameLength = IETFFrameSize[ft];
	}
	
	return ft;
}

/****************************************************************************
 * AMRCODEC_init
 * ���ܣ�AMRCODEC��ʼ�������Դ
 * ����: eEncodeMode     -amr ����ģʽ
 * ���:  
 * ����: �ɹ���������Ч��AMRCODEC���,ʧ�ܣ�����INVALID_AMR_HANDLE
 * ˵��:  
****************************************************************************/

AMRCODEC_HANDLE AMRCODEC_init(AMR_MODE eEncodeMode)
{
	AUDGlobalsT *paudGlobals = (AUDGlobalsT *)malloc(sizeof(AUDGlobalsT));
	if (paudGlobals == NULL)
	{
		printf("AMRCODEC_init() fatal error, memory fault.\r\n");
		return INVALID_AMR_HANDLE;
	}
	
	paudGlobals->bInitialized = 0;
	paudGlobals->decoder = NULL;
	paudGlobals->encodeMode = eEncodeMode;
	paudGlobals->encoder = NULL;
	paudGlobals->sidState = NULL;
	
	if ( 0 != GSMInitDecode( &paudGlobals->decoder, (Word8 *)"amr decoder" ) )
	{
		paudGlobals->decoder = NULL;
		printf("AMRCODEC_init()->decoder init failed.\r\n");
		free(paudGlobals);
		return INVALID_AMR_HANDLE;
	}
	
	if ( 0 != AMREncodeInit(&paudGlobals->encoder, &paudGlobals->sidState, FALSE) )
	{
		if (paudGlobals->decoder)
		{
			GSMDecodeFrameExit( &paudGlobals->decoder );
			paudGlobals->decoder = NULL;
		}
		paudGlobals->encoder = NULL;
		paudGlobals->sidState = NULL;
		printf("AMRCODEC_init()->encoder init failed.\r\n");
		free(paudGlobals);
		return INVALID_AMR_HANDLE;
	}
	
	paudGlobals->bInitialized = 1;
	
	return (AMRCODEC_HANDLE)paudGlobals;
}


/****************************************************************************
 * AMRCODEC_end
 * ���ܣ�AMRCODEC�������ͷ������Դ
 * ����: hHandle     -AMRCODEC�������
 * ���:  
 * ����: ��
 * ˵��:  
****************************************************************************/
void AMRCODEC_end(AMRCODEC_HANDLE hHandle)
{
	AUDGlobalsT *paudGlobals;
	
	if (hHandle == NULL)
	{ /* parameter error */
		return;
	}
	paudGlobals = (AUDGlobalsT *)hHandle;
	if (paudGlobals->decoder)
	{
		GSMDecodeFrameExit( &paudGlobals->decoder );
		paudGlobals->decoder = NULL;
	}
	if (paudGlobals->encoder)
	{
		AMREncodeExit( &paudGlobals->encoder, &paudGlobals->sidState );
		paudGlobals->encoder = NULL;
		paudGlobals->sidState = NULL;
	}
	if (paudGlobals->bInitialized)
	{
		paudGlobals->bInitialized = 0;
	}
	free(paudGlobals);
}


/****************************************************************************
 * AMRCODEC_dec
 * ���ܣ�amr ���룬����һ�ν���һ֡
 * ����:	hHandle     -AMRCODEC�������
			pAmrBuf     -arm ����buffer
 			nAmrLen  --arm ���ݳ���
 				 
 * ���: pPcmBuf     ---���pcm��buffer
 * ����: 
 * ˵��: pAmrBuf �� pPcmBuf ����Ӧ�ó����ṩ
****************************************************************************/

void AMRCODEC_dec(AMRCODEC_HANDLE hHandle, void* pAmrBuf, int nAmrLen, void* pPcmBuf )
{
	int frameLen = 0;
	Frame_Type_3GPP frameType;
	AUDGlobalsT *paudGlobals;
	
	if (hHandle == NULL)
	{ /* parameter error */
		printf("AMRCODEC_dec parameter error.\r\n");
		return;
	}
	paudGlobals = (AUDGlobalsT *)hHandle;
	if (paudGlobals->bInitialized)
	{
		frameType = getFrameTypeLength(pAmrBuf, &frameLen);
		if (nAmrLen != frameLen)
		{
			printf("AMRCODEC_dec please check if amrLen parameter wrong nAmrLen = %d, frameLen = %d \r\n", nAmrLen, frameLen);
		}
		if (AMRDecode(paudGlobals->decoder, frameType, (UWord8*)pAmrBuf+1, (Word16*)pPcmBuf, MIME_IETF) == -1)
		{
			printf("AMRCODEC_dec()->decode failed.\r\n");
		}
	}
	else
	{
		printf("AMRCODEC_dec not been initialized.\r\n");
	}
}

/****************************************************************************
 * AMRCODEC_enc
 * ���ܣ�pcm ���룬����һ�α���һ֡
 * ����: 	hHandle     -AMRCODEC�������
			pPcmBuf     -pcm ����buffer
 				 
 * ���: pAmrBuf     ---���amr��buffer
         pnAmrLen  ---amr�����ݳ���
         pnFrameType----amr ֡��ʽ
 * ����: 
 * ˵��: pAmrBuf �� pPcmBuf ����Ӧ�ó����ṩ
****************************************************************************/

void AMRCODEC_enc(AMRCODEC_HANDLE hHandle, void* pPcmBuf, void* pAmrBuf, int* pnAmrLen, int* pnFrameType)
{
	int bytesEncoded;
	AUDGlobalsT *paudGlobals;
	Frame_Type_3GPP frameType;
	
	if (hHandle == NULL)
	{ /* parameter error */
		printf("AMRCODEC_enc parameter error.\r\n");
		return;
	}
	paudGlobals = (AUDGlobalsT *)hHandle;
	if (paudGlobals->bInitialized)
	{
		bytesEncoded = AMREncode( paudGlobals->encoder,
								paudGlobals->sidState,
								static_cast<enum Mode>(paudGlobals->encodeMode),
								(Word16*)pPcmBuf,
								(UWord8*)pAmrBuf,
								&frameType,
								AMR_TX_WMF );
		if (bytesEncoded != -1)
		{
			if(pnFrameType) *pnFrameType = (int)frameType;
			if (pnAmrLen)
			{
				*pnAmrLen = bytesEncoded;
			}
			*((UWord8*)pAmrBuf) =  (frameType << 3) | 0x04;
		}
		else
		{
			printf("AMRCODEC_enc()->encode failed.\r\n");
		}
	}
	else
	{
		printf("AMRCODEC_enc not been initialized.\r\n");
	}
}

/***************************************************************************
			END
***************************************************************************/

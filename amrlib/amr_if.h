
#ifndef __AMR_IF_H__
#define __AMR_IF_H__

#define INVALID_AMR_HANDLE NULL

typedef void *AMRCODEC_HANDLE;

typedef enum { 
    E_AMR_MODE_MR475 = 0,
    E_AMR_MODE_MR515,
    E_AMR_MODE_MR59,
    E_AMR_MODE_MR67,
    E_AMR_MODE_MR74,
    E_AMR_MODE_MR795,
    E_AMR_MODE_MR102,
    E_AMR_MODE_MR122,
    E_AMR_MODE_MRDTX,
    E_AMR_MODE_MAX_MODES     /* number of (SPC) modes */
} AMR_MODE;

/*-------------------------------------------------------------------------
                �ӿ�����
-------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************
 * AMRCODEC_init
 * ���ܣ�AMRCODEC��ʼ�������Դ
 * ����: eEncodeMode     -amr ����ģʽ
 * ���:  
 * ����: �ɹ���������Ч��AMRCODEC���,ʧ�ܣ�����INVALID_AMR_HANDLE
 * ˵��:  
****************************************************************************/
AMRCODEC_HANDLE AMRCODEC_init(AMR_MODE eEncodeMode);


/****************************************************************************
 * AMRCODEC_end
 * ���ܣ�AMRCODEC�������ͷ������Դ
 * ����: hHandle     -AMRCODEC�������
 * ���:  
 * ����: ��
 * ˵��:  
****************************************************************************/
void AMRCODEC_end(AMRCODEC_HANDLE hHandle);



/****************************************************************************
 * AMRCODEC_dec
 * ���ܣ�amr ���룬����һ�ν���һ֡
 * ����: pAmrBuf     -arm ����
 		nAmrLen  --arm ���ݳ���
 				 
 * ���: pPcmBuf     ---���pcm��buffer
 * ����: 
 * ˵��: pAmrBuf �� pPcmBuf ����Ӧ�ó����ṩ
****************************************************************************/
void AMRCODEC_dec(AMRCODEC_HANDLE hHandle, void* pAmrBuf, int nAmrLen, void* pPcmBuf);


/****************************************************************************
 * AMRCODEC_enc
 * ���ܣ�pcm ���룬����һ�α���һ֡
 * ����: pPcmBuf     -pcm ����buffer
 				 
 * ���: pAmrBuf     ---���amr��buffer
         pnAmrLen  ---amr�ı��볤��
         pnFrameType----amr ֡��ʽ
 * ����: 
 * ˵��: pAmrBuf �� pPcmBuf ����Ӧ�ó����ṩ
****************************************************************************/
void AMRCODEC_enc(AMRCODEC_HANDLE hHandle, void* pPcmBuf, void* pAmrBuf, int* pnAmrLen, int* pnFrameType);


#ifdef __cplusplus
}
#endif

#endif // __AMR_IF_H__

/***************************************************************************
			END
***************************************************************************/

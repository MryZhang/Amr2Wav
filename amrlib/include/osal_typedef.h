/************************************************************************
*  �ļ���:      OSAL_types.h
*  ����:            ����������������
*  ����:            ����
*  ��Ȩ:		������Դͨ���ͿƼ����޹�˾ 2010 	   
*  ��ʷ��¼: 
************************************************************************/
#ifndef _osal_typedef_H_
#define _osal_typedef_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
	
#define OSCL_UNUSED_ARG(x) (void)(x)


#define OSCL_IMPORT_REF
#define OSCL_EXPORT_REF

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _osal_typedef_H_ */


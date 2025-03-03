/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2006, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attemp
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************/

/****************************************************************************
	Abstract:

***************************************************************************/


#ifndef __RRM_H
#define __RRM_H

#ifdef DOT11K_RRM_SUPPORT
#include "rtmp_type.h"
#include "rrm_cmm.h"

#define RRM_QUIET_CNT_DEC(_V, _M) \
	((_V) == 0 ? (_V = _M) : (_V)--)

#define IS_RRM_ENABLE(_P, _I) \
	((_P)->ApCfg.MBSSID[(_I)].RrmCfg.bDot11kRRMEnable == TRUE)

#define IS_RRM_CAPABLE(_P) \
	((_P)->CapabilityInfo & RRM_CAP_BIT)

#define IS_RRM_BEACON_MEASURE(_P) \
	((_P)->RrmEnCap.field.BeaconActiveMeasureCap == 1)

#define IS_RRM_QUIET(_P, _I) \
	((_P)->ApCfg.MBSSID[(_I)].RrmCfg.QuietCB.QuietState == RRM_QUIET_SILENT)

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
void RRM_ReadParametersFromFile(
	IN PRTMP_ADAPTER pAd,
	RTMP_STRING *tmpbuf,
	RTMP_STRING *buffer);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
INT Set_Dot11kRRM_Enable_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
INT Set_BeaconReq_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
INT Set_LinkMeasureReq_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
INT Set_TxStreamMeasureReq_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
INT Set_RRM_Selftest_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
INT RRM_InfoDisplay_Proc(RTMP_ADAPTER *pAd, RTMP_STRING *arg);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
VOID RRM_CfgInit(
	IN PRTMP_ADAPTER pAd);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
VOID RRM_QuietUpdata(
	IN PRTMP_ADAPTER pAd);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
VOID RRM_InsertBcnReqIE(
	IN PRTMP_ADAPTER pAd,
	OUT PUCHAR pFrameBuf,
	OUT PULONG pFrameLen,
	IN PUCHAR pBcnReq);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
VOID RRM_InsertBcnReqSsidSubIE(
	IN PRTMP_ADAPTER pAd,
	OUT PUCHAR pFrameBuf,
	OUT PULONG pFrameLen,
	IN PUCHAR pSsid,
	IN UINT8 SsidLen);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
VOID RRM_InsertBcnReqRepCndSubIE(
	IN PRTMP_ADAPTER pAd,
	OUT PUCHAR pFrameBuf,
	OUT PULONG pFrameLen,
	IN UINT8 RepCnd,
	IN UINT8 Threshold);

VOID RRM_InsertBcnReqRepDetailSubIE(
	IN PRTMP_ADAPTER pAd,
	OUT PUCHAR pFrameBuf,
	OUT PULONG pFrameLen,
	IN UINT8 Detail);

/*
	==========================================================================
	Description:
		Insert RRM Enable Capabilitys IE into frame.
		
	Parametrs:
		1. frame buffer pointer.
		2. frame length.
	
	Return	: None.
	==========================================================================
 */
VOID RRM_InsertRRMEnCapIE(
	IN PRTMP_ADAPTER pAd,
	OUT PUCHAR pFrameBuf,
	OUT PULONG pFrameLen,
	IN INT BssIdx);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
VOID RRM_InsertNeighborRepIE(
	IN PRTMP_ADAPTER pAd,
	OUT PUCHAR pFrameBuf,
	OUT PULONG pFrameLen,
	IN UINT8 Len,
	IN PUINT8 pBssid,
	IN RRM_BSSID_INFO BssidInfo,
	IN UINT8 RegClass,
	IN UINT8 Channel,
	IN UINT8 PhyType);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
VOID RRM_InsertNeighborTSFOffsetSubIE(
	IN PRTMP_ADAPTER pAd,
	OUT PUCHAR pFrameBuf,
	OUT PULONG pFrameLen,
	IN UINT16 TSFOffset,
	IN UINT16 BcnInterval);
	
/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
VOID RRM_InsertQuietIE(
	IN PRTMP_ADAPTER pAd,
	OUT PUCHAR pFrameBuf,
	OUT PULONG pFrameLen,
	IN UINT8 QuietCnt,
	IN UINT8 QuietPeriod,
	IN UINT8 QuietDuration,
	IN UINT8 QuietOffset);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
VOID RRM_InsertBssACDelayIE(
	IN PRTMP_ADAPTER pAd,
	OUT PUCHAR pFrameBuf,
	OUT PULONG pFrameLen);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
VOID RRM_InsertBssAvailableACIE(
	IN PRTMP_ADAPTER pAd,
	OUT PUCHAR pFrameBuf,
	OUT PULONG pFrameLen);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
VOID RRM_InsertRequestIE(
	IN PRTMP_ADAPTER pAd,
	OUT PUCHAR pFrameBuf,
	OUT PULONG pFrameLen);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
VOID RRM_InsertTxStreamReqIE(
	IN PRTMP_ADAPTER pAd,
	OUT PUCHAR pFrameBuf,
	OUT PULONG pFrameLen,
	IN PUCHAR pBuf);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
VOID RRM_InsertTxStreamReqTriggerReportSubIE(
	IN PRTMP_ADAPTER pAd,
	OUT PUCHAR pFrameBuf,
	OUT PULONG pFrameLen,
	IN PUCHAR pBuf);


/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
VOID RRM_EnqueueBcnReq(
	IN PRTMP_ADAPTER pAd,
	IN UINT8 Aid,
	IN UINT8 IfIdx,
	IN PRRM_MLME_BCN_REQ_INFO pMlmeBcnReq);


/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
VOID RRM_EnqueueNeighborRep(
	IN PRTMP_ADAPTER pAd,
	IN PMAC_TABLE_ENTRY pEntry,
	IN UINT8 DialogToken,
	IN PCHAR pSsid,
	IN UINT8 SsidLen);

VOID RRM_EnqueueLinkMeasureReq(
	IN PRTMP_ADAPTER pAd,
	IN UINT8 Aid,
	IN UINT8 apidx);

VOID RRM_EnqueueTxStreamMeasureReq(
	IN PRTMP_ADAPTER pAd,
	IN UINT8 Aid,
	IN UINT8 apidx,
	IN PRRM_MLME_TRANSMIT_REQ_INFO pMlmeTxMeasureReq);
/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
BOOLEAN RRM_PeerNeighborReqSanity(
	IN PRTMP_ADAPTER pAd,
	IN VOID *pMsg,
	IN ULONG MsgLen,
	OUT PUINT8 pDialogToken,
	OUT PCHAR *pSsid,
	OUT PUINT8 pSsidLen);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
BOOLEAN RRM_PeerMeasureReportSanity(
	IN PRTMP_ADAPTER pAd,
	IN VOID *pMsg,
	IN ULONG MsgLen,
	OUT PUINT8 pDialogToken,
	OUT PMEASURE_REPORT_INFO pMeasureReportInfo,
	OUT PVOID *pMeasureRep);

/*
	==========================================================================
	Description:
		
	Parametrs:
	
	Return	: None.
	==========================================================================
 */
VOID RRM_PeerNeighborReqAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem) ;

VOID RRM_PeerMeasureRepAction(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem);
#endif /* DOT11K_RRM_SUPPORT */

#endif /* __RRM_H */


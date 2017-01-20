/****************************************************************************
 *
 * MODULE:             http parse.h
 *
 * COMPONENT:          home kit interface
 *
 * REVISION:           $Revision:  1.0$
 *
 * DATED:              $Date: 2017-01-11 15:13:17 +0100 (Fri, 12 Dec 2016 $
 *
 * AUTHOR:             PCT
 *
 ****************************************************************************
 *
 * Copyright panchangtao@gmail.com 2017. All rights reserved
 *
 ***************************************************************************/
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "pairing.h"
#include "http_handle.h"
#include "hkdf.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define DBG_PAIR 1
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
static tePairStatus eSrpStartResponse(int iSockFd, char *pSetupCode, tsHttpEntry *psHttpEntry);
static tePairStatus eSrpVerifyResponse(int iSockFd, tsHttpEntry *psHttpEntry);
static uint8* puTlvTypeMalloc(teTlvValue eTlvValue, uint16 u16ValueLen, uint8 *puValueData, uint16 *pu16LengthOut);
static tePairStatus eTlvTypeGetObject(teTlvValue eTlvValue, uint8 *pBuffer, uint16 u16Len, tsTlvType *psTlvType);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
SRP *pSrp = NULL;
uint8 auSessionKey[64] = {0};
cstr *pSecretKey = NULL;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

tePairStatus ePairSetup(int iSockFd, char *pSetupCode, tsHttpEntry *psHttpEntry)
{
    tePairSetupState ePair = E_PAIR_SETUP_SRP_START_NULL;
    tsTlvType sTlvType;
    memset(&sTlvType, 0, sizeof(sTlvType));
    if(E_PAIRING_STATUS_OK != eTlvTypeGetObject(E_TLV_VALUE_TYPE_STATE, psHttpEntry->acContentData, psHttpEntry->u16ContentLen, &sTlvType)){
        WAR_vPrintln(DBG_PAIR, "Can't find kTLVType_State Data");
        return E_PAIRING_STATUS_ERROR;
    }
    ePair = (tePairSetupState)sTlvType.psValue[0];
    FREE(sTlvType.psValue);

    DBG_vPrintln(DBG_PAIR, "PairSetup:%d", ePair);
    switch(ePair){
        case (E_PAIR_SETUP_M1_SRP_START_REQUEST):{
            eSrpStartResponse(iSockFd, pSetupCode, psHttpEntry);
        }
            break;
        case (E_PAIR_SETUP_M3_SRP_VERIFY_REQUEST):{
            eSrpVerifyResponse(iSockFd, psHttpEntry);
        }
            break;
        default:
            break;
    }
    return E_PAIRING_STATUS_OK;
}
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/

static tePairStatus eSrpStartResponse(int iSockFd, char *pSetupCode, tsHttpEntry *psHttpEntry)
{
    const unsigned char modulusStr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2,
                                        0x21, 0x68, 0xC2, 0x34, 0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1,
                                        0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74, 0x02, 0x0B, 0xBE, 0xA6,
                                        0x3B, 0x13, 0x9B, 0x22, 0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD,
                                        0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B, 0x30, 0x2B, 0x0A, 0x6D,
                                        0xF2, 0x5F, 0x14, 0x37, 0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45,
                                        0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6, 0xF4, 0x4C, 0x42, 0xE9,
                                        0xA6, 0x37, 0xED, 0x6B, 0x0B, 0xFF, 0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED,
                                        0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5, 0xAE, 0x9F, 0x24, 0x11,
                                        0x7C, 0x4B, 0x1F, 0xE6, 0x49, 0x28, 0x66, 0x51, 0xEC, 0xE4, 0x5B, 0x3D,
                                        0xC2, 0x00, 0x7C, 0xB8, 0xA1, 0x63, 0xBF, 0x05, 0x98, 0xDA, 0x48, 0x36,
                                        0x1C, 0x55, 0xD3, 0x9A, 0x69, 0x16, 0x3F, 0xA8, 0xFD, 0x24, 0xCF, 0x5F,
                                        0x83, 0x65, 0x5D, 0x23, 0xDC, 0xA3, 0xAD, 0x96, 0x1C, 0x62, 0xF3, 0x56,
                                        0x20, 0x85, 0x52, 0xBB, 0x9E, 0xD5, 0x29, 0x07, 0x70, 0x96, 0x96, 0x6D,
                                        0x67, 0x0C, 0x35, 0x4E, 0x4A, 0xBC, 0x98, 0x04, 0xF1, 0x74, 0x6C, 0x08,
                                        0xCA, 0x18, 0x21, 0x7C, 0x32, 0x90, 0x5E, 0x46, 0x2E, 0x36, 0xCE, 0x3B,
                                        0xE3, 0x9E, 0x77, 0x2C, 0x18, 0x0E, 0x86, 0x03, 0x9B, 0x27, 0x83, 0xA2,
                                        0xEC, 0x07, 0xA2, 0x8F, 0xB5, 0xC5, 0x5D, 0xF0, 0x6F, 0x4C, 0x52, 0xC9,
                                        0xDE, 0x2B, 0xCB, 0xF6, 0x95, 0x58, 0x17, 0x18, 0x39, 0x95, 0x49, 0x7C,
                                        0xEA, 0x95, 0x6A, 0xE5, 0x15, 0xD2, 0x26, 0x18, 0x98, 0xFA, 0x05, 0x10,
                                        0x15, 0x72, 0x8E, 0x5A, 0x8A, 0xAA, 0xC4, 0x2D, 0xAD, 0x33, 0x17, 0x0D,
                                        0x04, 0x50, 0x7A, 0x33, 0xA8, 0x55, 0x21, 0xAB, 0xDF, 0x1C, 0xBA, 0x64,
                                        0xEC, 0xFB, 0x85, 0x04, 0x58, 0xDB, 0xEF, 0x0A, 0x8A, 0xEA, 0x71, 0x57,
                                        0x5D, 0x06, 0x0C, 0x7D, 0xB3, 0x97, 0x0F, 0x85, 0xA6, 0xE1, 0xE4, 0xC7,
                                        0xAB, 0xF5, 0xAE, 0x8C, 0xDB, 0x09, 0x33, 0xD7, 0x1E, 0x8C, 0x94, 0xE0,
                                        0x4A, 0x25, 0x61, 0x9D, 0xCE, 0xE3, 0xD2, 0x26, 0x1A, 0xD2, 0xEE, 0x6B,
                                        0xF1, 0x2F, 0xFA, 0x06, 0xD9, 0x8A, 0x08, 0x64, 0xD8, 0x76, 0x02, 0x73,
                                        0x3E, 0xC8, 0x6A, 0x64, 0x52, 0x1F, 0x2B, 0x18, 0x17, 0x7B, 0x20, 0x0C,
                                        0xBB, 0xE1, 0x17, 0x57, 0x7A, 0x61, 0x5D, 0x6C, 0x77, 0x09, 0x88, 0xC0,
                                        0xBA, 0xD9, 0x46, 0xE2, 0x08, 0xE2, 0x4F, 0xA0, 0x74, 0xE5, 0xAB, 0x31,
                                        0x43, 0xDB, 0x5B, 0xFC, 0xE0, 0xFD, 0x10, 0x8E, 0x4B, 0x82, 0xD1, 0x20,
                                        0xA9, 0x3A, 0xD2, 0xCA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    const unsigned char curveBasePoint[] = { 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    const unsigned char generator[] = {0x05};

    pSrp =  SRP_new(SRP6a_server_method());
    SRP_set_username(pSrp, "Pair-Setup");
    uint8 auSalt[16] = {0};
    for(int j = 0; j < 16; j++){
        auSalt[j] = (uint8)rand();
    }
    SRP_set_params(pSrp, modulusStr, sizeof(modulusStr)/sizeof(modulusStr[0]), generator, 1, auSalt, sizeof(auSalt));
    SRP_set_auth_password(pSrp, pSetupCode);
    cstr *psPublicKey = NULL;
    SRP_gen_pub(pSrp, &psPublicKey);

    uint8 data[MABF] = {0};
    uint16 LenOut = 0, LenOutTemp = 0;

    uint8 value_rep[1] = {E_PAIR_SETUP_M2_SRP_START_RESPONSE};
    uint8 *psStateRespData = puTlvTypeMalloc(E_TLV_VALUE_TYPE_STATE, 0x01, value_rep, &LenOutTemp);
    CHECK_POINTER(psStateRespData, E_PAIRING_STATUS_ERROR);
    memcpy(data, psStateRespData, LenOutTemp);
    LenOut += LenOutTemp;
    FREE(psStateRespData);

    uint8 *psKeyData = puTlvTypeMalloc(E_TLV_VALUE_TYPE_PUBLIC_KEY, (uint16) psPublicKey->length,
                                       (uint8 *) psPublicKey->data, &LenOutTemp);
    CHECK_POINTER(psKeyData, E_PAIRING_STATUS_ERROR);
    memcpy(&data[LenOut], psKeyData, LenOutTemp);
    LenOut += LenOutTemp;
    FREE(psKeyData);

    uint8 *psSaltData = puTlvTypeMalloc(E_TLV_VALUE_TYPE_SALT, sizeof(auSalt), auSalt, &LenOutTemp);
    CHECK_POINTER(psSaltData, E_PAIRING_STATUS_ERROR);
    memcpy(&data[LenOut], psSaltData, LenOutTemp);
    LenOut += LenOutTemp;
    FREE(psSaltData);

    uint8 value_req[1] = {E_PAIR_SETUP_M2_SRP_START_RESPONSE};
    uint8 *psStateReqData = puTlvTypeMalloc(E_TLV_VALUE_TYPE_STATE, 0x01, value_req, &LenOutTemp);
    CHECK_POINTER(psStateReqData, E_PAIRING_STATUS_ERROR);
    memcpy(data, psStateReqData, LenOutTemp);
    LenOut += LenOutTemp;
    FREE(psStateReqData);

    psHttpEntry->iHttpStatus = 200;
    eHttpResponse(iSockFd, psHttpEntry, data, LenOut);

    return E_PAIRING_STATUS_OK;
}

static tePairStatus eSrpVerifyResponse(int iSockFd, tsHttpEntry *psHttpEntry)
{
    cstr *pResponse = NULL;

    const char *pProofStr = NULL;
    int iProofLen = 0;

    tsTlvType sTlvPublicKey, sTlvProof;
    memset(&sTlvPublicKey, 0, sizeof(sTlvPublicKey));
    memset(&sTlvProof, 0, sizeof(sTlvProof));
    if(E_PAIRING_STATUS_OK != eTlvTypeGetObject(E_TLV_VALUE_TYPE_PUBLIC_KEY, psHttpEntry->acContentData, psHttpEntry->u16ContentLen, &sTlvPublicKey) ||
            E_PAIRING_STATUS_OK != eTlvTypeGetObject(E_TLV_VALUE_TYPE_PROOF, psHttpEntry->acContentData, psHttpEntry->u16ContentLen, &sTlvProof)     ){
        WAR_vPrintln(DBG_PAIR, "Can't find TLV Data");
        return E_PAIRING_STATUS_ERROR;
    }

    SRP_RESULT ret = SRP_compute_key(pSrp, &pSecretKey, sTlvPublicKey.psValue, sTlvPublicKey.u16Len);

    ret = SRP_verify(pSrp, sTlvProof.psValue, sTlvProof.u16Len);
    if(!SRP_OK(ret)){
        WAR_vPrintln(DBG_PAIR, "Invalid Setup Code");
        return E_PAIRING_STATUS_ERROR_KEY;
    }
    DBG_vPrintln(DBG_PAIR, "Setup Code is Correct");
    SRP_respond(pSrp, &pResponse);
    const char salt[] = "Pair-Setup-Encrypt-Salt";
    const char info[] = "Pair-Setup-Encrypt-Info";
    uint8 auSessionKey[64];
    int i = hkdf((const unsigned char*)salt, strlen(salt), (const unsigned char*)pSecretKey->data, pSecretKey->length,
                 (const unsigned char*)info, strlen(info), auSessionKey, 32);

    uint8 data[MABF] = {0};
    uint16 LenOut = 0, LenOutTemp = 0;

    uint8 value_rep[1] = {E_PAIR_SETUP_M4_SRP_VERIFY_RESPONSE};
    uint8 *psStateRespData = puTlvTypeMalloc(E_TLV_VALUE_TYPE_STATE, 0x01, value_rep, &LenOutTemp);
    CHECK_POINTER(psStateRespData, E_PAIRING_STATUS_ERROR);
    memcpy(data, psStateRespData, LenOutTemp);
    LenOut += LenOutTemp;
    FREE(psStateRespData);



    return E_PAIRING_STATUS_OK;
}

static uint8* puTlvTypeMalloc(teTlvValue eTlvValue, uint16 u16ValueLen, uint8 *puValueData, uint16 *pu16LengthOut)
{
    CHECK_POINTER(puValueData, NULL);
    CHECK_POINTER(pu16LengthOut, NULL);

    unsigned int num = (unsigned int)(u16ValueLen / TLV_FRAGMENTED);
    if(u16ValueLen % TLV_FRAGMENTED){
        num ++;
    }
    *pu16LengthOut = (uint16)(u16ValueLen + TLV_HEADER * num);
    uint8 *psReturnData = (uint8*)malloc(*pu16LengthOut * sizeof(uint8*));
    CHECK_POINTER(psReturnData, NULL);

    int i = 0;
    for (i = 0; i < num - 1; ++i) {
        psReturnData[TLV_FRAGMENTED*i] = eTlvValue;
        psReturnData[TLV_FRAGMENTED*i + TLV_TYPE_LEN] = TLV_FRAGMENTED;
        memcpy(&psReturnData[TLV_FRAGMENTED*i + TLV_HEADER], &puValueData[TLV_FRAGMENTED*i], TLV_FRAGMENTED);
    }
    psReturnData[(TLV_FRAGMENTED + TLV_HEADER) * i] = eTlvValue;
    psReturnData[(TLV_FRAGMENTED + TLV_HEADER) * i + TLV_TYPE_LEN] = (uint8)(u16ValueLen - TLV_FRAGMENTED * i);
    memcpy(&psReturnData[(TLV_FRAGMENTED + TLV_HEADER) * i + TLV_HEADER], &puValueData[TLV_FRAGMENTED * i], (size_t)(u16ValueLen - TLV_FRAGMENTED * i));

    return psReturnData;
}

static tePairStatus eTlvTypeGetObject(teTlvValue eTlvValue, uint8 *pBuffer, uint16 u16Len, tsTlvType *psTlvType)
{
    int index = 0;
    int offset = 0;
    int fragmented = 0;
    if(psTlvType->psValue != NULL){
        WAR_vPrintln(T_TRUE, "The Tlv value is not null, will be leak memory");
        return E_PAIRING_STATUS_ERROR;
    }

    do {
        if(pBuffer[index] == eTlvValue){
            fragmented = 1;
            psTlvType->u8Type = eTlvValue;
            psTlvType->u16Len += pBuffer[index + TLV_TYPE_LEN];
            uint8* psTemp = (uint8*)realloc(psTlvType->psValue, psTlvType->u16Len * sizeof(uint8));
            CHECK_POINTER(psTemp, E_PAIRING_STATUS_ERROR);
            psTlvType->psValue = psTemp;
            memcpy(&psTlvType->psValue[offset], &pBuffer[index + TLV_HEADER], pBuffer[index + TLV_TYPE_LEN]);
            offset = psTlvType->u16Len;
        }
        int len = pBuffer[index + TLV_TYPE_LEN] + TLV_HEADER;
        index += len;
        if((index >= u16Len)) break;
        if(fragmented && (pBuffer[index] != eTlvValue)) break;
    } while(T_TRUE);

    if(offset == 0){
        return E_PAIRING_STATUS_NOT_FOUND;
    }

    DBG_vPrintln(DBG_PAIR, "eTlvTypeGetObject[%d][%d]", psTlvType->u8Type, psTlvType->u16Len);
    if(DBG_PAIR){
        for (int i = 0; i < psTlvType->u16Len; ++i) {
            printf("0x%02x ", psTlvType->psValue[i]);
        }printf("\n");
    }

    return E_PAIRING_STATUS_OK;
}
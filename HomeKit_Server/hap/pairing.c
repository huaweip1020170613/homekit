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
#include <chacha20_simple.h>
#include <profile.h>
#include "pairing.h"
#include "poly1305.h"
#include "hkdf.h"
#include "ip.h"
#include "tlv.h"
#include "bonjour.h"
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

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
const uint8 generator[] = {0x05};
const uint8 modulusStr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2, 0x21, 0x68, 0xC2, 0x34, 0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1, 0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74, 0x02, 0x0B, 0xBE, 0xA6, 0x3B, 0x13, 0x9B, 0x22, 0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD, 0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B, 0x30, 0x2B, 0x0A, 0x6D, 0xF2, 0x5F, 0x14, 0x37, 0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45, 0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6, 0xF4, 0x4C, 0x42, 0xE9, 0xA6, 0x37, 0xED, 0x6B, 0x0B, 0xFF, 0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED, 0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5, 0xAE, 0x9F, 0x24, 0x11, 0x7C, 0x4B, 0x1F, 0xE6, 0x49, 0x28, 0x66, 0x51, 0xEC, 0xE4, 0x5B, 0x3D, 0xC2, 0x00, 0x7C, 0xB8, 0xA1, 0x63, 0xBF, 0x05, 0x98, 0xDA, 0x48, 0x36, 0x1C, 0x55, 0xD3, 0x9A, 0x69, 0x16, 0x3F, 0xA8, 0xFD, 0x24, 0xCF, 0x5F, 0x83, 0x65, 0x5D, 0x23, 0xDC, 0xA3, 0xAD, 0x96, 0x1C, 0x62, 0xF3, 0x56, 0x20, 0x85, 0x52, 0xBB, 0x9E, 0xD5, 0x29, 0x07, 0x70, 0x96, 0x96, 0x6D, 0x67, 0x0C, 0x35, 0x4E, 0x4A, 0xBC, 0x98, 0x04, 0xF1, 0x74, 0x6C, 0x08, 0xCA, 0x18, 0x21, 0x7C, 0x32, 0x90, 0x5E, 0x46, 0x2E, 0x36, 0xCE, 0x3B, 0xE3, 0x9E, 0x77, 0x2C, 0x18, 0x0E, 0x86, 0x03, 0x9B, 0x27, 0x83, 0xA2, 0xEC, 0x07, 0xA2, 0x8F, 0xB5, 0xC5, 0x5D, 0xF0, 0x6F, 0x4C, 0x52, 0xC9, 0xDE, 0x2B, 0xCB, 0xF6, 0x95, 0x58, 0x17, 0x18, 0x39, 0x95, 0x49, 0x7C, 0xEA, 0x95, 0x6A, 0xE5, 0x15, 0xD2, 0x26, 0x18, 0x98, 0xFA, 0x05, 0x10, 0x15, 0x72, 0x8E, 0x5A, 0x8A, 0xAA, 0xC4, 0x2D, 0xAD, 0x33, 0x17, 0x0D, 0x04, 0x50, 0x7A, 0x33, 0xA8, 0x55, 0x21, 0xAB, 0xDF, 0x1C, 0xBA, 0x64, 0xEC, 0xFB, 0x85, 0x04, 0x58, 0xDB, 0xEF, 0x0A, 0x8A, 0xEA, 0x71, 0x57, 0x5D, 0x06, 0x0C, 0x7D, 0xB3, 0x97, 0x0F, 0x85, 0xA6, 0xE1, 0xE4, 0xC7, 0xAB, 0xF5, 0xAE, 0x8C, 0xDB, 0x09, 0x33, 0xD7, 0x1E, 0x8C, 0x94, 0xE0, 0x4A, 0x25, 0x61, 0x9D, 0xCE, 0xE3, 0xD2, 0x26, 0x1A, 0xD2, 0xEE, 0x6B, 0xF1, 0x2F, 0xFA, 0x06, 0xD9, 0x8A, 0x08, 0x64, 0xD8, 0x76, 0x02, 0x73, 0x3E, 0xC8, 0x6A, 0x64, 0x52, 0x1F, 0x2B, 0x18, 0x17, 0x7B, 0x20, 0x0C, 0xBB, 0xE1, 0x17, 0x57, 0x7A, 0x61, 0x5D, 0x6C, 0x77, 0x09, 0x88, 0xC0, 0xBA, 0xD9, 0x46, 0xE2, 0x08, 0xE2, 0x4F, 0xA0, 0x74, 0xE5, 0xAB, 0x31, 0x43, 0xDB, 0x5B, 0xFC, 0xE0, 0xFD, 0x10, 0x8E, 0x4B, 0x82, 0xD1, 0x20, 0xA9, 0x3A, 0xD2, 0xCA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const uint8 accessorySecretKey[32] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC9, 0x0F, 0xDA, 0xA2, 0x21, 0x68, 0xC2, 0x34, 0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1, 0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74};
const uint8 curveBasePoint[] = { 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static tsPairSetup sPairSetup;
static tsPairVerify sPairVerify;
/****************************************************************************/
/***        Local    Functions                                            ***/
/****************************************************************************/
static teHapStatus eAccessoryPairedFinished()
{
    system("touch PairingFinished.txt");
    return E_HAP_STATUS_OK;
}
static teHapStatus eIOSDevicePairingIDSave(uint8 *buf, int len)
{
    CHECK_POINTER(buf, E_HAP_STATUS_ERROR);
    FILE *fp = fopen("IOSDevicePairingID.txt", "w");
    CHECK_POINTER(fp, E_HAP_STATUS_ERROR);
    if(len != fwrite(buf, 1, (size_t)len, fp)){
        fclose(fp);
        return E_HAP_STATUS_ERROR;
    }
    fclose(fp);
    return E_HAP_STATUS_OK;
}
static teHapStatus eIOSDevicePairingIDRead(uint8 *buf, int len)
{
    CHECK_POINTER(buf, E_HAP_STATUS_ERROR);
    FILE *fp = fopen("IOSDevicePairingID.txt", "r");
    CHECK_POINTER(fp, E_HAP_STATUS_ERROR);
    if(len != fread(buf, 1, (size_t)len, fp)){
        fclose(fp);
        return E_HAP_STATUS_ERROR;
    }
    fclose(fp);
    return E_HAP_STATUS_OK;
}
static teHapStatus eIOSDevicePermissionSave(uint8 *buf, int len)
{
    CHECK_POINTER(buf, E_HAP_STATUS_ERROR);
    FILE *fp = fopen("IOSDevicePermission.txt", "w");
    CHECK_POINTER(fp, E_HAP_STATUS_ERROR);
    if(len != fwrite(buf, 1, (size_t)len, fp)){
        fclose(fp);
        return E_HAP_STATUS_ERROR;
    }
    fclose(fp);
    return E_HAP_STATUS_OK;
}
static teHapStatus eIOSDeviceLTPKSave(uint8 *buf, int len)
{
    CHECK_POINTER(buf, E_HAP_STATUS_ERROR);
    FILE *fp = fopen("IOSDeviceLTPK.txt", "w");
    CHECK_POINTER(fp, E_HAP_STATUS_ERROR);
    if(len != fwrite(buf, 1, (size_t)len, fp)){
        fclose(fp);
        return E_HAP_STATUS_ERROR;
    }
    fclose(fp);
    return E_HAP_STATUS_OK;
}
static teHapStatus eIOSDeviceLTPKRead(uint8 *buf, int len)
{
    CHECK_POINTER(buf, E_HAP_STATUS_ERROR);
    FILE *fp = fopen("IOSDeviceLTPK.txt", "r");
    CHECK_POINTER(fp, E_HAP_STATUS_ERROR);
    if(len != fread(buf, 1, (size_t)len, fp)){
        fclose(fp);
        return E_HAP_STATUS_ERROR;
    }
    fclose(fp);
    return E_HAP_STATUS_OK;
}
static teHapStatus eIOSDeviceRemovePairing()
{
    system("rm IOSDeviceLTPK.txt IOSDevicePairingID.txt PairingFinished.txt");
    sPairSetup.bPaired = T_FALSE;
    return E_HAP_STATUS_OK;
}
static teHapStatus eM2SrpStartResponse(int iSockFd, char *pSetupCode)
{
    teHapStatus eStatus = E_HAP_STATUS_OK;
    uint8 value_err[1] = {0};
    tsIpMessage *psResponse = psIpResponseNew();
    CHECK_POINTER(psResponse, E_HAP_STATUS_ERROR);
    tsTlvMessage *psTlvRespMessage = &psResponse->psTlvPackage->sMessage;

    /* 1. check if the accessory is already paired */
    if(sPairSetup.bPaired){
        ERR_vPrintln(T_TRUE, " the accessory is already paired");
        sPairSetup.u8MaxTries++;
        eStatus = E_HAP_STATUS_ERROR;
        value_err[0] = E_TLV_ERROR_UNAVAILABLE;
        psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ERROR,value_err,1,psTlvRespMessage);
        goto Finished;
    }

    /* 2. check if the accessory has received more than 100 unsuccessful authentication attempts */
    if(sPairSetup.u8MaxTries >= MAX_TRIES_PAIR){
        ERR_vPrintln(T_TRUE, " the accessory has received more than 100 unsuccessful authentication attempts");
        sPairSetup.u8MaxTries++;
        eStatus = E_HAP_STATUS_ERROR;
        value_err[0] = E_TLV_ERROR_MAX_TRIES;
        psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ERROR,value_err,1,psTlvRespMessage);
        goto Finished;
    }

    /* 3. check if the accessory is currently performing a Pair Setup operation with a different controller */
    if(E_THREAD_OK != eLockLockTimed(&sPairSetup.mutex, 100)){
        ERR_vPrintln(T_TRUE, "the accessory is currently performing a Pair Setup operation with a different controller");
        sPairSetup.u8MaxTries++;
        eStatus = E_HAP_STATUS_ERROR;
        value_err[0] = E_TLV_ERROR_BUSY;
        psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ERROR,value_err,1,psTlvRespMessage);
        goto Finished;
    }

    /* 4. Create new SRP session */
    sPairSetup.pSrp = SRP_new(SRP6a_server_method());

    /* 5. Set SRP username */
    SRP_RESULT Ret = SRP_set_username(sPairSetup.pSrp, "Pair-Setup");
    CHECK_RESULT(Ret, SRP_SUCCESS, E_HAP_STATUS_ERROR);
    /* 6. Generate 16 bytes of random salt and set it with SRP_set_params() */
    uint8 auSaltChar[16];
    for (int i = 0; i < 16; i++) {
        auSaltChar[i] = (uint8)rand();
    }
    Ret = SRP_set_params(sPairSetup.pSrp, modulusStr, sizeof(modulusStr), generator, sizeof(generator), auSaltChar, sizeof(auSaltChar));
    CHECK_RESULT(Ret, SRP_SUCCESS, E_HAP_STATUS_ERROR);

    /* 7. generate a random setup code and set it with SRP_set_auth_password() */
    Ret = SRP_set_auth_password(sPairSetup.pSrp, pSetupCode);
    CHECK_RESULT(Ret, SRP_SUCCESS, E_HAP_STATUS_ERROR);

    /* 8. Present the Setup Code to the user */
    NOT_vPrintln(T_TRUE, "Setup Code:%s", pSetupCode);

    /* 9. Generate an SRP public key */
    cstr *pPublicKey = NULL;
    Ret = SRP_gen_pub(sPairSetup.pSrp, &pPublicKey);
    CHECK_RESULT(Ret, SRP_SUCCESS, E_HAP_STATUS_ERROR);

    /* 10. Respond to the iOS device's request with the following TLV */
    uint8 value_rep[1] = {E_PAIR_SETUP_M2_SRP_START_RESPONSE};
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,psTlvRespMessage);
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_SALT,auSaltChar,16,psTlvRespMessage);
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_PUBLIC_KEY,(uint8*)pPublicKey->data,(uint16)pPublicKey->length,psTlvRespMessage);
    FREE(pPublicKey);
Finished:
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,psTlvRespMessage);

    uint16 u16RespLen = 0;
    uint8 *pRespBuffer = NULL;
    psResponse->psTlvPackage->eTlvMessageGetData(psTlvRespMessage,&pRespBuffer,&u16RespLen);
    uint8 *psSendBuf = NULL;
    uint16 u16Len = u16HttpFormat(E_HTTP_STATUS_SUCCESS_OK, HTTP_PROTOCOL_HTTP, "application/pairing+tlv8", pRespBuffer, u16RespLen,
                                  &psSendBuf);
    eIpMessageRelease(psResponse);
    ssize_t iSend = send(iSockFd, psSendBuf, u16Len, 0);
    FREE(psSendBuf);
    if(-1 == iSend){
        ERR_vPrintln(T_TRUE, "Send Error:%s", strerror(errno));
    }
    DBG_vPrintln(DBG_PAIR, "Send Success:%d", (int)iSend);
    return eStatus;
}
static teHapStatus eM4SrpVerifyResponse(int iSockFd, tsIpMessage *psIpMsg)
{
    uint8 value_err[] = {E_TLV_ERROR_AUTHENTICATION};
    uint8 value_rep[] = {E_PAIR_SETUP_M4_SRP_VERIFY_RESPONSE};
    tsIpMessage *psResponse = psIpResponseNew();
    CHECK_POINTER(psResponse, E_HAP_STATUS_ERROR);
    tsTlvMessage *psTlvRespMessage = &psResponse->psTlvPackage->sMessage;
    tsTlvMessage *psTlvInMessage = &psIpMsg->psTlvPackage->sMessage;
    teHapStatus eStatus = E_HAP_STATUS_OK;

    uint16 u16PublicKeyLen = psIpMsg->psTlvPackage->pu16TlvRecordGetLen(psTlvInMessage, E_TLV_VALUE_TYPE_PUBLIC_KEY);
    uint8 *psPublicKeyBuf  = psIpMsg->psTlvPackage->psTlvRecordGetData(psTlvInMessage, E_TLV_VALUE_TYPE_PUBLIC_KEY);
    CHECK_POINTER(psPublicKeyBuf, E_HAP_STATUS_ERROR);
    uint16 u16ProofLen     = psIpMsg->psTlvPackage->pu16TlvRecordGetLen(psTlvInMessage, E_TLV_VALUE_TYPE_PROOF);
    uint8 *psProofBuf      = psIpMsg->psTlvPackage->psTlvRecordGetData(psTlvInMessage, E_TLV_VALUE_TYPE_PROOF);
    CHECK_POINTER(psProofBuf, E_HAP_STATUS_ERROR);

    /* 1. Use the iOS device's SRP public key to compute the SRP shared secret key with SRP_compute_key() */
    SRP_RESULT Ret = SRP_compute_key(sPairSetup.pSrp, &sPairSetup.pSecretKey, psPublicKeyBuf, u16PublicKeyLen);

    /* 2. Verify the iOS device's SRP proof with SRP_verify() */
    Ret = SRP_verify(sPairSetup.pSrp, psProofBuf, u16ProofLen);
    if (!SRP_OK(Ret)) {
        ERR_vPrintln(T_TRUE, "Verify the iOS device's SRP proof Failed");
        eStatus = E_HAP_STATUS_ERROR;
        psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ERROR,value_err,sizeof(value_err),psTlvRespMessage);
        goto Finished;
    }

    /* 3. Generate the accessory-side SRP proof */
    cstr *psRespProof = NULL;
    SRP_respond(sPairSetup.pSrp, &psRespProof);
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_PROOF, (uint8*)psRespProof->data, (uint16)psRespProof->length, psTlvRespMessage);
    FREE(psRespProof);
    /* 4. Generate the MFi challenge, MFiChallenge, from the SRP shared secret by using HKDF-SHA-512 */
    /* 5. Derive the MFi Challenge Data, MFiChallengeData, by SHA-1 hashing the MFiChallenge */
    /* 6. Generate the MFi proof, MFiProof, by performing challenge-response generation with the Apple Authentication Coprocessor */
    /* 7. Read the Accessory Certificate, AccessoryCertificate, from the Apple Authentication Coprocessor */
    /* 8. Construct a sub-TLV with the following TLV  */
    /* 9. Derive the symmetric session encryption key, SessionKey, from the SRP shared secret by using HKDF-SHA-512 with the following parameters */
    const char salt[] = "Pair-Setup-Encrypt-Salt";
    const char info[] = "Pair-Setup-Encrypt-Info";
    if (0 != hkdf((const unsigned char*)salt, (int)strlen(salt), (const unsigned char*)sPairSetup.pSecretKey->data,
                  sPairSetup.pSecretKey->length, (const unsigned char*)info, (int)strlen(info), sPairSetup.auSessionKey, LEN_HKDF_LEN)) {
        ERR_vPrintln(T_TRUE, "HKDF Failed");
        return E_HAP_STATUS_ERROR;
    }
    /* 10. Encrypt the sub-TLV, encryptedData, and generate the 16 byte auth tag, authTag. This uses the ChaCha20-Poly1305 AEAD algorithm */
    /* 11. Construct the response with the following TLV items */
    /* 12. Send the response to the iOS device */
Finished:
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,psTlvRespMessage);
    uint16 u16RespLen = 0;
    uint8 *pRespBuffer = NULL;
    psResponse->psTlvPackage->eTlvMessageGetData(psTlvRespMessage,&pRespBuffer,&u16RespLen);
    uint8 *psSendBuffer = NULL;
    uint16 u16SendLen = u16HttpFormat(E_HTTP_STATUS_SUCCESS_OK, HTTP_PROTOCOL_HTTP, "application/pairing+tlv8", pRespBuffer,
                                      u16RespLen, &psSendBuffer);
    eIpMessageRelease(psResponse);
    if(-1 == send(iSockFd, psSendBuffer, u16SendLen, 0)){
        ERR_vPrintln(T_TRUE, "Send Error:%s", strerror(errno));
    }
    FREE(psSendBuffer);

    return eStatus;
}
static teHapStatus eM6ExchangeResponse(int iSockFd, uint8 *psDeviceID, tsIpMessage *psIpMsg)
{
    uint8 value_err[] = {E_TLV_ERROR_AUTHENTICATION};
    uint8 value_rep[] = {E_PAIR_SETUP_M6_EXCHANGE_RESPONSE};
    tsIpMessage *psResponse = psIpResponseNew();
    CHECK_POINTER(psResponse, E_HAP_STATUS_ERROR);
    tsTlvMessage *psTlvRespMessage = &psResponse->psTlvPackage->sMessage;
    tsTlvMessage *psTlvInMessage = &psIpMsg->psTlvPackage->sMessage;
    teHapStatus eStatus = E_HAP_STATUS_OK;

    uint8 *psEncryptedPackage = psIpMsg->psTlvPackage->psTlvRecordGetData(psTlvInMessage, E_TLV_VALUE_TYPE_ENCRYPTED_DATA);
    uint16 u16EncryptedLen = psIpMsg->psTlvPackage->pu16TlvRecordGetLen(psTlvInMessage, E_TLV_VALUE_TYPE_ENCRYPTED_DATA);

    /* 1. Verify the iOS device's authTag */
    /* 2. Decrypt the sub-TLV in encryptedData. */
    uint8 *psDecryptedData = (uint8*)calloc(1, (size_t)(u16EncryptedLen - LEN_AUTH_TAG));
    if(E_HAP_STATUS_ERROR == eDecryptedMessageNoLen(psEncryptedPackage, (uint16) (u16EncryptedLen - LEN_AUTH_TAG),
                                                    sPairSetup.auSessionKey, (uint8*)"PS-Msg05", psDecryptedData)){
        psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ERROR,value_err,sizeof(value_err),psTlvRespMessage);
        eStatus = E_HAP_STATUS_ERROR;
        goto Finished;
    }

    tsTlvPackage *psSubTlvPackage = psTlvPackageParser(psDecryptedData, (uint16) (u16EncryptedLen - LEN_AUTH_TAG));
    FREE(psDecryptedData);
    uint8 *psIOSDevicePairingID = psSubTlvPackage->psTlvRecordGetData(&psSubTlvPackage->sMessage, E_TLV_VALUE_TYPE_IDENTIFIER);
    uint8 *psIOSDeviceLTPK = psSubTlvPackage->psTlvRecordGetData(&psSubTlvPackage->sMessage, E_TLV_VALUE_TYPE_PUBLIC_KEY);
    uint8 *psControllerSignature = psSubTlvPackage->psTlvRecordGetData(&psSubTlvPackage->sMessage, E_TLV_VALUE_TYPE_SIGNATURE);
    if((NULL == psIOSDevicePairingID) || (NULL == psIOSDeviceLTPK) || (NULL == psControllerSignature)){
        ERR_vPrintln(T_TRUE, "Decrypted Failed");
        eTlvPackageRelease(psSubTlvPackage);
        psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ERROR,value_err,sizeof(value_err),psTlvRespMessage);
        eStatus = E_HAP_STATUS_ERROR;
        goto Finished;
    }

    /* 3. Derive iOSDeviceX from the SRP shared secret by using HKDF-SHA-512 with the following parameters */
    uint8 auIOSDeviceInfo[100];
    const char salt[] = "Pair-Setup-Controller-Sign-Salt";
    const char info[] = "Pair-Setup-Controller-Sign-Info";
    if (0 != hkdf((const uint8*)salt, (int)strlen(salt), (const uint8*)sPairSetup.pSecretKey->data, sPairSetup.pSecretKey->length,
                  (const uint8*)info, (int)strlen(info), (uint8_t*)auIOSDeviceInfo, 32)) {
        eTlvPackageRelease(psSubTlvPackage);
        ERR_vPrintln(T_TRUE, "HKDF Failed");
        return E_HAP_STATUS_ERROR;
    }

    /* 4. Construct iOSDeviceInfo by concatenating iOSDeviceX with the iOSDevicePairingID, and the iOSDeviceLTPK  */
    memcpy(&auIOSDeviceInfo[32], psIOSDevicePairingID, 36);
    memcpy(&auIOSDeviceInfo[68], psIOSDeviceLTPK,  32);

    /* 5. Use Ed25519 to verify the signature of the constructed iOSDeviceInfo with the iOSDeviceLTPK from the decrypted sub-TLV */
    int ed25519_err = ed25519_sign_open(auIOSDeviceInfo, 100, psIOSDeviceLTPK, psControllerSignature);
    if (ed25519_err) {
        ERR_vPrintln(T_TRUE, "ed25519_sign_open error");
        eTlvPackageRelease(psSubTlvPackage);
        psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ERROR,value_err,sizeof(value_err),psTlvRespMessage);
        eStatus = E_HAP_STATUS_ERROR;
        goto Finished;
    }

    /* 6. Persistently save the iOSDevicePairingID and iOSDeviceLTPK as a pairing. */
    DBG_vPrintln(DBG_PAIR,"eIOSDevicePairingIDSave & eIOSDeviceLTPKSave");
    eIOSDevicePairingIDSave(psIOSDevicePairingID, 36);
    eIOSDeviceLTPKSave(psIOSDeviceLTPK, 32);
    memcpy(sPairSetup.auControllerIdentifier, psIOSDevicePairingID, 36);
    memcpy(sPairSetup.auControllerPublicKey, psIOSDeviceLTPK,  32);

    eTlvPackageRelease(psSubTlvPackage);
    DBG_vPrintln(DBG_PAIR, "M5 Verify Success");

    /* 1. Generate its Ed25519 long-term public key, AccessoryLTPK, and long-term secret key, AccessoryLTSK */
    ed25519_secret_key edAccessoryLTSK = {0};
    memcpy(edAccessoryLTSK, accessorySecretKey, sizeof(edAccessoryLTSK));
    ed25519_public_key edAccessoryLTPK = {0};
    ed25519_publickey(edAccessoryLTSK, edAccessoryLTPK);

    /* 2. Derive AccessoryX from the SRP shared secret by using HKDF-SHA-512 with the following parameters */
    const char salt2[] = "Pair-Setup-Accessory-Sign-Salt";
    const char info2[] = "Pair-Setup-Accessory-Sign-Info";
    uint8_t auAccessoryX[150];
    hkdf((uint8*)salt2, (int)strlen(salt2), (const uint8*)sPairSetup.pSecretKey->data, sPairSetup.pSecretKey->length,
         (uint8*)info2, (int)strlen(info2), auAccessoryX, LEN_HKDF_LEN);


    /* 3. Concatenate AccessoryX with the AccessoryPairingID and AccessoryLTPK.  */
    memcpy(&auAccessoryX[32], psDeviceID, LEN_DEVICE_ID);
    memcpy(&auAccessoryX[32 + LEN_DEVICE_ID], edAccessoryLTPK, 32);

    /* 4. Use Ed25519 to generate AccessorySignature by signing AccessoryInfo with its long-term secret key, AccessoryLTSK. */
    uint8 auAcceoosrySignature[64] = {0};
    ed25519_sign(auAccessoryX, 64 + LEN_DEVICE_ID, (const uint8*)edAccessoryLTSK, (const uint8*)edAccessoryLTPK, auAcceoosrySignature);

    /* 5. Construct the sub-TLV with the following TLV items */
    tsTlvPackage *pReturnTlvPackage = psTlvPackageGenerate();
    pReturnTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_IDENTIFIER, psDeviceID, LEN_DEVICE_ID, &pReturnTlvPackage->sMessage);
    pReturnTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_SIGNATURE,auAcceoosrySignature,64,&pReturnTlvPackage->sMessage);
    pReturnTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_PUBLIC_KEY,edAccessoryLTPK,32,&pReturnTlvPackage->sMessage);

    uint8 *psSubTlvData = NULL;
    uint16 u16SubTlvLength = 0;
    pReturnTlvPackage->eTlvMessageGetData(&pReturnTlvPackage->sMessage, &psSubTlvData, &u16SubTlvLength);

    /* 6. Encrypt the sub-TLV, encryptedData, and generate the 16 byte auth tag, authTag. */
    uint8 *psEncryptedData = calloc(1, u16SubTlvLength + LEN_AUTH_TAG);
    eEncryptedMessageNoLen(psSubTlvData, u16SubTlvLength, sPairSetup.auSessionKey, (uint8*)"PS-Msg06", psEncryptedData);
    eTlvPackageRelease(pReturnTlvPackage);
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ENCRYPTED_DATA,psEncryptedData,u16SubTlvLength + (uint16)LEN_AUTH_TAG,psTlvRespMessage);
    FREE(psEncryptedData);

    /* 7. Send the response to the iOS device with the following TLV items */
    eAccessoryPairedFinished();
Finished:
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,psTlvRespMessage);
    uint16 u16RespLen = 0;
    uint8 *pRespBuffer = NULL;
    psResponse->psTlvPackage->eTlvMessageGetData(psTlvRespMessage,&pRespBuffer,&u16RespLen);
    uint8 *psHttpResp = NULL;
    uint16 u16SendLen = u16HttpFormat(E_HTTP_STATUS_SUCCESS_OK, HTTP_PROTOCOL_HTTP, "application/pairing+tlv8", pRespBuffer,
                                      u16RespLen, &psHttpResp);
    eIpMessageRelease(psResponse);
    if(-1 == send(iSockFd, psHttpResp, u16SendLen, 0)){
        ERR_vPrintln(T_TRUE, "Send Error:%s", strerror(errno));
    }
    FREE(psHttpResp);

    return eStatus;
}
static teHapStatus eM2VerifyStartResponse(int iSockFd, uint8 *psDeviceID, tsIpMessage *psIpMsg)
{
    uint8 *psRepBuffer = 0;
    uint16 u16RepLen = 0;
    uint8 value_rep[1] = {E_PAIR_VERIFY_M2_START_RESPONSE};
    //uint8 value_err[] = {E_TLV_ERROR_AUTHENTICATION};
    tsIpMessage *psResponse = psIpResponseNew();
    tsTlvMessage *psTlvRespMsg = &psResponse->psTlvPackage->sMessage;
    tsTlvMessage *psTlvInMsg = &psIpMsg->psTlvPackage->sMessage;
    teHapStatus eStatus = E_HAP_STATUS_OK;

    /* 1. Generate new, random Curve25519 key pair */
    curved25519_key auSecretKey = {0};
    for (int i = 0; i < sizeof(auSecretKey); i++) {
        auSecretKey[i] = (uint8)rand();
    }

    /* 2. Generate the shared secret, SharedSecret, from its Curve25519 secret key and the iOS device's Curve25519 public key. */
    memcpy(sPairVerify.auControllerPublicKey, psIpMsg->psTlvPackage->psTlvRecordGetData(psTlvInMsg, E_TLV_VALUE_TYPE_PUBLIC_KEY), 32);
    curve25519_donna(sPairVerify.auPublicKey, auSecretKey, curveBasePoint);
    curve25519_donna(sPairVerify.auSharedKey, auSecretKey, sPairVerify.auControllerPublicKey);

    /* 3. Construct AccessoryInfo by concatenating the following items in order */
    uint8 auAccessoryInfo[100] = {0};
    memcpy(auAccessoryInfo, sPairVerify.auPublicKey, 32);
    memcpy(&auAccessoryInfo[32], psDeviceID, LEN_DEVICE_ID);
    memcpy(&auAccessoryInfo[32 + LEN_DEVICE_ID], sPairVerify.auControllerPublicKey, 32);

    /* 4. Use Ed25519 to generate AccessorySignature by signing AccessoryInfo with its long-term secret key, AccessoryLTSK */
    ed25519_secret_key edAccessoryLTSK = {0};
    memcpy(edAccessoryLTSK, accessorySecretKey, sizeof(edAccessoryLTSK));
    ed25519_public_key edPubKey = {0};
    ed25519_publickey(edAccessoryLTSK, edPubKey);
    uint8 auAccessorySignature[64] = {0};
    ed25519_sign(auAccessoryInfo, 64 + LEN_DEVICE_ID, edAccessoryLTSK, edPubKey, auAccessorySignature);

    /* 5. Construct a sub-TLV with the following items */
    tsTlvPackage* psSubTlvPackage = psTlvPackageGenerate();
    psSubTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_SIGNATURE,auAccessorySignature,sizeof(auAccessorySignature),&psSubTlvPackage->sMessage);
    //uint8 auIdRecordData[LEN_DEVICE_ID] = {0};
    //memcpy(auIdRecordData, psBonjour->sBonjourText.psDeviceID, LEN_DEVICE_ID);
    psSubTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_IDENTIFIER,psDeviceID,LEN_DEVICE_ID,&psSubTlvPackage->sMessage);

    /* 6. Derive the symmetric session encryption key, SessionKey, from the Curve25519 shared secret by using HKDF-SHA-512 */
    const char salt[] = "Pair-Verify-Encrypt-Salt";
    const char info[] = "Pair-Verify-Encrypt-Info";
    hkdf((const uint8*)salt, (int)strlen(salt), sPairVerify.auSharedKey, 32,
         (const uint8*)info, (int)strlen(info), sPairVerify.auSessionKey, 32);

    /* 7. Encrypt the sub-TLV, encryptedData, and generate the 16-byte auth tag, authTag. */
    uint8 *psSubMsgData = NULL;
    uint16 u16SubMsgLen = 0;
    psSubTlvPackage->eTlvMessageGetData(&psSubTlvPackage->sMessage,&psSubMsgData, &u16SubMsgLen);
    uint8 *psEncryptedData = (uint8*)malloc(u16SubMsgLen + LEN_AUTH_TAG);
    eEncryptedMessageNoLen(psSubMsgData, u16SubMsgLen, sPairVerify.auSessionKey, (uint8 *) "PV-Msg02", psEncryptedData);
    eTlvPackageRelease(psSubTlvPackage);

    /* 8. Construct the response with the following TLV items */
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_PUBLIC_KEY, sPairVerify.auPublicKey, 32, psTlvRespMsg);
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ENCRYPTED_DATA,psEncryptedData,u16SubMsgLen + (uint16)LEN_AUTH_TAG,psTlvRespMsg);
    FREE(psEncryptedData);

    /* 9. Send the response to the iOS device */

    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,psTlvRespMsg);
    psResponse->psTlvPackage->eTlvMessageGetData(psTlvRespMsg,&psRepBuffer,&u16RepLen);

    uint8 *psSendBuf = NULL;
    uint16 u16Len = u16HttpFormat(E_HTTP_STATUS_SUCCESS_OK, HTTP_PROTOCOL_HTTP, "application/pairing+tlv8", psRepBuffer, u16RepLen,
                                  &psSendBuf);
    eIpMessageRelease(psResponse);
    ssize_t iSend = send(iSockFd, psSendBuf, u16Len, 0);
    FREE(psSendBuf);
    if(-1 == iSend){
        ERR_vPrintln(T_TRUE, "Send Error:%s", strerror(errno));
    }
    DBG_vPrintln(DBG_PAIR, "Send Success:%d", (int)iSend);
    return eStatus;
}
static teHapStatus eM4VerifyFinishResponse(tsController *psSockFd, tsIpMessage *psIpMsg)
{
    uint8 *psRepBuffer = 0;
    uint16 u16RepLen = 0;
    uint8 value_rep[1] = {E_PAIR_VERIFY_M4_FINISHED_RESPONSE};
    uint8 value_err[] = {E_TLV_ERROR_AUTHENTICATION};
    tsIpMessage *psResponse = psIpResponseNew();
    tsTlvMessage *psTlvRespMsg = &psResponse->psTlvPackage->sMessage;
    tsTlvMessage *psTlvInMsg = &psIpMsg->psTlvPackage->sMessage;
    teHapStatus eStatus = E_HAP_STATUS_OK;

    uint8 *psEncryptedPackData = psIpMsg->psTlvPackage->psTlvRecordGetData(psTlvInMsg,E_TLV_VALUE_TYPE_ENCRYPTED_DATA);
    uint16 u16EncryptedPackLen = psIpMsg->psTlvPackage->pu16TlvRecordGetLen(psTlvInMsg,E_TLV_VALUE_TYPE_ENCRYPTED_DATA);

    /* 1. Verify the iOS device's authTag */
    uint16 u16DecryptedLen = u16EncryptedPackLen - (uint16)LEN_AUTH_TAG;
    uint8 *psDecryptedData = (uint8*)malloc(u16DecryptedLen);
    if(E_HAP_STATUS_OK != eDecryptedMessageNoLen(psEncryptedPackData, u16DecryptedLen, sPairVerify.auSessionKey, (uint8*)"PV-Msg03", psDecryptedData)){
        psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ERROR,value_err,sizeof(value_err),psTlvRespMsg);
        eStatus = E_HAP_STATUS_ERROR;
        goto Finished;
    }

    /* 2. Decrypt the sub-TLV in encryptedData */
    tsTlvPackage *psSubTlvPack = psTlvPackageParser(psDecryptedData, u16DecryptedLen);
    FREE(psDecryptedData);
    uint8 *controllerID = psSubTlvPack->psTlvRecordGetData(&psSubTlvPack->sMessage,E_TLV_VALUE_TYPE_IDENTIFIER);
    if(NULL == controllerID){
        ERR_vPrintln(T_TRUE, "Decrypted Failed");
        psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ERROR,value_err,sizeof(value_err),psTlvRespMsg);
        eStatus = E_HAP_STATUS_ERROR;
        goto Finished;
    }

    /* 3. Use the iOSDevicePairingID, to look up the iOSDeviceLTPK, */
    uint8 auIOSDevicePairingID[36] = {0};
    uint8 auIOSDeviceLTPK[32] = {0};
    eIOSDevicePairingIDRead(auIOSDevicePairingID, 36);
    if(bcmp(auIOSDevicePairingID, controllerID, 36) == 0){
        eIOSDeviceLTPKRead(auIOSDeviceLTPK, 32);
    } else {
        ERR_vPrintln(T_TRUE, "controllerID verify failed");
        psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ERROR,value_err,sizeof(value_err),psTlvRespMsg);
        eStatus = E_HAP_STATUS_ERROR;
        goto Finished;
    }

    /* 4. Use Ed25519 to verify iOSDeviceSignature using iOSDeviceLTPK against iOSDeviceInfo contained in the decrypted sub-TLV.  */
    uint8 auIOSDeviceInfo[100];
    memcpy(auIOSDeviceInfo, sPairVerify.auControllerPublicKey, 32);
    memcpy(&auIOSDeviceInfo[32], controllerID, 36);
    memcpy(&auIOSDeviceInfo[68], sPairVerify.auPublicKey, 32);

    int err = ed25519_sign_open(auIOSDeviceInfo, 100, auIOSDeviceLTPK,
                                psSubTlvPack->psTlvRecordGetData(&psSubTlvPack->sMessage,E_TLV_VALUE_TYPE_SIGNATURE));
    eTlvPackageRelease(psSubTlvPack);
    if (err == 0) {
        hkdf((uint8*)"Control-Salt", 12, sPairVerify.auSharedKey, 32,
             (uint8*)"Control-Read-Encryption-Key", 27, psSockFd->auAccessoryToControllerKey, 32);
        hkdf((uint8*)"Control-Salt", 12, sPairVerify.auSharedKey, 32,
             (uint8*)"Control-Write-Encryption-Key", 28, psSockFd->auControllerToAccessoryKey, 32);
        INF_vPrintln(DBG_PAIR, "PairVerify success\n");
    } else {
        psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ERROR,value_err,sizeof(value_err),psTlvRespMsg);
        ERR_vPrintln(DBG_PAIR, "Verify failed\n");
        goto Finished;
    }

    /* 5. Send the response to the iOS device with the following TLV items */
Finished:
    psResponse->psTlvPackage->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,psTlvRespMsg);
    psResponse->psTlvPackage->eTlvMessageGetData(psTlvRespMsg,&psRepBuffer,&u16RepLen);
    uint8 *psHttpResp = NULL;
    uint16 u16SendLen = u16HttpFormat(E_HTTP_STATUS_SUCCESS_OK, HTTP_PROTOCOL_HTTP, "application/pairing+tlv8", psRepBuffer,
                                      u16RepLen, &psHttpResp);
    eIpMessageRelease(psResponse);
    if(-1 == send(psSockFd->iSocketFd, psHttpResp, u16SendLen, 0)){
        ERR_vPrintln(T_TRUE, "Send Error:%s", strerror(errno));
    }
    FREE(psHttpResp);
    return eStatus;
}
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
teHapStatus ePairingInit()
{
    memset(&sPairSetup, 0, sizeof(tsPairSetup));
    memset(&sPairVerify, 0, sizeof(tsPairVerify));
    sPairSetup.bPaired = bAccessoryIsPaired();
    eLockCreate(&sPairSetup.mutex);
    return E_HAP_STATUS_OK;
}
teHapStatus ePairingFinished()
{
    eLockDestroy(&sPairSetup.mutex);
    return E_HAP_STATUS_OK;
}
bool_t bAccessoryIsPaired()
{
    if(access("PairingFinished.txt", F_OK)){
        return T_FALSE;
    }
    return T_TRUE;
}
teHapStatus eHandlePairSetup(uint8 *psBuffer, int iLen, int iSocketFd, tsBonjour *psBonjour)
{
    CHECK_POINTER(psBuffer, E_HAP_STATUS_ERROR);
    CHECK_POINTER(psBonjour, E_HAP_STATUS_ERROR);

    tsIpMessage *psIpMsg = NULL;
    do{
        psIpMsg = psIpMessageFormat(psBuffer, (uint16)iLen);
        tsTlvMessage *psTlvInMessage = &psIpMsg->psTlvPackage->sMessage;
        memcpy(&sPairSetup.eState, psIpMsg->psTlvPackage->psTlvRecordGetData(psTlvInMessage, E_TLV_VALUE_TYPE_STATE), 1);
        INF_vPrintln(DBG_PAIR, "State:%d", sPairSetup.eState);
        switch (sPairSetup.eState) {
            case E_PAIR_SETUP_M1_SRP_START_REQUEST: {
                INF_vPrintln(DBG_PAIR, "E_PAIR_SETUP_M1_SRP_START_REQUEST");
                eM2SrpStartResponse(iSocketFd, psBonjour->pcSetupCode);
            }break;
            case E_PAIR_SETUP_M3_SRP_VERIFY_REQUEST: {
                DBG_vPrintln(DBG_PAIR, "E_PAIR_SETUP_M3_SRP_VERIFY_REQUEST");
                eM4SrpVerifyResponse(iSocketFd, psIpMsg);
            }break;
            case E_PAIR_SETUP_M5_EXCHANGE_REQUEST: {
                DBG_vPrintln(DBG_PAIR, "E_PAIR_SETUP_M5_EXCHANGE_REQUEST");
                eM6ExchangeResponse(iSocketFd, psBonjour->sBonjourText.psDeviceID, psIpMsg);
                eLockunLock(&sPairSetup.mutex);
                SRP_free(sPairSetup.pSrp);
                psBonjour->sBonjourText.u8StatusFlag = 0x00;
                psBonjour->eBonjourUpdate();
                eIpMessageRelease(psIpMsg);
                return E_HAP_STATUS_OK;
            }
            default:
                break;
        }
        eIpMessageRelease(psIpMsg);
    }while (0 < (iLen = (int)read(iSocketFd, psBuffer, MABF)));

    return E_HAP_STATUS_SOCKET_ERROR;
}
teHapStatus eHandlePairVerify(uint8 *psBuffer, int iLen, tsController *psSocketFd, tsBonjour *psBonjour)
{
    CHECK_POINTER(psBuffer, E_HAP_STATUS_ERROR);
    CHECK_POINTER(psBonjour, E_HAP_STATUS_ERROR);

    tsIpMessage *psIpMsg = NULL;
    do {
        psIpMsg = psIpMessageFormat(psBuffer, (uint16)iLen);
        tsTlvMessage *psTlvInMsg = &psIpMsg->psTlvPackage->sMessage;
        memcpy(&sPairVerify.eState, psIpMsg->psTlvPackage->psTlvRecordGetData(psTlvInMsg, E_TLV_VALUE_TYPE_STATE), 1);
        switch (sPairVerify.eState) {
            case E_PAIR_VERIFY_M1_START_REQUEST:{
                NOT_vPrintln(DBG_PAIR, "E_PAIR_VERIFY_M1_START_REQUEST\n");
                eM2VerifyStartResponse(psSocketFd->iSocketFd, psBonjour->sBonjourText.psDeviceID, psIpMsg);
            }break;
            case E_PAIR_VERIFY_M3_FINISHED_REQUEST:{
                NOT_vPrintln(DBG_PAIR, "E_PAIR_VERIFY_M3_FINISHED_REQUEST\n");
                eM4VerifyFinishResponse(psSocketFd, psIpMsg);
                eIpMessageRelease(psIpMsg);
                return E_HAP_STATUS_OK;
            }break;
            default:
                break;
        }
        eIpMessageRelease(psIpMsg);
    }while (0 < (iLen = (int)read(psSocketFd->iSocketFd, psBuffer, MABF)));

    return E_HAP_STATUS_SOCKET_ERROR;
}
teHapStatus eHandlePairing(tsProfile *psProfile, const uint8 *psBuffer, uint16 u16Len, uint8 **ppResp, uint16 *pu16Len)
{
    teHapStatus eStatus = E_HAP_STATUS_OK;
    uint8 *psRepBuffer = NULL;
    uint16 u16RepLen = 0;
    tsHttpEntry *psHttp = psHttpParser(psBuffer, u16Len);
    tsTlvPackage *psTlvInMsg = psTlvPackageParser(psHttp->acContentData, psHttp->u16ContentLen);
    FREE(psHttp);
    tsTlvPackage *psTlvResp = psTlvPackageGenerate();
    teTlvMethod eMethod = E_TLV_METHOD_RESERVED;
    memcpy(&eMethod, psTlvInMsg->psTlvRecordGetData(&psTlvInMsg->sMessage, E_TLV_VALUE_TYPE_METHOD), 1);
    switch (eMethod){
        case E_TLV_METHOD_REMOVE_PAIRING:{
            NOT_vPrintln(DBG_PAIR, "E_TLV_METHOD_REMOVE_PAIRING");
            uint8 value_rep[] = {E_PAIRING_REMOVE_M2_REMOVE_PAIRING_RESPONSE};
            psTlvResp->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,&psTlvResp->sMessage);
            psTlvResp->eTlvMessageGetData(&psTlvResp->sMessage,&psRepBuffer,&u16RepLen);
            *pu16Len = u16HttpFormat(E_HTTP_STATUS_SUCCESS_OK, HTTP_PROTOCOL_HTTP, HTTP_TYPE_TLV8, psRepBuffer, u16RepLen,
                                     ppResp);
            eTlvPackageRelease(psTlvResp);
            eIOSDeviceRemovePairing();
            ((tsBonjour*)psProfile->psBonjour)->sBonjourText.u8StatusFlag = 0x01;
            ((tsBonjour*)psProfile->psBonjour)->eBonjourUpdate();
        }
            break;
        case E_TLV_METHOD_ADD_PAIRING:{
            NOT_vPrintln(DBG_PAIR, "E_TLV_METHOD_ADD_PAIRING");
            uint8 value_rep[] = {E_PAIRING_ADD_M2_ADD_PAIRING_RESPONSE};
            psTlvResp->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_STATE,value_rep,1,&psTlvResp->sMessage);

            uint8 *psIdentifier = psTlvInMsg->psTlvRecordGetData(&psTlvInMsg->sMessage, E_TLV_VALUE_TYPE_IDENTIFIER);
            uint8 *psPublicKey = psTlvInMsg->psTlvRecordGetData(&psTlvInMsg->sMessage, E_TLV_VALUE_TYPE_PUBLIC_KEY);
            uint8 *psPermissions = psTlvInMsg->psTlvRecordGetData(&psTlvInMsg->sMessage, E_TLV_VALUE_TYPE_PERMISSIONS);
            //TODO:Verify that the controller sending the request has the admin bit set in the local pairings list

            uint8 auIOSDevicePairingID[36] = {0};
            eIOSDevicePairingIDRead(auIOSDevicePairingID, 36);
            if(bcmp(auIOSDevicePairingID, psIdentifier, 36) == 0){
                eIOSDevicePermissionSave(psPermissions, psTlvInMsg->pu16TlvRecordGetLen(&psTlvInMsg->sMessage, E_TLV_VALUE_TYPE_PERMISSIONS));
            } else {
                ERR_vPrintln(T_TRUE, "controllerID verify failed");
                uint8 value_err[] = {E_TLV_ERROR_UNKNOW};
                psTlvResp->efTlvMessageAddRecord(E_TLV_VALUE_TYPE_ERROR,value_err,sizeof(value_err),&psTlvResp->sMessage);
                eStatus = E_HAP_STATUS_ERROR;
            }
            psTlvResp->eTlvMessageGetData(&psTlvResp->sMessage,&psRepBuffer,&u16RepLen);
            *pu16Len = u16HttpFormat(E_HTTP_STATUS_SUCCESS_OK, HTTP_PROTOCOL_HTTP, HTTP_TYPE_TLV8, psRepBuffer, u16RepLen,
                                     ppResp);
            eTlvPackageRelease(psTlvResp);
        }
            break;
        default:
            break;
    }
    eTlvPackageRelease(psTlvInMsg);
    return eStatus;
}
teHapStatus eHandleAccessoryPackage(tsProfile *psProfile, const uint8 *psData, uint16 u16Len, uint8 **ppsResp, uint16 *pu16Len, tsController *psController)
{
    CHECK_POINTER(psData, E_HAP_STATUS_ERROR);
    CHECK_POINTER(ppsResp, E_HAP_STATUS_ERROR);
    CHECK_POINTER(pu16Len, E_HAP_STATUS_ERROR);

    DBG_vPrintln(DBG_PAIR, "eHandleAccessoryPackage:%s\n", psData);
    uint8 *psBufHttp = NULL;
    uint16 u16LenHttp = 0;
    if (strstr((char*)psData, HTTP_URL_ACCESSORY))
    {
        //Publish the characteristics of the accessories
        NOT_vPrintln(DBG_PAIR, "Ask for accessories info\n");
        json_object *psJsonRet = psGetAccessoryInfo(psProfile->psAccessory);
        u16LenHttp = u16HttpFormat(E_HTTP_STATUS_SUCCESS_OK, HTTP_PROTOCOL_HTTP, HTTP_TYPE_JSON,
                                 (uint8 *) json_object_get_string(psJsonRet),
                                 (uint16) strlen(json_object_get_string(psJsonRet)), &psBufHttp);
        json_object_put(psJsonRet);
        DBG_vPrintln(DBG_PAIR, "Accessory Info:\n%s", psBufHttp);
        eHttpEncryptedSend(psBufHttp, u16LenHttp, psController);
        FREE(psBufHttp);
    }
    else if(strstr((char*)psData, HTTP_URL_CHARACTER))
    {
        tsHttpEntry *psHttp = psHttpParser(psData, u16Len);
        if(strstr((char*)psData, "PUT"))
        {
            NOT_vPrintln(DBG_PAIR, "Writing Characteristics Attribute:%s\n", psHttp->acContentData);
            eSetCharacteristicInfo(psProfile->psAccessory, psController, psHttp->acContentData, &psBufHttp, &u16LenHttp, psProfile->eHandleSetCmd);
            DBG_vPrintln(DBG_PAIR, "Return Http:\n%s", psBufHttp);
            eHttpEncryptedSend(psBufHttp, u16LenHttp, psController);
            FREE(psBufHttp);
        }
        else if(strstr((char*)psData, "GET"))
        {
            WAR_vPrintln(DBG_PAIR, "Reading Characteristics Attribute\n");
            json_object *psJsonRet = psGetCharacteristicInfo(psProfile->psAccessory, (char*)psHttp->acDirectory, psProfile->eHandleGetCmd);
            u16LenHttp = u16HttpFormat(E_HTTP_STATUS_SUCCESS_OK, HTTP_PROTOCOL_HTTP, HTTP_TYPE_JSON,
                                     (uint8 *) json_object_get_string(psJsonRet),
                                     (uint16) strlen(json_object_get_string(psJsonRet)), &psBufHttp);
            json_object_put(psJsonRet);
            DBG_vPrintln(DBG_PAIR, "Return Http:\n%s", psBufHttp);
            eHttpEncryptedSend(psBufHttp, u16LenHttp, psController);
            FREE(psBufHttp);
        }
        FREE(psHttp);
    }
    else if(strstr((char*)psData, HTTP_URL_PAIRINGS))
    {
        NOT_vPrintln(DBG_PAIR, "Controller Request Add/Remove/List Pairing");
        eHandlePairing(psProfile, psData, u16Len, &psBufHttp, &u16LenHttp);
        eHttpEncryptedSend(psBufHttp, u16LenHttp, psController);
        FREE(psBufHttp);
    }

    return E_HAP_STATUS_OK;
}

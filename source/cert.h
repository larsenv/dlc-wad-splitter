/*
 * cert.h
 *
 * Copyright (c) 2020, DarkMatterCore <pabloacurielz@gmail.com>.
 *
 * This file is part of dlc-wad-splitter (https://github.com/DarkMatterCore/dlc-wad-splitter).
 *
 * dlc-wad-splitter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * dlc-wad-splitter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#ifndef __CERT_H__
#define __CERT_H__

#include "signature.h"

#define SIGNED_CERT_MAX_SIZE    (u64)sizeof(CertSigRsa4096PubKeyRsa4096)
#define SIGNED_CERT_MIN_SIZE    (u64)sizeof(CertSigHmac160PubKeyEcc480)

typedef enum {
    CertType_None                     = 0,
    CertType_SigRsa4096_PubKeyRsa4096 = 1,
    CertType_SigRsa4096_PubKeyRsa2048 = 2,
    CertType_SigRsa4096_PubKeyEcc480  = 3,
    CertType_SigRsa2048_PubKeyRsa4096 = 4,
    CertType_SigRsa2048_PubKeyRsa2048 = 5,
    CertType_SigRsa2048_PubKeyEcc480  = 6,
    CertType_SigEcc480_PubKeyRsa4096  = 7,
    CertType_SigEcc480_PubKeyRsa2048  = 8,
    CertType_SigEcc480_PubKeyEcc480   = 9,
    CertType_SigHmac160_PubKeyRsa4096 = 10,
    CertType_SigHmac160_PubKeyRsa2048 = 11,
    CertType_SigHmac160_PubKeyEcc480  = 12
} CertType;

/// Always stored using big endian byte order.
typedef enum {
    CertPubKeyType_Rsa4096 = 0,
    CertPubKeyType_Rsa2048 = 1,
    CertPubKeyType_Ecc480  = 2
} CertPubKeyType;

/// Placed after the certificate signature block.
typedef struct {
    char issuer[0x40];
    u32 pub_key_type;   ///< CertPubKeyType. Stored using big endian byte order.
    char name[0x40];
    u32 date;           ///< Stored using big endian byte order.
} CertCommonBlock;

typedef struct {
    u8 public_key[0x200];
    u32 public_exponent;
    u8 padding[0x34];
} CertPublicKeyBlockRsa4096;

typedef struct {
    u8 public_key[0x100];
    u32 public_exponent;
    u8 padding[0x34];
} CertPublicKeyBlockRsa2048;

typedef struct {
    u8 public_key[0x3C];
    u8 padding[0x3C];
} CertPublicKeyBlockEcc480;

typedef struct {
    SignatureBlockRsa4096 sig_block;            ///< sig_type field is stored using big endian byte order.
    CertCommonBlock cert_common_block;          ///< pub_key_type field must be CertPubKeyType_Rsa4096.
    CertPublicKeyBlockRsa4096 pub_key_block;
} CertSigRsa4096PubKeyRsa4096;

typedef struct {
    SignatureBlockRsa4096 sig_block;            ///< sig_type field is stored using big endian byte order.
    CertCommonBlock cert_common_block;          ///< pub_key_type field must be CertPubKeyType_Rsa2048.
    CertPublicKeyBlockRsa2048 pub_key_block;
} CertSigRsa4096PubKeyRsa2048;

typedef struct {
    SignatureBlockRsa4096 sig_block;            ///< sig_type field is stored using big endian byte order.
    CertCommonBlock cert_common_block;          ///< pub_key_type field must be CertPubKeyType_Ecc480.
    CertPublicKeyBlockEcc480 pub_key_block;
} CertSigRsa4096PubKeyEcc480;

typedef struct {
    SignatureBlockRsa2048 sig_block;            ///< sig_type field is stored using big endian byte order.
    CertCommonBlock cert_common_block;          ///< pub_key_type field must be CertPubKeyType_Rsa4096.
    CertPublicKeyBlockRsa4096 pub_key_block;
} CertSigRsa2048PubKeyRsa4096;

typedef struct {
    SignatureBlockRsa2048 sig_block;            ///< sig_type field is stored using big endian byte order.
    CertCommonBlock cert_common_block;          ///< pub_key_type field must be CertPubKeyType_Rsa2048.
    CertPublicKeyBlockRsa2048 pub_key_block;
} CertSigRsa2048PubKeyRsa2048;

typedef struct {
    SignatureBlockRsa2048 sig_block;            ///< sig_type field is stored using big endian byte order.
    CertCommonBlock cert_common_block;          ///< pub_key_type field must be CertPubKeyType_Ecc480.
    CertPublicKeyBlockEcc480 pub_key_block;
} CertSigRsa2048PubKeyEcc480;

typedef struct {
    SignatureBlockEcc480 sig_block;             ///< sig_type field is stored using big endian byte order.
    CertCommonBlock cert_common_block;          ///< pub_key_type field must be CertPubKeyType_Rsa4096.
    CertPublicKeyBlockRsa4096 pub_key_block;
} CertSigEcc480PubKeyRsa4096;

typedef struct {
    SignatureBlockEcc480 sig_block;             ///< sig_type field is stored using big endian byte order.
    CertCommonBlock cert_common_block;          ///< pub_key_type field must be CertPubKeyType_Rsa2048.
    CertPublicKeyBlockRsa2048 pub_key_block;
} CertSigEcc480PubKeyRsa2048;

typedef struct {
    SignatureBlockEcc480 sig_block;             ///< sig_type field is stored using big endian byte order.
    CertCommonBlock cert_common_block;          ///< pub_key_type field must be CertPubKeyType_Ecc480.
    CertPublicKeyBlockEcc480 pub_key_block;
} CertSigEcc480PubKeyEcc480;

typedef struct {
    SignatureBlockHmac160 sig_block;            ///< sig_type field is stored using big endian byte order.
    CertCommonBlock cert_common_block;          ///< pub_key_type field must be CertPubKeyType_Rsa4096.
    CertPublicKeyBlockRsa4096 pub_key_block;
} CertSigHmac160PubKeyRsa4096;

typedef struct {
    SignatureBlockHmac160 sig_block;            ///< sig_type field is stored using big endian byte order.
    CertCommonBlock cert_common_block;          ///< pub_key_type field must be CertPubKeyType_Rsa2048.
    CertPublicKeyBlockRsa2048 pub_key_block;
} CertSigHmac160PubKeyRsa2048;

typedef struct {
    SignatureBlockHmac160 sig_block;            ///< sig_type field is stored using big endian byte order.
    CertCommonBlock cert_common_block;          ///< pub_key_type field must be CertPubKeyType_Ecc480.
    CertPublicKeyBlockEcc480 pub_key_block;
} CertSigHmac160PubKeyEcc480;

/// Used to store certificate data.
typedef struct {
    u8 type;                        ///< CertType.
    u64 size;                       ///< Raw certificate size.
    u8 data[SIGNED_CERT_MAX_SIZE];  ///< Raw certificate data.
} Certificate;

/// Used to store two or more certificates.
typedef struct {
    u8 *raw_chain;      ///< Raw certificate chain.
    u64 raw_chain_size; ///< Raw certificate chain size.
    u32 count;          ///< Certificate count.
    Certificate *certs; ///< Individual certificate info.
} CertificateChain;

/// Reads a certificate chain from a file and validates all signatures (whenever possible).
/// A CertificateChain element is filled with the data from all read certificates.
bool certReadCertificateChainFromFile(FILE *fd, u64 cert_chain_size, CertificateChain *out_chain);

/// Verifies the signature from a signed payload using a previously populated CertificateChain element.
/// Returns false if there's an error in the signature verification steps. Otherwise, returns true and the result from the signature verification is saved to out_result.
bool certVerifySignatureFromSignedPayload(CertificateChain *chain, void *signed_payload, u64 signed_payload_size, bool *out_result);

/// Helper inline functions.

ALWAYS_INLINE void certFreeCertificateChain(CertificateChain *chain)
{
    if (!chain) return;
    if (chain->raw_chain) free(chain->raw_chain);
    if (chain->certs) free(chain->certs);
    memset(chain, 0, sizeof(CertificateChain));
}

ALWAYS_INLINE CertCommonBlock *certGetCommonBlock(void *buf)
{
    return (CertCommonBlock*)signatureGetPayload(buf);
}

ALWAYS_INLINE bool certIsValidPublicKeyType(u32 type)
{
    return (type == CertPubKeyType_Rsa4096 || type == CertPubKeyType_Rsa2048 || type == CertPubKeyType_Ecc480);
}

ALWAYS_INLINE u8 *certGetPublicKey(CertCommonBlock *cert_common_block)
{
    return ((cert_common_block != NULL && certIsValidPublicKeyType(bswap_32(cert_common_block->pub_key_type))) ? ((u8*)cert_common_block + sizeof(CertCommonBlock)) : NULL);
}

ALWAYS_INLINE u64 certGetPublicKeySize(u32 type)
{
    return (u64)(type == CertPubKeyType_Rsa4096 ? MEMBER_SIZE(CertPublicKeyBlockRsa4096, public_key) : \
                (type == CertPubKeyType_Rsa2048 ? MEMBER_SIZE(CertPublicKeyBlockRsa2048, public_key) : \
                (type == CertPubKeyType_Ecc480  ? MEMBER_SIZE(CertPublicKeyBlockEcc480,  public_key) : 0)));
}

ALWAYS_INLINE u64 certGetPublicKeyBlockSize(u32 type)
{
    return (u64)(type == CertPubKeyType_Rsa4096 ? sizeof(CertPublicKeyBlockRsa4096) : \
                (type == CertPubKeyType_Rsa2048 ? sizeof(CertPublicKeyBlockRsa2048) : \
                (type == CertPubKeyType_Ecc480  ? sizeof(CertPublicKeyBlockEcc480)  : 0)));
}

ALWAYS_INLINE u32 certGetPublicExponent(CertCommonBlock *cert_common_block)
{
    u8 *public_key = certGetPublicKey(cert_common_block);
    return ((cert_common_block != NULL && public_key != NULL) ? bswap_32(*((u32*)(public_key + certGetPublicKeySize(bswap_32(cert_common_block->pub_key_type))))) : 0);
}

ALWAYS_INLINE bool certIsValidCertificate(void *buf)
{
    CertCommonBlock *cert_common_block = certGetCommonBlock(buf);
    return (cert_common_block != NULL && certIsValidPublicKeyType(bswap_32(cert_common_block->pub_key_type)));
}

ALWAYS_INLINE u64 certGetSignedCertificateSize(void *buf)
{
    if (!certIsValidCertificate(buf)) return 0;
    CertCommonBlock *cert_common_block = certGetCommonBlock(buf);
    return (signatureGetBlockSize(signatureGetSigType(buf)) + (u64)sizeof(CertCommonBlock) + certGetPublicKeyBlockSize(bswap_32(cert_common_block->pub_key_type)));
}

ALWAYS_INLINE u64 certGetSignedCertificateHashAreaSize(void *buf)
{
    if (!certIsValidCertificate(buf)) return 0;
    CertCommonBlock *cert_common_block = certGetCommonBlock(buf);
    return ((u64)sizeof(CertCommonBlock) + certGetPublicKeyBlockSize(bswap_32(cert_common_block->pub_key_type)));
}

#endif /* __CERT_H__ */

/*
 * signature.h
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

#ifndef __SIGNATURE_H__
#define __SIGNATURE_H__

typedef enum {
    SignatureType_Rsa4096Sha1   = 0x10000,
    SignatureType_Rsa2048Sha1   = 0x10001,
    SignatureType_Ecc480Sha1    = 0x10002,
    SignatureType_Rsa4096Sha256 = 0x10003,
    SignatureType_Rsa2048Sha256 = 0x10004,
    SignatureType_Ecc480Sha256  = 0x10005,
    SignatureType_Hmac160Sha1   = 0x10006
} SignatureType;

typedef struct {
    u32 sig_type;           ///< SignatureType_Rsa4096Sha1, SignatureType_Rsa4096Sha256.
    u8 signature[0x200];
    u8 padding[0x3C];
} SignatureBlockRsa4096;

typedef struct {
    u32 sig_type;           ///< SignatureType_Rsa2048Sha1, SignatureType_Rsa2048Sha256.
    u8 signature[0x100];
    u8 padding[0x3C];
} SignatureBlockRsa2048;

typedef struct {
    u32 sig_type;           ///< SignatureType_Ecc480Sha1, SignatureType_Ecc480Sha256.
    u8 signature[0x3C];
    u8 padding[0x40];
} SignatureBlockEcc480;

typedef struct {
    u32 sig_type;           ///< SignatureType_Hmac160Sha1.
    u8 signature[0x14];
    u8 padding[0x28];
} SignatureBlockHmac160;

/// Helper inline functions.

ALWAYS_INLINE u32 signatureGetSigType(void *buf)
{
    return (buf ? bswap_32(*((u32*)buf)) : 0);
}

ALWAYS_INLINE bool signatureIsValidSigType(u32 type)
{
    return (type == SignatureType_Rsa4096Sha1   || type == SignatureType_Rsa2048Sha1   || type == SignatureType_Ecc480Sha1   || \
            type == SignatureType_Rsa4096Sha256 || type == SignatureType_Rsa2048Sha256 || type == SignatureType_Ecc480Sha256 || \
            type == SignatureType_Hmac160Sha1);
}

ALWAYS_INLINE u8 *signatureGetSig(void *buf)
{
    return (buf ? ((u8*)buf + 4) : NULL);
}

ALWAYS_INLINE u64 signatureGetSigSize(u32 type)
{
    return (u64)((type == SignatureType_Rsa4096Sha1 || type == SignatureType_Rsa4096Sha256) ? MEMBER_SIZE(SignatureBlockRsa4096, signature) : \
                ((type == SignatureType_Rsa2048Sha1 || type == SignatureType_Rsa2048Sha256) ? MEMBER_SIZE(SignatureBlockRsa2048, signature) : \
                ((type == SignatureType_Ecc480Sha1  || type == SignatureType_Ecc480Sha256)  ? MEMBER_SIZE(SignatureBlockEcc480,  signature) : \
                 (type == SignatureType_Hmac160Sha1                                         ? MEMBER_SIZE(SignatureBlockHmac160, signature) : 0))));
}

ALWAYS_INLINE u64 signatureGetBlockSize(u32 type)
{
    return (u64)((type == SignatureType_Rsa4096Sha1 || type == SignatureType_Rsa4096Sha256) ? sizeof(SignatureBlockRsa4096) : \
                ((type == SignatureType_Rsa2048Sha1 || type == SignatureType_Rsa2048Sha256) ? sizeof(SignatureBlockRsa2048) : \
                ((type == SignatureType_Ecc480Sha1  || type == SignatureType_Ecc480Sha256)  ? sizeof(SignatureBlockEcc480)  : \
                 (type == SignatureType_Hmac160Sha1                                         ? sizeof(SignatureBlockHmac160) : 0))));
}

ALWAYS_INLINE void *signatureGetPayload(void *buf)
{
    if (!buf) return NULL;
    u32 sig_type = signatureGetSigType(buf);
    return (signatureIsValidSigType(sig_type) ? (void*)((u8*)buf + signatureGetBlockSize(sig_type)) : NULL);
}

#endif /* __SIGNATURE_H__ */

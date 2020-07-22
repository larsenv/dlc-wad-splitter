/*
 * wad.h
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

#ifndef __WAD_H__
#define __WAD_H__

#include "cert.h"
#include "tik.h"
#include "tmd.h"

#define WAD_HEADER_SIZE_STR(x)  ((x) == WadHeaderSize_InstallablePackage ? "InstallablePackage" : ((x) == WadHeaderSize_BackupPackage ? "BackupPackage" : "Unknown"))
#define WAD_TYPE_STR(x)         ((x) == WadType_NormalPackage ? "Normal" : ((x) == WadType_Boot2Package ? "Boot2" : ((x) == WadType_BackupPackage ? "Backup" : "Unknown")))
#define WAD_VERSION_STR(x)      ((x) == WadVersion_InstallablePackage ? "InstallablePackage" : ((x) == WadVersion_BackupPackage ? "BackupPackage" : "Unknown"))

typedef enum {
    WadHeaderSize_InstallablePackage = 0x20,    ///< Used with Normal and Boot2 WAD packages. Equivalent to `sizeof(WadInstallablePackageHeader)`.
    WadHeaderSize_BackupPackage      = 0x70     ///< Used with Backup WAD packages. Equivalent to `sizeof(WadBackupPackageHeader)`.
} WadHeaderSize;

typedef enum {
    WadType_NormalPackage = 0x4973,    ///< "Is".
    WadType_Boot2Package  = 0x6962,    ///< "ib".
    WadType_BackupPackage = 0x426B     ///< "Bk".
} WadType;

typedef enum {
    WadVersion_InstallablePackage = 0,  ///< Used with Normal and Boot2 WAD packages.
    WadVersion_BackupPackage      = 1   ///< Used with Backup WAD packages.
} WadVersion;

/// Used with installable WAD packages.
typedef struct {
    u32 header_size;        ///< WadHeaderSize_InstallablePackage.
    u16 type;               ///< WadType_NormalPackage or WadType_Boot2Package.
    u16 version;            ///< WadVersion_InstallablePackage.
    u32 cert_chain_size;    ///< Certificate chain size.
    u8 reserved[0x04];
    u32 ticket_size;        ///< Ticket size.
    u32 tmd_size;           ///< TMD size.
    u32 data_size;          ///< Encrypted content data size.
    u32 footer_size;        ///< Decrypted footer size.
    u8 padding[0x20];
} WadInstallablePackageHeader;

/// Used with both data.bin and content.bin files.
typedef struct {
    u32 header_size;            ///< WadHeaderSize_BackupPackage.
    u16 type;                   ///< WadType_BackupPackage.
    u16 version;                ///< WadVersion_BackupPackage.
    u32 console_id;             ///< Console ID.
    u32 save_file_count;        ///< Savegame file count. Only used with data.bin - set to zero otherwise.
    u32 save_file_data_size;    ///< Savegame file data size. Only used with data.bin - set to zero otherwise.
    u32 content_tmd_size;       ///< TMD size. Only used with content.bin - set to zero otherwise.
    u32 content_data_size;      ///< Encrypted content data size. Only used with content.bin - set to zero otherwise.
    u32 backup_area_size;       ///< Total size from the start of this header to EOF.
    u8 included_contents[0x40]; ///< Right-padded bitfield that indicates which contents from the TMD are included in the content.bin file. Set to zeroes in data.bin.
    u64 title_id;               ///< Title ID. Only used with data.bin - set to zero otherwise.
    u8 mac_address[0x06];       ///< Console MAC address. Only used with data.bin - set to zero otherwise.
    u8 reserved[0x02];
    u8 padding[0x10];
} WadBackupPackageHeader;

/// Unpacks an installable WAD package to a destination directory.
/// If valid pointers are provided, certificate chain data, ticket data and/or TMD data may be saved to them.
/// Footer data won't be unpacked.
bool wadUnpackInstallablePackage(const os_char_t *wad_path, os_char_t *out_path, CertificateChain *out_cert_chain, Ticket *out_ticket, TitleMetadata *out_tmd);

/// Generates split DLC WAD packages from unpacked WAD data.
bool wadGenerateSplitDlcPackages(os_char_t *unpacked_wad_path, os_char_t *out_path, CertificateChain *cert_chain, Ticket *ticket, TitleMetadata *tmd, u16 dlc_content_idx);

/// Byteswaps fields from an installable WAD package.
ALWAYS_INLINE void wadByteswapInstallablePackageHeaderFields(WadInstallablePackageHeader *wad_header)
{
    if (!wad_header || IS_BIG_ENDIAN) return;
    wad_header->header_size = __builtin_bswap32(wad_header->header_size);
    wad_header->type = __builtin_bswap16(wad_header->type);
    wad_header->version = __builtin_bswap16(wad_header->version);
    wad_header->cert_chain_size = __builtin_bswap32(wad_header->cert_chain_size);
    wad_header->ticket_size = __builtin_bswap32(wad_header->ticket_size);
    wad_header->tmd_size = __builtin_bswap32(wad_header->tmd_size);
    wad_header->data_size = __builtin_bswap32(wad_header->data_size);
    wad_header->footer_size = __builtin_bswap32(wad_header->footer_size);
}

#endif /* __WAD_H__ */

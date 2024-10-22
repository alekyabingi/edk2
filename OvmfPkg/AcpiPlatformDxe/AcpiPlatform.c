/** @file
  OVMF ACPI Platform Driver

  Copyright (C) 2015, Red Hat, Inc.
  Copyright (c) 2008 - 2014, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <OvmfPlatforms.h> // CLOUDHV_DEVICE_ID
#include <ConfidentialComputingGuestAttr.h>

#include <Library/AcpiPlatformLib.h>

#include "AcpiPlatform.h"
#include <DebugLib.h>

/**
  Effective entrypoint of Acpi Platform driver.

  @param  ImageHandle
  @param  SystemTable

  @return EFI_SUCCESS
  @return EFI_LOAD_ERROR
  @return EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
EFIAPI
InstallAcpiTables (
  IN   EFI_ACPI_TABLE_PROTOCOL  *AcpiTable
  )
{
  EFI_STATUS  Status;
  UINT16      HostBridgeDevId;

  HostBridgeDevId = PcdGet16 (PcdOvmfHostBridgePciDevId);
  DEBUG ((DEBUG_INFO, "InstallAcpiTables[TEST]: Entering the block\n"));

  DEBUG ((DEBUG_INFO, "InstallAcpiTables[TEST]: HostBridgeDevId: %d\n", HostBridgeDevId));
  if (HostBridgeDevId == CLOUDHV_DEVICE_ID) {
    if (CC_GUEST_IS_TDX (PcdGet64 (PcdConfidentialComputingGuestAttr))) {
      DEBUG ((DEBUG_INFO, "InstallAcpiTables[TEST]: Installing CloudHV TDX tables\n"));
      Status = InstallCloudHvTablesTdx (AcpiTable);
    } else {
      DEBUG ((DEBUG_INFO, "InstallAcpiTables[TEST]: Installing CloudHV tables\n"));
      Status = InstallCloudHvTables (AcpiTable);
    }
  } else {
    DEBUG ((DEBUG_INFO, "InstallAcpiTables[TEST]: Installing QEMU FW CFG tables\n"));
    Status = InstallQemuFwCfgTables (AcpiTable);
  }

  return Status;
}

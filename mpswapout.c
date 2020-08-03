/***************************************************************************//**
 * @file
 * @brief The multi-PAN swap out plugin helps backup and restore a previous
 * network before an NCP is updated to firmware with the multi-PAN feature.
 * Just in case the previous network is something non-standard and should be
 * moved to index 1.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include "af.h"
#include "app/framework/util/af-main.h"
#include "app/framework/util/util.h"
#include "app/util/zigbee-framework/zigbee-device-common.h"
#include "stack/include/trust-center.h"
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "mpswapout.h"

//------------------------------------------------------------------------------
// Globals
TcRestoreData_t restoreData;

//------------------------------------------------------------------------------
// External Declarations

//------------------------------------------------------------------------------
// Forward Declarations


//------------------------------------------------------------------------------
// Helper Functions

EmberStatus clearTcBackupStoredFields(void)
{
  static const TcRestoreData_t clearedBackup;
  restoreData = clearedBackup;
  return EMBER_SUCCESS;
}

EmberStatus restoreTcFromBackup(void)
{
  // If a network already exists, TC replacement does not make sense.
  if (EMBER_NO_NETWORK != emberAfNetworkState()) {
    emberAfAppPrintln("Cannot import TC data while network is up.");
    return EMBER_ERR_FATAL;
  }

  EmberNetworkParameters params;
  EmberStatus emberStatus;

  // Set node information and network parameters.
  emberAfAppPrintln("Setting node information and network parameters.");
  MEMCOPY(params.extendedPanId, restoreData.ePanId,
          sizeof(params.extendedPanId));
  params.panId = restoreData.panId;
  params.radioTxPower = restoreData.radioPower;
  params.radioChannel = restoreData.radioChannel;
  params.joinMethod = EMBER_USE_MAC_ASSOCIATION;

  // Now that the parameters have been configured, kick off the network.
  emberStatus = emberAfFormNetwork(&params);
  if (emberStatus == EMBER_SUCCESS) {
    emberAfAppPrintln("Successfully formed network with given params.");
  } else {
    emberAfAppPrintln("Failed to form network. Error: 0x%x", emberStatus);
  }
  return EMBER_SUCCESS;
}

EmberStatus loadRestoreDataFromFile(void)
{
  clearTcBackupStoredFields();
  FILE* backup = fopen("backup/backup.txt", "r");
  uint8_t length = 128;
  char line[length];
  char* linePtr;
  char* endToken;

  // For now, enforce formatting on the text file.
  // Line 1 : Node ID.
  if (NULL != fgets(line, length, backup)) {
    linePtr = line;
    endToken = strchr(linePtr, ':');
    endToken += 2;
    restoreData.nodeId = (uint8_t)strtol(endToken, NULL, 10);
  }
  // Line 2 : Pan ID.
  if (NULL != fgets(line, length, backup)) {
    linePtr = line;
    endToken = strchr(linePtr, ':');
    endToken += 2;
    restoreData.panId = (uint16_t)strtol(endToken, NULL, 0);
  }

  // Line 3 : Radio Channel.
  if (NULL != fgets(line, length, backup)) {
    linePtr = line;
    endToken = strchr(linePtr, ':');
    endToken += 2;
    restoreData.radioChannel = (uint8_t)strtol(endToken, NULL, 10);
  }

  // Line 4 : Radio Power.
  if (NULL != fgets(line, length, backup)) {
    linePtr = line;
    endToken = strchr(linePtr, ':');
    endToken += 2;
    restoreData.radioPower = (int8_t)strtol(endToken, NULL, 10);
  }

  // Line 5 : Extended Pan ID.
  if (NULL != fgets(line, length, backup)) {
    linePtr = line;
    endToken = strchr(linePtr, ':');
    endToken += 2;
    // Store each hex value when processing the string.
    char currentValue[2];
    for (int arrayPointer = 7; arrayPointer >= 0; arrayPointer--) {
      strncpy(currentValue, endToken, 2);
      restoreData.ePanId[arrayPointer] = (int)strtol(currentValue, NULL, 16);
      //emberAfAppPrintln("0x%x",restoreData.ePanId[arrayPointer]);
      endToken += 2;
    }
  }

  fclose(backup);
  return EMBER_SUCCESS;
}

EmberStatus saveTcBackupToFile(void)
{
  FILE *backup;
  // Clear the original backup to repopulate it with new parameters.
  backup = fopen("backup/backup.txt", "w");
  if (backup == NULL) {
    // Note that this will only work on a POSIX-based host.
    mkdir("backup", 0777);
    backup = fopen("backup/backup.txt", "w");
  }
  
  //emberAfAppPrintln("Node ID:         %d", restoreData.nodeId);
  fprintf(backup, "Node ID: 0x%d\n", restoreData.nodeId);
  //emberAfAppPrintln("Pan ID:          0x%2x", restoreData.panId);
  fprintf(backup, "Pan ID: 0x%2x\n", restoreData.panId);
  fprintf(backup, "Radio Channel: %d\n", restoreData.radioChannel);
  fprintf(backup, "Radio Power: %d\n", restoreData.radioPower);
  fprintf(backup, "Extended Pan Id: %X%X%X%X%X%X%X%X\n",
          restoreData.ePanId[7],
          restoreData.ePanId[6],
          restoreData.ePanId[5],
          restoreData.ePanId[4],
          restoreData.ePanId[3],
          restoreData.ePanId[2],
          restoreData.ePanId[1],
          restoreData.ePanId[0]);

  fclose(backup);
  return EMBER_SUCCESS;
}

EmberStatus saveCurrentNodeInformation(void)
{
  EmberStatus getStatus;
  EmberNodeType nodeType;
  EmberNetworkParameters params;
  getStatus = emberAfGetNetworkParameters(&nodeType, &params);
  if (getStatus == EMBER_SUCCESS) {
    restoreData.panId = params.panId;
    restoreData.nodeId = (unsigned int)EMBER_TRUST_CENTER_NODE_ID;
    restoreData.radioChannel = params.radioChannel;
    restoreData.radioPower = params.radioTxPower;
    MEMCOPY(&(restoreData.ePanId), &(params.extendedPanId),
            sizeof(params.extendedPanId));
    return EMBER_SUCCESS;
  } else {
    emberAfAppPrintln("Failed to retrieve node information to save.");
    return EMBER_ERR_FATAL;
  }

}
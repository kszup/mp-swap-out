/***************************************************************************//**
 * @file
 * @brief CLI for the Swap Out plugin.
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

#include "app/framework/include/af.h"

#include "mpswapout.h"

#if defined(EMBER_AF_GENERATE_CLI) || defined(EMBER_AF_API_COMMAND_INTERPRETER2)

// -----------------------------------------------------------------------------
// CLI Command Definitions

// plugin mp-swap-out print all
void emberAfPluginMpSwapOutPrintAll(void)
{
  emberAfPluginMpSwapOutPrintNodeInfo();
  emberAfPluginMpSwapOutPrintSecurityInfo();
}

// plugin mp-swap-out print node-info
void emberAfPluginMpSwapOutPrintNodeInfo(void)
{
  emberAfAppPrintln("================= ");
  emberAfAppPrintln("Node Information: ");
  // Node information.
  emberAfAppPrint("Node ID:         %u\n", (unsigned int)EMBER_TRUST_CENTER_NODE_ID);
  EmberStatus getStatus;
  EmberNodeType nodeType;
  EmberNetworkParameters params;
  getStatus = emberAfGetNetworkParameters(&nodeType, &params);
  if (getStatus == EMBER_SUCCESS) {
    emberAfAppPrintln("Pan ID:          0x%2x", params.panId);
    emberAfAppPrintln("Radio Channel:   %d", params.radioChannel);
    emberAfAppPrintln("Radio Power:     %d", params.radioTxPower);
    emberAfAppPrintln("Extended Pan ID: %X%X%X%X%X%X%X%X",
                      params.extendedPanId[7],
                      params.extendedPanId[6],
                      params.extendedPanId[5],
                      params.extendedPanId[4],
                      params.extendedPanId[3],
                      params.extendedPanId[2],
                      params.extendedPanId[1],
                      params.extendedPanId[0]);
  } else {
    emberAfAppPrintln("Pan ID, ePan Id, radio channel/power not found. \
                       Error : 0x%x", getStatus);
  }
}

// plugin mp-swap-out print security-info 
void emberAfPluginMpSwapOutPrintSecurityInfo(void)
{
  emberAfAppPrintln("===No Security=== ");
}
// plugin mp-swap-out save node-info
void emberAfPluginMpSwapOutSaveNodeInfo(void)
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
    emberAfAppPrintln("===Saving the following===");
    emberAfAppPrintln("Node ID:         %d", restoreData.nodeId);
    emberAfAppPrintln("Pan ID:          0x%2x", restoreData.panId);
    emberAfAppPrintln("Radio Channel:   %d", restoreData.radioChannel);
    emberAfAppPrintln("Radio Power:     %d", restoreData.radioPower);
    emberAfAppPrintln("Extended Pan Id: %X%X%X%X%X%X%X%X\n",
                    restoreData.ePanId[7],
                    restoreData.ePanId[6],
                    restoreData.ePanId[5],
                    restoreData.ePanId[4],
                    restoreData.ePanId[3],
                    restoreData.ePanId[2],
                    restoreData.ePanId[1],
                    restoreData.ePanId[0]);
    saveTcBackupToFile();
  } else {
    emberAfAppPrintln("Failed to retrieve node information to save.");
  }
}

// plugin mp-swap-out save security-info
void emberAfPluginMpSwapOutSaveSecurityInfo(void)
{
  emberAfAppPrintln("===No Security=== ");
}

// plugin mp-swap-out read backup
void emberAfPluginMpSwapOutReadBackup(void)
{
  emberAfAppPrintln("===Incomplete==== ");
}

// plugin mp-swap-out restore node-info
void emberAfPluginMpSwapOutRestoreNodeInfo(void)
{
  emberAfAppPrintln("Restoring backup from file.");
  loadRestoreDataFromFile();
  emberAfAppPrintln("Attempting to restore trust center from backup.");
  restoreTcFromBackup();
}

// plugin mp-swap-out restore security-info
void emberAfPluginMpSwapOutRestoreSecurityInfo(void)
{
  emberAfAppPrintln("===No Security=== ");
}

#endif /*
          defined(EMBER_AF_GENERATE_CLI)
        || defined(EMBER_AF_API_COMMAND_INTERPRETER2)
        */

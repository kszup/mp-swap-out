/***************************************************************************//**
 * @file
 * @brief Definitions for the Swap Out plugin.
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

/**************************************************************************//**
* This code is provided as-is.

* Header file for Host/NCP multi-PAN swap out application.
******************************************************************************/
#ifndef MP_SWAP_OUT_CALLBACKS_H
  #define MP_SWAP_OUT_CALLBACKS_H

// Comment and uncomment this line to enable and disable DEBUG functionality.
//#define DEBUG

/// Maintain all necessary TC data in a struct for straightforward access.
typedef struct {
  int8u nodeId;            /// The ID of the trust center node.
  int16u panId;            /// 16-bit PAN ID for the network.
  int8s radioPower;        /// The network's operating radio power.
  int8u radioChannel;      /// The network's operating radio channel.
  int8u ePanId[8];         /// 64-bit extended PAN ID for the network.
  //EmberEUI64 eui64;        /// EUI64 Manufacturing ID for the TC device.
  //EmberKeyData globalKey;  /// Global key: value stored in globalKey.contents.
  //int32u globalFC;         /// The frame counter for the global key.
  //EmberKeyData networkKey; /// Network key: value stored in networkKey.contents.
  //int32u networkFC;        /// The frame counter for the network key.
  //int8u networkKeySeqNum;  /// The sequence number for the network key.
} TcRestoreData_t;

extern TcRestoreData_t restoreData;
// plugin mp-swap-out print all
void emberAfPluginMpSwapOutPrintAll(void);

// plugin mp-swap-out print node-info
void emberAfPluginMpSwapOutPrintNodeInfo(void);

// plugin mp-swap-out print security-info 
void emberAfPluginMpSwapOutPrintSecurityInfo(void);

// plugin mp-swap-out save node-info
void emberAfPluginMpSwapOutSaveNodeInfo(void);

// plugin mp-swap-out save security-info
void emberAfPluginMpSwapOutSaveSecurityInfo(void);

// plugin mp-swap-out read backup
void emberAfPluginMpSwapOutReadBackup(void);

// plugin mp-swap-out restore node-info
void emberAfPluginMpSwapOutRestoreNodeInfo(void);

// plugin mp-swap-out restore security-info
void emberAfPluginMpSwapOutRestoreSecurityInfo(void);

EmberStatus loadRestoreDataFromFile(void);
EmberStatus restoreTcFromBackup(void);
EmberStatus clearTcBackupStoredFields(void);
EmberStatus saveTcBackupToFile(void);
EmberStatus saveCurrentNodeInformation(void);
#endif
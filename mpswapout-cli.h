/***************************************************************************//**
 * @file
 * @brief Gateway specific behavior for a host application.
 *  In this case we assume our application is running on
 *   a PC with Unix library support, connected to an NCP via serial uart.
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

extern EmberCommandEntry emberAfPluginMpSwapOutCommands[];

#if defined(MP_SWAP_OUT)
  #define EMBER_AF_PLUGIN_MP_SWAP_OUT_COMMANDS               \
  emberCommandEntrySubMenu("mp-swap-out",                    \
                           emberAfPluginMpSwapOutCommands, \
                           "Commands for restoring network parameters."),

#else
  #define EMBER_AF_PLUGIN_MP_SWAP_OUT_COMMANDS

#endif

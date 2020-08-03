# mp-swap-out

This plugin has a set of CLI calls to help with swapping out network parameters of a particular network index in a Multi-PAN capable device. It is assumed that the user understands how to build a host application and NCP image. Silicon Labs has many User Guides, Application Notes, sample apps, and knowledge base articles on the topic.

## Motivation

When upgrading a host+NCP device to use the Multi-PAN feature offered by Silicon Labs there is a use case to shift the network to a secondary index.  

## Dependencies

-  EmberZNet 6.8.x +
-- Multi-PAN library
-- Multi-Network Library
- Readline
- ncurses

## Overview
Currently, this plugin is not fully featured for any network swap out. It is focused on a network that uses Zigbee Security Profile 0 (no-security). Although, there has been room left to add additional functionality. The commands can print, save, and backup network data to a file (backup/backup.txt).

### Use
The use case here will be to move a network PAN to the secondary network index after an upgrade.

#### Setup
1. Copy the contents of this repo to the plugin directory of the EmberZNet SDK (protocol/zigbee/app/framework/plugin-host/mp-swap-out) and restart Simplicity Studio so that AppBuilder can recognize the new plugin and populate the Plugins tab.
2. Add the plugin to the host project through AppBuilder and build. The NCP does not require any changes.

#### Example procedure
The CLI is documented although not very verbosely so an example is provided. This example, is focused on an old custom network that does not use available securit from the zigbee protocol. It is assumed that the host is based on a Linux system. Let's call the original host+NCP system "ZC" (short for zigbee coorindator) and the new system "UpgradedZC". The network should have a few devices connected to it to make sure they can communicate with the coordinator after the upgrade and swap-out. The example will backup the network data to a file on the ZC system and then restore that data to Network Index 1 on UpgradedZC system.

```
ZC> plugin mp-swap-out print node-info

================= 
Node Information: 
Node ID:         0
Pan ID:          0xBEEF
Radio Channel:   11
Radio Power:     1
Extended Pan ID: 819E7A854F19DF62

ZC> plugin mp-swap-out save node-info

===Saving the following===
Node ID:         0
Pan ID:          0xBEEF
Radio Channel:   11
Radio Power:     1
Extended Pan Id: 819E7A854F19DF62

ZC> network leave
```

UPGRADE THE HOST AND NCP TO NEW APPLICATION AND NCP with the Multi-PAN feature. Switch t

```
UpgradedZC> network set 1

UpgradedZC> plugin mp-swap-out restore node-info

Restoring backup from file.
Attempting to restore trust center from backup.
Setting node information and network parameters.
Forming on ch 11, panId 0xBEEF
Successfully formed network with given params.
EMBER_NETWORK_UP 0x0000

```

## References
- https://www.silabs.com/community/wireless/zigbee-and-thread/knowledge-base.entry.html/2018/08/07/trust_center_replace-0wp0

#include "actionbluetooth.h"

ActionBlueTooth::ActionBlueTooth()
{
}

void
ActionBlueTooth::activate(const Rule &rule) {
    qDebug("ActionBlueTooth::activate: %d", rule.getBlueToothMode());
    // Quick hack to get compile on Desktop. Problem fixed in qt-mobility master
#ifdef __arm__
    switch (rule.getBlueToothMode()) {
    case 0: _bt.setHostMode(QBluetoothLocalDevice::HostPoweredOff); break;
    case 1: _bt.setHostMode(QBluetoothLocalDevice::HostConnectable); break;
    // This doesn't seem to turn BlueTooth visibility on in device
    case 2: _bt.setHostMode(QBluetoothLocalDevice::HostDiscoverable); break;
    }
#endif
}

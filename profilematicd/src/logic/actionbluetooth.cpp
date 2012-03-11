#include "actionbluetooth.h"

ActionBlueTooth::ActionBlueTooth()
    : _previousMode(-1)
{
}

void
ActionBlueTooth::activate(const Rule &rule) {
    qDebug("ActionBlueTooth::activate: %d", rule.getBlueToothMode());
    // Quick hack to get compile on Desktop. Problem fixed in qt-mobility master
#ifdef __arm__

    int blueToothMode = rule.getBlueToothMode();

    if ((blueToothMode < 0 || rule.isDefaultRule()) && _previousMode >= 0) {
        qDebug("ActionBlueTooth::activate restore, blueTooth not set or is default rule for rule %s",
               qPrintable(rule.getRuleName()));
        qDebug("ActionBlueTooth::activate previous rule had restore, restoring %d",
               _previousMode);
        blueToothMode = _previousMode;
        _previousMode = -1;
    }
    else if (blueToothMode < 0) {
        qDebug("ActionBlueTooth::activate not setting for rule %s",
               qPrintable(rule.getRuleName()));
        return;
    }

    if (rule.getRestoreBlueToothMode()) {
        switch (_bt.hostMode()) {
        case QBluetoothLocalDevice::HostPoweredOff:
            _previousMode = 0; break;
        case QBluetoothLocalDevice::HostConnectable:
            _previousMode = 1; break;
        case QBluetoothLocalDevice::HostDiscoverable:
            _previousMode = 2; break;
        default:
            qWarning("ActionBlueTooth::activate unrecognized getHostMode() %d", _bt.hostMode());
            _previousMode = -1;
        }
    } else {
        _previousMode = -1;
    }

    switch (blueToothMode) {
    case 0: _bt.setHostMode(QBluetoothLocalDevice::HostPoweredOff); break;
    case 1: _bt.setHostMode(QBluetoothLocalDevice::HostConnectable); break;
    case 2: _bt.setHostMode(QBluetoothLocalDevice::HostDiscoverable); break;
    }
#endif
}

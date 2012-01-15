#include "harmattan_platformutil.h"

#include <qmdevicemode.h>

HarmattanPlatformUtil::HarmattanPlatformUtil(QObject *parent)
    : PlatformUtil(parent)
{
}

HarmattanPlatformUtil::~HarmattanPlatformUtil() {}

int
HarmattanPlatformUtil::cellId() {
    return _networkInfo.cellId();
}

void
HarmattanPlatformUtil::monitorCellId(bool monitor) {
    qDebug("HarmattanPlatformUtil::monitorCellId(%d)", monitor);
    if (monitor) {
        connect(&_networkInfo, SIGNAL(cellIdChanged(int)), this, SIGNAL(cellIdChanged(int)), Qt::UniqueConnection);
    } else {
        disconnect(&_networkInfo, SIGNAL(cellIdChanged(int)), this, SIGNAL(cellIdChanged(int)));
    }
}

void
HarmattanPlatformUtil::setFlightMode(int flightMode) {
  MeeGo::QmDeviceMode deviceMode;

  qDebug("HarmattanPlatformUtil::Setting flightMode to %d", flightMode);
  if (flightMode == 0 || flightMode == 1) {
      bool success = deviceMode.setMode(flightMode == 1
                         ? MeeGo::QmDeviceMode::Flight
                         : MeeGo::QmDeviceMode::Normal);

      qDebug("HarmattanPlatformUtil::Setting flightMode success %d", success);
  }
}

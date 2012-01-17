#include "harmattan_platformutil.h"

#include <qmdevicemode.h>

HarmattanPlatformUtil::HarmattanPlatformUtil(QObject *parent)
    : PlatformUtil(parent)
{
}

HarmattanPlatformUtil::~HarmattanPlatformUtil() {}

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

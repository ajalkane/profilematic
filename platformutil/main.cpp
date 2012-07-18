#include <cstdlib>
#include <cstdio>

#include <QCoreApplication>
#include <QProcess>
#include <qmdevicemode.h>

int setFlightMode(bool on) {
    MeeGo::QmDeviceMode deviceMode;

    bool returnValue = deviceMode.setMode(on
                                      ? MeeGo::QmDeviceMode::Flight
                                      : MeeGo::QmDeviceMode::Normal);

    qDebug("platformutil::Setting flightMode return_value %d", returnValue);
    return returnValue ? 0 : 1;
}

int hasDeviceModeCredential() {
    int exitCode = QProcess::execute("/usr/bin/accli -t mce::DeviceModeControl");
    qDebug("platformutil::hasDeviceModeCredential return value %d", exitCode);
    return exitCode;
}

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc < 2) {
        puts("Insufficient arguments");
    }

    char *command = argv[1];

    if (strcmp("flightModeOn", command) == 0) {
        return setFlightMode(true);
    } else if (strcmp("flightModeOff", command) == 0) {
        return setFlightMode(false);
    } else if (strcmp("hasDeviceModeCredential", command) == 0) {
        return hasDeviceModeCredential();
    } else {
        puts("Unrecognized argument");
        return 1;
    }

    return 0;
}


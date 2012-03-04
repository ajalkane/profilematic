#include <QProcess>

#include "actioncommandline.h"

ActionCommandLine::ActionCommandLine(PlatformUtil *platformUtil)
    : _platformUtil(platformUtil)
{
}

void
ActionCommandLine::activate(const Rule &rule) {
    if (!rule.getCommandLine().isEmpty()) {
        qDebug("ActionCommandLine::activate() executing %s", qPrintable(rule.getCommandLine()));
        bool success = QProcess::startDetached(rule.getCommandLine());
        qDebug("ActionCommandLine::success %d", success);
        if (!success) {
            _platformUtil->publishNotification("Custom action failed for " + rule.getRuleName());
        }

    } else {
        qDebug("ActionCommandLine::activate() no command line");
    }
}

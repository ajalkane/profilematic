#include <QProcess>

#include "actioncommandline.h"

ActionCommandLine::ActionCommandLine(PlatformUtil *platformUtil)
    : _platformUtil(platformUtil)
{
}

bool
ActionCommandLine::activateOnce(const Rule::IdType &, const RuleAction &rule) {
    if (!rule.getCommandLine().isEmpty()) {
        qDebug("ActionCommandLine::activate() executing %s", qPrintable(rule.getCommandLine()));
        bool success = QProcess::startDetached(rule.getCommandLine());
        qDebug("ActionCommandLine::success %d", success);
        if (!success) {
            // TODO platformUtil to singleton
            // _platformUtil->publishNotification("Custom action failed");
        }
        return true;
    } else {
        qDebug("ActionCommandLine::activate() no command line");
    }
    return false;
}

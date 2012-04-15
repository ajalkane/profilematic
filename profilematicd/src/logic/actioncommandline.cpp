#include <QProcess>

#include "actioncommandline.h"

ActionCommandLine::ActionCommandLine()
{
}

bool
ActionCommandLine::runCommandLine(const QString &commandLine) const {
    if (!commandLine.isEmpty()) {
        qDebug("ActionCommandLine::runCommandLine() executing %s", qPrintable(commandLine));
        bool success = QProcess::startDetached(commandLine);
        qDebug("ActionCommandLine::success %d", success);
        if (!success) {
            PlatformUtil::instance()->publishNotification("Custom action failed");
        }
        return true;
    } else {
        qDebug("ActionCommandLine::runCommandLine() no command line");
    }
    return false;
}

bool
ActionCommandLine::activateOnce(const Rule::IdType &, const RuleAction &rule) {
    runCommandLine(rule.getCommandLine());
    return !rule.getCommandLineExit().isEmpty();
}

void
ActionCommandLine::activateOnceExit(const Rule::IdType &, const RuleAction &rule) {
    runCommandLine(rule.getCommandLineExit());
}

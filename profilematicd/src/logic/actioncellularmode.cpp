#include "actioncellularmode.h"

ActionCellularMode::ActionCellularMode(PlatformUtil *platformUtil)
    : _platformUtil(platformUtil)
{
}

void
ActionCellularMode::activate(const RuleAction &rule) {
    int cellularMode = rule.getCellularMode();
    qDebug("ActionCellularMode::activate: %d", cellularMode);
    // Quick hack to get compile on Desktop. Problem fixed in qt-mobility master
    if (cellularMode > -1) {
        _platformUtil->setCellularMode(rule.getCellularMode());
    }
}

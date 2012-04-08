#include "actioncellularmode.h"

ActionCellularMode::ActionCellularMode()
{
}

bool
ActionCellularMode::activateDifferent(const Rule::IdType &, const RuleAction &rule) {
    int cellularMode = rule.getCellularMode();
    qDebug("ActionCellularMode::activate: %d", cellularMode);
    // Quick hack to get compile on Desktop. Problem fixed in qt-mobility master
    if (cellularMode > -1) {
        PlatformUtil::instance()->setCellularMode(rule.getCellularMode());
        return true;
    }
    return false;
}

#include "actioncellularmode.h"

ActionCellularMode::ActionCellularMode()
    : _previousCellularMode(-1)
{
}

bool
ActionCellularMode::activateDifferent(const Rule::IdType &ruleId, const RuleAction &rule) {
    int cellularMode = rule.getCellularMode();
    qDebug("ActionCellularMode::activate: %d", cellularMode);
    bool activated = true;

    if (useRestoreAction(ruleId, cellularMode >= 0, _previousCellularMode >= 0)) {
        qDebug("ActionCellularMode::activate restore");
        qDebug("ActionCellularMode::activate previous rule had restore cellularMode, restoring cellularMode %d",
               _previousCellularMode);
        cellularMode = _previousCellularMode;
        _previousCellularMode = -1;
        // Restore is not returned as activation
        activated = false;
    }
    else if (cellularMode < 0) {
        qDebug("ActionCellularMode::activate not setting flight mode");
        return false;
    }

    if (rule.getRestoreCellularMode()) {
        _previousCellularMode = PlatformUtil::instance()->cellularMode();
    } else {
        _previousCellularMode = -1;
    }

    PlatformUtil::instance()->setCellularMode(cellularMode);

    return activated;
}

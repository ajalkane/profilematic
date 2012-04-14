#include <limits.h>
#include "conditionmanageridle.h"

#define IDLE_MODE_UNKNOWN -1
#define IDLE_MODE_NOT 0
#define IDLE_MODE 1

ConditionManagerIdle::ConditionManagerIdle(QObject *parent)
    : ConditionManager(parent),
      _currentMinIdleSecs(-1),
      _nextMinWakeupSecs(-1),
      _currentIdleMode(IDLE_MODE_UNKNOWN),
    _hasActive(false)
{
    _timer.setSingleShot(true);
    connect(PlatformUtil::instance(), SIGNAL(userActivityIdleChanged(bool)), this, SLOT(userActivityIdleChanged(bool)));
    connect(&_timer, SIGNAL(timeout()), this, SIGNAL(refreshNeeded()));

    qDebug("ConditionManagerIdle::ConditionManagerIdle current isIdle %d", PlatformUtil::instance()->isUserActivityIdle());
}

ConditionManagerIdle::~ConditionManagerIdle() {
}

void
ConditionManagerIdle::startRefresh() {
    _currentMinIdleSecs = INT_MAX;
    _nextMinWakeupSecs = INT_MAX;
    _refreshTime = QDateTime::currentDateTime();
    _timer.stop();
    _hasActive = false;
}

bool
ConditionManagerIdle::refresh(const RuleCondition &condition) {
    int idleForSecs = condition.getIdleForSecs();
    if (idleForSecs < 0) {
        qDebug("ConditionManagerIdle not set %d", idleForSecs);
        return true;
    }

    _currentMinIdleSecs = qMin(_currentMinIdleSecs, idleForSecs);

    if (_currentIdleMode == IDLE_MODE_UNKNOWN) {
        _currentIdleMode = PlatformUtil::instance()->isUserActivityIdle();
    }

    if (_currentIdleMode == IDLE_MODE_NOT) {
        qDebug("ConditionManagerIdle idle mode not active");
        return false;
    }

    int secsAfterIdle = _idleStartTime.secsTo(_refreshTime);
    int secsToActivation = idleForSecs - secsAfterIdle;
    if (secsToActivation <= 0) {
        qDebug("ConditionManagerIdle::refresh matched, idleForSecs-secsAfterIdle=%d", secsToActivation);
        _hasActive = true;
        return true;
    } else {
        _nextMinWakeupSecs = qMin(_nextMinWakeupSecs, secsToActivation);
    }
    return false;
}

void
ConditionManagerIdle::endRefresh() {
    if (_nextMinWakeupSecs != INT_MAX) {
        _scheduleWakeup(_nextMinWakeupSecs);
    }

    if (_currentMinIdleSecs != INT_MAX) {
        qDebug("ConditionManagerIdle monitorUserActivityIdle");
        PlatformUtil::instance()->monitorUserActivityIdle(true);
    } else {
        qDebug("ConditionManagerIdle not monitorUserActivityIdle");
        PlatformUtil::instance()->monitorUserActivityIdle(false);
        _currentIdleMode = IDLE_MODE_UNKNOWN;
    }
}

void
ConditionManagerIdle::_scheduleWakeup(int secsToWakeUp) {
    int secsToWakeUpEnd = secsToWakeUp + (secsToWakeUp < 120 ? 1 : 58);
    _timer.start(secsToWakeUp, secsToWakeUpEnd);
    qDebug("%s ConditionManagerIdle::_scheduleWakeup startTimer to %d-%d secs",
           qPrintable(_idleStartTime.toString()), secsToWakeUp, secsToWakeUpEnd);
}

void
ConditionManagerIdle::userActivityIdleChanged(bool isIdle)
{
    qDebug("ConditionManagerIdle::userActivityIdleChanged %d", isIdle);
    if (isIdle && _currentMinIdleSecs >= 0) {
        _idleStartTime = QDateTime::currentDateTime();
        _currentIdleMode = IDLE_MODE;
        _scheduleWakeup(_currentMinIdleSecs);

    } else if (!isIdle) {
        _timer.stop();
        _idleStartTime = QDateTime();
        _currentIdleMode = IDLE_MODE_NOT;
        if (_hasActive) {
            qDebug("%s ConditionManagerIdle at least one rule was active due to idle, requesting refresh", qPrintable(_refreshTime.toString()));
            emit refreshNeeded();
        }
    }
}

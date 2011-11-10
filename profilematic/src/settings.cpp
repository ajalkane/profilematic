/**********************************************************************
 * Copyright 2011 Arto Jalkanen
 *
 * This file is part of ProfileMatic.
 *
 * ProfileMatic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ProfileMatic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ProfileMatic.  If not, see <http://www.gnu.org/licenses/>
**/
#include <QSettings>

#include "settings.h"

Settings::Settings()
{
}

void
Settings::write(const Rules &rules) {
    qDebug("Settings::write()");

    // IMPROVE constants.h
    // So wrong by the API specifications, but so right by the end results (no, I don't like doing it this way)
    QSettings s("ProfileMatic", "rules");
    s.clear();

    s.beginWriteArray("rules", rules.size());
    for (int i = 0; i < rules.size(); ++i) {
        s.setArrayIndex(i);

        const RuleItem &r = rules.at(i);
        s.setValue("ruleActive", r.ruleActive);

        s.setValue("ruleId", r.rule.getRuleId());
        s.setValue("ruleName", r.rule.getRuleName());
        _writeIntList(s, "days", "dayId", r.rule.getDays().toList());
        s.setValue("timeStart", r.rule.getTimeStart().toString());
        s.setValue("profile", r.rule.getProfile());
        s.setValue("profileVolume", r.rule.getProfileVolume());
    }
    s.endArray();
}

void
Settings::read(Rules &rules) {
    // IMPROVE constants.h
    // So wrong by the API specifications, but so right by the end results (no, I don't like doing it this way)
    QSettings s("ProfileMatic", "rules");
    // QSettings s("ajalkane", "ProfileMatic");
    int size = s.beginReadArray("rules");
    for (int i = 0; i < size; ++i) {
        s.setArrayIndex(i);

        RuleItem r;
        r.ruleActive = s.value("ruleActive").toBool();

        r.rule.setRuleId(s.value("ruleId").toInt());
        r.rule.setRuleName(s.value("ruleName").toString());
        QList<int> daysList;
        _readIntList(s, "days", "dayId", daysList);
        r.rule.setDays(QSet<int>::fromList(daysList));
        QString timeStartStr = s.value("timeStart").toString();
        if (!timeStartStr.isEmpty()) {
            QTime time = QTime::fromString(timeStartStr);
            r.rule.setTimeStart(time);
        }
        r.rule.setProfile(s.value("profile").toString());

        bool profileVolumeOk = false;
        int profileVolume = s.value("profileVolume").toInt(&profileVolumeOk);
        if (profileVolumeOk) {
            r.rule.setProfileVolume(profileVolume);
        }
        rules << r;
    }
    s.endArray();
}

void
Settings::_writeIntList(QSettings &s, const QString &prefix, const QString &key, const QList<int> &values) {
    qDebug("QSettings:__writeIntList prefix %s, key %s, size %d", qPrintable(prefix), qPrintable(key), values.size());
    s.beginWriteArray(prefix, values.size());
    for (int i = 0; i < values.size(); ++i) {
        qDebug("Writing prefix %s, key %s for index %d, size %d", qPrintable(prefix), qPrintable(key), i, values.size());
        s.setArrayIndex(i);
        s.setValue(key, values.at(i));
    }
    s.endArray();
}

void
Settings::_readIntList(QSettings &s, const QString &prefix, const QString &key, QList<int> &values) {
    int size = s.beginReadArray(prefix);
    for (int i = 0; i < size; ++i) {
        s.setArrayIndex(i);
        bool ok = false;
        QString valueStr = s.value(key).toString();
        int value = valueStr.toInt(&ok);
        if (ok) {
            values << value;
        } else {
            qDebug("Settings: invalid setting (not an int) at %s/%d/%s: %s",
                   qPrintable(prefix), i, qPrintable(key), qPrintable(valueStr));
        }
    }
    s.endArray();
}

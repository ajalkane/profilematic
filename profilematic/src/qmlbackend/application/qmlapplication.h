#ifndef QMLAPPLICATION_H
#define QMLAPPLICATION_H

#include <QString>

class QmlApplication {
    QString _applicationLauncher;
    QString _applicationName;
    QString _iconUri;

public:
    QmlApplication();
    QmlApplication(const QmlApplication &o);
    QmlApplication &operator=(const QmlApplication &o);

    inline const QString& applicationLauncher() const { return _applicationLauncher; }
    inline const QString& applicationName() const { return _applicationName; }
    inline const QString& iconUri() const { return _iconUri; }

    inline void setApplicationLauncher(const QString &applicationLauncher) { _applicationLauncher = applicationLauncher; }
    inline void setApplicationName(const QString &applicationName) { _applicationName = applicationName; }
    inline void setIconUri(const QString &iconUri) { _iconUri = iconUri; }

    inline bool isUsable() const {
        return !_applicationLauncher.isEmpty()
            && !_applicationName.isEmpty();
    }
};

#endif // QMLAPPLICATION_H

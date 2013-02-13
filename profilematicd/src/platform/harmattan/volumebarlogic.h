/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#ifndef VOLUMEBARLOGIC_H
#define VOLUMEBARLOGIC_H

#include <QObject>
#include <dbus/dbus.h>

#if (HAVE_LIBRESOURCEQT && HAVE_QMSYSTEM)
#include <policy/resource-set.h>
#include <qmkeys.h>
#endif

//! Provides informations and actions on PulseAudio MainVolume API
class VolumeBarLogic : public QObject
{
Q_OBJECT

public:
    //! Construct a VolumeBarLogic instance
    VolumeBarLogic (QObject *parent = NULL);
    //! Destructs a VolumeBarLogic instance
    virtual ~VolumeBarLogic ();

    //! get the current volume-level value
    quint32 volume ();
    //! get the current volume-level stepcount
    quint32 maxVolume ();

public slots:
    /*! a method for change the volume-level
     * \param value The desired volume-level (valid range: [0 - maxvalue-1])
     */
    void setVolume (quint32 value);
    /*! an internal slot whats stores the actual volume values and emits the volumeChanged signal
     *  [this is public because it called from a C callback function...]
     * \param value The new volume-level what we got from PulseAudio
     * \param maxvalue The new stepcount what we got from PulseAudio
     */
    void stepsUpdated (quint32 value, quint32 maxvalue);

private slots:
    //! an internal method which queries the actual values from PulseAudio
    void initValues ();

#if (HAVE_LIBRESOURCEQT && HAVE_QMSYSTEM)
    /*!
     * Internal slot to handle the hardware volume-key presses (see QmKeys API documentation)
     *
     * \param key the key type
     * \param state the key new state
     */
    void hwKeyEvent (MeeGo::QmKeys::Key key, MeeGo::QmKeys::State state);

    //! An internal slot to handle the case when we got the hardware volume keys resource
    void hwKeyResourceAcquired();

    //! An internal slot to handle the case when we lost the hardware volume keys resource
    void hwKeyResourceLost();
#endif

private:
    Q_DISABLE_COPY(VolumeBarLogic)

    /*! an internal method which registers a signal handler to
     * listen for PulseAudio MainVolume1 StepsUpdated signal
     */
    void addSignalMatch ();

    /*! opens a private D-Bus connection for PulseAudio daemon, and optionally
     * calls initValues () after successfully connection.
     * \param init An option where can be enabled the initialization.
     */
    void openConnection (bool init = false);

    //! a method for ensure the connection to PulseAudio daemon
    void ping ();

    /*! the signal handler for PulseAudios MainVolume1 signal
     * \param conn The D-Bus connection structure
     * \param message The signal message
     * \param logic The VolumeBarLogic instance who is handling this signal
     */
    static void stepsUpdatedSignal (DBusConnection *conn, DBusMessage *message, VolumeBarLogic *logic);

    //! A member for accessing D-Bus connection structure
    DBusConnection  *dbus_conn;

    //! The current volume level (< currentmax)
    quint32     currentvolume;
    //! The stepcount of volume
    quint32     currentmax;

#if (HAVE_LIBRESOURCEQT && HAVE_QMSYSTEM)
    //! an QmKeys instance to get signals about volume-key presses
    MeeGo::QmKeys *hwkeys;

    //! A resource object for volume(zoom)-hardware keys
    ResourcePolicy::ResourceSet *hwkeyResource;
#endif

#ifdef UNIT_TEST
friend class Ut_VolumeBarLogic;
#endif
};

#endif


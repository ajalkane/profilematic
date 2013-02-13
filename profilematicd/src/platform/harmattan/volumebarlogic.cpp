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
#include "volumebarlogic.h"

#include <cstdlib>
#include <cstring>
#include <dbus/dbus-glib-lowlevel.h>

#include <QTimer>
#include <QVariant>
#include <QString>
#include <QDebug>

#define DBUS_ERR_CHECK(err) \
    if (dbus_error_is_set (&err)) \
    { \
        qWarning() << err.message; \
        dbus_error_free (&err); \
    }

#define DEFAULT_ADDRESS "unix:path=/var/run/pulse/dbus-socket"

#define VOLUME_SV    "com.Nokia.MainVolume1"
#define VOLUME_PATH  "/com/meego/mainvolume1"
#define VOLUME_IF    "com.Nokia.MainVolume1"

VolumeBarLogic::VolumeBarLogic(QObject *parent) :
    QObject(parent),
    dbus_conn (NULL),
    currentvolume (0),
    currentmax (0)
#if (HAVE_LIBRESOURCEQT && HAVE_QMSYSTEM)
    ,hwkeys(new MeeGo::QmKeys(this))
#endif
{
    openConnection (true);

#if (HAVE_LIBRESOURCEQT && HAVE_QMSYSTEM)
    hwkeyResource = new ResourcePolicy::ResourceSet("event");
    hwkeyResource->setAlwaysReply();

    ResourcePolicy::ScaleButtonResource *volumeKeys = new ResourcePolicy::ScaleButtonResource;
    hwkeyResource->addResourceObject(volumeKeys);
    connect(hwkeyResource, SIGNAL(resourcesGranted(QList<ResourcePolicy::ResourceType>)), this, SLOT(hwKeyResourceAcquired()));
    connect(hwkeyResource, SIGNAL(lostResources()), this, SLOT(hwKeyResourceLost()));

    hwkeyResource->acquire();
#endif
}

VolumeBarLogic::~VolumeBarLogic ()
{
    if (dbus_conn) {
        dbus_connection_unref (dbus_conn);
    }

#if (HAVE_LIBRESOURCEQT && HAVE_QMSYSTEM)
    hwkeyResource->deleteResource(ResourcePolicy::ScaleButtonType);
#endif
}

void VolumeBarLogic::openConnection (bool init)
{
    /*
     * Check the connection first, maybe this function
     * only called because of lost connection
     */
    if ((dbus_conn != NULL) && (dbus_connection_get_is_connected (dbus_conn)))
        return;

    DBusError dbus_err;
    char *pa_bus_address = getenv ("PULSE_DBUS_SERVER");

    if (pa_bus_address == NULL)
        pa_bus_address = (char *) DEFAULT_ADDRESS;

    dbus_error_init (&dbus_err);

    dbus_conn = dbus_connection_open (pa_bus_address, &dbus_err);

    DBUS_ERR_CHECK (dbus_err);

    if (dbus_conn != NULL) {
        dbus_connection_setup_with_g_main (dbus_conn, NULL);

        dbus_connection_add_filter (
            dbus_conn,
            (DBusHandleMessageFunction) VolumeBarLogic::stepsUpdatedSignal,
            (void *) this, NULL);

        if (init == true)
            initValues ();
    }
}

void VolumeBarLogic::initValues ()
{
    DBusMessage *msg;
    DBusMessage *reply;
    DBusError    error;

    dbus_error_init (&error);

    msg = dbus_message_new_method_call (VOLUME_SV,
                                        VOLUME_PATH,
                                        "org.freedesktop.DBus.Properties",
                                        "GetAll");
    const char *volume_if = VOLUME_IF;
    dbus_message_append_args (msg,
                              DBUS_TYPE_STRING, &volume_if,
                              DBUS_TYPE_INVALID);

    reply =
        dbus_connection_send_with_reply_and_block (
                        dbus_conn, msg, -1, &error);

    DBUS_ERR_CHECK (error);

    dbus_message_unref (msg);

    if (reply && (dbus_message_get_type (reply) ==
                  DBUS_MESSAGE_TYPE_METHOD_RETURN)) {
        DBusMessageIter iter;
        dbus_message_iter_init (reply, &iter);
        // Recurse into the array [array of dicts]
        while (dbus_message_iter_get_arg_type (&iter) != DBUS_TYPE_INVALID) {
            DBusMessageIter dict_entry;
            dbus_message_iter_recurse (&iter, &dict_entry);

            // Recurse into the dict [ dict_entry (string, variant(int)) ]
            while (dbus_message_iter_get_arg_type (&dict_entry) != DBUS_TYPE_INVALID) {
                DBusMessageIter in_dict;
                // Recurse into the dict_entry [ string, variant(int) ]
                dbus_message_iter_recurse (&dict_entry, &in_dict);
                {
                  char *prop_name = NULL;
                  // Get the string value, "property name"
                  dbus_message_iter_get_basic (&in_dict, &prop_name);

                  dbus_message_iter_next (&in_dict);

                  DBusMessageIter variant;
                  // Recurese into the variant [ variant(int) ]
                  dbus_message_iter_recurse (&in_dict, &variant);

                  quint32 value;
                  // Get the variant value which is uint32
                  dbus_message_iter_get_basic (&variant, &value);

                  if (prop_name &&
                      strcmp (prop_name, "StepCount") == 0)
                    currentmax = value;
                  else if (prop_name &&
                           strcmp (prop_name, "CurrentStep") == 0)
                    currentvolume = value;
                }

                dbus_message_iter_next (&dict_entry);
            }
            dbus_message_iter_next (&iter);
        }
    }

    if (reply)
        dbus_message_unref (reply);

    addSignalMatch ();
}

void VolumeBarLogic::addSignalMatch ()
{
    DBusMessage     *message = NULL;
    char            *signal = (char *) "com.Nokia.MainVolume1.StepsUpdated";
    char           **emptyarray = { NULL };

    message = dbus_message_new_method_call (NULL,
                                            "/org/pulseaudio/core1",
                                            NULL,
                                            "ListenForSignal");

    if (message != NULL) {
        dbus_message_append_args (message,
                                  DBUS_TYPE_STRING, &signal,
                                  DBUS_TYPE_ARRAY, DBUS_TYPE_OBJECT_PATH, &emptyarray, 0,
                                  DBUS_TYPE_INVALID);

        dbus_connection_send (dbus_conn, message, NULL);
    } else {
        qWarning() << "Cannot listen for PulseAudio signals [out of memory]";
    }

    if (message)
        dbus_message_unref (message);
}

void VolumeBarLogic::stepsUpdatedSignal (
    DBusConnection *conn, DBusMessage *message, VolumeBarLogic *logic)
{
    Q_UNUSED (conn);

    if (message && dbus_message_has_member (message, "StepsUpdated")) {
        DBusError error;
        quint32   value = 0;
        quint32   maxvalue = 0;

        dbus_error_init (&error);

        if (dbus_message_get_args (message, &error,
                                   DBUS_TYPE_UINT32, &maxvalue,
                                   DBUS_TYPE_UINT32, &value,
                                   DBUS_TYPE_INVALID)) {
            logic->stepsUpdated (value, maxvalue);
        }

        DBUS_ERR_CHECK (error);
    }
}

void VolumeBarLogic::stepsUpdated (quint32 value, quint32 maxvalue)
{
    currentvolume = value;
    currentmax = maxvalue;
}

void VolumeBarLogic::setVolume (quint32 value)
{
    currentvolume = value;

    // Check the connection, maybe PulseAudio restarted meanwhile
    openConnection ();

    // Don't try to set the volume via d-bus when it isn't available
    if (dbus_conn == NULL)
        return;

    DBusMessage     *message;
    char            *volume_if = (char *) VOLUME_IF;
    char            *method    = (char *) "CurrentStep";

    message = dbus_message_new_method_call (VOLUME_SV,
                                            VOLUME_PATH,
                                            "org.freedesktop.DBus.Properties",
                                            "Set");

    if (message &&
        dbus_message_append_args (message,
                                  DBUS_TYPE_STRING, &volume_if,
                                  DBUS_TYPE_STRING, &method,
                                  DBUS_TYPE_INVALID)) {
        DBusMessageIter  append;
        DBusMessageIter  sub;

        // Create and append the variant argument ...
        dbus_message_iter_init_append (message, &append);

        dbus_message_iter_open_container (&append,
                                          DBUS_TYPE_VARIANT,
                                          DBUS_TYPE_UINT32_AS_STRING,
                                          &sub);
        // Set the variant argument value:
        dbus_message_iter_append_basic (&sub, DBUS_TYPE_UINT32, &value);
        // Close the append iterator
        dbus_message_iter_close_container (&append, &sub);

        // Send/flush the message immediately:
        dbus_connection_send (dbus_conn, message, NULL);
    } else {
        qWarning() << "Cannot set volume! [not enough memory]";
    }

    if (message)
        dbus_message_unref (message);
}

quint32 VolumeBarLogic::volume ()
{
    ping ();

    return currentvolume;
}

quint32 VolumeBarLogic::maxVolume ()
{
    ping ();

    return currentmax;
}

/*
 * This function should be called before we're showing the
 * current volume on the screen...
 *
 * Because sometimes we can lost the connection to PulseAudio,
 * and meanwhile PulseAudio volume-values changed, and we
 * didn't get any info about those...
 */
void VolumeBarLogic::ping ()
{
    if ((dbus_conn != NULL) &&
        (dbus_connection_get_is_connected (dbus_conn)))
        return;

    /*
     * Connection lost, re-query the values
     */
    openConnection (true);

    stepsUpdated (currentvolume, currentmax);
}

#if (HAVE_LIBRESOURCEQT && HAVE_QMSYSTEM)
void VolumeBarLogic::hwKeyEvent(MeeGo::QmKeys::Key key, MeeGo::QmKeys::State state)
{
    if (state == MeeGo::QmKeys::KeyUp) {
        // Do nothing on key releases
        return;
    }

    int volumeChange = 0;
    switch (key) {
    case MeeGo::QmKeys::VolumeUp:
        volumeChange = 1;
        break;
    case MeeGo::QmKeys::VolumeDown:
        volumeChange = -1;
        break;
    default:
        // no-op for other hw keys
        return;
    }

    int volume = currentvolume + volumeChange;

    // Keep the volume within limits
    if (volume >= (int)currentmax) {
        volume = currentmax - 1;
    } else if (volume < 0) {
        volume = 0;
    }

    // Take the new volume into use
    setVolume(volume);
}

void VolumeBarLogic::hwKeyResourceAcquired()
{
    // Disconnect from everything first
    hwkeys->disconnect ();

    connect(hwkeys, SIGNAL (keyEvent (MeeGo::QmKeys::Key, MeeGo::QmKeys::State)), this, SLOT (hwKeyEvent (MeeGo::QmKeys::Key, MeeGo::QmKeys::State)));
}

void VolumeBarLogic::hwKeyResourceLost()
{
    hwkeys->disconnect();
}
#endif

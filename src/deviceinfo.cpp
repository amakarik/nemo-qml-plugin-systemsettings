/*
 * Copyright (C) 2017 Jolla Ltd. <simo.piiroinen@jollamobile.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include "deviceinfo.h"

#include <QFile>
#include <QSet>

#include <ssusysinfo.h>

class DeviceInfoPrivate
{
public:
    DeviceInfoPrivate();
    ~DeviceInfoPrivate();

    QSet<DeviceInfo::Feature> m_features;
    QSet<Qt::Key> m_keys;
};

DeviceInfoPrivate::DeviceInfoPrivate()
{
    ssusysinfo_t *si = ssusysinfo_create();

    hw_feature_t *features = ssusysinfo_get_hw_features(si);
    if (features) {
        for (size_t i = 0; features[i]; ++i) {
            m_features.insert(static_cast<DeviceInfo::Feature>(features[i]));
        }
        free(features);
    }

    hw_key_t *keys = ssusysinfo_get_hw_keys(si);
    if (keys) {
        for (size_t i = 0; keys[i]; ++i) {
            m_keys.insert(static_cast<Qt::Key>(keys[i]));
        }
        free(keys);
    }

    ssusysinfo_delete(si);
}

DeviceInfoPrivate::~DeviceInfoPrivate()
{
}

DeviceInfo::DeviceInfo(QObject *parent)
    : QObject(parent)
    , d_ptr(new DeviceInfoPrivate())
{
}

DeviceInfo::~DeviceInfo()
{
    delete d_ptr;
    d_ptr = 0;
}

bool DeviceInfo::hasFeature(DeviceInfo::Feature feature) const
{
    Q_D(const DeviceInfo);
    return d->m_features.contains(feature);
}

bool DeviceInfo::hasHardwareKey(Qt::Key key) const
{
    Q_D(const DeviceInfo);
    return d->m_keys.contains(key);
}

bool DeviceInfo::hasAndroidSupport() const
{
    return QFile::exists(QLatin1String("/opt/alien/system/bin/alien_launcher"));
}

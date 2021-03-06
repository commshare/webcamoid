/* Webcamoid, webcam capture application.
 * Copyright (C) 2011-2016  Gonzalo Exequiel Pedone
 *
 * Webcamoid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Webcamoid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Webcamoid. If not, see <http://www.gnu.org/licenses/>.
 *
 * Web-Site: http://webcamoid.github.io/
 */

#ifndef CONVERTAUDIO_H
#define CONVERTAUDIO_H

#include <QtConcurrent>
#include <akaudiopacket.h>
#include <gst/audio/audio.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>

class ConvertAudio: public QObject
{
    Q_OBJECT

    public:
        explicit ConvertAudio(QObject *parent=NULL);
        ~ConvertAudio();

        Q_INVOKABLE AkPacket convert(const AkAudioPacket &packet,
                                     const AkCaps &oCaps);

    private:
        QThreadPool m_threadPool;
        GstElement *m_pipeline;
        GstElement *m_source;
        GstElement *m_sink;
        GMainLoop *m_mainLoop;
        guint m_busWatchId;

        void waitState(GstState state);
        static gboolean busCallback(GstBus *bus,
                                    GstMessage *message,
                                    gpointer userData);
};

#endif // CONVERTAUDIO_H

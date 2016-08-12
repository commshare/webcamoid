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
#include <QDebug> /*for qDebug()*/

#include "framegrabber.h"

FrameGrabber::FrameGrabber(): QObject()
{
     timer = new QTimer(this);
     timer->setInterval(1000);   //定时时间间隔为1000ms
     connect(timer, SIGNAL(timeout()), this, SLOT(countfps()));
     first=1;
     count=0;

}

FrameGrabber::~FrameGrabber()
{
    if(timer->isActive() ){
        qDebug()<<"stop timier";
        timer->stop();
    }
}
void FrameGrabber::countfps(){
    qDebug()<<"cur fps:";//<<++count;
}

ULONG FrameGrabber::AddRef()
{
    return S_OK;
}

ULONG FrameGrabber::Release()
{
    return S_OK;
}

HRESULT FrameGrabber::QueryInterface(const IID &riid, void **ppvObject)
{
    if (NULL == ppvObject)
        return E_POINTER;

    if (riid == __uuidof(IUnknown)) {
        *ppvObject = static_cast<IUnknown *>(this);

        return S_OK;
    }

    if (riid == __uuidof(ISampleGrabberCB)) {
        *ppvObject = static_cast<ISampleGrabberCB *>(this);

        return S_OK;
    }

    return E_NOTIMPL;
}

HRESULT FrameGrabber::SampleCB(double time, IMediaSample *sample)
{
   // qDebug()<<"got SampleCB";
    if(first)   {
         timer->start();
         first=0;
    }
    BYTE *buffer = NULL;
    LONG bufferSize = sample->GetSize();

    HRESULT hr = sample->GetPointer(&buffer);

    if (FAILED(hr))
        return S_FALSE;

    QByteArray oBuffer(reinterpret_cast<char *>(buffer), bufferSize);

    emit this->frameReady(time, oBuffer);

    return S_OK;
}

HRESULT FrameGrabber::BufferCB(double time, BYTE *buffer, long bufferSize)
{
    QByteArray oBuffer(reinterpret_cast<char *>(buffer), bufferSize);

    emit this->frameReady(time, oBuffer);

    return S_OK;
}

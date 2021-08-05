/* Copyright (c) 2014, Stanislaw Halik <sthalik@misaki.pl>

 * Permission to use, copy, modify, and/or distribute this
 * software for any purpose with or without fee is hereby granted,
 * provided that the above copyright notice and this permission
 * notice appear in all copies.
 */

#include "ftnoir_tracker_witmotion.h"
#include "api/plugin-api.hpp"

#include <cmath>
#include <iterator>

witmotion::witmotion()
    : yaw{ 0 }, pitch{ 0 }, roll{ 0 }, yaw_center{ 0 }, pitch_center{ 0 }, roll_center{ 0 }
{
}

witmotion::~witmotion()
{
    requestInterruption();
    wait();
}

void witmotion::run()
{
    int32_t recv_pose[3];

    while (!isInterruptionRequested())
    {
        if (sock.hasPendingDatagrams())
        {
            bool ok = false;

            do
            {
                const qint64 sz = sock.readDatagram(reinterpret_cast<char*>(recv_pose), sizeof(recv_pose));
                if (sz > 0)
                    ok = true;
            } while (sock.hasPendingDatagrams());

            if (ok)
            {
                QMutexLocker foo(&mutex);
                yaw = 180.0 * recv_pose[0] / 32678.0;
                pitch = 180.0 * recv_pose[1] / 32678.0;
                roll = 180.0 * recv_pose[2] / 32678.0;
            }

            if (first)
            {
                first = false;
                center();
            }
        }

        (void)sock.waitForReadyRead(20);
    }
}

module_status witmotion::start_tracker(QFrame*)
{
    if (!sock.bind(QHostAddress::Any, quint16(s.port), QUdpSocket::DontShareAddress))
        return error(tr("Can't bind socket -- %1").arg(sock.errorString()));

    sock.moveToThread(this);
    start();

    return status_ok();
}

void witmotion::data(double* data)
{
    data[TX] = 0;
    data[TY] = 0;
    data[TZ] = 0;

    double _yaw, _pitch, _roll;

    {
        QMutexLocker foo(&mutex);

        _yaw = yaw_center - yaw;
        _pitch = pitch_center - pitch;
        _roll = roll_center - roll;
    }

    data[Yaw] = _yaw;
    data[Pitch] = _pitch;
    data[Roll] = _roll;

    //qDebug() << "1 YAW: " << (int)_yaw << " PITCH: " << (int)_pitch << " ROLL: " << (int)_roll;

    //    int values[] = {
    //        0, 90, -90, 180, -180,
    //    };
    //    int indices[] = {
    //        s.add_yaw,
    //        s.add_pitch,
    //        s.add_roll,
    //    };
    //
    //    for (int i = 0; i < 3; i++)
    //    {
    //        const int k = indices[i];
    //        if (k >= 0 && k < std::distance(std::begin(values), std::end(values)))
    //            data[Yaw + i] += values[k];
    //    }
}

bool witmotion::center()
{
    QMutexLocker foo(&mutex);

    if (first)
    {
//        qDebug() << "WIT CENTER FALSE";
        return false;
    }

    yaw_center = yaw;
    pitch_center = pitch;
    roll_center = roll;

//    qDebug() << "WIT CENTER";

    return true;
}

OPENTRACK_DECLARE_TRACKER(witmotion, dialog_witmotion, witmotion_receiver_dll)

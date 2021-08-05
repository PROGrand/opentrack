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

witmotion::witmotion() :
    last_recv_pose { 0,0,0, 0,0,0 },
    last_recv_pose2 { 0,0,0, 0,0,0 }
{}

witmotion::~witmotion()
{
    requestInterruption();
    wait();
}

void witmotion::run()
{
    QByteArray datagram;
    datagram.resize(sizeof(last_recv_pose));

    while (!isInterruptionRequested())
    {
        if (sock.hasPendingDatagrams())
        {
            QMutexLocker foo(&mutex);

            bool ok = false;

            do
            {
                const qint64 sz = sock.readDatagram(reinterpret_cast<char*>(last_recv_pose2), sizeof(double[3]));
                if (sz > 0)
                    ok = true;
            }
            while (sock.hasPendingDatagrams());

            if (ok)
            {
                for (unsigned i = 0; i < 3; i++)
                {
                    int val = std::fpclassify(last_recv_pose2[i]);
                    if (val == FP_NAN || val == FP_INFINITE)
                    {
                        ok = false;
                        break;
                    }
                }
            }

            if (ok)
            {
                for (unsigned i = 0; i < 3; i++)
                    last_recv_pose[i] = last_recv_pose2[i];
            }
        }

        (void) sock.waitForReadyRead(73);
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

void witmotion::data(double *data)
{
    QMutexLocker foo(&mutex);
    for (int i = 0; i < 6; i++)
        data[i] = last_recv_pose[i];

    int values[] = {
        0,
        90,
        -90,
        180,
        -180,
    };
    int indices[] = {
        s.add_yaw,
        s.add_pitch,
        s.add_roll,
    };

    for (int i = 0; i < 3; i++)
    {
        const int k = indices[i];
        if (k >= 0 && k < std::distance(std::begin(values), std::end(values)))
            data[Yaw + i] += values[k];
    }
}


OPENTRACK_DECLARE_TRACKER(witmotion, dialog_witmotion, witmotion_receiver_dll)

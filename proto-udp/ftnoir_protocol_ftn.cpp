/* Homepage         http://facetracknoir.sourceforge.net/home/default.htm        *
 *                                                                               *
 * ISC License (ISC)                                                             *
 *                                                                               *
 * Copyright (c) 2015, Wim Vriend                                                *
 *                                                                               *
 * Permission to use, copy, modify, and/or distribute this software for any      *
 * purpose with or without fee is hereby granted, provided that the above        *
 * copyright notice and this permission notice appear in all copies.             *
 */
#include "ftnoir_protocol_ftn.h"
#include <QFile>
#include "api/plugin-api.hpp"

witmotion::witmotion()
{
    set_dest_address();

    QObject::connect(s.b.get(), &bundle_::changed,
                     this, &witmotion::set_dest_address,
                     Qt::DirectConnection);
}

void witmotion::pose(const double *headpose, const double*)
{
    QMutexLocker l(&lock);

    outSocket.writeDatagram((const char *) headpose, sizeof(double[6]), dest_ip, dest_port);
}

void witmotion::set_dest_address()
{
    QMutexLocker l(&lock);

    dest_port = (unsigned short)s.port;
    dest_ip = QHostAddress((s.ip1.to<unsigned>() & 0xff) << 24 |
                           (s.ip2.to<unsigned>() & 0xff) << 16 |
                           (s.ip3.to<unsigned>() & 0xff) << 8  |
                           (s.ip4.to<unsigned>() & 0xff) << 0  );
}

module_status witmotion::initialize()
{
    if (outSocket.bind(QHostAddress::Any, 0, QUdpSocket::DontShareAddress))
        return status_ok();
    else
        return error(tr("Can't bind socket: %1").arg(outSocket.errorString()));
}

OPENTRACK_DECLARE_PROTOCOL(witmotion, FTNControls, udp_sender_dll)

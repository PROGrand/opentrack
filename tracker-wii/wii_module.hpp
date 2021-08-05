#pragma once

#include "api/plugin-api.hpp"

class wii_metadata_pt : public Metadata
{
    Q_OBJECT

    QString name() override { return tr("WiiPointTracker 1.1"); };
    QIcon icon() override { return QIcon(":/Resources/wii.png"); };
};

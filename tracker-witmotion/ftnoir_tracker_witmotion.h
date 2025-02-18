#pragma once
#include "ui_ftnoir_ftnclientcontrols.h"
#include <QUdpSocket>
#include <QThread>
#include <cmath>
#include "api/plugin-api.hpp"
#include "options/options.hpp"
using namespace options;

struct settings : opts {
    value<int> port;
    value<int> add_yaw, add_pitch, add_roll;
    settings() :
        opts("witmotion-tracker"),
        port(b, "port", 4242),
        add_yaw(b, "add-yaw", 0),
        add_pitch(b, "add-pitch", 0),
        add_roll(b, "add-roll", 0)
    {}
};

class witmotion : protected QThread, public ITracker
{
    Q_OBJECT
public:
    witmotion();
    ~witmotion() override;
    module_status start_tracker(QFrame *) override;
    void data(double *data) override;
    bool center() override;
protected:
    void run() override;
private:
    QUdpSocket sock;
    volatile bool first{true};
    double yaw, pitch, roll;
    double yaw_center, pitch_center, roll_center;
    QMutex mutex;
    settings s;
};

class dialog_witmotion : public ITrackerDialog
{
    Q_OBJECT
public:
    dialog_witmotion();
    void register_tracker(ITracker *) override {}
    void unregister_tracker() override {}
private:
    Ui::UICFTNClientControls ui;
    settings s;
private slots:
    void doOK();
    void doCancel();
};

class witmotion_receiver_dll : public Metadata
{
    Q_OBJECT

    QString name() { return tr("WitMotion"); }
    QIcon icon() { return QIcon(":/images/opentrack.png"); }
};


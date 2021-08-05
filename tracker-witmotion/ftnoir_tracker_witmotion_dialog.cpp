#include "api/plugin-api.hpp"
#include "ftnoir_tracker_witmotion.h"

dialog_witmotion::dialog_witmotion()
{
    ui.setupUi( this );

    connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(doOK()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(doCancel()));

    tie_setting(s.port, ui.spinPortNumber);
    tie_setting(s.add_yaw, ui.add_yaw);
    tie_setting(s.add_pitch, ui.add_pitch);
    tie_setting(s.add_roll, ui.add_roll);
}

void dialog_witmotion::doOK() {
    s.b->save();
    close();
}

void dialog_witmotion::doCancel() {
    close();
}


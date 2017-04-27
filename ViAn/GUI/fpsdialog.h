#ifndef FPSDIALOG_H
#define FPSDIALOG_H

#include "Library/customdialog.h"

class FPSDialog {
public:
    FPSDialog();
    float get_fps(QWidget* parent);
private:
    // Constants for the FPS dialog. Defines the limits, number
    // of decimals, default value, and step for the input box.
    float FPS_MIN = 1, FPS_MAX = 25, FPS_STEP = 0.1, FPS_DEFAULT = 1;
    int FPS_DECIMALS = 1;
};

#endif // FPSDIALOG_H

#include "fpsdialog.h"

/**
 * @brief FPSDialog::FPSDialog
 */
FPSDialog::FPSDialog() {
}

/**
 * @brief FPSDialog::get_fps
 * Requests the user to enter FPS in a dialog window.
 * @param parent The parent window.
 * @return Returns the FPS, or -1 if cancelled.
 */
float FPSDialog::get_fps(QWidget* parent) {
    float fps = FPS_DEFAULT;

    // Create the text shown in the dialog
    QString fps_text = QString("FPS [");
    fps_text.append(QString::number(FPS_MIN));
    fps_text.append(" - ");
    fps_text.append(QString::number(FPS_MAX));
    fps_text.append("]: ");

    // Create the dialog
    CustomDialog dialog("FPS", parent);
    dialog.addLabel("Enter value:");
    dialog.addDblSpinBoxF(fps_text, FPS_MIN, FPS_MAX, &fps, FPS_DECIMALS, FPS_STEP,
                          "Choose FPS value with the input box.");

    // Show the dialog (execution will stop here until the dialog is finished)
    dialog.exec();

    if (dialog.wasCancelled()) {
        return -1;
    }
    return fps;
}

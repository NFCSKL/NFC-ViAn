#include "overlaytester.h"

/**
 * @brief OverlayTester::OverlayTester
 */
OverlayTester::OverlayTester() {
}

/**
 * @brief OverlayTester::exec
 */
void OverlayTester::exec() {
    cout << "Integration testing: Overlay\n";
    cout << "Running test...\n";
    main->set_status_bar("hej");
}

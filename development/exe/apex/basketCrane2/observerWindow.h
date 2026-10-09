#ifndef CRANE2_OBSERVER_WINDOW_H
#define CRANE2_OBSERVER_WINDOW_H
#include <QMainWindow>
#include <QVector>
#include <QString>
class QTableWidget;
class QLabel;
namespace Crane2 {
// Deliberately independent of the control window and all PLC/HMI library classes.
class ObserverWindow : public QMainWindow {
public:
    explicit ObserverWindow(QWidget* parent = nullptr);
private:
    void refresh();
    struct View { QTableWidget* table; QString sql; QString connection; };
    QVector<View> views_;
    QLabel* status_;
};
}
#endif

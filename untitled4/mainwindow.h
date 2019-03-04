#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sendSetup(int device);
    void sendToggleStream();

private:
    Ui::MainWindow *ui;
    QThread *thread;

    void setup();

private slots:
    void receiveFrame(QImage frame);
    void receiveToggleStream();
};

#endif // MAINWINDOW_H

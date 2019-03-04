#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include "opencvworker.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->labelView->setScaledContents(true);
    setup();
}

MainWindow::~MainWindow()
{
    thread->quit();
    while(!thread->isFinished());
    delete thread;
    delete ui;
}

void MainWindow::setup()
{
    thread = new QThread();
    OpenCvWorker *worker = new OpenCvWorker();
    QTimer *workerTrigger = new QTimer();
    workerTrigger->setInterval(1);

    connect(workerTrigger, SIGNAL(timeout()), worker, SLOT(receiveGrabFrame()));
        connect(thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), workerTrigger, SLOT(deleteLater()));
        connect(this, SIGNAL(sendSetup(int)), worker, SLOT(receiveSetup(int)));
        connect(this, SIGNAL(sendToggleStream()), worker, SLOT(receiveToggleStream()));
        connect(ui->pushButtonPlay_2, SIGNAL(clicked(bool)), this, SLOT(receiveToggleStream()));
        connect(ui->checkBoxEnableBinaryThreshold_2, SIGNAL(toggled(bool)), worker, SLOT(receiveEnableBinaryThreshold()));
        connect(ui->spinBoxBinaryThreshold_2, SIGNAL(valueChanged(int)), worker, SLOT(receiveBinaryThreshold(int)));
        connect(worker, SIGNAL(sendFrame(QImage)), this, SLOT(receiveFrame(QImage)));
        connect(thread, SIGNAL(finished()), worker, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), workerTrigger, SLOT(deleteLater()));

        //ui->checkBoxEnableBinaryThreshold_2
        workerTrigger->start();
        worker->moveToThread(thread);
    //    worker->deleteLater();
        workerTrigger->moveToThread(thread);
    //    workerTrigger->deleteLater();

        thread->start();

        emit sendSetup(0);
}

void MainWindow::receiveFrame(QImage frame)
{
    ui->labelView->setPixmap(QPixmap::fromImage(frame));
}

void MainWindow::receiveToggleStream()
{
    if(!ui->pushButtonPlay_2->text().compare(">")) ui->pushButtonPlay_2->setText("||");
    else ui->pushButtonPlay_2->setText(">");

    emit sendToggleStream();
}

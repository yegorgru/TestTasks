#include <iostream>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mTimer = std::make_unique<QTimer>(this);
    connect(mTimer.get(), SIGNAL(timeout()), this, SLOT(makeScreenshot()));
    connect(ui->startStopButton, &QPushButton::clicked, this, &MainWindow::buttonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeScreenshot() {
    std::cout << "Make screenhot" << std::endl;
}

void MainWindow::buttonClicked(bool checked) {
    if(checked) {
        mTimer->start(60000);
    }
    else {
        mTimer->stop();
    }
}


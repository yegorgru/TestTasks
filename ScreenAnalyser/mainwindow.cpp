#include <iostream>

#include <QPixmap>
#include <QScreen>

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
    hide();
    QTimer::singleShot(500, this, SLOT(processScreenshot()));
}

void MainWindow::processScreenshot() {
    setScreenScene();
    saveToDb();
}

void MainWindow::setScreenScene() {
    QScreen* screen = QGuiApplication::screens()[0];
    mImage = screen->grabWindow(0);
    mScene = new QGraphicsScene(this);
    mScene->addPixmap(mImage);
    mScene->setSceneRect(mImage.rect());
    ui->screenshotView->setScene(mScene);
    show();
}

void MainWindow::saveToDb() {
    QByteArray inByteArray;
    QBuffer inBuffer(&inByteArray);
    inBuffer.open(QIODevice::WriteOnly);
    mImage.save(&inBuffer, "PNG");
    mStorage.insertScreenshot(inByteArray);
}

void MainWindow::buttonClicked(bool checked) {
    if(checked) {
        mTimer->start(60000);
    }
    else {
        mTimer->stop();
    }
}


#include <iostream>

#include <QPixmap>
#include <QScreen>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    mCurrentScreenId = ScreenshotStorage::LAST_SCREEN_ID;
    mLoadedScreenId = ScreenshotStorage::LAST_SCREEN_ID;
    mModelOffset = 0;
    ui->setupUi(this);
    mTimer = std::make_unique<QTimer>(this);
    connect(mTimer.get(), SIGNAL(timeout()), this, SLOT(makeScreenshot()));
    connect(ui->startStopButton, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->prevButton, &QPushButton::clicked, this, &MainWindow::loadPrevPage);
    connect(ui->nextButton, &QPushButton::clicked, this, &MainWindow::loadNextPage);
    connect(ui->screensTableView, &QTableView::clicked, this, &MainWindow::onScreensTableClicked);
    ui->screensTableView->setModel(&mModel);
    loadCurrentPage();
    loadCurrentScreenshot();
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
    grabScreen();
    saveToDb();
    loadCurrentPage();
    loadCurrentScreenshot();
}

void MainWindow::grabScreen() {
    QScreen* screen = QGuiApplication::screens()[0];
    mImage = screen->grabWindow(0);
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
        mTimer->start(6000);
    }
    else {
        mTimer->stop();
    }
}

void MainWindow::loadCurrentPage() {
    if(mModelOffset == 0) {
        mStorage.loadScreensPage(mModel, mModelOffset);
    }
}

void MainWindow::loadPrevPage() {
    if(mModelOffset != 0) {
        mModelOffset -= 10;
        mStorage.loadScreensPage(mModel, mModelOffset);
    }
}

void MainWindow::loadNextPage() {
    mModelOffset += 10;
    mStorage.loadScreensPage(mModel, mModelOffset);
    if(mModel.rowCount() == 0) {
        loadPrevPage();
    }
}

void MainWindow::loadCurrentScreenshot() {
    if(mLoadedScreenId == mCurrentScreenId && mCurrentScreenId != ScreenshotStorage::LAST_SCREEN_ID) {
        return;
    }
    auto bytes = mStorage.getScreenshotById(mCurrentScreenId);
    if(mImage.loadFromData(bytes, "PNG"))
    {
        mScene = new QGraphicsScene(this);
        mScene->addPixmap(mImage);
        mScene->setSceneRect(mImage.rect());
        ui->screenshotView->setScene(mScene);
    }
    else {
        qCritical() << "Failed to load screenshot";
    }
}

void MainWindow::onScreensTableClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        int indexRow = index.row();
        mCurrentScreenId = mModel.record(indexRow).field("ScreenshotID").value().toInt();
        if(indexRow == 0 && mModelOffset == 0) {
            mCurrentScreenId = ScreenshotStorage::LAST_SCREEN_ID;
        }
        loadCurrentScreenshot();
    }
}

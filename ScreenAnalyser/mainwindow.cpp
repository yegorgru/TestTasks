#include <QPixmap>
#include <QScreen>

#include "mainwindow.h"
#include "imageworker.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mUi(std::make_unique<Ui::MainWindow>())
    , mTimer(std::make_unique<QTimer>(this))
    , mScene(this)
    , mModelOffset(0)
    , mCurrentScreenId(ScreenshotStorage::LAST_SCREEN_ID)
    , mLoadedScreenId(ScreenshotStorage::LAST_SCREEN_ID)
    , mStorage("RootStorage")
{
    mUi->setupUi(this);

    mUi->screensTableView->setModel(&mModel);

    setupConnections();
    setupWorkerThread();

    refresh();
}

MainWindow::~MainWindow()
{
    mWorkerThread.quit();
    mWorkerThread.wait();
}

void MainWindow::refresh() {
    loadCurrentPage();
    loadCurrentScreenshot();
}

void MainWindow::setupConnections() {
    connect(mTimer.get(), SIGNAL(timeout()), this, SLOT(makeScreenshot()));
    connect(mUi->startStopButton, &QPushButton::clicked, this, &MainWindow::startStopButtonClicked);
    connect(mUi->prevButton, &QPushButton::clicked, this, &MainWindow::loadPrevPage);
    connect(mUi->nextButton, &QPushButton::clicked, this, &MainWindow::loadNextPage);
    connect(mUi->screensTableView, &QTableView::clicked, this, &MainWindow::onScreensTableClicked);
}

void MainWindow::setupWorkerThread() {
    ImageWorker* imageWorker = new ImageWorker;
    imageWorker->moveToThread(&mWorkerThread);
    connect(&mWorkerThread, &QThread::finished, imageWorker, &QObject::deleteLater);
    connect(this, &MainWindow::processNextScreenshot, imageWorker, &ImageWorker::processScreenshot);
    connect(imageWorker, &ImageWorker::processingFinished, this, &MainWindow::refresh);
    mWorkerThread.start();
}

void MainWindow::loadCurrentPage() {
    if(mModelOffset == 0) {
        mStorage.loadScreensPage(mModel, mModelOffset);
    }
}

void MainWindow::loadCurrentScreenshot() {
    if(mLoadedScreenId == mCurrentScreenId && mCurrentScreenId != ScreenshotStorage::LAST_SCREEN_ID) {
        return;
    }
    mStorage.prepareScreenshotById(mCurrentScreenId);
    auto bytes = mStorage.getImage();
    if(bytes.isEmpty()) {
        return;
    }
    if(mImage.loadFromData(bytes, "PNG"))
    {
        mScene.clear();
        mScene.addPixmap(mImage);
        mScene.setSceneRect(mImage.rect());
        mUi->screenshotView->setScene(&mScene);

        mUi->idLabel->setText(QString("ID: ") + mStorage.getScreenshotId());
        mUi->percentageLabel->setText(QString("Similarity percentage: ") + mStorage.getSimilarityPercentage());
        mUi->hashsumLabel->setText(QString("Hashsum: ") + mStorage.getHashsum());
        mUi->dateTimeLabel->setText(QString("Date and time: ") + mStorage.getDateTime());
    }
    else {
        qCritical() << "Failed to load screenshot";
    }
}

void MainWindow::startStopButtonClicked(bool checked) {
    if(checked) {
        mTimer->start(60000);
    }
    else {
        mTimer->stop();
    }
}

void MainWindow::makeScreenshot() {
    hide();
    QTimer::singleShot(500, this, &MainWindow::processScreenshot);
}

void MainWindow::processScreenshot() {
    auto screens = QGuiApplication::screens();
    mImage = screens[0]->grabWindow(0);
    show();
    emit processNextScreenshot(mImage);
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

void MainWindow::onScreensTableClicked(const QModelIndex& index) {
    if (index.isValid()) {
        int indexRow = index.row();
        mCurrentScreenId = mModel.record(indexRow).field("ScreenshotID").value().toInt();
        if(indexRow == 0 && mModelOffset == 0) {
            mCurrentScreenId = ScreenshotStorage::LAST_SCREEN_ID;
        }
        loadCurrentScreenshot();
    }
}

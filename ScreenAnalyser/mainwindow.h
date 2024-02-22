#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "screenshotstorage.h"

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <QPixmap>

#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void refresh();

private:
    void setupConnections();
    void setupWorkerThread();
    void loadCurrentPage();
    void loadCurrentScreenshot();

private slots:
    void startStopButtonClicked(bool checked);
    void makeScreenshot();
    void processScreenshot();
    void loadPrevPage();
    void loadNextPage();
    void onScreensTableClicked(const QModelIndex &index);

signals:
    void processNextScreenshot(const QPixmap& image);

private:
    using Timer = std::unique_ptr<QTimer>;
    using UI = std::unique_ptr<Ui::MainWindow>;
private:
    UI mUi;
    Timer mTimer;
    QGraphicsScene mScene;
    QPixmap mImage;
    QSqlQueryModel mModel;
    int mModelOffset;
    int mCurrentScreenId;
    int mLoadedScreenId;
    QThread mWorkerThread;
    ScreenshotStorage mStorage;
};
#endif // MAINWINDOW_H

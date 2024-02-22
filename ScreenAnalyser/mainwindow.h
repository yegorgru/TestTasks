#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "screenshotstorage.h"
#include "imageworker.h"

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <QPixmap>

#include <memory>
#include <future>

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

private:
    void loadCurrentPage();
    void loadCurrentScreenshot();

public slots:
    void loadNewData();

private slots:
    void buttonClicked(bool checked);
    void makeScreenshot();
    void processScreenshot();
    void loadPrevPage();
    void loadNextPage();
    void onScreensTableClicked(const QModelIndex &index);

signals:
    void processNextScreenshot(const QPixmap& image);

private:
    using Timer = std::unique_ptr<QTimer>;
private:
    Ui::MainWindow *ui;
    Timer mTimer;
    QGraphicsScene *mScene;
    QPixmap mImage;
    QSqlQueryModel mModel;
    int mModelOffset;
    int mCurrentScreenId;
    int mLoadedScreenId;
    QThread mWorkerThread;
    ScreenshotStorage mStorage;
};
#endif // MAINWINDOW_H

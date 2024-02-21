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

private:
    void grabScreen();
    void saveToDb();
    void loadCurrentPage();
    void loadCurrentScreenshot();


private slots:
    void buttonClicked(bool checked);
    void makeScreenshot();
    void processScreenshot();
    void loadPrevPage();
    void loadNextPage();
    void onScreensTableClicked(const QModelIndex &index);

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
    ScreenshotStorage mStorage;
};
#endif // MAINWINDOW_H

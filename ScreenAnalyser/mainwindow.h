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
public:

private:
    void setScreenScene();
    void saveToDb();


private slots:
    void buttonClicked(bool checked);
    void makeScreenshot();
    void processScreenshot();

private:
    using Timer = std::unique_ptr<QTimer>;
private:
    Ui::MainWindow *ui;
    Timer mTimer;
    QGraphicsScene *mScene;
    QPixmap mImage;
    ScreenshotStorage mStorage;
};
#endif // MAINWINDOW_H

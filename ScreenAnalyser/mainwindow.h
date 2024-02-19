#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

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


private slots:
    void buttonClicked(bool checked);
    void makeScreenshot();

private:
    using Timer = std::unique_ptr<QTimer>;
private:
    Ui::MainWindow *ui;
    Timer mTimer;
};
#endif // MAINWINDOW_H

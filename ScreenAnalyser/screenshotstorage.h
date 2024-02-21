#ifndef SCREENSHOTSTORAGE_H
#define SCREENSHOTSTORAGE_H

#include <QtSql>



class ScreenshotStorage
{
public:
    ScreenshotStorage();
    void insertScreenshot(QByteArray& img);
    void loadScreensPage(QSqlQueryModel& model, int offset);
    QByteArray getScreenshotById(int screenshotId);
public:
    static const int LAST_SCREEN_ID = -1;
private:
    QByteArray getLastScreenshot();

private:
    QSqlDatabase mDatabase;
};

#endif // SCREENSHOTSTORAGE_H

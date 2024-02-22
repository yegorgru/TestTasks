#ifndef SCREENSHOTSTORAGE_H
#define SCREENSHOTSTORAGE_H

#include <QtSql>



class ScreenshotStorage
{
public:
    ScreenshotStorage();
    void insertScreenshot(QByteArray& img, int hashsum, int percentage);
    void loadScreensPage(QSqlQueryModel& model, int offset);
    QSqlRecord getScreenshotById(int screenshotId);
public:
    static const int LAST_SCREEN_ID = -1;
private:
    QSqlRecord getLastScreenshot();

private:
    QSqlDatabase mDatabase;
};

#endif // SCREENSHOTSTORAGE_H

#ifndef SCREENSHOTSTORAGE_H
#define SCREENSHOTSTORAGE_H

#include <QtSql>

class ScreenshotStorage
{
public:
    ScreenshotStorage();
    void insertScreenshot(QByteArray& img);
    void loadScreensPage(QSqlQueryModel& model, int offset);

private:
    QSqlDatabase mDatabase;
};

#endif // SCREENSHOTSTORAGE_H

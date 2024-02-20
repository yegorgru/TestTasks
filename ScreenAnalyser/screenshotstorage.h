#ifndef SCREENSHOTSTORAGE_H
#define SCREENSHOTSTORAGE_H

#include <QtSql>

class ScreenshotStorage
{
public:
    ScreenshotStorage();
    void insertScreenshot(QByteArray& img);

private:
    QSqlDatabase mDatabase;
};

#endif // SCREENSHOTSTORAGE_H

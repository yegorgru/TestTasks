#ifndef SCREENSHOTSTORAGE_H
#define SCREENSHOTSTORAGE_H

#include <QtSql>

class ScreenshotStorage
{
public:
    static const int LAST_SCREEN_ID = -1;
public:
    ScreenshotStorage(const std::string& name);
public:
    void insertScreenshot(QByteArray& img, int hashsum, int percentage);
    void loadScreensPage(QSqlQueryModel& model, int offset);
    void prepareScreenshotById(int screenshotId);
public:
    QByteArray getImage() const;
    QString getScreenshotId() const;
    QString getHashsum() const;
    QString getSimilarityPercentage() const;
    QString getDateTime() const;
private:
    void prepareLastScreenshot();
private:
    QSqlRecord mRecord;
    QSqlDatabase mDatabase;
};

#endif // SCREENSHOTSTORAGE_H

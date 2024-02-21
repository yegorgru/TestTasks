#include "screenshotstorage.h"

ScreenshotStorage::ScreenshotStorage():
    mDatabase(QSqlDatabase::addDatabase("QSQLITE"))
{
    QString dbPath = qgetenv("DB_PATH");
    mDatabase.setDatabaseName(qgetenv("DB_PATH"));
    if(!mDatabase.open()) {
        qCritical() << "Failed to open database " << dbPath << ". Check DB_PATH environment variable is set";
    }
}

void ScreenshotStorage::insertScreenshot(QByteArray& img) {
    if(!mDatabase.isOpen()) {
        qWarning() << "Database is not open. Ignoring insert";
    }
    QSqlQuery query;
    bool res = query.prepare("INSERT INTO SCREENSHOT (IMAGE, HASHSUM, PERCENTAGE) "
                  "VALUES (:IMAGE, :HASHSUM, :PERCENTAGE)");
    if(!res) {
        qCritical() << "Failed to prepare insert query: " << query.lastError().text();
        return;
    }
    query.bindValue(":IMAGE", img, QSql::In | QSql::Binary);
    query.bindValue(":HASHSUM", 2);
    query.bindValue(":PERCENTAGE", 3);
    if(query.exec()) {
        qInfo() << "Screenshot saved to database successfully";
    }
    else {
        qCritical() << "Insert failed: " << query.lastError().text();
    }
}

void ScreenshotStorage::loadScreensPage(QSqlQueryModel& model, int offset) {
    QSqlQuery query;
    bool res = query.prepare("SELECT ScreenshotID, HashSum, Percentage, DateTime FROM Screenshot ORDER BY ScreenshotID DESC LIMIT :FIRST_RECORD , 10");
    if(!res) {
        qCritical() << "Failed to prepare select query: " << query.lastError().text();
        return;
    }
    query.bindValue(":FIRST_RECORD", offset);
    if(query.exec()) {
        qInfo() << "Selected rows successfully";
    }
    else {
        qCritical() << "Select failed: " << query.lastError().text();
        return;
    }
    model.setQuery(query);
}

QSqlRecord ScreenshotStorage::getLastScreenshot(){
    QSqlQuery query;
    query.prepare("SELECT Image, ScreenshotId, Hashsum, Percentage, DateTime FROM Screenshot ORDER BY ScreenshotID DESC LIMIT 0, 1");
    query.exec();
    query.first();
    return query.record();
}

QSqlRecord ScreenshotStorage::getScreenshotById(int screenshotId) {
    if(screenshotId == LAST_SCREEN_ID) {
        return getLastScreenshot();
    }
    QSqlQuery query;
    query.prepare("SELECT Image, ScreenshotId, Hashsum, Percentage, DateTime FROM Screenshot WHERE ScreenshotId = :SCREENSHOT_ID");
    query.bindValue(":SCREENSHOT_ID", screenshotId);
    query.exec();
    query.first();
    return query.record();
}

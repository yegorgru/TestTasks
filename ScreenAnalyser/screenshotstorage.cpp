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

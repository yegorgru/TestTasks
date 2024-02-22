#include "screenshotstorage.h"

#include <QString>

ScreenshotStorage::ScreenshotStorage(const std::string& name)
    : mDatabase(QSqlDatabase::addDatabase("QSQLITE", QString::fromStdString(name)))
{
    QString dbPath = qgetenv("DB_PATH");
    mDatabase.setDatabaseName(qgetenv("DB_PATH"));
    if(!mDatabase.open()) {
        qCritical() << "Failed to open database " << dbPath << ". Check DB_PATH environment variable is set";
    }
}

void ScreenshotStorage::insertScreenshot(QByteArray& img, int hashsum, int percentage) {
    if(!mDatabase.isOpen()) {
        qCritical() << "Database is not open. Ignoring insert";
        return;
    }
    QSqlQuery query(mDatabase);
    if(!query.prepare("INSERT INTO SCREENSHOT (IMAGE, HASHSUM, PERCENTAGE) VALUES (:IMAGE, :HASHSUM, :PERCENTAGE)")) {
        qCritical() << "Failed to prepare insert query: " << query.lastError().text();
        return;
    }
    query.bindValue(":IMAGE", img, QSql::In | QSql::Binary);
    query.bindValue(":HASHSUM", hashsum);
    query.bindValue(":PERCENTAGE", percentage);
    if(!query.exec()) {
        qCritical() << "Insert failed: " << query.lastError().text();
    }
}

void ScreenshotStorage::loadScreensPage(QSqlQueryModel& model, int offset) {
    if(!mDatabase.isOpen()) {
        qCritical() << "Database is not open";
        return;
    }
    QSqlQuery query(mDatabase);
    if(!query.prepare("SELECT ScreenshotID, HashSum, Percentage, DateTime FROM Screenshot ORDER BY ScreenshotID DESC LIMIT :FIRST_RECORD , 10")) {
        qCritical() << "Failed to prepare select query: " << query.lastError().text();
        return;
    }
    query.bindValue(":FIRST_RECORD", offset);
    if(!query.exec()) {
        qCritical() << "Select failed: " << query.lastError().text();
        return;
    }
    model.setQuery(query);
}

void ScreenshotStorage::prepareScreenshotById(int screenshotId) {
    if(!mDatabase.isOpen()) {
        qCritical() << "Database is not open";
        return;
    }
    if(screenshotId == LAST_SCREEN_ID) {
        return prepareLastScreenshot();
    }
    QSqlQuery query(mDatabase);
    if(!query.prepare("SELECT Image, ScreenshotId, Hashsum, Percentage, DateTime FROM Screenshot WHERE ScreenshotId = :SCREENSHOT_ID")) {
        qCritical() << "Failed to prepare select query: " << query.lastError().text();
        return;
    }
    query.bindValue(":SCREENSHOT_ID", screenshotId);
    if(!query.exec()) {
        qCritical() << "Select failed: " << query.lastError().text();
        return;
    }
    query.first();
    mRecord = query.record();
}

QByteArray ScreenshotStorage::getImage() const {
    return mRecord.value("Image").toByteArray();
}

QString ScreenshotStorage::getScreenshotId() const {
    return mRecord.value("ScreenshotId").toString();
}

QString ScreenshotStorage::getHashsum() const {
    return mRecord.value("Hashsum").toString();
}

QString ScreenshotStorage::getSimilarityPercentage() const {
    return mRecord.value("Percentage").toString();
}

QString ScreenshotStorage::getDateTime() const {
    return mRecord.value("DateTime").toString();
}

void ScreenshotStorage::prepareLastScreenshot() {
    QSqlQuery query(mDatabase);
    if(!query.prepare("SELECT Image, ScreenshotId, Hashsum, Percentage, DateTime FROM Screenshot ORDER BY ScreenshotID DESC LIMIT 0, 1")) {
        qCritical() << "Failed to prepare select query: " << query.lastError().text();
        return;
    }
    if(!query.exec()) {
        qCritical() << "Select failed: " << query.lastError().text();
        return;
    }
    query.first();
    mRecord = query.record();
}

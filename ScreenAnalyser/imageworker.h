#ifndef IMAGEWORKER_H
#define IMAGEWORKER_H

#include <QObject>

#include "screenshotstorage.h"

class ImageWorker : public QObject
{
    Q_OBJECT
public:
    ImageWorker();

public slots:
    void processScreenshot(const QPixmap& image);

signals:
    void processingFinished();

private:
    QPixmap mPrevImage;
    ScreenshotStorage mStorage;
};

#endif // IMAGEWORKER_H

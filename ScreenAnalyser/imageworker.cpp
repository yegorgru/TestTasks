#include "imageworker.h"
#include "imagealgorithms.h"

ImageWorker::ImageWorker()
    : mStorage("Thread storage")
{

}

void ImageWorker::processScreenshot(const QPixmap& image) {
    QByteArray inByteArray;
    QBuffer inBuffer(&inByteArray);
    inBuffer.open(QIODevice::WriteOnly);
    image.save(&inBuffer, "PNG");
    int hashsum = ImageAlgorithms::findHashSum(image.toImage());
    int percentage = mPrevImage.isNull() ? 0 : ImageAlgorithms::compareByPixels(image.toImage(), mPrevImage.toImage());
    mStorage.insertScreenshot(inByteArray, hashsum, percentage);
    mPrevImage = image.copy();
    emit processingFinished();
}

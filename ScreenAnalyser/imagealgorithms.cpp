#include "imagealgorithms.h"

namespace ImageAlgorithms {
    int compareByPixels(const QImage& lhs, const QImage& rhs) {
        if(lhs.rect() != rhs.rect()) {
            return compareByPixels(lhs, rhs.scaled(lhs.width(), lhs.height()));
        }
        int counter = 0;
        for(size_t i = 0; i < lhs.width(); ++i) {
            for(size_t j = 0; j < lhs.height(); ++j) {
                if(lhs.pixel(i, j) == rhs.pixel(i, j)) {
                    ++counter;
                }
            }
        }
        return static_cast<double>(counter) / (lhs.width() * lhs.height()) * 100;
    }

    int findHashSum(const QImage& img) {
        QImage scaledImage = img.scaled(256, 256, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        quint64 hash = 0;
        for (int y = 0; y < scaledImage.height(); ++y) {
            for (int x = 0; x < scaledImage.width(); ++x) {
                QRgb pixel = scaledImage.pixel(x, y);
                hash += qRed(pixel) + qGreen(pixel) + qBlue(pixel);
            }
        }
        hash /= 3 * scaledImage.width() * scaledImage.height();
        return hash;
    }
}

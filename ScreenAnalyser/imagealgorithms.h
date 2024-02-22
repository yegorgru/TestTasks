#ifndef IMAGEALGORITHMS_H
#define IMAGEALGORITHMS_H

#include <QPixmap>

namespace ImageAlgorithms {
    int compareByPixels(const QImage& lhs, const QImage& rhs);
    int findHashSum(const QImage& img);
}

#endif // IMAGEALGORITHMS_H

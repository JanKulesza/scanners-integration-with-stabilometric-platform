#include "scannerstab.h"
#include "events.h"
#include <QRect>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QColor>
#include <algorithm>

enum WhichFoot {
    LEFT,
    RIGHT
};

static QRect boundingRectOfFoot(const QPixmap& pix, WhichFoot whichFoot )
{
    QImage img = pix.toImage().convertToFormat(QImage::Format_ARGB32);

    if (img.isNull())
        return QRect();

    // TODO: Improve getting foot color
    int cx = img.width() / 2;
    int cy = img.height() / 2;
    QColor refColor("#cdb0a0");

    int thresholdDiff = 25; // adjust if needed

    int minX = img.width();
    int minY = img.height();
    int maxX = -1;
    int maxY = -1;

    for (int y = 0; y < img.height(); ++y)
    {
        const QRgb* row = reinterpret_cast<const QRgb*>(img.scanLine(y));
        for (int x = 0; x < img.width(); ++x)
        {
            QColor c(row[x]);
            // Total difference between color channels
            int diff = std::abs(c.red() - refColor.red()) +
                std::abs(c.green() - refColor.green()) +
                std::abs(c.blue() - refColor.blue());
            if (diff < thresholdDiff)
            {
                minX = std::min(minX, x);
                maxX = std::max(maxX, x);
                minY = std::min(minY, y);
                maxY = std::max(maxY, y);
            }
        }
    }

    if (maxX < 0 || maxY < 0)
        return QRect();
    if(whichFoot == RIGHT)
        return QRect(QPoint(0, minY), QPoint(maxX, maxY));
    
    return QRect(QPoint(minX, minY), QPoint(img.width(), maxY));
}

scannerstab::scannerstab(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.graphicsView, &Custom_View::footDropped,
        [this](const QPixmap& pix) {
            leftPixmap = pix;
            qDebug() << "Lewy obraz wczytany, rozmiar:" << pix.size();
            if (!rightPixmap.isNull()) {
                computeAndDisplayArea();
            }
        });
    
    connect(ui.graphicsView_2, &Custom_View::footDropped,
        [this](const QPixmap& pix) {
            rightPixmap = pix;
            qDebug() << "Prawy obraz wczytany, rozmiar:" << pix.size();
            if (!leftPixmap.isNull()) {
                computeAndDisplayArea();
            }
        });
}

scannerstab::~scannerstab()
{
}

void scannerstab::computeAndDisplayArea()
{
    // Obtain bounding rectangles for left and right feet
    QRect rectLeft = boundingRectOfFoot(leftPixmap, LEFT);
    QRect rectRight = boundingRectOfFoot(rightPixmap, RIGHT);

    qDebug() << "Lewy bounding rect:" << rectLeft;
    qDebug() << "Prawy bounding rect:" << rectRight;

    // --- Scanner settings and conversion factors ---
    int dpi = 2400;             // Scanner optical resolution (pixels per inch)
    double gapCm = 2.0;         // Desired gap between feet in centimeters (corrected to 2 cm)
    double inchInCm = 2.54;     // 1 inch = 2.54 cm

    // Convert gap (in cm) to pixels:
    // pixels = (gap in cm / cm per inch) * dpi
    int gapPx = static_cast<int>((gapCm / inchInCm) * dpi + 0.5);
    qDebug() << "Gap (in pixels) corresponding to" << gapCm << "cm:" << gapPx;

    // --- Position right foot relative to left foot ---
    // Shift the right rectangle to the right by the width of the left rectangle plus the gap
    QRect shiftedRight = rectRight.translated(rectLeft.width() + gapPx, 0);

    // Get the bounding rectangle that covers both the left foot and the shifted right foot
    QRect combined = rectLeft.united(shiftedRight);

    // --- Calculate areas in pixels ---
    double areaLeftPx = rectLeft.width() * rectLeft.height();
    double areaRightPx = rectRight.width() * rectRight.height();
    // Instead of summing individual areas and adding a length (gapPx), use the union's area:
    double areaCombinedPx = combined.width() * combined.height();

    // --- Convert pixel area to square centimeters ---
    // One pixel corresponds to (2.54 / dpi) centimeters, so for area:
    double cmPerPx = inchInCm / dpi;  // cm per pixel
    double px2ToCm2 = cmPerPx * cmPerPx;  // conversion factor from px² to cm²

    double areaLeftCm2 = areaLeftPx * px2ToCm2;
    double areaRightCm2 = areaRightPx * px2ToCm2;
    double areaCombinedCm2 = areaCombinedPx * px2ToCm2;

    qDebug() << "Combined bounding rect:" << combined << "Area (px²):" << areaCombinedPx;
    qDebug() << "Lewy:" << areaLeftCm2 << "cm², Prawy:" << areaRightCm2 << "cm², Razem:" << areaCombinedCm2 << "cm²";

    // --- Update UI label ---
    ui.label->setText(QString("Lewy: %1 cm², Prawy: %2 cm², Razem: %3 cm²")
        .arg(areaLeftCm2, 0, 'f', 2)
        .arg(areaRightCm2, 0, 'f', 2)
        .arg(areaCombinedCm2, 0, 'f', 2));

    // --- Draw bounding rectangles on the images ---
    if (!leftPixmap.isNull())
    {
        QPixmap leftWithRect = leftPixmap;
        QPainter painter(&leftWithRect);
        painter.setPen(QPen(Qt::red, 3));       // Red outline for left foot
        painter.setBrush(Qt::NoBrush);          // Transparent fill
        painter.drawRect(rectLeft);
        ui.graphicsView->scene()->addPixmap(leftWithRect);  // Add annotated left image to view
    }

    if (!rightPixmap.isNull())
    {
        QPixmap rightWithRect = rightPixmap;
        QPainter painter(&rightWithRect);
        painter.setPen(QPen(Qt::blue, 3));      // Blue outline for right foot
        painter.setBrush(Qt::NoBrush);          // Transparent fill
        painter.drawRect(rectRight);
        ui.graphicsView_2->scene()->addPixmap(rightWithRect); // Add annotated right image to view
    }
}



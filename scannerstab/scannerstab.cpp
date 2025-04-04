#include "scannerstab.h"
#include "events.h"
#include <QRect>
#include <QDebug>
#include <QImage>
#include <QColor>
#include <algorithm>

static QRect boundingRectOfFoot(const QPixmap& pix)
{
    QImage img = pix.toImage().convertToFormat(QImage::Format_ARGB32);

    if (img.isNull())
        return QRect();

    // TODO: Improve getting foot color
    int cx = img.width() / 2;
    int cy = img.height() / 2;
    QColor refColor(img.pixel(cx, cy));

    int thresholdDiff = 30; // adjust if needed

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

    return QRect(QPoint(minX, minY), QPoint(maxX, maxY));
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
    // Calculate bounding rectangle for left and right scan
    QRect rectLeft = boundingRectOfFoot(leftPixmap);
    QRect rectRight = boundingRectOfFoot(rightPixmap);

    qDebug() << "Lewy bounding rect:" << rectLeft;
    qDebug() << "Prawy bounding rect:" << rectRight;

    // Calculate 2 cm in pixels – we assume for example DPI = 300 (adjust, if needed)
    int dpi = 300;
    double gapCm = 2.0;
    double inchInCm = 2.54;
    int gapPx = static_cast<int>((gapCm / inchInCm) * dpi + 0.5);
    qDebug() << "Przerwa (w pikselach) odpowiadająca 2 cm:" << gapPx;


    // To create one rectangle covering both feet,
    // we move the bounding rect of the right foot by the width of the left rect + gapPx
    QRect shiftedRight = rectRight.translated(rectLeft.width() + gapPx, 0);

    // Connect left and right rectangle
    QRect combined = rectLeft.united(shiftedRight);
    double areaCombined = combined.width() * combined.height();

    // Calculate area
    double areaLeft = rectLeft.width() * rectLeft.height();
    double areaRight = rectRight.width() * rectRight.height();

    qDebug() << "Scalony bounding rect:" << combined << "Pole (px^2):" << areaCombined;

    ui.label->setText(QString("Lewy: %1 px², Prawy: %2 px², Razem: %3 px²")
        .arg(areaLeft)
        .arg(areaRight)
        .arg(areaCombined));
}

#include "events.h"
#include <QUrl>
#include <QDebug>

Custom_View::Custom_View(QWidget* parent) : QGraphicsView(parent)
{
    setAcceptDrops(true);
    scene = new QGraphicsScene(this);
    setScene(scene);
}

void Custom_View::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void Custom_View::dragLeaveEvent(QDragLeaveEvent* event)
{
    event->accept();
}

void Custom_View::dragMoveEvent(QDragMoveEvent* event)
{
    event->accept();
    event->acceptProposedAction();
}

void Custom_View::dropEvent(QDropEvent* event)
{
    if (event->mimeData()->hasUrls()) {
        QList<QUrl> urlList = event->mimeData()->urls();
        for (const QUrl& url : urlList)
        {
            if (url.isLocalFile())
            {
                QString filePath = url.toLocalFile();
                QPixmap pixmap(filePath);
                if (!pixmap.isNull())
                {
                    // Clear scene before adding next image
                    scene->clear();
                    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);
                    scene->addItem(item);

                    // adjsut aspect ratio
                    scene->setSceneRect(pixmap.rect());
                    setSceneRect(scene->sceneRect());
                    fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

                    emit footDropped(pixmap);
                    event->acceptProposedAction();
                    return;
                }
            }
        }
    }
}

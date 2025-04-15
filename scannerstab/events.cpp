#include "events.h"
#include <QUrl>
#include <QDebug>

Custom_View::Custom_View(QWidget* parent) : QGraphicsView(parent)
{
    setAcceptDrops(true);
    m_scene = new QGraphicsScene(this);
    setScene(m_scene);
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
                    m_scene->clear();
                    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);
                    m_scene->addItem(item);

                    // adjsut aspect ratio
                    m_scene->setSceneRect(pixmap.rect());
                    setSceneRect(m_scene->sceneRect());
                    fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);

                    emit footDropped(pixmap);
                    event->acceptProposedAction();
                    return;
                }
            }
        }
    }
}

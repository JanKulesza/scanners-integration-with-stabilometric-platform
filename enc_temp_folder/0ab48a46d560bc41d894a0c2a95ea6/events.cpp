#include "events.h"

Events::Events()
{
}

Custom_View::Custom_View(QWidget* parent) : QGraphicsView(parent) // No default argument here
{
    setAcceptDrops(true);
    scene = new QGraphicsScene(this);
    setScene(scene);
}

void Custom_View::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls())
    {
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
    if (event->source() == this) return;

    if (event->mimeData()->hasUrls())
    {
        QList<QUrl> urllist = event->mimeData()->urls();
        foreach(QUrl url, urllist)
        {
            if (url.isLocalFile())
            {
                QString FilePath = url.toLocalFile();
                QPixmap pixmap(FilePath);
                if (!pixmap.isNull())
                {
                    if (scene->items().count() > 0) scene->clear(); // Clear scene when dropping a new image file

                    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);

                    scene->setSceneRect(pixmap.rect());
                    setSceneRect(scene->sceneRect());
                    fitInView(scene->sceneRect(), Qt::KeepAspectRatio); // Fit image in GraphicsView

                    scene->addItem(item);
                    event->acceptProposedAction();
                }
            }
        }
    }
}
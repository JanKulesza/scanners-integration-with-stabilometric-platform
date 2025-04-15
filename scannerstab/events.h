#ifndef EVENTS_H
#define EVENTS_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QWidget>
#include <QMimeData>
#include <QGraphicsPixmapItem>
#include <QUrl>
#include <QPixmap>

class Custom_View : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Custom_View(QWidget* parent = nullptr);

signals:
    void footDropped(const QPixmap& pix);

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private:
    // name m_scene to not override graphicsView properties
    QGraphicsScene* m_scene;
};

#endif // EVENTS_H

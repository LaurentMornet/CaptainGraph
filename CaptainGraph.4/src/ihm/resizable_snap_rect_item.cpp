#include "resizable_snap_rect_item.h"
#include "configuration_frame.h"
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <cmath>
#include <QPen>
#include <QBrush>

ResizableSnapRectItem::ResizableSnapRectItem(
    short idx,
    const QRectF& rect, 
    QGraphicsItem* parent, 
    ConfigurationFrame* configuration_frame)
    : QObject(),
    QGraphicsRectItem(snapped_rect(rect, configuration_frame->grid_step()), parent),
    _configuration_frame(configuration_frame),
    _original_rect(rect) {
    _rectangle_idx = idx;
    setFlags(
        ItemIsMovable |
        ItemIsSelectable |
        ItemSendsGeometryChanges
    );
    _original_rect.setTopLeft(snap_point(rect.topLeft()));
    setAcceptHoverEvents(true);
    setPen(QPen(Qt::white, 2));
    setBrush(QBrush(QColor(0, 120, 255, 80)));
}
QRectF ResizableSnapRectItem::snapped_rect(const QRectF& rect, double grid_step) {
    auto soft_snap = [grid_step](double value) {
        return std::floor(value / grid_step) * grid_step;
    };
    // Snap du coin supérieur gauche
    double snapped_left = soft_snap(rect.left());
    double snapped_top = soft_snap(rect.top());
    // Snap de la taille (largeur et hauteur)
    double snapped_width = std::max(
        MIN_SIZE,  // Ne pas descendre en dessous de la taille minimale
        soft_snap(rect.width())
    );
    double snapped_height = std::max(
        MIN_SIZE,
        soft_snap(rect.height())
    );
    return QRectF(snapped_left, snapped_top, snapped_width, snapped_height);
}
// ------------------ SNAP SOUPLE ----------------------
double ResizableSnapRectItem::soft_snap(double value) const {
    double grid_step = _configuration_frame->grid_step();
    double snapped = std::floor(value / grid_step) * grid_step;
    if (std::abs(value - snapped) < SNAP_RANGE)
        return snapped;
    return value;
}
QPointF ResizableSnapRectItem::snap_point(const QPointF& point) const {
    return QPointF(soft_snap(point.x()), soft_snap(point.y()));
}
QPointF ResizableSnapRectItem::snap_position(const QPointF& position) const {
    // Pour le déplacement, on snap la position de l'item
    return QPointF(soft_snap(position.x()), soft_snap(position.y()));
}
// ------------------ HANDLES --------------------------
QRectF ResizableSnapRectItem::handle_rect(const QPointF& c) const {
    return QRectF(
        c.x() - HANDLE_SIZE / 2.0,
        c.y() - HANDLE_SIZE / 2.0,
        HANDLE_SIZE,
        HANDLE_SIZE
    );
}
ResizableSnapRectItem::HandleType ResizableSnapRectItem::detect_handle(const QPointF& pos) const {
    QRectF r = rect();

    if (handle_rect(r.topLeft()).contains(pos))     return HandleType::ResizeTL;
    if (handle_rect(r.topRight()).contains(pos))    return HandleType::ResizeTR;
    if (handle_rect(r.bottomLeft()).contains(pos))  return HandleType::ResizeBL;
    if (handle_rect(r.bottomRight()).contains(pos)) return HandleType::ResizeBR;

    if (r.contains(pos)) return HandleType::Move;
    return HandleType::None;
}
// ------------------ INTERACTION AMÉLIORÉE ----------------------
void ResizableSnapRectItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    _current_handle = detect_handle(event->pos());
    
    if (_current_handle == HandleType::Move) {
        _last_scene_pos = event->scenePos();
        _drag_start_pos = pos();
        // Désactiver temporairement le snap natif de Qt
        setFlag(ItemIsMovable, false);
    } else if (_current_handle != HandleType::None) {
        // Pour le redimensionnement
        _original_rect = rect();
        _last_scene_pos = event->scenePos();
    }
    
    event->accept();  // Important: accepter l'événement
}
void ResizableSnapRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (_current_handle == HandleType::Move) {
        // Calcul simple du déplacement
        QPointF scene_pos = event->scenePos();
        QPointF delta = scene_pos - _last_scene_pos;
        // Appliquer le déplacement
        QPointF new_pos = _drag_start_pos + delta;
        // Appliquer le snap
        new_pos = snap_position(new_pos);
        // Mettre à jour la position
        setPos(new_pos);
        // Mettre à jour l'affichage
        update();
    } else if (_current_handle != HandleType::None) {
        // Redimensionnement
        QPointF scene_pos = event->scenePos();
        QPointF local_pos = mapFromScene(scene_pos);
        QRectF r = _original_rect;
        switch (_current_handle) {
            case HandleType::ResizeTL: {
                QPointF snapped = snap_point(local_pos);
                r.setTopLeft(snapped);
                break;
            }
            case HandleType::ResizeTR: {
                QPointF snapped = snap_point(local_pos);
                r.setTopRight(snapped);
                break;
            }
            case HandleType::ResizeBL: {
                QPointF snapped = snap_point(local_pos);
                r.setBottomLeft(snapped);
                break;
            }
            case HandleType::ResizeBR: {
                QPointF snapped = snap_point(local_pos);
                r.setBottomRight(snapped);
                break;
            }
            default: break;
        }
        // Contraintes de taille minimale
        if (r.width() < MIN_SIZE) {
            if (_current_handle == HandleType::ResizeTL || _current_handle == HandleType::ResizeBL)
                r.setLeft(r.right() - MIN_SIZE);
            else
                r.setRight(r.left() + MIN_SIZE);
        }
        
        if (r.height() < MIN_SIZE) {
            if (_current_handle == HandleType::ResizeTL || _current_handle == HandleType::ResizeTR)
                r.setTop(r.bottom() - MIN_SIZE);
            else
                r.setBottom(r.top() + MIN_SIZE);
        }
        
        prepareGeometryChange();
        setRect(r.normalized());
    }
    
    event->accept();
}
QRectF ResizableSnapRectItem::scene_rectangle() const {
    return sceneBoundingRect();
}
void ResizableSnapRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (_current_handle == HandleType::Move) {
        // Snap final
        QPointF final_pos = snap_position(pos());
        setPos(final_pos);
        // Réactiver le déplacement natif
        setFlag(ItemIsMovable, true);
    } else if (_current_handle != HandleType::None) {
        // Snap final pour le redimensionnement
        QRectF r = rect();
        QPointF tl = mapToScene(r.topLeft());
        QPointF br = mapToScene(r.bottomRight());
        tl = snap_point(tl);
        br = snap_point(br);
        QPointF local_tl = mapFromScene(tl);
        QPointF local_br = mapFromScene(br);
        prepareGeometryChange();
        setRect(QRectF(local_tl, local_br).normalized());
    }
    emit rectangle_changed(_rectangle_idx, scene_rectangle());
    _current_handle = HandleType::None;
    event->accept();
}
void ResizableSnapRectItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event) {
    auto h = detect_handle(event->pos());
    switch (h) {
    case HandleType::ResizeTL:
    case HandleType::ResizeBR:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case HandleType::ResizeTR:
    case HandleType::ResizeBL:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case HandleType::Move:
        setCursor(Qt::OpenHandCursor);  // Curseur plus approprié
        break;
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }
    event->accept();
}
// ------------------ CONVERSION % ----------------------
QRectF ResizableSnapRectItem::logical_rect(const QSizeF& scene_size) const {
    QRectF r = sceneBoundingRect();

    return QRectF(
        r.left()   / scene_size.width(),
        r.top()    / scene_size.height(),
        r.width()  / scene_size.width(),
        r.height() / scene_size.height()
    );
}
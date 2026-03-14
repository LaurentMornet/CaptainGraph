#pragma once

#include <QGraphicsRectItem>
#include <QObject>

class ConfigurationFrame;

class ResizableSnapRectItem : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    explicit ResizableSnapRectItem(short i, const QRectF& rect, QGraphicsItem* parent, ConfigurationFrame* configuration_frame=nullptr);
    static QRectF snapped_rect(const QRectF& rect, double grid_step);
    QRectF logical_rect(const QSizeF& scene_size) const;
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
private:
    enum class HandleType {
        None=0,
        Move=1,
        ResizeTL=2, 
        ResizeTR=3, 
        ResizeBL=4, 
        ResizeBR=5
    };
    ConfigurationFrame* _configuration_frame;
    short _rectangle_idx;
    HandleType _current_handle = HandleType::None;
    QPointF _last_scene_pos;    // Coordonnées de la scène au début du déplacement
    QPointF _drag_start_pos;    // Position initiale de l'item au début du déplacement
    QRectF _original_rect;      // Rectangle original pour redimensionnement

    constexpr static double HANDLE_SIZE = 10.0;
    constexpr static double SNAP_RANGE  = 5.0;
    constexpr static double MIN_SIZE    = 10.0;

    HandleType detect_handle(const QPointF& pos) const;
    QRectF handle_rect(const QPointF& c) const;
    double soft_snap(double value) const;
    QPointF snap_point(const QPointF& point) const;
    
    // méthode pour snap de déplacement
    QPointF snap_position(const QPointF& position) const;
    QRectF scene_rectangle() const;
signals:
    void rectangle_changed(int, QRectF);
};
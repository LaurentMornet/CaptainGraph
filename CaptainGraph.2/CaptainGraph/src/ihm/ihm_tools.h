#pragma once

#include <QApplication>
#include <QIcon>
#include <QPainter>
#include <QPalette>
#include <QPixmap>

class IhmTools {
public:
    static inline bool is_dark_mode() noexcept {
        const QColor bg = qApp->palette().color(QPalette::Window);
        return bg.value() < 128;
    }
    static inline QIcon load_colored_icon(const QString& path)  {
        QColor color = IhmTools::is_dark_mode() ? Qt::white : Qt::black;
        QImage img(path);
        if (img.isNull()) {
            QPixmap px(path);
            if (px.isNull()) {
                qWarning() << "⚠️ Impossible de charger l'icône:" << path;
                return QIcon();
            }
            img = px.toImage();
        }

        QPainter p(&img);
        p.setCompositionMode(QPainter::CompositionMode_SourceIn);
        p.fillRect(img.rect(), color);
        p.end();

        return QIcon(QPixmap::fromImage(img));
    }
};
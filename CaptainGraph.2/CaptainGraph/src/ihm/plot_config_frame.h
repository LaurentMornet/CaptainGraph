#pragma once

#include <cfloat>
#include <map>
#include <vector>
#include <string>
#include <variant>

#include <QFrame>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QStyleFactory>
#include <QPushButton>
#include <QTableView>
#include <QList>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QVariant>
#include <QHeaderView>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QColorDialog>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QToolBar>
#include <QPointer>
#include <QUndoCommand>

#include "../common/plot.h"
#include "undo_manager.h"
#include "graph_config_frame.h"

// class PlotChangeCommand: public QUndoCommand {
// private:
//     bool _style=true;
//     Plot* _plot;
//     std::string _key;
//     StyleItemValue _old_value;
//     StyleItemValue _new_value;
//     std::map<std::string, StyleItemValue> _old_styledef;
//     std::map<std::string, StyleItemValue> _new_styledef;
//     QAbstractItemModel* _model;
//     QModelIndex _index;
//     int _param;
// public:
//     PlotChangeCommand(Plot* plot, const std::string& key,
//         const StyleItemValue& old_value,
//         const StyleItemValue& new_value,
//         QAbstractItemModel* model,
//         const QModelIndex& index);
//     PlotChangeCommand(Plot* plot, const std::string& key,
//         const StyleItemValue& old_value,
//         const StyleItemValue& new_value,
//         int param);
//     PlotChangeCommand(Plot* plot, const std::string& key,
//         const std::map<std::string, StyleItemValue> old_value,
//         QAbstractItemModel* model,
//         const QModelIndex& index);
//     void undo() override;
//     void redo() override;
// };

/* ========================================================================== */
/*                              PlotContext                               */
/* ========================================================================== */

/**
 * @brief Structure de contexte pour relier une cellule de la table à son style Plot.
 */
struct PlotContext {
    Plot* plot = nullptr;
    std::vector<std::string> keys;
    std::string key;
    std::string label;
    StyleItemType type;
    StyleItemValue value;
    std::vector<StyleItemArg> args;
};

class PlotConfigFrame;

/* ========================================================================== */
/*                              StyleDelegate                                 */
/* ========================================================================== */

/**
 * @brief Délégué de style Qt pour éditer les valeurs des items de style dans la table.
 */
class StyleDelegate : public QStyledItemDelegate {
    Q_OBJECT
private:
    PlotConfigFrame* _frame;

public:
    explicit StyleDelegate(PlotConfigFrame* frame, QObject* parent = nullptr);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                              const QModelIndex& index) const override;
};

/* ========================================================================== */
/*                             StyleTableModel                                */
/* ========================================================================== */

/**
 * @brief Modèle Qt représentant les paramètres de style d’un Plot.
 */
class StyleTableModel : public QStandardItemModel {
    Q_OBJECT
private:
    PlotConfigFrame* _frame;

public:
    explicit StyleTableModel(PlotConfigFrame* frame);

    void set_style_data();
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
};

/* ========================================================================== */
/*                              PlotConfigFrame                               */
/* ========================================================================== */

/**
 * @brief Widget principal d’édition des propriétés d’un Plot.
 *
 * Ce widget permet de :
 *  - Modifier le titre et le type de graphique
 *  - Configurer les "features"
 *  - Ajuster les paramètres de style (couleurs, lignes, marqueurs, etc.)
 *  - Appliquer ou réinitialiser les modifications
 */
class PlotConfigFrame : public QFrame {
    Q_OBJECT
    friend class StyleTableModel;
    friend class StyleDelegate;

private:
    /* === Données === */
    std::map<std::string, Plot> _buffer_plots;  ///< Copies tampon pour édition
    // Plot* _ref_plot;                            ///< Référence vers le modèle source
    PlotAdapter* _plot_adapter;
    bool _inhibit_type_callback;
    /* === Widgets === */
    QLineEdit* _edt_label;
    QComboBox* _cb_type;
    QToolBar* _toolbar;
    QFrame* _box_header;
    QFrame* _box_features; 
    QList<QLineEdit*> _edt_features;
    QList<QLabel*> _lbl_features;
    StyleTableModel* _table_style_model;
    QTableView* _table_style;

    QSize sizeHint() const override;

    /* === Méthodes internes === */
    PlotContext context_for_index(const QModelIndex& index) const;
    void _update_features_form();

    /** 
     * @brief Initialise une QComboBox avec des icônes recolorées (adaptées au thème clair/sombre).
     */
    void _populate_icon_combo(QComboBox* combo,
                              const std::string& custom_icon_map_id,
                              const QSize& icon_size) const;

private slots:
    void _cb_type_changed(int index);
    void _cb_reset_defaults();
    void _cb_label_changed();
    void _cb_feature_changed(int index, QString value);
    void _cb_delete_plot();
private:
    explicit PlotConfigFrame(QWidget* parent = nullptr);
    virtual ~PlotConfigFrame() = default;
public:
    static PlotConfigFrame * instance;
    static PlotConfigFrame * get_instance(QWidget * parent = nullptr);

    /** @brief Affecte le Plot à éditer (copié en interne). */
    void set_plot_Adapter(PlotAdapter* plot_adapter);
};


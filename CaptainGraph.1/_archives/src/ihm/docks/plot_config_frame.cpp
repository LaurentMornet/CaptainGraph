#include "plot_config_frame.h"

PlotConfigFrame::PlotConfigFrame(QWidget * parent)
    : QFrame(parent), _basic(true), _metaplot(nullptr)  {
    _setup();
}

void PlotConfigFrame::_setup() {
    _layout = new QVBoxLayout(this);
    _edt_title = new AutoLabelWidget(this, "Mon premier plot", true);
    connect(_edt_title, &AutoLabelWidget::signal_auto_checked, this, &PlotConfigFrame::_cb_auto_title);
    _cb_auto_title();
    setLayout(_layout);
    _layout->addWidget(_edt_title);
    connect(this, &PlotConfigFrame::selected, this, &PlotConfigFrame::slot_selected);

    _cb_type = new QComboBox(this);
    _cb_type->setStyle(QStyleFactory::create("Fusion"));
    _cb_type->setIconSize(QSize(50, 50));
    _cb_type->setFixedHeight(50);
    RESOURCES   += resources/resources.qrc
    _cb_type->addItem(QIcon(":/resources/metaplots/bar.png"), "Bar");
    _cb_type->addItem(QIcon(":/resources/metaplots/scatter3d.png"), "Scatter 3D");
    _cb_type->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(_cb_type, &QComboBox::currentIndexChanged, this, &PlotConfigFrame::_cb_type_changed);
    _layout->addWidget(_cb_type);

    _stack_style = new QStackedWidget(this);
    _scatter_frame = new GenericStyleFrame(this);
    QList<StyleFieldDesc> scatter_fields = {
        {"line_color", "Couleur de ligne", FieldType::Color, "#0000ff"},
        {"line_style", "Style de ligne", FieldType::Enum, "solid", {"solid", "dash", "dot"}},
        {"marker_size", "Taille Marqueur", FieldType::Int, 5},
        {"show_legend", "Afficher la légende", FieldType::Bool, true}
    };
    _scatter_frame->set_fields(scatter_fields);
    ScatterStyle scatter_style;
    scatter_style.line_color = "red";
    scatter_style.marker_size = 10;
    _scatter_frame->set_values(StyleMapper::to_map(&scatter_style));
    // _scatter_frame = new ScatterStyleFrame(this);
    _bar_frame = new BarStyleFrame(this);
    _stack_style->addWidget(_scatter_frame);
    _stack_style->addWidget(_bar_frame);
    _layout->addWidget(_stack_style);

    auto * validate_btn = new QPushButton(this);
    _layout->addWidget(validate_btn);
    _current_plot = std::make_unique<ScatterMetaPlot>();
    // _scatter_frame->set_style(_current_plot->style().get());

    connect(validate_btn, &QPushButton::clicked, this, &PlotConfigFrame::_cb_validate);
}
/**
 * A supprimer, pas important pour l'instant
 * mais démontre l'utilisation du "auto label"
 */
void PlotConfigFrame::_cb_auto_title() {
    _edt_title->set_auto_text(_metaplot ? QString(_metaplot->title().c_str()): "");
}
/**
 * Lorsqu'un plot est sélectionné: callback
 */
void PlotConfigFrame::slot_selected(MetaPlot * metaplot) {
    _metaplot = metaplot;
    _edt_title->set_noauto_text(_metaplot ? QString(_metaplot->title().c_str()): "");
    _cb_auto_title();
}
/**
 * Changement de type: adaptation du widget
 */
void PlotConfigFrame::_cb_type_changed(int selection) {
    if(_cb_type->currentText() == "Scatter") {
        _stack_style->setCurrentWidget(_scatter_frame);
    }
    else if(_cb_type->currentText() == "Bar") {
        _stack_style->setCurrentWidget(_bar_frame);
    }
}
/**
 * Validation des données et recopie
 * les informations dans le plot correspondant
 */
void PlotConfigFrame::_cb_validate() {
    IStyleEditor * editor = qobject_cast<IStyleEditor*>(_stack_style->currentWidget());
    if(!editor) return;
    std::unique_ptr<BaseStyle> new_style = editor->get_style();
    _current_plot->set_style(new_style);
    qDebug() << "style appliqué";
}
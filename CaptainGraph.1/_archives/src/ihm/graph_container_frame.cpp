#include "graph_container_frame.h"

GraphContainerFrame::GraphContainerFrame(QWidget * parent)
    :QFrame(parent) {
    setFrameShape(QFrame::Shape::StyledPanel);
    auto * layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    setMinimumSize(GraphContainerFrame::MIN_CHILD, GraphContainerFrame::MIN_CHILD);
    // Split menu
    auto * toolbar = new QToolBar(this);
    toolbar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    toolbar->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    toolbar->setContentsMargins(0, 0, 0, 0);

    toolbar->setStyleSheet("background-color: transparent");
    layout->addWidget(toolbar);
    
    auto * menubutton = new QToolButton(this);
    menubutton->setFixedSize(15, 15);
    menubutton->setIcon(this->style()->standardIcon(QStyle::StandardPixmap::SP_FileDialogListView));
    menubutton->setText("Split/Merge");
    menubutton->setPopupMode(QToolButton::InstantPopup);
    auto * menu = new QMenu(this);
    // split actions
    auto * act_h = menu->addAction(this->style()->standardIcon(QStyle::StandardPixmap::SP_ToolBarHorizontalExtensionButton), "Horizontal split");
    connect(act_h, &QAction::triggered, this, [this]() { this->_split(Qt::Orientation::Horizontal); });
    auto * act_v = menu->addAction(this->style()->standardIcon(QStyle::StandardPixmap::SP_ToolBarVerticalExtensionButton), "Vertical split");
    connect(act_v, &QAction::triggered, this, [this]() { this->_split(Qt::Orientation::Vertical); });
    menubutton->setMenu(menu);
    toolbar->addWidget(menubutton);
    // merge actions
    auto * merge_menu = menu->addMenu(QStringLiteral(" Merge"));
    _act_merge_left = merge_menu->addAction(this->style()->standardIcon(QStyle::StandardPixmap::SP_ArrowLeft), "With Left");
    connect(_act_merge_left, &QAction::triggered, this, [this]() { this->_merge("left"); });
    _act_merge_right = merge_menu->addAction(this->style()->standardIcon(QStyle::StandardPixmap::SP_ArrowRight), "With Right");
    connect(_act_merge_right, &QAction::triggered, this, [this]() { this->_merge("right"); }); 
    _act_merge_up = merge_menu->addAction(this->style()->standardIcon(QStyle::StandardPixmap::SP_ArrowUp), "With Up");
    connect(_act_merge_up, &QAction::triggered, this, [this]() { this->_merge("up"); });
    _act_merge_down = merge_menu->addAction(this->style()->standardIcon(QStyle::StandardPixmap::SP_ArrowDown), "With Down");
    connect(_act_merge_down, &QAction::triggered, this, [this]() { this->_merge("down"); });
    // Activation / Désactivation des options de merge dans le menu
    connect(menu, &QMenu::aboutToShow, this, &GraphContainerFrame::_update_merge_actions);
    // Contents
    GraphFrame * _content_frame = new GraphFrame(this);
    layout->addWidget(_content_frame);
}
/**
 * ------------ MERGE ------------
 */
/**
 * Active ou désactive les options de merge selon la position dans le splitter
 */
void GraphContainerFrame::_update_merge_actions() {
    QSplitter * parent = (QSplitter*)parentWidget();
    int idx = parent->indexOf(this);
    bool is_horizontal = parent->orientation() == Qt::Orientation::Horizontal;
    _act_merge_left->setEnabled(is_horizontal && idx > 0);
    _act_merge_right->setEnabled(is_horizontal && idx < parent->count() - 1);
    _act_merge_up->setEnabled(!is_horizontal && idx > 0);
    _act_merge_down->setEnabled(!is_horizontal && idx < parent->count() - 1);
}
void GraphContainerFrame::_merge(const QString & direction) {
    QSplitter * parent = (QSplitter*)parentWidget();
    int idx = parent->indexOf(this);
    if(parent->orientation() == Qt::Orientation::Horizontal) {
        if(direction == "left" && idx > 0)
            _delete_widget(parent->widget(idx - 1));
        else if(direction == "right" && idx < parent->count() - 1)
            _delete_widget(parent->widget(idx + 1));
    }
    else if(parent->orientation() == Qt::Orientation::Vertical) {
        if(direction == "up" && idx > 0)
            _delete_widget(parent->widget(idx - 1));
        else if(direction == "down" && idx < parent->count() - 1)
            _delete_widget(parent->widget(idx + 1));
    }
    if(parent->count() == 1) {
        auto * only_child = parent->widget(0);
        auto * gparent = parent->parentWidget();
        if(gparent->metaObject()->className() == QStringLiteral("QSplitter")) {
            auto gidx = ((QSplitter*)gparent)->indexOf(parent);
            parent->setParent(nullptr);
            ((QSplitter*)gparent)->insertWidget(gidx, only_child);
        }
    }
}
void GraphContainerFrame::_delete_widget(QWidget * widget) {
    if(widget->metaObject()->className() == QStringLiteral("QSplitter")) {
        while(((QSplitter*)widget)->count() > 0) {
            auto * child = ((QSplitter*)widget)->widget(0);
            child->setParent(nullptr);
            child->deleteLater();
        }
        widget->setParent(nullptr);
        widget->deleteLater();
    }
    else {
        widget->setParent(nullptr);
        widget->deleteLater();
    }
}
/**
 * SPLIT
 */
void GraphContainerFrame::_split(Qt::Orientation orientation) {
    QSplitter * parent = (QSplitter*)parentWidget();
    if(!parent) return;
    int idx = parent->indexOf(this);
    QList<int> prev_sizes = parent->sizes();
    if(parent->orientation() == orientation) {
        auto * new_frame = new GraphContainerFrame(parent);
        parent->insertWidget(idx+1, new_frame);
        QTimer::singleShot(0, [this, parent, prev_sizes, idx]() {
            this->_restore_sizes_after_insert_same_orientation(parent, prev_sizes, idx);
        });
    }
    else {
        auto * new_splitter = new QSplitter(orientation, parent);
        parent->insertWidget(idx, new_splitter);
        new_splitter->addWidget(this);
        new_splitter->addWidget(new GraphContainerFrame(parent));
        QTimer::singleShot(0, [this, parent, new_splitter, prev_sizes, idx]() {
            this->_restore_sizes_after_nested_insert(parent, new_splitter, prev_sizes, idx);
        });
    }
}

void GraphContainerFrame::_restore_sizes_after_insert_same_orientation(
        QSplitter * parent,
        QList<int> prev_sizes,
        int idx) {
    int left = std::max(MIN_CHILD, prev_sizes[idx] / 2);
    int right = std::max(MIN_CHILD, prev_sizes[idx] - left);
    QList<int> new_sizes;
    for (int i = 0; i < idx; ++i)
        new_sizes.append(prev_sizes[i]);
    new_sizes.append(left);
    new_sizes.append(right);
    for (int i = idx + 1; i < prev_sizes.size(); ++i)
        new_sizes.append(prev_sizes[i]);
    parent->setSizes(new_sizes);
    for (int i = 0; i < parent->count(); ++i)
        parent->setStretchFactor(i, 1);
    parent->setChildrenCollapsible(false);
}

void GraphContainerFrame::_restore_sizes_after_nested_insert(
        QSplitter * parent,
        QSplitter * new_splitter,
        QList<int> prev_sizes,
        int idx) {
    parent->setSizes(prev_sizes);
    int space_for_nested = (idx < prev_sizes.size()) ? prev_sizes[idx] : 200;
    int a = std::max(MIN_CHILD, space_for_nested / 2);
    int b = std::max(MIN_CHILD, space_for_nested - a);
    QList<int> nested_sizes;
    nested_sizes.append(a);
    nested_sizes.append(b);
    new_splitter->setSizes(nested_sizes);
    for (int i = 0; i < new_splitter->count(); ++i)
        new_splitter->setStretchFactor(i, 1);
    new_splitter->setChildrenCollapsible(false);
}


void GraphContainerFrame::update_graph() {
    this->_content_frame->update_graph();
}
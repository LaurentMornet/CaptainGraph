#include "layout_panel.h"
#include <QMenu>
#include <QAction>
#include <QToolButton>
#include "ihm_tools.h"

LayoutPanel::LayoutPanel(LayoutAdapter* layout_adapter, const QString& layoutId, QWidget* parent): 
    QFrame(parent), 
    _layout_adapter(layout_adapter), 
    _id(layoutId) {

    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    setContentsMargins(0,0,0,0);

    auto* v = new QVBoxLayout(this);
    v->setContentsMargins(2,2,2,2);
    v->setSpacing(0);

    auto* qlayout = new QHBoxLayout();
    qlayout->setContentsMargins(0,0,0,0);
    qlayout->addStretch();

    auto * popup_btn = new QPushButton(IhmTools::load_colored_icon(":/resources/appicons/popup_menu.svg"), "", this);
    popup_btn->setFixedSize(20, 20);
    popup_btn->setFocusPolicy(Qt::NoFocus);
    qlayout->addWidget(popup_btn);
    connect(popup_btn, &QPushButton::clicked, this, [this]() { _toolbar->toggleViewAction()->trigger(); });

    _toolbar = new QToolBar(this);
    _toolbar->setIconSize(QSize(20, 20));
    _toolbar->setFixedHeight(36);
    QAction* act_splith = _toolbar->addAction(IhmTools::load_icon(":/resources/appicons/splith.svg"), "Split Horizontally");
    QAction* act_splitv = _toolbar->addAction(IhmTools::load_icon(":/resources/appicons/splitv.svg"), "Split Vertically");
    QAction* act_merge_top = _toolbar->addAction(IhmTools::load_icon(":/resources/appicons/merge_top.svg"), "Merge with top sibling");
    QAction* act_merge_right = _toolbar->addAction(IhmTools::load_icon(":/resources/appicons/merge_right.svg"), "Merge with right sibling");
    QAction* act_merge_bottom = _toolbar->addAction(IhmTools::load_icon(":/resources/appicons/merge_bottom.svg"), "Merge with bottom sibling");
    QAction* act_merge_left = _toolbar->addAction(IhmTools::load_icon(":/resources/appicons/merge_left.svg"), "Merge with left sibling");

    if(!_layout_adapter->can_merge(_id, MergeDirection::Top))    act_merge_top->setVisible(false);
    if(!_layout_adapter->can_merge(_id, MergeDirection::Right))  act_merge_right->setVisible(false);
    if(!_layout_adapter->can_merge(_id, MergeDirection::Bottom)) act_merge_bottom->setVisible(false);
    if(!_layout_adapter->can_merge(_id, MergeDirection::Left))   act_merge_left->setVisible(false);

    connect(act_splith, &QAction::triggered, this, [this](){ emit request_split(_id, LayoutType::Horizontal); });
    connect(act_splitv, &QAction::triggered, this, [this](){ emit request_split(_id, LayoutType::Vertical); });
    connect(act_merge_top, &QAction::triggered, this, [this]() { emit request_merge(_id, MergeDirection::Top); });
    connect(act_merge_right, &QAction::triggered, this, [this]() { emit request_merge(_id, MergeDirection::Right); });
    connect(act_merge_bottom, &QAction::triggered, this, [this]() { emit request_merge(_id, MergeDirection::Bottom); });
    connect(act_merge_left, &QAction::triggered, this, [this]() { emit request_merge(_id, MergeDirection::Left); });

    _toolbar->toggleViewAction()->setChecked(false);
    _toolbar->setVisible(false);
    qlayout->addWidget(_toolbar);
    v->addLayout(qlayout);
    v->addStretch();
}
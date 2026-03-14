#include "undo_manager.h"
UndoManager * UndoManager::instance = nullptr;
UndoManager::UndoManager(QObject * parent) : QObject(parent) {
    _undo_stack = new QUndoStack(this);
}

UndoManager * UndoManager::get_instance(QObject * parent) {
    if(!UndoManager::instance && parent)
        instance = new UndoManager(parent);
    return instance;
}

void UndoManager::push(QUndoCommand * cmd) {
    UndoManager::get_instance()->undo_stack()->push(cmd);
}

QUndoStack * UndoManager::undo_stack() {
    return _undo_stack;
}
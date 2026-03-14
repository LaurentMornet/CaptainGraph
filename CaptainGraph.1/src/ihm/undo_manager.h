#pragma once

#include <QUndoStack>

class UndoManager: public QObject {
    Q_OBJECT
private:
    static UndoManager * instance;
    explicit UndoManager(QObject * parent);
    virtual ~UndoManager() = default;
    QUndoStack * _undo_stack;
public:
    static UndoManager * get_instance(QObject * object = nullptr);
    static void push(QUndoCommand * cmd);
    QUndoStack * undo_stack();
};
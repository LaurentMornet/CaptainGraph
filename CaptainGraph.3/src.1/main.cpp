#include "main.h"
#include <QStyleFactory>
#include <QLocale>

int main(int argc, char** argv) {
    Manager manager;

    QLocale curLocale(QLocale("C"));
    QLocale::setDefault(curLocale);

    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("fusion"));
    MainWindow main_window(&manager);
    main_window.show();
    return app.exec();
}
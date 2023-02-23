#include "MainWindow.h"

#include <QApplication>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    MainWindow tab;
    tab.show();

    return app.exec();
}

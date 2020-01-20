#include <QApplication>
#include "window.h"

#include "httpwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    window MyWindow;

    //HttpWindow w;
    //w.show();






    MyWindow.showMaximized();
    MyWindow.changeMargin();








    return app.exec();


}






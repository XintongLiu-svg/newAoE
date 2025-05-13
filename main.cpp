#include "MainWidget.h"
#include <QApplication>
#include <QResource>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QResource::registerResource("D:/gamee/newAOEv202g/res.rcc");

    int MapJudge = 0;
    QStringList args = a.arguments();
    if (args.indexOf("-f") != -1)
    {
        MapJudge = 1;
    }
    if(args.indexOf("-r") != -1)
    {
        MapJudge = 2;
    }
    MainWidget w(MapJudge);
    w.show();

    return a.exec();
}

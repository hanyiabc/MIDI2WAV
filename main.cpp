// TODO: implement your entry point here
#include <qapplication.h>
#include <qinstrument.hpp>
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QInstrument widget;
	widget.show();
	app.exec();
	return 0;
}

#include <QApplication>
#include "midiplayer.hpp"
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MIDIPlayer widget;
	widget.show();
	app.exec();
  return 0;
}

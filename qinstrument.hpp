#ifndef QINSTRUMENT_HPP
#define QINSTRUMENT_HPP
#include <qwidget.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qdebug.h>
#include <qstring.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qgroupbox.h>
#include <qboxlayout.h>
#include <qdir.h>
#include <qfiledialog.h>
#include <interactive_instrument.hpp>
#include "track.hpp"
#include "signal.hpp"
#include "midi.hpp"
#include <string>
#include "wav.hpp"
using std::string;
using std::vector;
// TODO 
class QInstrument : public QWidget 
{
	Q_OBJECT
public:
	QInstrument(QWidget * parent = nullptr);
	//~QInstrument();
private slots:
	void browseIn();
	void browseOut();
	void synthesize();
	void enableFirst(bool enabled);
	void enableSecond(bool enabled);

	void changeAttack(double d);
	void changeVelocity(double d);
	void changeDecay(double d);
	void changeRelease(double d);


private:
	bool enabled1 = false;
	bool enabled2 = false;
	double attackValue;
	double velocityValue;
	double decayValue;
	double releaseValue;
	QLineEdit * inputpath;
	QLineEdit * outputpath;
	QPushButton * synth;
	QGroupBox * InsPara();
	QGroupBox * MIDIin();
	QGroupBox * WAVout();
	QMessageBox message;

	
};


#endif
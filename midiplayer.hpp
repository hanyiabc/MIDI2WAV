#ifndef MIDIPLAYER_HPP
#define MIDIPLAYER_HPP
#include <qwidget.h>
#include <qdebug.h>
#include <qstring.h>
#include <qlineedit.h>
#include <qgroupbox.h>
#include <qboxlayout.h>
#include <qpushbutton.h>
#include <qdir.h>
#include <qlabel.h>
#include <qfiledialog.h>
#include <qslider.h>
#include "midi.hpp"
#include "track.hpp"
#include <qmessagebox.h>
#include <qiodevice.h>
#include <qaudiooutput.h>
#include "instrument.hpp"
#include "shared_double_buffer.hpp"
#include <thread>
#include <string>
#include <qevent.h>
#include "synthesizer.hpp"
#include <cmath>
#include <vector>
using std::vector;
// define class MIDIPlayer here
class MIDIPlayer : public QWidget
{
	Q_OBJECT
public:
	MIDIPlayer(QWidget * parent = nullptr);
	~MIDIPlayer();

private:
	QLineEdit * input;
	QGroupBox * AudioCtrl();
	QGroupBox * MIDIin();
	QMessageBox message;

	QAudioOutput *audio;
	QIODevice *device;
	std::size_t buffer_size;
	DoubleBuffer buffer;
	vector<char> innerBuffer;
	Track myTrack;
	int sampleRate;
	MessageQueue events;
	Synthesizer synth;
	std::thread th1;
	TrackListType TrackList;
	//void closeEvent(QCloseEvent *closeEvent);
	double volumeValue;
private slots:

	void play();
	void browseIn();
	void pause();
	void stop();
	void handleNotify();
	void load();
	void handleStateChanged();
	void changeVolume();
	void mute();
	void enterLoad();
};


#endif

#include "midiplayer.hpp"
#include <fstream>
using std::ofstream;
ofstream out("log.hex");
const size_t DOUBLE_BUFFER_SIZE = 2048;
// Implement class MIDIPlayer here

MIDIPlayer::MIDIPlayer(QWidget * parent) : QWidget(parent)
{
	auto layout = new QVBoxLayout(this);
	auto inGroup = MIDIin();
	auto CtrlGroup = AudioCtrl();
	inGroup->setObjectName("ingroup");
	CtrlGroup->setObjectName("ctrlgroup");
	layout->addWidget(inGroup);
	layout->addWidget(CtrlGroup);
	setLayout(layout);

	QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
	auto rates = info.supportedSampleRates();

	sampleRate = rates.at(0);
	for (int i = 1; i < rates.size(); ++i) {
		if (rates.at(i) < sampleRate)
			sampleRate = rates.at(i);
	}

	QAudioFormat format;
	format.setSampleRate(sampleRate);
	format.setChannelCount(1);
	format.setSampleSize(16);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::SignedInt);

	if (!info.isFormatSupported(format)) {
		qWarning()
			<< "Raw audio format not supported by backend, cannot play audio.";
	}
	else {
		audio = new QAudioOutput(format);
		audio->setObjectName("audio");
		connect(audio, SIGNAL(notify()), this, SLOT(handleNotify()));
		connect(audio, &QAudioOutput::stateChanged, this, &MIDIPlayer::handleStateChanged);

		buffer_size = audio->bufferSize();
		buffer_size = buffer_size > 0 ? buffer_size : 1024;

		audio->setNotifyInterval(25);
	}
	synth = Synthesizer(&events, &myTrack, &buffer, 1);
	th1 = std::thread(synth);
	buffer.reserve(DOUBLE_BUFFER_SIZE);
	auto stop = this->findChild<QPushButton*>("stop");
	stop->setDisabled(true);
	auto pause = this->findChild<QPushButton*>("pause");
	pause->setDisabled(true);
}

MIDIPlayer::~MIDIPlayer()
{
	audio->stop();
	events.push("Q");
	events.push("Q");
	th1.join();
}

QGroupBox * MIDIPlayer::AudioCtrl()
{
	auto hbox1 = new QHBoxLayout;
	auto hbox2 = new QHBoxLayout;
	auto vbox = new QVBoxLayout;
	auto volumeLabel = new QLabel("Volume", this);
	auto volumeSlider = new QSlider(this);
	auto playButton = new QPushButton("Play", this);
	auto pauseButton = new QPushButton("Pause", this);
	auto stopButton = new QPushButton("Stop", this);
	auto muteButton = new QPushButton("Mute", this);
	volumeSlider->setObjectName("volume");
	playButton->setObjectName("play");
	pauseButton->setObjectName("pause");
	stopButton->setObjectName("stop");
	muteButton->setObjectName("mute");
	

	volumeSlider->setOrientation(Qt::Orientation::Horizontal);
	volumeSlider->setMaximum(100);
	volumeSlider->setMinimum(0);
	volumeSlider->setValue(50);
	
	hbox1->addWidget(playButton);
	hbox1->addWidget(pauseButton);
	hbox1->addWidget(stopButton);
	hbox2->addWidget(volumeLabel);
	hbox2->addWidget(volumeSlider);
	hbox2->addWidget(muteButton);
	auto groupBox = new QGroupBox("Audio Controls",this);
	vbox->addLayout(hbox1);
	vbox->addLayout(hbox2);
	groupBox->setLayout(vbox);

	connect(playButton, &QPushButton::clicked, this, &MIDIPlayer::play);
	connect(pauseButton, &QPushButton::clicked, this, &MIDIPlayer::pause);
	connect(stopButton, &QPushButton::clicked, this, &MIDIPlayer::stop);
	connect(volumeSlider, &QSlider::valueChanged, this, &MIDIPlayer::changeVolume);
	connect(muteButton, &QPushButton::clicked, this, &MIDIPlayer::mute);

	return groupBox;
}

QGroupBox * MIDIPlayer::MIDIin()
{
	input = new QLineEdit(this);
	auto hbox = new QHBoxLayout;
	auto button = new QPushButton("&Browse...",this);
	hbox->addWidget(input);
	input->setObjectName("inputpath");
	hbox->addWidget(button);
	auto * groupBox = new QGroupBox("MIDI Input");
	groupBox->setLayout(hbox);
	connect(button, &QPushButton::clicked, this, &MIDIPlayer::browseIn);
	connect(input, &QLineEdit::returnPressed, this, &MIDIPlayer::enterLoad);
	return groupBox;
}


void MIDIPlayer::browseIn()
{
	static QString lastTime(QDir::currentPath());
	
	lastTime = QDir::toNativeSeparators(QFileDialog::getOpenFileName(this, "Find MIDI FIle", lastTime, tr("MIDI Files (*.mid *.MID)")));
	if (!lastTime.isEmpty())
	{
		input->setText(lastTime);
		load();
	}
}

void MIDIPlayer::pause()
{
	audio->suspend();
}

void MIDIPlayer::stop()
{
	events.push("E");
	audio->stop();
}

void MIDIPlayer::play()
{
	
	if (audio->state()==QAudio::SuspendedState)
	{
		auto pause = this->findChild<QPushButton*>("pause");
		pause->setDisabled(false);
		audio->resume();
	}
	else
	{
		if (TrackList.empty())
		{

			message.setText("Invalid Input File/Path");
			message.exec();
		}
		else
		{
			auto box = this->findChild<QGroupBox *>("ingroup");
			box->setDisabled(true);
			auto play = this->findChild<QPushButton*>("play");
			play->setDisabled(true);
			myTrack = *TrackList.begin();
			events.push("S" + std::to_string(sampleRate));
			device = audio->start();
			handleNotify();
		}
		
	}
	
}

void MIDIPlayer::handleNotify()
{
	/*auto er = audio->error();
	switch (er)
	{
	case QAudio::NoError:
		qDebug() << "No Error";

		break;
	case QAudio::OpenError:
		qDebug() << "Open Error";

		break;
	case QAudio::IOError:
		qDebug() << "IO Error";

		break;
	case QAudio::UnderrunError:
		qDebug() << "UnderFlowerror";

		break;
	case QAudio::FatalError:
		qDebug() << "Fatal Error";

		break;
	default:
		break;
	}*/

	int bytesFree = audio->bytesFree();
	int bytesToWrite = bytesFree > buffer_size ? buffer_size : bytesFree;
	int samplesToWrite = bytesToWrite >> 1;
	bytesToWrite = 2 * samplesToWrite;

	innerBuffer.reserve(bytesToWrite);

	double sample;
	for (int i = 0; i < samplesToWrite; ++i)
	{
		sample = buffer.pop();
		int16_t temp = static_cast<int16_t>(lround(sample));
		char *byte1 = reinterpret_cast<char *>(&temp);
		char *byte2 = (byte1+1);
		innerBuffer.push_back(*byte1);
		innerBuffer.push_back(*byte2);
	}
	if (bytesToWrite > 0)
	{
			device->write(innerBuffer.data(), bytesToWrite);
	}
	innerBuffer.clear();

}
void MIDIPlayer::load()
{
	TrackList = readMIDIFromFile(input->text().toStdString());
}
void MIDIPlayer::handleStateChanged()
{
	auto state = audio->state();
	auto stop = this->findChild<QPushButton*>("stop");
	auto pause = this->findChild<QPushButton*>("pause");
	auto play = this->findChild<QPushButton*>("play");
	auto box = this->findChild<QGroupBox *>("ingroup");

	switch (state)
	{
	case QAudio::ActiveState:
		qDebug() << "ActiveState";
		stop->setDisabled(false);
		play->setDisabled(true);
		pause->setDisabled(false);
		break;
	case QAudio::SuspendedState:
		qDebug() << "SuspendedState";
		play->setDisabled(false);
		pause->setDisabled(true);
		break;
	case QAudio::StoppedState:
		qDebug() << "StoppedState";
		box->setDisabled(false);
		play->setDisabled(false);
		pause->setDisabled(true);
		stop->setDisabled(true);
		break;
	case QAudio::IdleState:
		qDebug() << "IdleState";
		break;
	default:
		break;
	}
}

void MIDIPlayer::changeVolume()
{
	audio->setVolume((this->findChild<QSlider*>("volume")->value()) / 100.0);
	qDebug() << audio->volume();
}

void MIDIPlayer::mute()
{
	this->findChild<QSlider*>("volume")->setValue(0.0);
}

void MIDIPlayer::enterLoad()
{
	load();
	if (TrackList.empty())
	{

		message.setText("Invalid Input File/Path");
		message.exec();
	}
}


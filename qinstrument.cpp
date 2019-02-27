#include "qinstrument.hpp"
#include <limits>
// TODO

QInstrument::QInstrument(QWidget * parent) : QWidget(parent)
{
	/*inputpath = new QLineEdit("D:\\OneDrive\\ECE3574_projs\\project2Examples\\test00.mid", this);
	outputpath = new QLineEdit("D:\\OneDrive\\ECE3574_projs\\compare\\out", this);*/
	inputpath = new QLineEdit(this);
	outputpath = new QLineEdit(this);
	synth = new QPushButton("Synthesize", this);
	inputpath->setObjectName("inputpath");
	outputpath->setObjectName("outputpath");
	synth->setObjectName("synth");
	auto layout = new QVBoxLayout;
	//auto layout = new QGridLayout;
	/*layout->addWidget(InsPara(), 0, 0);
	layout->addWidget(MIDIin(), 1, 0);
	layout->addWidget(WAVout(), 2, 0);
	layout->addWidget(synth, 3, 0);*/
	layout->addWidget(InsPara());
	layout->addWidget(MIDIin());
	layout->addWidget(WAVout());
	layout->addWidget(synth);
	setLayout(layout);

	connect(synth, &QPushButton::clicked, this, &QInstrument::synthesize);
}

QGroupBox * QInstrument::InsPara()
{
	QRadioButton * harmonic1 = new QRadioButton("Enable First Harmonic", this);
	QRadioButton * harmonic2 = new QRadioButton("Enable Second Harmonic", this);
	QDoubleSpinBox * velocity= new QDoubleSpinBox(this);
	QDoubleSpinBox * attack = new QDoubleSpinBox(this);
	QDoubleSpinBox * decay = new QDoubleSpinBox(this);
	QDoubleSpinBox * release = new QDoubleSpinBox(this);
	harmonic1->setObjectName("harmonic1");
	harmonic2->setObjectName("harmonic2");
	velocity->setObjectName("velocity");
	attack->setObjectName("attack");
	decay->setObjectName("decay");
	release->setObjectName("release");

	harmonic1->setAutoExclusive(false);
	harmonic2->setAutoExclusive(false);

	velocity->setDecimals(1);
	velocity->setMaximum(std::numeric_limits<double>::max());
	
	attack->setDecimals(6);
	attack->setMaximum(1);
	attack->setSingleStep(0.000001);

	decay->setDecimals(6);
	decay->setMaximum(1);
	decay->setSingleStep(0.000001);

	release->setDecimals(6);
	release->setMaximum(1);
	release->setSingleStep(0.000001);

	velocity->setValue(1000.0);
	attack->setValue(0.06250);
	decay->setValue(0.050);
	release->setValue(0.050);

	velocityValue = 1000.0;
	attackValue = 0.0625;
	decayValue = 0.05;
	releaseValue = 0.05;

	connect(harmonic1, static_cast<void(QRadioButton::*)(bool)>(&QRadioButton::clicked), this, static_cast<void(QInstrument::*)(bool)>(&QInstrument::enableFirst));
	connect(harmonic2, static_cast<void(QRadioButton::*)(bool)>(&QRadioButton::clicked), this, static_cast<void(QInstrument::*)(bool)>(&QInstrument::enableSecond));
	connect(velocity, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, static_cast<void(QInstrument::*)(double)>(&QInstrument::changeVelocity));
	connect(attack, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, static_cast<void(QInstrument::*)(double)>(&QInstrument::changeAttack));
	connect(decay, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, static_cast<void(QInstrument::*)(double)>(&QInstrument::changeDecay));
	connect(release, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, static_cast<void(QInstrument::*)(double)>(&QInstrument::changeRelease));
	QVBoxLayout * vbox = new QVBoxLayout;
	QHBoxLayout * hbox = new QHBoxLayout;
	QVBoxLayout * textBoxes = new QVBoxLayout;
	auto Lvelocity = new QLabel("Velocity", this);
	auto Lattack = new QLabel("Attack Time (s):", this);
	auto Ldecay = new QLabel("Decay Time (s):", this);
	auto Lrelease = new QLabel("Release Time (s):", this);
	textBoxes->addWidget(Lvelocity);
	textBoxes->addWidget(Lattack);
	textBoxes->addWidget(Ldecay);
	textBoxes->addWidget(Lrelease);

	hbox->addWidget(harmonic1);
	hbox->addWidget(harmonic2);
	vbox->addWidget(velocity);
	vbox->addWidget(attack);
	vbox->addWidget(decay);
	vbox->addWidget(release);
	QGridLayout * layout = new QGridLayout;
	layout->addLayout(hbox, 0, 0);
	layout->addLayout(vbox, 1, 1);
	layout->addLayout(textBoxes, 1, 0);
	QGroupBox * groupBox = new QGroupBox("Instrument Parameters");
	groupBox->setLayout(layout);
	return groupBox;
}

QGroupBox * QInstrument::MIDIin()
{
	auto hbox = new QHBoxLayout;
	auto button = new QPushButton("&Browse...");
	hbox->addWidget(inputpath);
	hbox->addWidget(button);
	QGroupBox * groupBox = new QGroupBox("MIDI Input");
	groupBox->setLayout(hbox);
	connect(button, &QPushButton::clicked, this, &QInstrument::browseIn);
	return groupBox;
}

QGroupBox * QInstrument::WAVout()
{
	auto hbox = new QHBoxLayout;
	auto button = new QPushButton("&Browse...");
	hbox->addWidget(outputpath);
	hbox->addWidget(button);
	QGroupBox * groupBox = new QGroupBox("Wave Output");
	groupBox->setLayout(hbox);
	connect(button, &QPushButton::clicked, this, &QInstrument::browseOut);
	return groupBox;
}
void QInstrument::browseIn() 
{
	static QString lastTime(QDir::currentPath());
		lastTime =	QDir::toNativeSeparators(QFileDialog::getOpenFileName(this, "Find MIDI FIle", lastTime, tr("MIDI Files (*.mid *.MID)")));
		inputpath->setText(lastTime);

	
}
void QInstrument::browseOut()
{
	static QString lastTime(QDir::currentPath());
	lastTime = QDir::toNativeSeparators(QFileDialog::getSaveFileName(this, "Target Wave File",lastTime , "Waveform Files"));
	outputpath->setText(lastTime);

}

void QInstrument::synthesize()
{
	auto TrackList = readMIDIFromFile(inputpath->text().toStdString());
	if (TrackList.empty())
	{
		message.setText("Invalid Input File/Path");
		message.exec();
	}
		
	else if (outputpath->text() == "")
	{
		message.setText("Please specify output file name/path");
		message.exec();
	}
	else
	{
		size_t count = 0;
		for (auto it = TrackList.begin(); it != TrackList.end(); it++)
		{
			auto generatedName = string(outputpath->text().toStdString()) + "-" + std::to_string(count) + ".wav";

			InteractiveInstrument instrument(*it);
			instrument.enableFirstHarmonic(enabled1);
			instrument.enableSecondHarmonic(enabled2);
			instrument.setAttackTime(attackValue);
			instrument.setDecayTime(decayValue);
			instrument.setReleaseTime(releaseValue);
			instrument.setVelocity(velocityValue);
			Signal mySignal;
			double inc = 1.0 / (double)mySignal.getSampleRate();
			while (!instrument.halted())
			{
				mySignal.push_back(instrument.sample(inc));
			}

			if (!write_wav(mySignal, generatedName))
			{
				message.setText("Unable to write to file");
				message.exec();
				break;
			}
			count++;
		}
	}
}

void QInstrument::enableFirst(bool enabled)
{
	enabled1 = enabled;
}

void QInstrument::enableSecond(bool enabled)
{
	enabled2 = enabled;
}

void QInstrument::changeAttack(double d)
{
	attackValue = d;
}

void QInstrument::changeVelocity(double d)
{
	velocityValue = d;
}

void QInstrument::changeDecay(double d)
{
	decayValue = d;
}

void QInstrument::changeRelease(double d)
{
	releaseValue = d;
}

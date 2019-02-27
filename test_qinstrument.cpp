#include <QTest>
#include <string>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <qpoint.h>
#include <limits>
#include "qinstrument.hpp"
#include <test_config.hpp>
using std::to_string;
class TestQInstrument : public QObject {
  Q_OBJECT

private slots:

  void initTestCase();
  void show();
  void testButton();
  void testSpinBox();
  void testLineEdit();
  void testRadioButton();
  void testInstrumentConstructor();
  void testInstrumentSetters();


  void generateDefault();
  void generate_1st();
  void generate_2nd();
  void generate_both();


private:
  QInstrument widget;
};

void TestQInstrument::initTestCase(){

  {
    auto w = widget.findChild<QRadioButton *>("harmonic1");
    QVERIFY2(w, "Could not find first harmonic button");
  }

  {
    auto w = widget.findChild<QRadioButton *>("harmonic2");
    QVERIFY2(w, "Could not find second harmonic button");
  }

  {
    auto w = widget.findChild<QDoubleSpinBox *>("velocity");
    QVERIFY2(w, "Could not find velocity edit widget");
  }

  {
    auto w = widget.findChild<QDoubleSpinBox *>("attack");
    QVERIFY2(w, "Could not find attack edit widget");
  }
  
  {
    auto w = widget.findChild<QDoubleSpinBox *>("decay");
    QVERIFY2(w, "Could not find decay edit widget");
  }
  
  {
    auto w = widget.findChild<QDoubleSpinBox *>("release");
    QVERIFY2(w, "Could not find release edit widget");
  }


  {
    auto w = widget.findChild<QLineEdit *>("inputpath");
    QVERIFY2(w, "Could not find a QLineEdit widget");
  }

  {
    auto w = widget.findChild<QLineEdit *>("outputpath");
    QVERIFY2(w, "Could not find a QLineEdit widget");
  }

  {
    auto w = widget.findChild<QPushButton *>("synth");
    QVERIFY2(w, "Could not find synthesizer button");
  }
}

QTEST_MAIN(TestQInstrument)
#include "test_qinstrument.moc"

void TestQInstrument::show()
{
	//widget.show();
}

void TestQInstrument::testButton()
{
	
}

void TestQInstrument::testSpinBox()
{
	auto velocity = widget.findChild<QDoubleSpinBox *>("velocity");
	QVERIFY(velocity);
	if (velocity)
	{
		QTest::mouseClick(velocity, Qt::LeftButton, Qt::NoModifier, QPoint(), -1);
		QTest::keyClick(velocity, Qt::Key_Up, Qt::NoModifier, -1);
		QTest::keyClick(velocity, Qt::Key_Up, Qt::NoModifier, -1);
		QCOMPARE(velocity->value(), 1002.0);
		velocity->setValue(std::numeric_limits<double>::max());
		QCOMPARE(velocity->value(), std::numeric_limits<double>::max());
		velocity->setValue(-1);
		QCOMPARE(velocity->value(), 0.0);
		QTest::keyClick(velocity, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(velocity, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(velocity, Qt::Key_Backspace, Qt::NoModifier, -1);

		QTest::keyClicks(velocity, "9999999", Qt::NoModifier, -1);
		QCOMPARE(velocity->value(), 9999999.0);

	}
	auto attack = widget.findChild<QDoubleSpinBox *>("attack");
	QVERIFY(attack);
	if (attack)
	{
		QTest::mouseClick(attack, Qt::LeftButton, Qt::NoModifier, QPoint(), -1);
		QTest::keyClick(attack, Qt::Key_Up, Qt::NoModifier, -1);
		QTest::keyClick(attack, Qt::Key_Up, Qt::NoModifier, -1);
		QCOMPARE(attack->value(), 0.062502);
		attack->setValue(2);
		QCOMPARE(attack->value(), 1.0);
		attack->setValue(-1);
		QCOMPARE(attack->value(), 0.0);

		QTest::keyClick(attack, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(attack, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(attack, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(attack, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(attack, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(attack, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(attack, Qt::Key_Backspace, Qt::NoModifier, -1);
		//QTest::keyClick(attack, Qt::Key_Backspace, Qt::NoModifier, 50);
		QTest::keyClicks(attack, "999", Qt::NoModifier, -1);

		QCOMPARE(attack->value(), 0.0);

		QTest::keyClick(attack, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(attack, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(attack, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(attack, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(attack, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(attack, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(attack, Qt::Key_Backspace, Qt::NoModifier, -1);

		QTest::keyClicks(attack, "0.9999999999", Qt::NoModifier, -1);
		QCOMPARE(attack->value(), 0.999999);


	}
	auto decay = widget.findChild<QDoubleSpinBox *>("decay");
	QVERIFY(decay);
	if (decay)
	{
		QTest::mouseClick(decay, Qt::LeftButton, Qt::NoModifier, QPoint(), -1);
		QTest::keyClick(decay, Qt::Key_Up, Qt::NoModifier, -1);
		QTest::keyClick(decay, Qt::Key_Up, Qt::NoModifier, -1);
		QCOMPARE(decay->value(), 0.050002);
		decay->setValue(2);
		QCOMPARE(decay->value(), 1.0);
		decay->setValue(-1);
		QCOMPARE(decay->value(), 0.0);

		QTest::keyClick(decay, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(decay, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(decay, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(decay, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(decay, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(decay, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(decay, Qt::Key_Backspace, Qt::NoModifier, -1);
		//QTest::keyClick(attack, Qt::Key_Backspace, Qt::NoModifier, 50);
		QTest::keyClicks(decay, "999", Qt::NoModifier, -1);

		QCOMPARE(decay->value(), 0.0);

		QTest::keyClick(decay, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(decay, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(decay, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(decay, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(decay, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(decay, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(decay, Qt::Key_Backspace, Qt::NoModifier, -1);

		QTest::keyClicks(decay, "0.9999999999", Qt::NoModifier, -1);
		QCOMPARE(decay->value(), 0.999999);

	}
	auto release = widget.findChild<QDoubleSpinBox *>("release");
	QVERIFY(release);
	if (release)
	{
		QTest::mouseClick(release, Qt::LeftButton, Qt::NoModifier, QPoint(), -1);
		QTest::keyClick(release, Qt::Key_Up, Qt::NoModifier, -1);
		QTest::keyClick(release, Qt::Key_Up, Qt::NoModifier, -1);
		QCOMPARE(release->value(), 0.050002);
		release->setValue(2);
		QCOMPARE(release->value(), 1.0);
		release->setValue(-1);
		QCOMPARE(release->value(), 0.0);

		QTest::keyClick(release, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(release, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(release, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(release, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(release, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(release, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(release, Qt::Key_Backspace, Qt::NoModifier, -1);
		//QTest::keyClick(attack, Qt::Key_Backspace, Qt::NoModifier, 50);
		QTest::keyClicks(release, "999", Qt::NoModifier, -1);

		QCOMPARE(release->value(), 0.0);

		QTest::keyClick(release, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(release, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(release, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(release, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(release, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(release, Qt::Key_Backspace, Qt::NoModifier, -1);
		QTest::keyClick(release, Qt::Key_Backspace, Qt::NoModifier, -1);

		QTest::keyClicks(release, "0.9999999999", Qt::NoModifier, -1);
		QCOMPARE(release->value(), 0.999999);


	}



}

void TestQInstrument::testLineEdit()
{
	auto inputpath = widget.findChild<QLineEdit *>("inputpath");
	auto outputpath = widget.findChild<QLineEdit *>("outputpath");
	QVERIFY(inputpath);
	QVERIFY(outputpath);
	if (inputpath)
	{
		QTest::mouseClick(inputpath, Qt::LeftButton, Qt::NoModifier, QPoint(), -1);
		QTest::keyClicks(inputpath, "/this/is/a/sample/path", Qt::NoModifier, -1);
		QCOMPARE(inputpath->text(), tr("/this/is/a/sample/path"));
	}
	if (outputpath)
	{
		QTest::mouseClick(outputpath, Qt::LeftButton, Qt::NoModifier, QPoint(), -1);
		QTest::keyClicks(outputpath, "/this/is/a/sample/path", Qt::NoModifier, -1);
		QCOMPARE(inputpath->text(), tr("/this/is/a/sample/path"));
	}

}

void TestQInstrument::testRadioButton()
{
	auto harmonic1 = widget.findChild<QRadioButton *>("harmonic1");
	auto harmonic2 = widget.findChild<QRadioButton *>("harmonic2");

	QVERIFY(harmonic1);
	QVERIFY(harmonic2);

	if (harmonic1 && harmonic2)
	{
		QCOMPARE(harmonic1->isChecked(), false);
		QTest::mouseClick(harmonic1, Qt::LeftButton, Qt::NoModifier, QPoint(), -1);
		QCOMPARE(harmonic1->isChecked(), true);
		QTest::mouseClick(harmonic1, Qt::LeftButton, Qt::NoModifier, QPoint(), -1);
		QCOMPARE(harmonic1->isChecked(), false);
		QTest::mouseClick(harmonic2, Qt::LeftButton, Qt::NoModifier, QPoint(), -1);
		QCOMPARE(harmonic2->isChecked(), true);
		QTest::mouseClick(harmonic2, Qt::LeftButton, Qt::NoModifier, QPoint(), -1);
		QCOMPARE(harmonic2->isChecked(), false);
		QTest::mouseClick(harmonic2, Qt::LeftButton, Qt::NoModifier, QPoint(), -1);
		QTest::mouseClick(harmonic1, Qt::LeftButton, Qt::NoModifier, QPoint(), -1);
		QCOMPARE(harmonic2->isChecked() && harmonic2->isChecked(), true);

	}
}

void TestQInstrument::testInstrumentConstructor()
{
	Track myTrack;
	myTrack.addTempoEvent(500000, 100);
	InteractiveInstrument inistrument(myTrack);
	QCOMPARE(inistrument.halted(), false);
}

void TestQInstrument::testInstrumentSetters()
{

	auto myTrack = readMIDIFromFile(TEST_FILE_DIR + "/test04.mid");
	auto it = myTrack.begin();
	InteractiveInstrument instrument(*it);
	instrument.setAttackTime(0.065);
	instrument.setVelocity(10000.0);
	instrument.setDecayTime(0.05);
	instrument.setReleaseTime(0.05);
	QCOMPARE(instrument.halted(), false);

	Signal mySignal;
	double inc = 1.0 / (double)mySignal.getSampleRate();
	while (!instrument.halted())
	{
		mySignal.push_back(instrument.sample(inc));
	}

	QCOMPARE(instrument.halted(), true);
	QCOMPARE(mySignal.size(), (size_t)121276);
	QCOMPARE(mySignal[0], 0.0);
	QCOMPARE(mySignal[11026], 0.48851999867129536);
	QCOMPARE(mySignal[11222], 30.528773581475434);
	QCOMPARE(mySignal[23799], -9074.1148463329391);
	QCOMPARE(mySignal[49999], 18528.374461928852);
	QCOMPARE(mySignal[112455], 0.0);

	instrument.setAttackTime(0.25);
	instrument.setDecayTime(0.2);
	instrument.setReleaseTime(0.2);
	//instrument.setVelocity(1);
	instrument.enableFirstHarmonic(true);
	instrument.enableSecondHarmonic(true);
	instrument.reset();
	QVERIFY(!instrument.halted());
	Signal my2ndSignal;
	while (!instrument.halted())
	{
		my2ndSignal.push_back(instrument.sample(inc));
	}
	QVERIFY(my2ndSignal.size() >= 0);
	QVERIFY(instrument.halted());
	QCOMPARE(my2ndSignal.size(), (size_t)121276);
	QCOMPARE(my2ndSignal[0], 0.0);

	QCOMPARE(my2ndSignal[11026], 0.25353273550591515);
	QCOMPARE(my2ndSignal[11222], 119.21929416878461);
	QCOMPARE(my2ndSignal[23799], -2581.3953738698046);
	QCOMPARE(my2ndSignal[49999], 2832.9804886266757);
	QCOMPARE(my2ndSignal[119077], 0.00000000000000000);
}

void TestQInstrument::generateDefault()
{
	for (size_t i = 0; i < 5; i++)
	{
		auto myTrack = readMIDIFromFile(TEST_FILE_DIR + "/test0" + to_string(i) + ".mid");
		auto it = myTrack.begin();
		InteractiveInstrument instrument(*it);
		instrument.setAttackTime(0.065);
		instrument.setVelocity(10000.0);
		instrument.setDecayTime(0.05);
		instrument.setReleaseTime(0.05);

		Signal mySignal;
		double inc = 1.0 / (double)mySignal.getSampleRate();
		while (!instrument.halted())
		{
			mySignal.push_back(instrument.sample(inc));
		}
		write_wav(mySignal, BUILD_FILE_DIR + "/tests/test0" + to_string(i) + ".wav");
	}
	
}

void TestQInstrument::generate_1st()
{
	for (size_t i = 0; i < 5; i++)
	{
		auto myTrack = readMIDIFromFile(TEST_FILE_DIR + "/test0" + to_string(i) + ".mid");
		auto it = myTrack.begin();
		InteractiveInstrument instrument(*it);
		instrument.setAttackTime(0.065);
		instrument.setVelocity(10000.0);
		instrument.setDecayTime(0.05);
		instrument.setReleaseTime(0.05);
		instrument.enableFirstHarmonic(true);
		Signal mySignal;
		double inc = 1.0 / (double)mySignal.getSampleRate();
		while (!instrument.halted())
		{
			mySignal.push_back(instrument.sample(inc));
		}
		write_wav(mySignal, BUILD_FILE_DIR + "/tests/test0" + to_string(i) +  "_harmonic1" + ".wav");
	}
}

void TestQInstrument::generate_2nd()
{
	for (size_t i = 0; i < 5; i++)
	{
		auto myTrack = readMIDIFromFile(TEST_FILE_DIR + "/test0" + to_string(i) + ".mid");
		auto it = myTrack.begin();
		InteractiveInstrument instrument(*it);
		instrument.setAttackTime(0.065);
		instrument.setVelocity(10000.0);
		instrument.setDecayTime(0.05);
		instrument.setReleaseTime(0.05);
		instrument.enableSecondHarmonic(true);
		Signal mySignal;
		double inc = 1.0 / (double)mySignal.getSampleRate();
		while (!instrument.halted())
		{
			mySignal.push_back(instrument.sample(inc));
		}
		write_wav(mySignal, BUILD_FILE_DIR + "/tests/test0" + to_string(i) + "_harmonic2" + ".wav");
	}
}

void TestQInstrument::generate_both()
{
	for (size_t i = 0; i < 5; i++)
	{
		auto myTrack = readMIDIFromFile(TEST_FILE_DIR + "/test0" + to_string(i) + ".mid");
		auto it = myTrack.begin();
		InteractiveInstrument instrument(*it);
		instrument.setAttackTime(0.065);
		instrument.setVelocity(10000.0);
		instrument.setDecayTime(0.05);
		instrument.setReleaseTime(0.05);
		instrument.enableFirstHarmonic(true);
		instrument.enableSecondHarmonic(true);
		Signal mySignal;
		double inc = 1.0 / (double)mySignal.getSampleRate();
		while (!instrument.halted())
		{
			mySignal.push_back(instrument.sample(inc));
		}
		write_wav(mySignal, BUILD_FILE_DIR + "/tests/test0" + to_string(i) + "_harmonicboth" + ".wav");
	}
}


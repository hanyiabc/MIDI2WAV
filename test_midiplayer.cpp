#include <QTest>
#include <QPushButton>
#include <QSlider>
#include <QLineEdit>
#include "shared_double_buffer.hpp"
#include "midiplayer.hpp"
#include <thread>
#include <string>
#include "instrument.hpp"
#include "event.hpp"
#include "test_config.hpp"
#include "track.hpp"
#include <qgroupbox.h>
#include <qaudio.h>
using std::thread;

class TestMIDIPlayer : public QObject {
  Q_OBJECT

private slots:

  void initTestCase();
  void testDoubleBuffer();
  void testMessageQueue();
  void multiThTestDoubleBuffer();
  void multiThTestMq();
  void testUI();
  void TestInstrumentConstructor();
  void TestInstSample();
  // TODO: add your tests
  
private:

  MIDIPlayer widget;
};

void TestMIDIPlayer::initTestCase(){

  {
    auto w = widget.findChild<QLineEdit *>("inputpath");
    QVERIFY2(w, "Could not find a QLineEdit widget");
  }

  {
    auto w = widget.findChild<QPushButton *>("play");
    QVERIFY2(w, "Could not find play button");
  }

  {
    auto w = widget.findChild<QPushButton *>("pause");
    QVERIFY2(w, "Could not find pause button");
  }
  
  {
    auto w = widget.findChild<QPushButton *>("stop");
    QVERIFY2(w, "Could not find stop button");
  }

  {
    auto w = widget.findChild<QSlider *>("volume");
    QVERIFY2(w, "Could not find volume slider");
  }
  
  {
    auto w = widget.findChild<QPushButton *>("mute");
    QVERIFY2(w, "Could not find mute button");

  }
}

void TestMIDIPlayer::testDoubleBuffer()
{
	DoubleBuffer a;
	a.reserve(100);
	QVERIFY(!a.isFull());
	size_t temp = 100;
	QCOMPARE(a.capacity(), temp);
	temp = 0;
	QCOMPARE(a.size(), temp);
	a.push_back(0.0);
	QCOMPARE(a.pop(), 0.0);
	for (size_t i = 0; i < 100; i++)
	{
		a.push_back(i);
	}
	QVERIFY(a.isFull());
	a.pop();
	a.erase(50);
	temp = 49;
	QCOMPARE(a.size(), temp);
	QCOMPARE(a.pop(), 51.0);
}

void TestMIDIPlayer::testMessageQueue()
{
	MessageQueue mq;
	QVERIFY(mq.empty());
	mq.push("AAA");
	QVERIFY(!mq.empty());
	string s = "";
	QVERIFY(mq.try_pop(s));
	QCOMPARE(s.c_str(), "AAA");
	QVERIFY(!mq.try_pop(s));
	mq.push("AAA");
	mq.wait_and_pop(s);
	QCOMPARE(s.c_str(), "AAA");
}

void f1(DoubleBuffer * buffer)
{
	for (size_t i = 5000; i < 10000; i++)
	{
		buffer->push_back(i);
	}
}

void TestMIDIPlayer::multiThTestDoubleBuffer()
{
	DoubleBuffer buffer;
	buffer.reserve(6000);
	for (size_t i = 0; i < 5000; i++)
	{
		buffer.push_back(i);
	}
	std::thread th1(f1, &buffer);
	th1.join();
	for (size_t i = 0; i < 10000; i++)
	{
		QCOMPARE(buffer.pop(), (double)i);
	}

}
void f2(MessageQueue * mq, MessageQueue * mq2)
{
	while (true)
	{
		string m = "";
		mq->wait_and_pop(m);
		bool flag = false;
		switch (m[0])
		{
		case 'A':
			mq2->push("A");
			break;
		case 'B':
			mq2->push("B");
			break;
		case 'E':
			mq2->push("E");
			flag = true;
			break;
		default:
			break;
		}
		if (flag)
		{
			break;
		}
	}
}
void TestMIDIPlayer::multiThTestMq()
{
	MessageQueue mq;
	MessageQueue mq2;
	std::thread th1(f2, &mq, &mq2);
	mq.push("A");
	mq.push("B");
	mq.push("E");
	string m = "";
	mq2.wait_and_pop(m);
	QCOMPARE(m.c_str(), "A");
	mq2.wait_and_pop(m);
	QCOMPARE(m.c_str(), "B");
	mq2.wait_and_pop(m);
	QCOMPARE(m.c_str(), "E");
	th1.join();
}

void TestMIDIPlayer::testUI()
{
	//widget.show();
	auto inputpath = widget.findChild<QLineEdit *>("inputpath");
	QTest::mouseClick(inputpath, Qt::LeftButton, Qt::NoModifier, QPoint(), -1);
	QTest::keyClicks(inputpath, "/this/is/a/sample/path", Qt::NoModifier, -1);
	QCOMPARE(inputpath->text(), tr("/this/is/a/sample/path"));
	inputpath->setText((TEST_FILE_DIR + "/test4.mid").c_str());
	QTest::keyClick(inputpath, Qt::Key::Key_Enter, Qt::NoModifier, -1);
	auto play = widget.findChild<QPushButton *>("play");
	auto pause = widget.findChild<QPushButton *>("pause");
	//auto play = widget.findChild<QPushButton *>("play");
	auto box = widget.findChild<QGroupBox *>("ingroup");
	auto stop = widget.findChild<QPushButton *>("stop");

	QVERIFY(play->isEnabled());
	QVERIFY(box->isEnabled());
	QVERIFY(!pause->isEnabled());
	QVERIFY(box->isEnabled());
	QVERIFY(!stop->isEnabled());

	QTest::mouseClick(play, Qt::LeftButton, Qt::NoModifier, QPoint(), -1);
	QVERIFY(!play->isEnabled());
	QVERIFY(!box->isEnabled());
	//auto audio = widget.findChild<QAudioOutput *>("audio");
	//QCOMPARE(audio->state(), QAudio::State::ActiveState);

	QTest::mouseClick(pause, Qt::LeftButton, Qt::NoModifier, QPoint(), -1);
	//QCOMPARE(audio->state(), QAudio::State::SuspendedState);
	QVERIFY(play->isEnabled());
	QVERIFY(!pause->isEnabled());
	QVERIFY(stop->isEnabled());

	QTest::mouseClick(play, Qt::LeftButton, Qt::NoModifier, QPoint(), -1);
	//auto state = audio->state();
	//QCOMPARE(audio->state(), QAudio::State::IdleState);
	
	QTest::mouseClick(stop, Qt::LeftButton, Qt::NoModifier, QPoint(), -1);
	QVERIFY(play->isEnabled());
	QVERIFY(box->isEnabled());
	QVERIFY(!pause->isEnabled());
	QVERIFY(box->isEnabled());
	QVERIFY(!stop->isEnabled());

	auto volume = widget.findChild<QSlider *>("volume");
	QCOMPARE(volume->value(), 50);
	QTest::keyClick(volume, Qt::Key::Key_Left, Qt::KeyboardModifier::NoModifier, -1);
	QCOMPARE(volume->value(), 49);
	//QCOMPARE(audio->volume(), 0.49);

	auto mute = widget.findChild<QPushButton *>("mute");
	QTest::mouseClick(mute, Qt::LeftButton, Qt::NoModifier, QPoint(), -1);
	QCOMPARE(volume->value(), 0);
	//QCOMPARE(audio->volume(), 0.0);


}

void TestMIDIPlayer::TestInstrumentConstructor()
{
	Track myTrack;
	myTrack.addTempoEvent(500000, 100);
	DefaultInstrument I(myTrack);
	QVERIFY(!I.halted());
}

void TestMIDIPlayer::TestInstSample()
{
	Track myTrack(480);
	myTrack.addTempoEvent(500000, 0);
	myTrack.addNoteEvent(9, 5, 20, 0);
	myTrack.addNoteEvent(9, 17, 20, 0);
	myTrack.addNoteEvent(9, 17, 20, 0);
	myTrack.addNoteEvent(9, 29, 20, 0);
	myTrack.addNoteEvent(9, 41, 20, 0);
	myTrack.addNoteEvent(9, 53, 20, 0);
	myTrack.addNoteEvent(9, 65, 20, 0);
	myTrack.addNoteEvent(9, 89, 20, 0);
	myTrack.addNoteEvent(9, 101, 20, 0);
	myTrack.addNoteEvent(9, 113, 20, 0);
	myTrack.addNoteEvent(8, 5, 20, 1);
	myTrack.addNoteEvent(8, 17, 20, 1);
	myTrack.addNoteEvent(8, 17, 20, 1);
	myTrack.addNoteEvent(8, 29, 20, 1);
	myTrack.addNoteEvent(8, 41, 20, 1);
	myTrack.addNoteEvent(8, 53, 20, 1);
	myTrack.addNoteEvent(8, 65, 20, 1);
	myTrack.addNoteEvent(8, 89, 20, 1);
	myTrack.addNoteEvent(8, 101, 20, 1);
	myTrack.addNoteEvent(8, 113, 20, 1);
	myTrack.addEndEvent(300);

	DefaultInstrument I(myTrack);
	QVERIFY(I.sample(1.0 / 44100.0) == 0);
	vector<double> temp;
	while (!I.halted())
	{
		temp.push_back(I.sample(1.0 / 44100.0));
	}
	vector<int16_t> converted;

	for (size_t i = 0; i < 10; i++)
	{
		converted.push_back((int16_t)temp[i]);
	}
	QCOMPARE(converted[0], (int16_t)2);
	QCOMPARE(converted[1], (int16_t)6);
	QCOMPARE(converted[2], (int16_t)10);
	QCOMPARE(converted[3], (int16_t)12);

}


QTEST_MAIN(TestMIDIPlayer)
#include "test_midiplayer.moc"
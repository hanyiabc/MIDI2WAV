// This is a demostration program that shows you how to use the audio output
// capabilites of Qt. It just opens a window and outputs Gaussian IID noise
// to the audio device.

#include <QApplication>
#include <QAudioOutput>
#include <QDebug>
#include <QWidget>

#include <cmath>
#include <random>

// a normal distributed random number generator
class NormalRnd {
public:
  NormalRnd(double m = 0.0, double s = 1.0) : mean(m), stddev(s) {
    std::random_device rd;
    gen.seed(rd());
  }

  void operator()(double &x) { x = mean + stddev * p(gen); }

private:
  double mean, stddev;

  std::mt19937 gen;
  std::normal_distribution<double> p;
};

class Noise : public QWidget {
  Q_OBJECT

public:
  Noise(QWidget *parent = nullptr) : QWidget(parent) {

    rnd = NormalRnd(0.0, 500.0);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    auto rates = info.supportedSampleRates();
	
    int sampleRate = rates.at(0);
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
    else{
      audio = new QAudioOutput(format, this);

      connect(audio, SIGNAL(notify()), this, SLOT(handleNotify()));

      buffer_size = audio->bufferSize();
      buffer_size = buffer_size > 0 ? buffer_size : 1024;

      audio->setNotifyInterval(25);

      device = audio->start();
      handleNotify();
    }
  }

private slots:

  void handleNotify() {

    int bytesFree = audio->bytesFree();
    int bytesToWrite = bytesFree > buffer_size ? buffer_size : bytesFree;
    int samplesToWrite = bytesToWrite >> 1;
    bytesToWrite = 2 * samplesToWrite;

    buffer.reserve(bytesToWrite);

    double s;
    for (int i = 0; i < samplesToWrite; ++i) {
      rnd(s); // IID Gaussian noise
      int16_t temp = static_cast<int16_t>(lround(s));
      char *byte1 = reinterpret_cast<char *>(&temp);
      char *byte2 = ++byte1;
      buffer.push_back(*byte1);
      buffer.push_back(*byte2);
    }

    if (bytesToWrite > 0) {
      device->write(buffer.data(), bytesToWrite);
    }

    buffer.clear();
  }

private:
  std::size_t buffer_size;
  std::vector<char> buffer;

  QAudioOutput *audio;
  QIODevice *device;

  NormalRnd rnd;
};

#include "noise_generator.moc"

int main(int argc, char *argv[]) {

  QApplication app(argc, argv);

  Noise widget;

  widget.show();

  return app.exec();
}

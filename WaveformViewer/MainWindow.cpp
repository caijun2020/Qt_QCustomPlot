#include "MainWindow.h"
#include "ui_MainWindow.h"

const double PI = 3.1415926;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_drawWaveTimer(new QTimer),
    m_scrollGridLayout(new QGridLayout),
    m_waveMaxNum(3),
    m_sampleSize(512),
    m_freqHz(100)
{
    ui->setupUi(this);

    initUI();

    connect(m_drawWaveTimer, SIGNAL(timeout()), this, SLOT(updateWaveform()));
    m_drawWaveTimer->start(1000/m_freqHz);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_drawWaveTimer;
    delete m_scrollGridLayout;

    deInitUI();
}

void MainWindow::updateWaveform()
{
    static int cnt = 0;

//    static QTime time(QTime::currentTime());
//    double interval = time.elapsed(); // time elapsed since start of demo, in seconds
//    qDebug() << "time.elapsed() = " << interval << " ms";
//    time.restart();

    if(++cnt >= m_freqHz)
    {
        cnt = 0;
    }

    float value = sin((2 * PI / m_freqHz) * cnt);

    for(int i = 0; i < m_waveList.size(); i++)
    {
        m_waveList.at(i)->addData(value, 0);    // data for channel 1(Wave1) of waveform
        m_waveList.at(i)->addData(value*2, 1);  // data for channel 2(Wave2) of waveform
    }
}

void MainWindow::initUI()
{
    m_labelList.clear();
    m_waveList.clear();

    for(int i = 0; i < m_waveMaxNum; i++)
    {
        QHBoxLayout *pHLayout = new QHBoxLayout();

        QLabel *pLabel = new QLabel();
        pLabel->setText(QString::fromUtf8("Waveform%1").arg(i+1));
        pLabel->setMinimumHeight(150);  // Set minimum height
        m_labelList.append(pLabel);

        WaveformViewer *pWave = new WaveformViewer(2);
        m_waveList.append(pWave);

        //pWave->setColor(1, Qt::blue);
        pWave->setBrush(0);
        pWave->setBrush(1, QBrush(QColor(255, 255, 0, 50)));
        //pWave->setLineWidth(1, 2);

        pHLayout->addWidget(pWave);
        pHLayout->addWidget(pLabel);

        pHLayout->setStretch(0, 9);
        pHLayout->setStretch(1, 1);

        m_scrollGridLayout->addLayout(pHLayout, i, 0);
    }

    ui->scrollArea->widget()->setLayout(m_scrollGridLayout);
}

void MainWindow::deInitUI()
{
    for(int i = 0; i < m_labelList.size(); i++)
    {
        delete m_labelList.at(i);
    }

    for(int i = 0; i < m_waveList.size(); i++)
    {
        delete m_waveList.at(i);
    }
}

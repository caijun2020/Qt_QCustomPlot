#include "WaveformViewer.h"
#include <QDebug>

WaveformViewer::WaveformViewer(uint32_t waveCnt, QWidget *parent) :
    QWidget(parent),
    customPlot(new QCustomPlot(this)),
    m_waveCnt(waveCnt),
    m_legendVisibleFlag(true),
    m_axisAutoScale(true),
    m_xRangeSecond(10),
    m_lastScaleTime(0),
    m_waveRefreshMode(DYNAMIC_WAVE),
    time(QTime::currentTime())
{
    initPlot(m_waveCnt);
}

WaveformViewer::~WaveformViewer()
{
    delete customPlot;
}

void WaveformViewer::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    //qDebug() << "WaveformViewer::resizeEvent() this->size()=" << this->size();
    customPlot->resize(this->size());
}

void WaveformViewer::initPlot(uint32_t waveCnt)
{
    m_waveCnt = waveCnt;

    m_waveInfo.clear();
    m_waveInfo.reserve(m_waveCnt);

    for(uint32_t i = 0; i < m_waveCnt; i++)
    {
        struct WAVE_INFO info;

        info.name = QString("Wave%1").arg(i+1);
        info.width = 1;

        switch (i) {
        case 0:
            info.color = Qt::green;
            break;
        case 1:
            info.color = Qt::red;
            break;
        case 2:
            info.color = Qt::yellow;
            break;
        default:
            info.color = Qt::blue;
            break;
        }

        m_waveInfo.append(info);

        customPlot->addGraph(); // blue line
        //customPlot->graph()->setPen(QPen(m_waveInfo[i].color));
        //customPlot->graph()->setName(m_waveInfo[i].name);
        setColor(i, info.color);
        setLabel(i, info.name);

        customPlot->graph()->setAntialiasedFill(true);// Enable Anti aliased
    }

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%hh:%mm:%ss");
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->xAxis->setTickLength(2);

    customPlot->axisRect()->setupFullAxesBox();


    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    customPlot->legend->setVisible(m_legendVisibleFlag);

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    switch (m_waveRefreshMode)
    {
    case REPEAT_WAVE:
        customPlot->xAxis->setRange(0, m_xRangeSecond, Qt::AlignLeft);
        break;
    case DYNAMIC_WAVE:
        break;
    default:
        break;
    }
}

void WaveformViewer::setColor(uint32_t chn, QColor color)
{
    if((int)chn < m_waveInfo.size())
    {
        m_waveInfo[chn].color = color;
        m_waveInfo[chn].pen.setColor(color);
        customPlot->graph(chn)->setPen(m_waveInfo[chn].pen);
    }
}

void WaveformViewer::setBrush(uint32_t chn, QBrush brush)
{
    if((int)chn < m_waveInfo.size())
    {
        m_waveInfo[chn].brush = brush;
        customPlot->graph(chn)->setBrush(m_waveInfo[chn].brush);
    }
}

void WaveformViewer::setLineWidth(uint32_t chn, int width)
{
    if((int)chn < m_waveInfo.size())
    {
        m_waveInfo[chn].width = width;
        m_waveInfo[chn].pen.setWidth(width);
        customPlot->graph(chn)->setPen(m_waveInfo[chn].pen);
    }
}

void WaveformViewer::setLabel(uint32_t chn, QString name)
{
    if((int)chn < m_waveInfo.size())
    {
        m_waveInfo[chn].name = name;
        customPlot->graph(chn)->setName(name);
    }
}

int WaveformViewer::getWaveCount() const
{
    return m_waveCnt;
}

QColor WaveformViewer::getWaveColor(uint32_t chn) const
{
    QColor ret;

    if((int)chn < m_waveInfo.size())
    {
        ret = m_waveInfo[chn].color;
    }

    return ret;
}

void WaveformViewer::setAutoScale(bool scaleFlag)
{
    m_axisAutoScale = scaleFlag;
}

void WaveformViewer::setXRange(int sec)
{
    m_xRangeSecond = sec;
}

void WaveformViewer::setWaveRefreshMode(int mode)
{
    m_waveRefreshMode = mode;
}

int WaveformViewer::getWaveRefreshMode() const
{
    return m_waveRefreshMode;
}

void WaveformViewer::setLegendVisible(bool visible)
{
    m_legendVisibleFlag = visible;
    customPlot->legend->setVisible(m_legendVisibleFlag);
}

void WaveformViewer::addData(double value, uint32_t chn)
{
    //key: x axis unit
    //double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    // calculate new data arrived time
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds

    if(chn >= m_waveCnt)
    {
        return;
    }

    customPlot->graph(chn)->addData(key, value); // Add data to curve

    // For channel 0, auto scale Y axis
    // For other channel, only enlarge
    customPlot->graph(chn)->rescaleValueAxis(true); // Auto scale Y axis

    // Periodically(every m_xRangeSecond seconds) rescale axes
    if(key - m_lastScaleTime > m_xRangeSecond)
    {
        m_lastScaleTime = key;

        if(m_axisAutoScale)
        {
            customPlot->graph(chn)->rescaleAxes();
        }

        switch (m_waveRefreshMode)
        {
        case REPEAT_WAVE:
            customPlot->xAxis->setRange(key, m_xRangeSecond, Qt::AlignLeft);
            break;
        case DYNAMIC_WAVE:
            break;
        default:
            break;
        }
    }

    // Set x axis range
    switch (m_waveRefreshMode)
    {
    case REPEAT_WAVE:
        break;
    case DYNAMIC_WAVE:
        customPlot->xAxis->setRange(key + 0.25, m_xRangeSecond, Qt::AlignRight);
        //customPlot->xAxis->setRange(key + 0.25, m_xRangeSecond, Qt::AlignCenter);
        break;
    default:
        break;
    }

    customPlot->replot();

}

void WaveformViewer::setData(const QVector<double> &key, const QVector<double> &value, uint32_t chn)
{
    if(chn >= m_waveCnt)
    {
        return;
    }

    customPlot->graph(chn)->addData(key, value); // Add data to curve

    customPlot->graph(chn)->rescaleAxes();

    customPlot->replot();
}

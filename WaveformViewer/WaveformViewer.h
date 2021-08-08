#ifndef WAVEFORMVIEWER_H
#define WAVEFORMVIEWER_H
#include <QWidget>
#include <QList>
#include <QTime>
#include "qcustomplot.h"

class WaveformViewer : public QWidget
{
    Q_OBJECT
public:
    explicit WaveformViewer(uint32_t waveCnt = 1, QWidget *parent = nullptr);
    virtual ~WaveformViewer();

    // Init waveform plot parameters
    void initPlot(uint32_t waveCnt = 1);

    // Set waveform color
    void setColor(uint32_t chn = 0, QColor color = Qt::blue);

    // Set waveform fill area with x asix
    void setBrush(uint32_t chn = 0, QBrush brush = QBrush(QColor(0, 0, 255, 20)));

    // Set waveform line width
    void setLineWidth(uint32_t chn = 0, int width = 1);

    // Set waveform name
    void setLabel(uint32_t chn, QString name);

    // Return waveform count
    int getWaveCount() const;

    // Return waveform color
    QColor getWaveColor(uint32_t chn = 0) const;

    // Add new data to waveform
    void addData(double value, uint32_t chn = 0);

    // Set vector data
    void setData(const QVector<double> &key, const QVector<double> &value, uint32_t chn = 0);

    // Set auto scale
    void setAutoScale(bool scaleFlag = true);

    // Set X axis range, Unit:second
    void setXRange(int sec);

    // Set/Get waveform mode(dynamic mode, repeat mode)
    void setWaveRefreshMode(int mode);
    int getWaveRefreshMode() const;

    // Set Legend visible or invisible
    void setLegendVisible(bool visible);

    struct WAVE_INFO
    {
        QString name;
        QColor color;
        uint32_t width;
        QPen pen;
        QBrush brush;
    };

    enum WAVE_MODE
    {
        DYNAMIC_WAVE = 0,
        REPEAT_WAVE
    };

protected:
    void resizeEvent(QResizeEvent *e);

signals:

private:
    QCustomPlot *customPlot;

    uint32_t m_waveCnt; // Support total waveform number
    QList<WAVE_INFO> m_waveInfo;

    bool m_legendVisibleFlag;   // Flag used to show/hide legend
    bool m_axisAutoScale;       // Flag used to auto scale axis

    int m_xRangeSecond;     // X axis time length
    int m_lastScaleTime;

    int m_waveRefreshMode;  // Refresh mode: dynamic/repeative waveform

    QTime time;
};

#endif // WAVEFORMVIEWER_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QGridLayout>
#include "WaveformViewer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void updateWaveform();

private:
    Ui::MainWindow *ui;

    QTimer *m_drawWaveTimer;

    QGridLayout *m_scrollGridLayout;
    int m_waveMaxNum;

    QList<QLabel *> m_labelList; // Store QLabel
    QList<WaveformViewer *> m_waveList; // Store WaveformViewer

    int m_sampleSize;
    int m_freqHz;

    // Init UI by code
    void initUI();

    // Destroy UI
    void deInitUI();
};

#endif // MAINWINDOW_H

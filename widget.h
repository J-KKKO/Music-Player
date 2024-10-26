#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QDir>
#include <QStringList>
#include <QUrl>
#include <QTime>
#include <QTimer>
#include <QPainter>
#include <QLabel>
#include <QListWidget>
#include <QFont>
#include <QScrollBar>
#include <QSwipeGesture>
#include "lyric.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    void musicPlayerInit();
    void scanMusic();
    void timerInit();
    void lyricInit();
    void volumeInit();
    void listInit();

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    bool event(QEvent *event) override;
    int startPoint;

signals:
    void wheel_signal(int flag);                        //-1 向上  1向下

private slots:
    void on_playButton_clicked();

    void on_nextButton_clicked();

    void on_lastButton_clicked();

    void totalDurationChanged(qint64 duration);

    void nowpositionChanged(qint64 position);

    void on_horizontalSlider_sliderReleased();

    void on_timerout();

    void showSongInfo();

    void on_cycleButton_stateChanged(int state);

    void on_volumeButton_clicked(bool checked);

    void on_verticalSlider_valueChanged(int value);

    void on_listButton_clicked(bool checked);

    void on_listWidget_currentRowChanged(int currentRow);

    void on_wheel(int flag);

    void scroll(int flag);

    void on_scrolltimerout();

    void on_selectButton_clicked();

private:
    Ui::Widget *ui;
    QMediaPlayer *musicPlayer;
    QMediaPlaylist *musicPlaylist;
    QStringList filename;


    /*cd部分*/
    float angle;
    QTimer *timer;

    /*歌词部分*/
    Lyric *lyric;
    QLabel *lyricLabels[100];
    QTimer *scrolltimer;
};

#endif // WIDGET_H

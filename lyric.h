#ifndef LYRIC_H
#define LYRIC_H

#include <QObject>
#include <QFile>
#include <QLabel>
#include <QGroupBox>
#include <QFont>

struct lineLyric
{
    int pos;            //歌词位置
    qint64 time;        //歌词时间ms
    QString lyricStr;   //歌词内容
};

class Lyric : public QObject
{
    Q_OBJECT
public:
    int t = 0;
    int scrollflag=0;
    struct lineLyric lyrics[100]; // 存储多行歌词的数组，最大一百行
    explicit Lyric(QObject *parent = nullptr);

    void getLineLyric(QString path);   // 解析歌词文件，并将每一行歌词与对应时间保存到 lyrics 数组

    int findNeedPos(qint64 time);      // 根据时间找到应显示的歌词位置

    void lyricCenterColor(QGroupBox * groupBox); // 改变中央歌词的大小和颜色

    void lyricMove(int distance,QLabel* lyric_p[],QGroupBox *groupBox);       // 移动歌词

    void movePos(qint64 time,QLabel* lyric_p[],QGroupBox *groupBox);         // 根据给定时间调整歌词显示的位置

    void restateLyric(QLabel* lyric_p[]); //复位label

    void setText(QLabel* lyric_p[]); // 添加歌词到各个label

    void lyricDown(QLabel* lyric_p[],QGroupBox *groupBox);
    void lyricUp(QLabel* lyric_p[],QGroupBox *groupBox);
signals:

public slots:

private:
    int lyricSize=0;
    int currentpos=0;

};

#endif // LYRIC_H

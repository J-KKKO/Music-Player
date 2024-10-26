#include "widget.h"
#include "ui_widget.h"
#include <iostream>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_AcceptTouchEvents); // 让系统接收触控事件

    musicPlayerInit();
    scanMusic();

    timerInit();

    lyricInit();

    volumeInit();

    listInit();

    connect(musicPlayer, &QMediaPlayer::durationChanged,this,&Widget::totalDurationChanged);
    connect(musicPlayer, &QMediaPlayer::positionChanged,this,&Widget::nowpositionChanged);
    connect(timer, &QTimer::timeout, this, &Widget::on_timerout);
    connect(this,&Widget::wheel_signal,this,&Widget::on_wheel);
    connect(scrolltimer, &QTimer::timeout, this, &Widget::on_scrolltimerout);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::musicPlayerInit()
{
    musicPlayer = new QMediaPlayer(this);
    musicPlaylist = new QMediaPlaylist(this);

    /* 设置播放模式， Loop 是列表循环 */
    musicPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
    /* 设置音乐播放器的列表为 musicPlaylist */
    musicPlayer->setPlaylist(musicPlaylist);
}

void Widget::scanMusic()
{
    QDir dir("/home/alientek/music");
//    QDir dir("/home/music");

    QStringList filter;
    filter << "*.mp3";
    QStringList filelist = dir.entryList(filter, QDir::Files);

    for (int i = 0; i < filelist.size(); i++)
    {
        QString filePath = dir.filePath(filelist.at(i)); // 获取完整路径
        musicPlaylist->addMedia(QUrl::fromLocalFile(filePath));

        filename.append(filePath.split(".").at(0).split("/").last());

        // 添加文件名到列表小部件
        QListWidgetItem *item = new QListWidgetItem(filename[i], ui->listWidget);
        // 设置字体
        QFont font("Display", 15, QFont::Bold); // 字体名称、大小、样式
        item->setFont(font);

        qDebug() << filePath << endl; // 输出完整路径
    }

    qDebug()<< "音乐数量：" << musicPlaylist->mediaCount() << endl;
}

void Widget::on_playButton_clicked()
{
    int state = musicPlayer->state();

    switch(state)
    {
    case QMediaPlayer::StoppedState:
        musicPlayer->play();
        timer->start(100);
        break;
    case QMediaPlayer::PlayingState:
        musicPlayer->pause();
        timer->stop();
        break;
    case QMediaPlayer::PausedState:
        musicPlayer->play();
        timer->start(100);
        break;
    }
}

void Widget::on_nextButton_clicked()
{
    int state = musicPlayer->state();
    int currentIndex = musicPlaylist->currentIndex();
    musicPlaylist->setCurrentIndex(currentIndex+1);

    int nextIndex = musicPlaylist->currentIndex();
//    qDebug()<< "音乐索引：" << nextIndex << endl;

    /*各种循环模式基于自动播放的前提，不支持手动，不能使点击下一首之后回到第一首，且不会自动播放*/
    if (nextIndex == -1)
    {
        nextIndex = 0; // 如果到达末尾，则回到列表的第一首歌
        musicPlaylist->setCurrentIndex(nextIndex);
//        qDebug()<< "改变后音乐索引：" << musicPlaylist->currentIndex() << endl;
    }

    /*自动播放*/
    if(state == QMediaPlayer::PlayingState)
    {
        musicPlayer->play(); // 自动播放下一首歌
    }
}

void Widget::on_lastButton_clicked()
{
    int state = musicPlayer->state();
    int currentIndex = musicPlaylist->currentIndex();
    musicPlaylist->setCurrentIndex(currentIndex-1);

    int nextIndex = musicPlaylist->currentIndex();
    if (nextIndex == -1)
    {
        nextIndex = musicPlaylist->mediaCount()-1; // 如果到达头部之前，则回到列表的最后首歌
        musicPlaylist->setCurrentIndex(nextIndex);
    }


    /*自动播放*/
    if(state == QMediaPlayer::PlayingState)
    {
        musicPlayer->play(); // 自动播放下一首歌
    }
}

void Widget::totalDurationChanged(qint64 duration)
{
    // 创建 QTime 对象，从毫秒值初始化时间
    QTime time(0, 0, 0, 0); // 初始化时间为 00:00:00:000
    time = time.addMSecs(duration);

    // 格式化时间字符串，格式为 mm:ss
    QString formattedDuration = time.toString("mm:ss");
    ui->totalTimeLabel->setText(formattedDuration);
    ui->horizontalSlider->setMaximum(duration); // 设置滑动条最大值
//    qDebug() << "总音乐时长" << formattedDuration << endl;

    showSongInfo();

    /*获取歌词*/
    int index = musicPlaylist->currentIndex();
    lyric->getLineLyric("/home/alientek/music/"+filename[index]+".lrc");
//    lyric->getLineLyric("/home/music/"+filename[index]+".lrc");  // 加载歌词数据
    // 初始化显示歌词
    lyric->setText(lyricLabels);
    qDebug() << "总时间变了";
}

void Widget::nowpositionChanged(qint64 position)
{
    ui->horizontalSlider->setValue(position);

    // 创建 QTime 对象，从毫秒值初始化时间
    QTime time(0, 0, 0, 0); // 初始化时间为 00:00:00:000
    time = time.addMSecs(position);

    // 格式化时间字符串，格式为 mm:ss
    QString formattedDuration = time.toString("mm:ss");
    ui->nowTimeLabel->setText(formattedDuration);
//    qDebug() << "此刻音乐时长" << formattedDuration << endl;

    // 调用Lyric类的movePos函数，更新歌词的显示位置
    if(!lyric->scrollflag)
    {
        qDebug() << "movePos执行了";
        lyric->movePos(position, lyricLabels, ui->groupBox);
    }
}

void Widget::on_horizontalSlider_sliderReleased()
{
    int currentPosition = ui->horizontalSlider->value(); // 获取当前刻度值
    musicPlayer->setPosition(currentPosition); // 设置到播放对象里
}

void Widget::on_timerout()
{
    /* 需要更新界面，不设置不更新 */
    this->update();
}

void Widget::showSongInfo()
{
    /*获取文件名，来显示歌名和歌手*/
    int index = musicPlaylist->currentIndex();

    ui->songLabel->setText(filename[index].split("-").first());
    ui->singerLabel->setText(filename[index].split("-").last());
}

void Widget::timerInit()
{
    /* 定时器实例化 */
    timer = new QTimer(this);
    /* 默认角度为0 */
    angle = 0;
    /* 定时100ms,为了避免一直转，我就放在播放和暂停槽函数那里了 */
    //    timer->start(100);

    scrolltimer = new QTimer(this);
    scrolltimer->start(5000);
}

void Widget::lyricInit()
{
    // 在groupbox中动态创建用于显示歌词的QLabel数组
    for (int i = 0; i < 100; i++)
    {
        lyricLabels[i] = new QLabel(ui->groupBox);  // groupBox是用于显示歌词的UI控件
        lyricLabels[i]->setGeometry(0, 60 * i, ui->groupBox->width() - 20, 60);  // 设置每个歌词标签的位置
        lyricLabels[i]->setAlignment(Qt::AlignCenter);
    }

    lyric = new Lyric();

    ui->selectButton->hide();
}

void Widget::volumeInit()
{
    ui->verticalSlider->hide();
}

void Widget::listInit()
{
    ui->listWidget->hide();
    QScrollBar *verticalScrollBar = new QScrollBar(Qt::Vertical);
    verticalScrollBar->setStyleSheet(
        "QScrollBar:vertical { width: 30px;}"
        "QScrollBar::handle:vertical { background: rgb(255, 255, 255); border-radius: 10px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
     );
    ui->listWidget->setVerticalScrollBar(verticalScrollBar);
}

void Widget::paintEvent(QPaintEvent *event)
{
    /* 指定父对象，this指本窗口,在paintEvent创建painter，执行完之后会自动销毁 */
    QPainter painter(this);

    /* 设置抗锯齿，流畅转换 */
    painter.setRenderHints(QPainter::Antialiasing
                           | QPainter::SmoothPixmapTransform);

    angle += 0.5;
    /* 计算旋转角度 */
    if (angle == 360)
        angle = 0;

    QImage image;
    image.load(":/images/cd.png");
    //                           调整图像大小时保持其原有的宽高比
    image = image.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QRectF rect(150,80,
              image.width(),image.height());

    /* 默认参考点为左上角原点（0,0），因为旋转需要以图形的中心为参考点，
     * 我们使用translate把参考点设置为CD图形的中心点坐标 */
    painter.translate(0 + rect.x() + rect.width() / 2,
                       0 + rect.y() + rect.height() / 2);

    painter.rotate(angle);

    /* 现在参考点为CD图形的中心，需要把它设置回原点的位置，
     * 所以需要减去上面加上的数 */
    painter.translate(0 - (rect.x() + rect.width() / 2),
                      0 - (rect.y() + rect.height() / 2));

    /* 画图 */
    painter.drawImage(rect, image);
}

void Widget::on_cycleButton_stateChanged(int state)
{
    // 仅支持自动播放，不支持手动上一首下一首
    switch(state)
    {
    case 0:
        musicPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
        break;
    case 1:
        musicPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        break;
    case 2:
        musicPlaylist->setPlaybackMode(QMediaPlaylist::Random);
    }
}

void Widget::on_volumeButton_clicked(bool checked)
{
    if(checked)
    {
        ui->verticalSlider->show();
    }
    else
    {
        ui->verticalSlider->hide();
    }
}

void Widget::on_verticalSlider_valueChanged(int value)
{
    musicPlayer->setVolume(value);
    if(!value)
    {
        ui->volumeButton->setStyleSheet("QPushButton{border-image:url(:/images/volume-off.png)}");
    }
    else
    {
        ui->volumeButton->setStyleSheet("QPushButton{border-image:url(:/images/volume.png)}");
    }
}

void Widget::on_listButton_clicked(bool checked)
{
    if(checked)
    {
        ui->listWidget->show();
    }
    else
    {
        ui->listWidget->hide();
    }
}

void Widget::on_listWidget_currentRowChanged(int currentRow)
{
    musicPlaylist->setCurrentIndex(currentRow-1);
    musicPlayer->play(); // 自动播放
}

void Widget::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0)
    {                    // 当滚轮远离使用者时
        emit wheel_signal(-1);
    }
    else if(event->angleDelta().y() < 0)
    {                                     // 当滚轮向使用者方向旋转时
        emit wheel_signal(1);
    }
}

bool Widget::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::TouchBegin:
    {
        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
        QPointF touchPointF = touchEvent->touchPoints().first().pos();
        QPoint touchPoint(touchPointF.toPoint()); // 转换为 QPoint

        // 检查触摸点下的控件
        QWidget *widgetUnderTouch = QApplication::widgetAt(touchPoint);
        if (qobject_cast<QLabel *>(widgetUnderTouch))
        {
            startPoint = touchPoint.y(); // 记录开始点
            qDebug() << "start" << endl;
            return true; // 事件被处理
        }
        else
        {
            // 如果是别的区域，返回不处理触摸事件
            return false;
        }
    }
    case QEvent::TouchUpdate:
    {
        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
        // 处理触摸更新事件
        int currentPoint = touchEvent->touchPoints().first().pos().y();
        // 计算 Y 坐标差
        if (currentPoint - startPoint >= 60)
        {
            // 向上滑动逻辑
            scroll(-1);
            qDebug() << "Down" <<endl;
            startPoint = currentPoint; // 更新上次触摸点
        }
        else if (currentPoint - startPoint <= -60)
        {
            // 向下滑动逻辑
            scroll(1);
            qDebug() << "up" <<endl;
            startPoint = currentPoint; // 更新上次触摸点
        }
        return true; // 事件被处理
    }
    case QEvent::TouchEnd:
    {
        qDebug() << "end" <<endl;
        return true; // 事件被处理
    }
    default:
        break;
    }
    return QWidget::event(event); // 继续处理其他事件
}

//bool Widget::event(QEvent *event)
//{
//    qDebug() << "Event type:" << event->type(); // 输出事件类型
//    if (event->type() == QEvent::TouchBegin || event->type() == QEvent::TouchUpdate || event->type() == QEvent::TouchEnd)
//    {
//        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
//        qDebug() << "触控事件检测到！";

//        for (const QTouchEvent::TouchPoint &point : touchEvent->touchPoints())
//        {
//            qDebug() << "触控点位置:" << point.pos();
//        }

//        return true; // 事件被处理
//    }
//    return QWidget::event(event); // 继续处理其他事件
//}

void Widget::on_wheel(int flag)
{
    lyric->scrollflag = 1;
    ui->selectButton->show();

   if(flag==1)     //up
   {
       lyric->lyricUp(lyricLabels,ui->groupBox);
   }
   else           //down
   {
       lyric->lyricDown(lyricLabels,ui->groupBox);
   }
}

void Widget::scroll(int flag)
{
    lyric->scrollflag = 1;
    ui->selectButton->show();

   if(flag==1)     //up
   {
       lyric->lyricUp(lyricLabels,ui->groupBox);
   }
   else           //down
   {
       lyric->lyricDown(lyricLabels,ui->groupBox);
   }
}

void Widget::on_scrolltimerout()
{
    if(lyric->scrollflag == 1)
    {
        lyric->scrollflag = 0;
        QLabel *labelCentre = (QLabel*)ui->groupBox->childAt(0, 180);
        if (labelCentre != nullptr)
        {
            labelCentre->setStyleSheet("QLabel{color:grey;}");
            QFont fontCentre("Display", 15, QFont::Light);// 字体名称、大小、样式
            labelCentre->setFont(fontCentre);
        }
        ui->selectButton->hide();
    }
}

void Widget::on_selectButton_clicked()
{
    QWidget* widgetAtPoint = ui->groupBox->childAt(0, 180);
    int selectpos;
    if (widgetAtPoint)
    {
        QList<QWidget*> children = ui->groupBox->findChildren<QWidget*>();// 获取 groupBox 中所有子控件的列表
        selectpos = children.indexOf(widgetAtPoint);// 获取该子控件在列表中的索引
        // index 是该子控件在 groupBox 中的索引
        qint64 selecttime = lyric->lyrics[selectpos].time;
        musicPlayer->setPosition(selecttime);
        lyric->movePos(selecttime,lyricLabels,ui->groupBox);
    }
}

# Music Player

这是一个简单的本地音乐播放器项目，使用C++和Qt框架开发。

## 目录

- [项目结构](#项目结构)
- [文件说明](#文件说明)
- [安装和运行](#安装和运行)
- [功能](#功能)
- [使用说明](#使用说明)
- [贡献](#贡献)
- [许可证](#许可证)

## 项目结构

- [images/](images/)
- [lyric.cpp](lyric.cpp)
- [lyric.h](lyric.h)
- [main.cpp](main.cpp)
- [music_player.pro](music_player.pro)
- [music_player.pro.user](music_player.pro.user)
- [README.md](README.md)
- [res.qrc](res.qrc)
- [widget.cpp](widget.cpp)
- [widget.h](widget.h)
- [widget.ui](widget.ui)

## 文件说明

- `images/`：存放项目使用的图片资源。
- `lyric.cpp` 和 `lyric.h`：处理歌词相关的功能。
- `main.cpp`：项目的入口文件。
- `music_player.pro` 和 `music_player.pro.user`：Qt项目文件。
- `README.md`：项目的说明文档。
- `res.qrc`：Qt资源文件。
- `widget.cpp` 和 `widget.h`：主窗口的实现和声明。
- `widget.ui`：主窗口的UI设计文件。

## 安装和运行

### 环境要求

- Qt 5.12或更高版本
- C++编译器（如GCC或MSVC）

### 编译步骤

1. 克隆项目到本地：

    ```sh
    git clone https://github.com/yourusername/music_player.git
    cd music_player
    ```

2. 使用Qt Creator打开`music_player.pro`文件，或者在终端中运行以下命令：

    ```sh
    qmake music_player.pro
    make
    ```

3. 编译完成后，运行生成的可执行文件：

    ```sh
    ./music_player
    ```

## 功能

- 播放音乐文件
- 同步显示歌词
- 仿照网易云音乐的用户界面
- 播放控制（播放、暂停、停止、上一曲、下一曲）
- 音量控制
- 滑动歌词（pc端上鼠标滑动、开发板上触屏滑动）
- 滑动歌词后选择播放位置

## 使用说明

1. 启动音乐播放器，在对应文件夹中放入.mp3和.lrc文件。
2. 直接播放或使用音乐列表功能加载音乐文件。
3. 使用播放控制按钮进行播放、暂停等操作。
4. 如果音乐文件包含歌词，会自动显示歌词。
5. 通过滑动歌词可以选择播放位置。

## 贡献

欢迎提交问题和贡献代码。请确保在提交之前阅读并遵循项目的贡献指南。

### 提交代码

1. Fork本仓库。
2. 创建一个新的分支：

    ```sh
    git checkout -b feature-branch
    ```

3. 提交你的修改：

    ```sh
    git commit -am 'Add new feature'
    ```

4. 推送到远程分支：

    ```sh
    git push origin feature-branch
    ```

5. 创建一个Pull Request。

## 许可证

该项目使用MIT许可证。详情请参阅LICENSE文件。

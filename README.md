# learn_qt

学 Qt 时写的三个小游戏，作为记录留存。

- **推箱子**：跟着 B 站教程做的入门项目
- **飞机大战**：在教程基础上补充了音效和动画
- **斗地主**：完全自己实现的，主要练习网络编程

## 推箱子（pushBox）

最简单的一个，主要练习了：

- `.qrc` 资源文件的使用，理解 `:/src/xxx.png` 这种路径的含义
- 把地图数据存到 `map.txt` 里，改关卡不用动代码
- 把 `GameMap`、`Player` 和界面 `Widget` 拆开写，第一次有意识做"前后端分离"

文件结构：

```
pushBox/
├── main.cpp
├── widget.cpp/h          # 主窗口
├── GameMap.cpp/h         # 地图，从 map.txt 读
├── Player.cpp/h          # 玩家
├── publicEnum.h          # 方向、格子类型的枚举
├── map.txt               # 关卡数据
├── img.qrc               # background.jpg、gameMap.png
└── src/
```

地图是 CSV 格式，每行逗号分隔的数字代表不同格子类型（墙、地面、箱子、目标点）。

## 飞机大战（PlaneFighting）

B 站老师那版只有基础功能，没有音效，爆炸也没有动画。我在此基础上补充了：

**补充的动画帧**（通过连续图片切换实现）：
- 小敌机爆炸 4 帧：`enemy1_down1` ~ `down4`
- 中敌机爆炸 4 帧 + 受击图 `enemy2_hit`
- 大敌机爆炸 6 帧 + 受击图 + 双形态 `enemy3_n1/n2`
- 玩家坠毁 4 帧：`me_destroy_1` ~ `4`

**补充的音效**（一共 14 个）：
- 背景音乐 `game_music.ogg`
- 射击、按钮点击、各种敌机击落、道具拾取、炸弹使用、升级等

另外加了 `GameItemPool` 对象池，避免频繁 new/delete。

类继承关系：

```
GameItem
   ├── Plane
   │     ├── Player
   │     └── Enemy
   └── Bullet
```

```
PlaneFighting/
├── GameManager.cpp/h         # 主控制
├── GameItem.cpp/h            # 物品基类
├── Plane.cpp/h
├── Player.cpp/h
├── Enemy.cpp/h
├── Bullet.cpp/h
├── GameItemPool.cpp/h        # 对象池
├── GameInitialConfig.cpp/h   # 配置
├── img.qrc
└── src/
    ├── images/               # 30 多张图
    └── sound/                # 14 个音频
```

依赖 Qt6 的 `Widgets` 和 `Multimedia`（音效需要）。

## 斗地主（landlord）

最复杂的一个。没有教程参考，自己查文档实现的。C/S 架构，客户端和服务端是两个独立工程：

```
landlord/
├── LandlordClientFinal/   # 客户端
└── LandlordServerFinal/   # 服务端
```

用 `QTcpSocket` / `QTcpServer` 走 TCP。

### 协议设计

放在 `protocolString.h` 里，用单字符作为消息标识，大小写区分方向：

- 客户端发给服务端：小写 `a` ~ `z`
- 服务端发给客户端：大写 `A` ~ `Z`

举几个例子：

| 字符 | 含义 |
| --- | --- |
| `a` | 客户端准备好了 |
| `A` | 服务端欢迎反馈 |
| `g` | 出牌 |
| `O` | 广播某玩家出了什么 |
| `S` | 倒计时显示 |

流程基本是：登录 → 匹配房间 → 发牌 → 抢地主 → 发三张底牌 → 出牌 → 判胜负 → 再来一局。

服务端用了多线程（`GetLandlordWorker`）处理抢地主，避免阻塞主线程的网络监听。

### 客户端

```
LandlordClientFinal/
├── Widget.cpp/h        # 主窗口 + 网络通信
├── Loading.cpp/h       # 加载界面
├── Game.cpp/h          # 游戏界面
├── Card.cpp/h          # 卡牌
├── CardPicConfig.h     # 卡牌图片配置
├── protocolString.h    # 协议
├── img.qrc             # 1s.png ~ 4s.png、Card.png
└── *.ui
```

依赖 Qt6 的 `Widgets` 和 `Network`。

### 服务端

```
LandlordServerFinal/
├── Widget.cpp/h              # 主窗口 + QTcpServer 监听
├── GameManager.cpp/h          # 游戏管理
├── Card.cpp/h
├── GetLandlordWorker.cpp/h   # 抢地主线程
├── Getlandlord_Worker.cpp/h  # 早期命名，迭代过程中的痕迹
└── protocolString.h
```

### 不足之处

第一次写网络程序，有不少问题：

- 没做断线重连
- 牌型识别写得不够清晰
- 早期命名 `GetLandlordWorker` 和 `Getlandlord_Worker` 不一致，是迭代过程中留下的

不过基本能正常运行，也借此熟悉了 TCP 编程、协议设计、多线程这些内容。

## 编译方式

每个项目都有独立的 `CMakeLists.txt`，用 Qt Creator 打开对应文件夹的 CMakeLists.txt 即可编译。

- 需要 Qt 6 + CMake 3.16 以上
- 编译器支持 C++17 即可
- 斗地主要先启动服务端，再启动客户端

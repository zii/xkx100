# xkx100
侠客行100 UTF-8 中文版
(基于MudRen/xkx100的fork)

## 新增特性

### 玩家指令

- `home [+<数字>|<数字>|-<数字>]` - 记录/移动到/删除回城点（1-9），共可设 9 个
- `localcmds (lc)` - 查看当前房间所有可用指令
- `deposit/cun all` - 将身上所有现金一次性存入钱庄
- `withdraw/qu <数量>[g|s|c]` - 取款，不跟单位默认白银（例: 100=100银, 100g=100金, 100c=100文）
- `verify <武功名称>` - 查询此项技能的功能和数据

### 全局别名

- `lm` → `map`
- `sk` → `skills`
- `map` → `map all`
- `sc` → `score`
- `lc` → `localcmds`

### 环境变量

- `set no_more 1` - map 不分页
- `set no_hunger 1` - 不饥渴
- `set no_weather 1` - 不显示天气信息

### 巫师指令

- `sppasswd <id> NO` - 免密登陆
- `roomid <房间名关键词>` - 按房间名查找房间档名
- `npcid <关键词>` - 按 NPC 名字查找所在位置

## 使用说明

项目下载

    # clone xkx100
    git clone https://github.com/zii/xkx100.git


请使用 fluffos 最新版驱动

    driver config.ini

## Docker 运行

两种方式：

### 方式一：docker-compose（推荐）

项目已包含 `docker-compose.yml` 配置，默认使用最新版 FluffOS master：

```bash
# 启动
docker compose up -d

# 查看日志
docker compose logs -f

# 停止
docker compose down

# 重启
docker compose restart
```

配置文件 `docker-compose.yml` 中可修改镜像版本：
```yaml
image: ghcr.io/fluffos/fluffos:master    # 最新开发版
# 或
image: ghcr.io/fluffos/fluffos:v2019.20220507  # 稳定版
```

### 方式二：直接 docker run

使用 Docker 镜像 `ghcr.io/fluffos/fluffos:v2019.20220507`：

```bash
cd /path/to/xkx100

docker run -d --name xkx100 \
  -v $(pwd):/mudlib \
  -p 5555:5555 \
  -p 6666:6666 \
  -p 8888:8888 \
  --entrypoint sh \
  ghcr.io/fluffos/fluffos:v2019.20220507 \
  -c "cd /mudlib && /fluffos/bin/driver /mudlib/config.ini"
```

> 注意：`cd /mudlib &&` 是必须的，因为 driver 在读取 `mudlib directory` 配置前会先打开日志文件，而容器默认工作目录不是 `/mudlib`。

**管理命令：**

| 命令 | 说明 |
|------|------|
| `docker logs xkx100` | 查看启动日志 |
| `docker logs -f xkx100` | 实时跟踪日志 |
| `docker stop xkx100` | 停止服务 |
| `docker start xkx100` | 启动服务 |
| `docker rm xkx100` | 删除容器 |

**验证：**

```bash
telnet localhost 5555   # GBK编码
telnet localhost 6666   # UTF-8编码
```

游戏端口：5555(GBK编码)、6666(UTF8编码)；网页访问端口：8888

## macOS 容器运行时：Colima

Docker Desktop 在 macOS 上占用资源较多（通常 4-5GB），导致系统卡顿。推荐使用 [Colima](https://github.com/abiosoft/colima) 作为轻量替代：

```bash
# 安装
brew install colima

# 启动（按需分配资源，跑 MUD 最低 0.375G/1CPU 即可）
colima start --cpu 1 --memory 0.5 --disk 2

# docker compose 启动游戏
docker compose up -d
```

Colima 内存占用约 Docker Desktop 的 1/4，对 MUD 这种轻负载绰绰有余。

**常用命令：**

| 命令 | 说明 |
|------|------|
| `colima start --cpu 1 --memory 1 --disk 10` | 启动（资源按需调整） |
| `colima stop` | 停止（不删除虚拟机） |
| `colima delete` | 删除虚拟机，重建全新的 |
| `colima ssh -- free -h` | 查看 VM 内部内存使用 |

**配置文件：** `~/.colima/default/colima.yaml`，可直接编辑 `memory`、`cpu` 等参数，改后重启生效。

## 性能优化

### tmpfs 热路径加速

FluffOS 在启动时会将 `d/`、`kungfu/`、`clone/` 等目录的大量 LPC 文件读入内存。在 macOS 上通过 Docker bind mount（走 sshfs）访问这些文件有显著的 I/O 开销。

本项目通过 Docker tmpfs + 启动时复制的方式，将热路径文件放在容器的**内存文件系统**中运行：

```yaml
tmpfs:
  - /mudlib/d:rw,noexec,nosuid,size=64M       # 房间文件 ~9000 个
  - /mudlib/kungfu:rw,noexec,nosuid,size=16M   # 武功文件
  - /mudlib/clone:rw,noexec,nosuid,size=16M    # 克隆对象
```

**工作流程：**
```
宿主机硬盘 (ext4/apfs)  →  sshfs bind mount  →  容器 tmpfs (内存)
     ↑ 改代码在这写                                   ↓
                                        FluffOS 加载 LPC 时读这里
```

**开发时注意：** tmpfs 中的文件是容器启动时的快照，宿主机上改 `d/`、`kungfu/`、`clone/` 不会自动同步到容器内。需要开启同步脚本：

```bash
# 需先安装 fswatch
brew install fswatch

# 监控文件变更，实时同步到容器 tmpfs
./sync-container.sh
```

该脚本通过 `base64` 管道写入容器内部，绕过 `docker cp` 在 tmpfs 上建文件权限不可靠的问题。

`entrypoint` 负责在容器启动时把数据拷入 tmpfs，39M 数据耗时 < 1s：
```sh
cp -a /d-source/. /mudlib/d/
cp -a /kungfu-source/. /mudlib/kungfu/
cp -a /clone-source/. /mudlib/clone/
```

**效果对比：**

| 场景 | bind mount（sshfs） | tmpfs |
|------|-------------------|-------|
| roomid 遍历 8965 个文件 | macOS → sshfs → VM | 纯内存 |
| update 房间 | 同上 | 纳秒级 |
| goto 新区域首次加载 | 高延迟 | 低延迟 |
| 开发改代码 | 直接编辑，实时生效 | 正常，改在宿主机上，重启容器拉新 |

**优势：** 热数据（LPC 源码）在 tmpfs 中读写，冷数据（玩家存档、日志）走 sshfs bind mount，既保证了开发体验（改代码即改即生效），又让高频 I/O 路径达到纯内存速度，甚至优于 FluffOS 直接在原生 ext4 上运行的效果。

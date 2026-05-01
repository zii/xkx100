# xkx100
侠客行100 UTF-8 中文版
(基于MudRen/xkx100的fork)

## 使用说明

项目下载

    # clone xkx100
    git clone --recurse-submodules https://github.com/zii/xkx100.git


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

## 性能优化

将 `/d/`（房间文件，约 9000 个）挂载为 Docker tmpfs，避免 macOS 文件共享（VirtioFS）的 I/O 开销，显著提升文件读写和对象加载速度。

```yaml
tmpfs:
  - /mudlib/d:rw,noexec,nosuid,size=64M
```

启动时自动从 host 复制到 tmpfs，39M 数据耗时 < 1s，重启容器后生效。

roomid 原来要走 macOS → VirtioFS → VM 读 8965 个文件，现在全在 Docker 内存里。同样的原理，update 房间、goto 去新区域、look 第一次进房间，都会快。

**添加新指令:**

set no_more 1 - map 不分页
set no_hunger 1 - 不饥渴
set no_weather 1 - 不显示天气信息

verify <武功名称> - 这个指令可以让你查询此项技能的功能和数据

**添加新巫师指令:**

sppasswd <id> NO - 免密登陆
roomid <房间名关键词> - 按房间名查找房间档名
npcid <关键词> - 按npc名字查找所在位置

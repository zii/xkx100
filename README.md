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

本LIB为GBK旧版升级到utf-8版，目前代码基本无编译错误，原代码BUG已基本修复，升级造成的BUG已知部分已修复，不过中文字符判断（中文判断`[0..1]`需要改为`[0..0]`）可能存在遗漏，目前已知存在以下问题：

1. 中文字符排版显示未完全修复，比如地图，会影响排版美观；
2. 表情系统无效，因为emoted.o文件中空，可自己使用 edemote 指令补充；
3. 技能中文翻译不全，因为e2c_dict.o是从[炎黄LIB](https://github.com/oiuv/mud)中复制过来的，对游戏中显示为英文的地方需要自己补充翻译（补充方式如：chinese city=扬州）

**添加新指令:**

set no_more 1 - map 不分页
set no_hunger 1 - 不饥渴

**添加新巫师指令:**

sppasswd <id> NO 免密登陆

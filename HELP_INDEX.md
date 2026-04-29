# xkx100 帮助信息总览

> 根据游戏内帮助系统整理，涵盖所有帮助主题、指令和文档。

## 帮助系统使用

| 命令 | 说明 |
|------|------|
| `help` | 显示主菜单 |
| `help <主题>` | 查看指定主题帮助 |
| `help cmds` | 列出所有可用指令 |
| `help <指令>` | 查看指令帮助 |
| `help <技能>` | 查看武功技能帮助 |
| `help <技能>.<招式>` | 查看技能的特殊招式帮助 |

帮助文件搜索路径：`/doc/help/` → `/doc/skill/` → `/doc/efuns/` → `/doc/applies/`

---

## 一、游戏概况

| 主题 | 文件 | 说明 |
|------|------|------|
| `intro` | `/doc/help/intro` | 侠客行一百简介 |
| `newbie` | `/doc/help/newbie` | 新手指南 |
| `rules` | `/doc/help/rules` | 玩家规则 |
| `setup` | `/doc/help/setup` | 环境变量设置 |
| `gift-char` | `/doc/help/gift-char` | 天赋与属性关系 |
| `user-level` | `/doc/help/user-level` | 玩家进阶篇 |
| `cmds` | 内置 | 命令一览（列出所有可用指令） |
| `map` | 内置 | 地图系统 |

---

## 二、新手指南

| 主题 | 说明 |
|------|------|
| `newbie-basic` | 基础操作指南 |
| `newbie-command` | 指令使用指南 |
| `newbie-cookery` | 烹饪入门 |
| `newbie-genius` | 天赋选择指南 |
| `newbie-mission` | 任务入门 |

#### 各门派新手指南

| 主题 | 门派 | 主题 | 门派 |
|------|------|------|------|
| `newbie-shaolin` | 少林派 | `newbie-shaolin1` | 少林派(续) |
| `newbie-wudang` | 武当派 | `newbie-emei` | 峨嵋派 |
| `newbie-huashan` | 华山派 | `newbie-quanzhen` | 全真教 |
| `newbie-gumu` | 古墓派 | `newbie-mingjiao` | 明教 |
| `newbie-hengshan` | 恒山派 | `newbie-kunlun` | 昆仑派 |
| `newbie-gaibang` | 丐帮 | `newbie-dali` | 大理段家 |
| `newbie-baituo` | 白驼山 | `newbie-xiaoyao` | 逍遥派 |
| `newbie-lingxiao` | 灵鹫宫 | `newbie-songshan` | 嵩山派 |
| `newbie-tiezhang` | 铁掌帮 | `newbie-wudujiao` | 五毒教 |
| `newbie-yunlong` | 云龙门 | | |

---

## 三、指令大全

### 玩家基本指令 (`/cmds/usr/`)

| 指令 | 说明 |
|------|------|
| `alias` | 设置命令别名 |
| `boss` | 老板键 |
| `bossgone` | 取消老板键 |
| `cls` | 清屏 |
| `describe` | 设置描述 |
| `exit` | 退出游戏 |
| `finger` | 查询玩家信息 |
| `help` | 帮助系统 |
| `hp` | 查看状态（精、气、神、内力） |
| `id` | 查看物品ID |
| `inventory` | 查看背包 |
| `map` | 地图系统 |
| `mudlist` | 站点列表 |
| `news` | 新闻 |
| `nick` | 设置昵称 |
| `passwd` | 修改密码 |
| `quest` | 查看任务 |
| `quit` | 退出 |
| `save` | 存档 |
| `score` | 查看详细属性 |
| `set` | 设置环境变量 |
| `sos` | 求救 |
| `suicide` | 自杀 |
| `time` | 游戏时间 |
| `title` | 设置头衔 |
| `unset` | 取消环境变量 |
| `version` | 版本信息 |
| `who` | 在线玩家列表 |
| `wimpy` | 设置懦弱模式 |
| `wizlist` | 巫师列表 |

### 玩家动作指令 (`/cmds/std/`)

| 指令 | 说明 | 指令 | 说明 |
|------|------|------|------|
| `ansuan` | 暗算 | `ask` | 询问 |
| `attack` | 攻击 | `baitan` | 摆摊 |
| `beg` | 乞讨 | `bugs` | 提交Bug |
| `buy` | 购买 | `ce` | 测算 |
| `check` | 检查 | `chi` | 吃 |
| `close` | 关闭 | `come` | 召唤 |
| `cuifa` | 催发 | `dating` | 打听 |
| `demote` | 开除 | `drink` | 喝 |
| `drop` | 丢弃 | `eat` | 吃 |
| `emote` | 表情动作 | `feed` | 喂食 |
| `femote` | 表情动作 | `fight` | 较量 |
| `follow` | 跟随 | `gamble` | 赌博 |
| `get` | 捡起 | `give` | 给与 |
| `go` | 移动 | `guard` | 保护 |
| `halt` | 停止战斗 | `han` | 喊 |
| `hbless` | 早安 | `he` | 治疗 |
| `hit` | 击打 | `imbibe` | 饮 |
| `jiasi` | 假死 | `jiebai` | 结拜 |
| `kantou` | 砍头 | `kill` | 杀死 |
| `list` | 列表 | `locate` | 定位 |
| `location` | 所在地 | `look` | 查看 |
| `mbless` | 晚安 | `open` | 打开 |
| `piao` | 嫖 | `pour` | 倒 |
| `put` | 放入 | `qiecuo` | 切磋 |
| `qixing` | 骑行 | `release` | 释放 |
| `remove` | 脱下 | `reply` | 回复 |
| `ride` | 骑乘 | `say` | 说话 |
| `semote` | 表情列表 | `setwmsg` | 设置留言 |
| `shexin` | 摄心 | `shop` | 店铺 |
| `shoplist` | 店铺列表 | `sleep` | 睡觉 |
| `stay` | 停留 | `steal` | 偷窃 |
| `stop` | 停止 | `surrender` | 投降 |
| `task` | 任务 | `team` | 组队 |
| `tell` | 私聊 | `touxi` | 偷袭 |
| `train` | 训练 | `trap` | 陷阱 |
| `tune` | 频道开关 | `unride` | 下马 |
| `unwield` | 收起武器 | `vote` | 投票 |
| `wear` | 穿戴 | `whisper` | 耳语 |
| `wield` | 装备武器 | | |

### 武功技能指令 (`/cmds/skill/`)

| 指令 | 说明 | 指令 | 说明 |
|------|------|------|------|
| `abandon` | 放弃技能 | `apprentice` | 拜师 |
| `bai` | 拜师 | `bei` | 备招 |
| `burning` | 焚心 | `cha` | 查看技能 |
| `dazuo` | 打坐 | `du` | 读书 |
| `editskill` | 编辑技能 | `enable` | 激发技能 |
| `enforce` | 加力 | `exercise` | 练功 |
| `exert` | 运功 | `expell` | 逐出师门 |
| `fangqi` | 放弃 | `fennu` | 愤怒 |
| `jiali` | 加力 | `jiao` | 教 |
| `jifa` | 激发 | `jingzuo` | 静坐 |
| `kaichu` | 开除 | `learn` | 学习 |
| `lian` | 练习 | `mylian` | 练习 |
| `perform` | 绝技 | `persuade` | 说服 |
| `practice` | 练习 | `prepare` | 备招 |
| `quanjia` | 劝架 | `recruit` | 收徒 |
| `research` | 研究 | `respirate` | 吐纳 |
| `shou` | 收徒 | `skills` | 技能列表 |
| `study` | 学习 | `teach` | 教学 |
| `tuna` | 吐纳 | `xue` | 学 |
| `yanjiu` | 研究 | `yanlian` | 演练 |
| `yong` | 用 | `yun` | 运功 |

---

## 四、技能系统

| 主题 | 文件 | 说明 |
|------|------|------|
| `skill` | `/doc/help/skill` | 武技篇总览 |
| `weapon` | `/doc/help/weapon` | 兵器篇 |
| `combine` | `/doc/help/combine` | 武功互备列表 |
| `knowledge` | `/doc/help/knowledge` | 知识技能 |
| `neigong` | `/doc/help/neigong` | 内功功用 |

技能类型一览：

| 类型 | 说明 | 类型 | 说明 |
|------|------|------|------|
| sword | 剑法 | blade | 刀法 |
| staff | 杖法 | club | 棍法 |
| stick | 棒法 | hammer | 锤法 |
| axe | 斧法 | spear | 枪法 |
| whip | 鞭法 | dagger | 短兵 |
| throwing | 暗器 | force | 内功 |
| dodge | 轻功 | parry | 招架 |
| cuff | 拳法 | strike | 掌法 |
| claw | 爪法 | hand | 手法 |
| finger | 指法 | leg | 腿法 |
| array | 阵法 | magic | 法术 |
| unarmed | 拳脚 | | |

---

## 五、城市地图

| 主题 | 说明 | 主题 | 说明 |
|------|------|------|------|
| `yangzhou` | 扬州 | `beijing` | 北京 |
| `changan` | 长安 | `chengdu` | 成都 |
| `dali` | 大理 | `foshan` | 佛山 |
| `fuzhou` | 福州 | `hangzhou` | 杭州 |
| `luoyang` | 洛阳 | `suzhou` | 苏州 |
| `wuxi` | 无锡 | `xiangyang` | 襄阳 |
| `yanping` | 延平 | `yueyang` | 岳阳 |
| `nanyang` | 南阳 | `quanzhou` | 泉州 |
| `jiangling` | 江陵 | `jiaxing` | 嘉兴 |
| `lanzhou` | 兰州 | `lingzhou` | 灵州 |
| `hanzhong` | 汉中 | `hengyang` | 衡阳 |
| `gaochang` | 高昌 | `xiakedao` | 侠客岛 |
| `yixing` | 宜兴 | `city` | 城市篇总览 |

---

## 六、门派大全

| 主题 | 门派 | 主题 | 门派 |
|------|------|------|------|
| `shaolin` | 少林派 | `nanshaolin` | 南少林 |
| `wudang` | 武当派 | `emei` | 峨嵋派 |
| `huashan` | 华山派 | `henshan` | 衡山派 |
| `hengshan` | 恒山派 | `songshan` | 嵩山派 |
| `taishan` | 泰山派 | `quanzhen` | 全真教 |
| `gumu` | 古墓派 | `mingjiao` | 明教 |
| `riyuejiao` | 日月神教 | `xingxiu` | 星宿派 |
| `xiaoyao` | 逍遥派 | `lingjiu` | 灵鹫宫 |
| `taohua` | 桃花岛 | `gaibang` | 丐帮 |
| `dali` | 大理段家 | `tiezhang` | 铁掌帮 |
| `baituo` | 白驼山 | `murong` | 慕容世家 |
| `shenlong` | 神龙教 | `tiandihui` | 天地会 |
| `honghua` | 红花会 | `lingxiao` | 凌霄城 |
| `mizong` | 密宗 | `jinshe` | 金蛇洞 |
| `changle` | 长乐帮 | `wudujiao` | 五毒教 |
| `kunlun` | 昆仑派 | `qingcheng` | 青城派 |
| `xiakedao` | 侠客岛 | | |

---

## 七、任务系统

| 主题 | 文件 | 说明 |
|------|------|------|
| `mission` | `/doc/help/mission` | 任务总览 |

#### 子任务列表 (`/doc/help/ms_*`)

| 主题 | 说明 | 主题 | 说明 |
|------|------|------|------|
| `ms_daniao` | 打鸟任务 | `ms_tangbu` | 唐捕任务 |
| `ms_madai` | 马岱任务 | `ms_caishi` | 采石任务 |
| `ms_fenyao` | 分药任务 | `ms_wuguan` | 武馆任务 |
| `ms_songxin` | 送信任务 | `ms_shangdui` | 商队任务 |
| `ms_xiaobao` | 小宝任务 | `ms_xuncheng` | 巡城任务 |
| `ms_shan` | 善事任务 | `ms_helian` | 赫连任务 |
| `ms_miji` | 秘籍任务 | `ms_betrayer` | 判师任务 |
| `ms_jianxi` | 奸细任务 | `ms_killer` | 杀手任务 |
| `ms_bukuai` | 捕快任务 | `ms_haibu` | 海捕任务 |
| `ms_renzhi` | 仁至任务 | `ms_baobiao` | 保镖任务 |
| `ms_volunteer` | 志愿任务 | `ms_yishi` | 议事任务 |
| `ms_zjjianxi` | 终极奸细 | `ms_shoucheng` | 守城任务 |
| `ms_feige` | 飞鸽任务 | `ms_baowu` | 宝物任务 |
| `ms_xingyi` | 行医任务 | `ms_xunfang` | 寻访任务 |
| `ms_feizei` | 飞贼任务 | `ms_task` | 循环任务 |
| `ms_xunshan` | 巡山任务 | `ms_zhangmen` | 掌门任务 |
| `ms_dazongshi` | 大宗师任务 | `ms_diaoyu` | 钓鱼任务 |
| `ms_gaibang` | 丐帮任务 | | |

---

## 八、生活系统

| 主题 | 文件 | 说明 |
|------|------|------|
| `trade` | `/doc/help/trade` | 行商篇（经商贸易） |
| `house` | `/doc/help/house` | 家庭篇（结婚、家庭） |
| `cookery` | `/doc/help/cookery` | 烹饪篇 |
| `medical` | `/doc/help/medical` | 丹药篇 |
| `pet` | `/doc/help/pet` | 宠物篇 |
| `transport` | `/doc/help/transport` | 交通篇（骑马、马车等） |
| `family` | `/doc/help/family` | 门派篇（出师、判师等） |
| `party` | `/doc/help/party` | 帮会篇 |
| `relation` | `/doc/help/relation` | 交流篇（社交关系） |
| `wuguan` | `/doc/help/wuguan` | 武馆篇 |

---

## 九、娱乐竞技

| 主题 | 文件 | 说明 |
|------|------|------|
| `contest` | `/doc/help/contest` | 比武篇 |
| `hslj` | `/doc/help/hslj` | 华山论剑 |
| `game` | `/doc/help/game` | 附属游戏总览 |
| `event` | `/doc/help/event` | 自然奇观 |

#### 小游戏

| 主题 | 说明 | 主题 | 说明 |
|------|------|------|------|
| `chess_cmds` | 围棋指令 | `go_cmds` | 围棋 |
| `mj_cmds` | 麻将指令 | `mj_rules` | 麻将规则 |
| `pig_cmds` | 拱猪指令 | `pig_rules` | 拱猪规则 |
| `pj_rules` | 牌九规则 | `poke_rules` | 扑克规则 |
| `sea_cmds` | 海战指令 | `fly_cmds` | 飞行棋 |
| `flower` | 花花游戏 | `bobing` | 博饼 |
| `ttt` | 井字游戏 | `yummi` | 五子棋 |
| `board` | 留言板 | `channels` | 频道系统 |
| `mail` | 邮件系统 | | |

---

## 十、巫师相关

### 巫师帮助主题

| 主题 | 文件 | 说明 |
|------|------|------|
| `wiz-rules` | `/doc/help/wiz-rules` | 巫师规则篇 |
| `wiz-setup` | `/doc/help/wiz-setup` | 巫师环境变量设置篇 |

### 巫师等级体系

| 等级 | 称号(男/女) | 说明 |
|------|-------------|------|
| `(player)` | 普通玩家 | 无权限 |
| `(immortal)` | 仙童/女仙童 | 最低权限 |
| `(apprentice)` | 半仙/仙女 | 见习巫师 |
| `(wizard)` | 神仙/神女 | 正式巫师 |
| `(arch)` | 仙官/仙妃 | 管理员 |
| `(admin)` | 天帝/天后 | 最高权限 |

### 巫师指令

####  immortal（仙童）— `/cmds/imm/`

| 指令 | 说明 |
|------|------|
| `cd` | 切换目录 |
| `edemote` | 编辑表情 |
| `flyto` | 飞往地点 |
| `full` | 满状态 |
| `goto` | 传送 |
| `ls` | 文件列表 |
| `mem` | 内存信息 |
| `pwd` | 当前目录 |
| `status` | 系统状态 |
| `where` | 查询玩家位置 |

#### wizard（神仙）— `/cmds/wiz/` 核心指令

| 指令 | 说明 |
|------|------|
| `call` | 调用任意函数（万能修改器） |
| `data` | 查看对象原始数据 |
| `sc` | 查看玩家属性 |
| `spl` / `spsk` | 查看技能 |
| `sphp` | 查看HP状态 |
| `spy` | 查看先天/后天属性 |
| `info` / `iinfo` | 查看对象信息 |
| `clone` / `spclone` | 克隆物品 |
| `dest` | 销毁对象 |
| `update` / `updatedir` | 更新/重载文件 |
| `goto` / `summon` / `spcome` | 传送与召唤 |
| `snoop` / `npcsnp` | 监听 |
| `shout` | 广播 |
| `reboot` | 重启游戏 |
| `rehash` | 重设文件缓存 |
| `ban` | 封禁 |
| `kickout` / `kickto` | 踢人/送监狱 |
| `purge` | 删除角色 |
| `sppasswd` | 改密码 |
| `name` | 改名 |
| `body` | 满血恢复 |
| `who1` ~ `who4` | 高级玩家列表 |

#### arch（仙官）— `/cmds/arch/`

| 指令 | 说明 |
|------|------|
| `promote` | 晋升（限 arch 以下） |
| `wizlock` | 巫师锁定 |
| `register` | 注册控制 |
| `restore` | 恢复玩家数据 |
| `recover` | 恢复数据 |
| `cleanup` | 清理对象 |
| `possess` | 附身NPC |
| `reboot` | 重启 |

#### admin（天帝）— `/cmds/adm/`

| 指令 | 说明 |
|------|------|
| `promote` | 晋升任意等级 |
| `eval` | 执行任意LPC代码 |
| `shutdown` | 关机 |
| `adcp` | 管理员复制 |
| `adrm` | 管理员删除 |
| `profile` | 性能分析 |
| `dump` | 导出所有对象 |
| `loadall` | 批量编译检查 |
| `config` | 查看驱动配置 |

---

### 巫师文档 (`/doc/wiz/`)

| 文件 | 说明 |
|------|------|
| `wizhelp1` | ES2 巫师教程（创建房间、对象、NPC 等基础） |
| `wizhelp2` | ES2 巫师手册（巫师等级、权限、职责） |
| `wizhelp3` | 高级巫师主题 |
| `wizhelp4` | 巫师参考大全 |
| `book` | 书籍写作指南 |
| `skill` | 技能系统文档 |
| `skillsetup` | 技能设置指南 |
| `skillstru` | 技能结构指南 |
| `weapon` | 武器系统指南 |
| `FEATURE.ZIP` | 特性文档（压缩） |

---

## 十一、游戏机制

| 主题 | 文件 | 说明 |
|------|------|------|
| `book` | `/doc/help/book` | 书籍篇 |
| `creates` | `/doc/help/creates` | 自创武功篇 |
| `calamity` | `/doc/help/calamity` | 江湖浩劫 |
| `event` | `/doc/help/event` | 自然奇观 |
| `combine` | `/doc/help/combine` | 武功互备 |
| `bug` | `/doc/help/bug` | 臭虫篇（Bug 报告） |
| `gift` | `/doc/help/gift` | 天赋说明 |
| `whatsnew` | `/doc/help/whatsnew` | 更新说明 |

---

## 十二、LPC 语言文档 (`/doc/lpc/`)

LPC 是 FluffOS 驱动的脚本语言，用于编写 MUD 游戏内容。

| 文件 | 说明 |
|------|------|
| `Introduction` | LPC 简介 |
| `chapter1` | 第1章：编码环境介绍 |
| `chapter2` | 第2章：LPC 程序结构 |
| `chapter3` | 第3章：LPC 数据类型 |
| `chapter4` | 第4章：函数 |
| `chapter5` | 第5章：继承基础 |
| `chapter6` | 第6章：变量处理 |
| `chapter7` | 第7章：流程控制 |
| `chapter8` | 第8章：数据类型对象 |

### 语法结构 (`/doc/lpc/constructs/`)

| 文件 | 说明 |
|------|------|
| `for` | for 循环 |
| `function` | 函数 |
| `if` | if 条件判断 |
| `include` | 文件包含 |
| `inherit` | 继承 |
| `prototypes` | 函数原型 |
| `switch` | switch 分支 |
| `while` | while 循环 |

### 预处理 (`/doc/lpc/preprocessor/`)

| 文件 | 说明 |
|------|------|
| `define` | 宏定义 |
| `include` | 文件包含指令 |

### 数据类型 (`/doc/lpc/types/`)

| 文件 | 说明 |
|------|------|
| `array.2d` | 二维数组 |
| `buffer` | 缓冲区 |
| `float` | 浮点数 |
| `function` | 函数类型 |
| `general` | 通用类型 |
| `mappings` | 映射 |
| `strings` | 字符串 |
| `substructures` | 子结构 |

---

## 十三、驱动函数（eFun）文档 (`/doc/efuns/`)

约 234 个 efun 帮助文件，涵盖所有 FluffOS 驱动内置函数。常用 efun 包括：

| 函数 | 说明 |
|------|------|
| `add_action` | 添加玩家指令 |
| `call_out` | 延时调用 |
| `call_other` | 调用其他对象函数 |
| `clone_object` | 克隆对象 |
| `destruct` | 销毁对象 |
| `environment` | 获取环境对象 |
| `file_name` | 获取对象文件名 |
| `find_object` | 查找对象 |
| `find_player` | 查找玩家 |
| `function_exists` | 检查函数存在 |
| `inherits` | 检查继承关系 |
| `living` | 检查是否为活物 |
| `load_object` | 加载对象 |
| `move_object` | 移动对象 |
| `new` | 创建新对象 |
| `present` | 检查物品存在 |
| `query_entire_dbase` | 获取完整数据 |
| `random` | 随机数 |
| `read_file` | 读取文件 |
| `say` | 周围说话 |
| `set_light` | 设置光线 |
| `shout` | 全服广播 |
| `tell_object` | 私聊对象 |
| `tell_room` | 房间广播 |
| `this_object` | 当前对象 |
| `this_player` | 当前玩家 |
| `users` | 所有玩家列表 |
| `write` | 写信息 |
| `write_file` | 写文件 |

---

*本索引由游戏帮助系统自动整理生成。在游戏中使用 `help <主题>` 查看详细内容。*

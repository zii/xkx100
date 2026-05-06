// home.c
// 玩家回城点命令
// home +<数字> 记录当前房间为回城点
// home <数字>  瞬间移动到回城点
// home         查看所有回城点
// home -<数字> 删除回城点

inherit F_CLEAN_UP;

// 目录 → 地名映射
mapping area_names = ([
    "city":"扬州", "shaolin":"少林寺", "wudang":"武当山", "emei":"峨嵋山",
    "huashan":"华山", "gaibang":"丐帮", "gumu":"古墓", "mingjiao":"明教",
    "dali":"大理", "xingxiu":"星宿海", "taohua":"桃花岛", "quanzhen":"全真教",
    "shenlong":"神龙岛", "baituo":"白驼山", "lingjiu":"灵鹫宫", "tiezhang":"铁掌帮",
    "xueshan":"雪山", "wudujiao":"五毒教", "xiaoyao":"逍遥", "kunlun":"昆仑山",
    "taishan":"泰山", "henshan":"衡山", "hengshan":"恒山", "songshan":"嵩山",
    "heimuya":"黑木崖", "lingxiao":"凌霄城", "yunlong":"天地会", "honghua":"红花会",
    "murong":"姑苏", "nanshaolin":"南少林", "qingcheng":"青城山", "xiakedao":"侠客岛",
    "jiaxing":"嘉兴", "suzhou":"苏州", "hangzhou":"杭州", "chengdu":"成都",
    "beijing":"北京", "kaifeng":"开封", "luoyang":"洛阳", "yangzhou":"扬州",
    "changan":"长安", "fuzhou":"福州", "quanzhou":"泉州", "nanyang":"南阳",
    "lanzhou":"兰州", "guangzhou":"广州", "xiangyang":"襄阳", "yueyang":"岳阳",
    "jinshe":"金蛇洞", "meizhuang":"梅庄", "yanziwu":"燕子坞",
    "liangzhu":"梁祝", "guiyun":"归云庄", "wanjiegu":"万劫谷",
    "tianlongsi":"天龙寺", "xuedao":"血刀门", "yubifeng":"玉笔峰", "mingyu":"明玉", "bibo":"碧波",
    "liangshan":"梁山", "nanling":"南岭", "xihu":"西湖", "penglai":"蓬莱",
]);

string get_area(string room_file)
{
    // 从 /d/xxx/... 中提取目录名
    string *parts = explode(room_file, "/");
    for (int i = 0; i < sizeof(parts) - 1; i++)
        if (parts[i] == "d" && sizeof(parts) > i + 1)
        {
            string dir = parts[i + 1];
            string area = area_names[dir];
            return stringp(area) ? area : dir;
        }
    return "";
}

int main(object me, string arg)
{
        mapping points;
        int num;
        string room_file;

        if (arg && arg != "")
        {
                // home +<数字> : 记录回城点
                if (sscanf(arg, "+%d", num) == 1)
                {
                        if (num < 1 || num > 9)
                                return notify_fail("回城点编号必须是 1-9 之间的数字。\n");

                        room_file = base_name(environment(me));
                        if (!room_file || room_file == "")
                                return notify_fail("这里无法记录为回城点。\n");

                        points = me->query("home_points");
                        if (!mapp(points))
                                points = ([]);

                        points[num] = room_file;
                        me->set("home_points", points);
                        write(sprintf("已将当前房间设为回城点 %d。\n", num));
                        return 1;
                }

                // home -<数字> : 删除回城点
                if (sscanf(arg, "-%d", num) == 1)
                {
                        if (num < 1 || num > 9)
                                return notify_fail("回城点编号必须是 1-9 之间的数字。\n");

                        points = me->query("home_points");
                        if (!mapp(points) || !points[num])
                                return notify_fail(sprintf("回城点 %d 不存在。\n", num));

                        map_delete(points, num);
                        me->set("home_points", points);
                        write(sprintf("已删除回城点 %d。\n", num));
                        return 1;
                }

                // home <数字> : 移动到回城点
                if (sscanf(arg, "%d", num) == 1)
                {
                        if (num < 1 || num > 9)
                                return notify_fail("回城点编号必须是 1-9 之间的数字。\n");

                        points = me->query("home_points");
                        if (!mapp(points) || !points[num])
                                return notify_fail(sprintf("回城点 %d 不存在。\n", num));

                        room_file = points[num];
                        if (file_size(room_file + ".c") <= 0)
                        {
                                map_delete(points, num);
                                me->set("home_points", points);
                                return notify_fail(sprintf("回城点 %d 的房间已不存在，已自动删除。\n", num));
                        }

                        message_vision("$N身影一晃，瞬间消失了。\n", me);
                        me->move(room_file);
                        message_vision("$N突然凭空出现。\n", me);
                        return 1;
                }

                return notify_fail("指令格式：home +<数字> | home <数字> | home -<数字> | home\n");
        }

        // home : 查看所有回城点
        points = me->query("home_points");
        if (!mapp(points) || !sizeof(points))
        {
                write("你目前没有设置任何回城点。\n");
                write("使用 home +<数字> 来记录当前位置（1-9）。\n");
                return 1;
        }

        write("当前回城点：\n");
        foreach (int n in sort_array(keys(points), 1))
        {
                room_file = points[n];
                if (file_size(room_file + ".c") <= 0)
                {
                        write(sprintf("  %d. (已不存在)\n", n));
                        continue;
                }
                object room = load_object(room_file);
                string name = room ? room->query("short") : 0;
                if (!name || name == "") name = room_file;
                else {
                    string area = get_area(room_file);
                    if (area != "") name = area + name;
                }
                write(sprintf("  %d. %-16s %s\n", n, name, room_file));
        }
        return 1;
}

int help(object me)
{
        write(@HELP
指令格式 :

  home             查看所有回城点
  home +<数字>     将当前位置设为回城点（1-9）
  home <数字>      瞬间移动到指定回城点
  home -<数字>     删除指定回城点

保存的回城点在玩家数据中，每次连线都有效。
巫师请使用 wiz/home 命令返回工作室。
HELP
        );
        return 1;
}

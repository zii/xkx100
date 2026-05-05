// home.c
// 玩家回城点命令
// home +<数字> 记录当前房间为回城点
// home <数字>  瞬间移动到回城点
// home         查看所有回城点
// home -<数字> 删除回城点

inherit F_CLEAN_UP;

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
        write("━━━━━━━━━━━━━━━━━━━━\n");
        foreach (int n in sort_array(keys(points), 1))
        {
                room_file = points[n];
                if (file_size(room_file + ".c") <= 0)
                {
                        write(sprintf("  %d. (已不存在)\n", n));
                        continue;
                }
                write(sprintf("  %d. %s\n", n, room_file));
        }
        write("━━━━━━━━━━━━━━━━━━━━\n");
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

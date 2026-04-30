// roomid.c - 按房间名查找房间档名（文件路径）
// 用法: roomid <关键词>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    string *dirs, *files, key;
    int i, j, found;
    object room;

    if (!arg || !(key = lower_case(trim(arg))))
        return notify_fail("指令格式: roomid <房间名关键词>\n");

    // 扫描 /d/ 下的所有房间文件（以及子目录）
    dirs = ({ "/d/" });
    found = 0;

    while (sizeof(dirs))
    {
        string dir = dirs[0];
        dirs = dirs[1..];
        files = get_dir(dir + "*.c", -1);

        for (i = 0; i < sizeof(files); i++)
        {
            // 载入房间对象（如已载入则直接拿到）
            room = find_object(dir + files[i][0]);
            if (!room) room = load_object(dir + files[i][0]);

            if (room && room->query("name"))
            {
                string name = lower_case(room->query("name"));
                if (strsrch(name, key) >= 0)
                {
                    write(sprintf("%-60s %s.c\n",
                        room->query("name"),
                        dir + files[i][0]));
                    found++;
                }
            }
            if (room && roomp(room))
                destruct(room); // 清理临时载入的对象
        }

        // 加入子目录
        files = get_dir(dir + "*", -1);
        for (i = 0; i < sizeof(files); i++)
            if (files[i][1] == -2) // 目录
                dirs += ({ dir + files[i][0] + "/" });
    }

    if (!found)
        write("没有找到房间名包含 \"" + arg + "\" 的房间。\n");
    else
        write("\n共找到 " + found + " 个房间。\n");

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式: roomid <房间名关键词>

查找房间名包含指定关键词的房间档名（文件路径）。房间名不区分大小写。

范例:
    roomid 扬州
    roomid 武庙

HELP
    );
    return 1;
}

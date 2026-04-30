// roomid.c - 按房间名查找房间档名（文件路径）
// 用法: roomid <关键词>
// 通过文本扫描快速查找，不加载房间对象

inherit F_CLEAN_UP;

// 从文件内容中提取 set("short", ...) 或 set("name", ...) 定义的房间名
// 处理格式:
//   set("short", "武庙");
//   set("short","北柳巷");
//   set("short", YEL"秦淮客寓"NOR);
//   set("short", HIG "五行洞" NOR);
string extract_room_name(string content)
{
    string *lines = explode(content, "\n");

    for (int i = 0; i < sizeof(lines); i++)
    {
        string line = lines[i];
        string after;
        int pos, comma, q1, q2;

        // 匹配 set("short",
        pos = strsrch(line, "set(\"short\"");
        if (pos >= 0)
            after = line[pos + 11..]; // skip 'set("short"'
        else
        {
            // 部分房间设置 name，作为后备
            pos = strsrch(line, "set(\"name\"");
            if (pos >= 0)
                after = line[pos + 10..]; // skip 'set("name"'
            else
                continue;
        }

        // 跳过逗号后的内容
        comma = strsrch(after, ",");
        if (comma < 0) continue;
        after = after[comma + 1..];

        // 找到第一个 " 和第二个 " 之间的文本即为房间名
        // 这天然跳过了 ANSI 宏前缀（如 YEL、HIG 等）
        q1 = strsrch(after, "\"");
        if (q1 < 0) continue;
        q2 = strsrch(after[q1 + 1..], "\"");
        if (q2 < 0) continue;

        return after[q1 + 1..q1 + q2];
    }

    return 0;
}

int main(object me, string arg)
{
    string *dirs, *files, key;
    int i, found;

    if (!arg || !(key = lower_case(trim(arg))))
        return notify_fail("指令格式: roomid <房间名关键词>\n");

    // 递归扫描 /d/ 下所有目录
    dirs = ({ "/d/" });
    found = 0;

    while (sizeof(dirs))
    {
        string dir = dirs[0];
        dirs = dirs[1..];
        files = get_dir(dir + "*.c", -1);

        for (i = 0; i < sizeof(files); i++)
        {
            string path = dir + files[i][0];
            string content, name;

            // 只读前 30 行 — short/name 定义基本都在文件开头
            content = read_file(path, 1, 30);
            if (!content) continue;

            name = extract_room_name(content);
            if (!name) continue;

            if (strsrch(lower_case(name), key) >= 0)
            {
                write(sprintf("%-30s  %s\n", name, path));
                found++;
            }
        }

        // 收集子目录
        files = get_dir(dir + "*", -1);
        for (i = 0; i < sizeof(files); i++)
            if (files[i][1] == -2)
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
通过扫描文件系统查找，不加载房间对象，速度快。

范例:
    roomid 扬州
    roomid 武庙
    roomid 少林

HELP
    );
    return 1;
}

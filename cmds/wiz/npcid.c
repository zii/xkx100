// npcid.c - 按名称模糊搜索 NPC
// 用法: npcid <关键词>
//
// 纯文本扫描，不加载任何对象。OS page cache 自动加速文件 I/O。
// 开发时修改房间/NPC 文件即时生效，不受编译错误影响。

inherit F_CLEAN_UP;

// ─── NPC 文件文本解析 ───

string extract_npc_name(string content)
{
    string *lines = explode(content, "\n");

    for (int i = 0; i < sizeof(lines); i++)
    {
        string line = lines[i];
        int pos, q2;

        pos = strsrch(line, "set_name(\"");
        if (pos < 0) continue;

        q2 = strsrch(line[pos + 10..], "\"");
        if (q2 < 1) continue;

        return line[pos + 10..pos + 10 + q2 - 1];
    }
    return 0;
}

string extract_npc_id(string content)
{
    string *lines = explode(content, "\n");

    for (int i = 0; i < sizeof(lines); i++)
    {
        string line = lines[i];
        int pos, q2;

        pos = strsrch(line, "({ \"");
        if (pos < 0) continue;

        q2 = strsrch(line[pos + 4..], "\"");
        if (q2 < 1) continue;

        return lower_case(line[pos + 4..pos + 4 + q2 - 1]);
    }
    return 0;
}

// ─── 房间 NPC 引用扫描 ───

string *extract_npc_refs(string content, string room_dir)
{
    string *result = ({});

    int p = strsrch(content, "\"objects\"");
    if (p < 0) return result;
    content = content[p..];

    p = strsrch(content, "([");
    if (p < 0) return result;

    int end = strsrch(content, "]);");
    if (end < 0) end = strsrch(content, "])");
    if (end < 0) return result;

    string block = content[p + 2..end - 1];
    string *lines = explode(block, "\n");

    for (int i = 0; i < sizeof(lines); i++)
    {
        string line = trim(lines[i]);
        if (line == "" || line[0] == '/' || line[0] == '#') continue;

        int col = strsrch(line, ":");
        if (col < 0) continue;

        string left = line[0..col - 1];
        int q1 = strsrch(left, "\"");
        if (q1 < 0) continue;

        int has_dir = (strsrch(left[0..q1 - 1], "__DIR__") >= 0);

        int q2 = strsrch(left[q1 + 1..], "\"");
        if (q2 < 0) continue;

        string path = left[q1 + 1..q1 + 1 + q2 - 1];
        if (has_dir || path[0] != '/')
            path = room_dir + path;

        if (strsrch(path, ".") < 0)
            path += ".c";

        result += ({ path });
    }
    return result;
}

string get_room_short(string path)
{
    string content = read_file(path, 1, 30);
    if (!content) return 0;

    int pos = strsrch(content, "set(\"short\"");
    if (pos < 0) return 0;

    string after = content[pos + 11..];
    int comma = strsrch(after, ",");
    if (comma < 0) return 0;
    after = after[comma + 1..];

    int q1 = strsrch(after, "\"");
    if (q1 < 0) return 0;
    int q2 = strsrch(after[q1 + 1..], "\"");
    if (q2 < 0) return 0;

    return after[q1 + 1..q1 + q2];
}

mapping build_room_refs()
{
    mapping refs = ([]);
    string *dirs = ({ "/d/" });

    while (sizeof(dirs))
    {
        string dir = dirs[0];
        dirs = dirs[1..];

        // 先收集子目录
        mixed *subs = get_dir(dir + "*", -1);
        for (int i = 0; i < sizeof(subs); i++)
            if (subs[i][1] == -2)
                dirs += ({ dir + subs[i][0] + "/" });

        // 处理当前目录
        mixed *files = get_dir(dir + "*.c", -1);
        if (!sizeof(files)) continue;

        for (int i = 0; i < sizeof(files); i++)
        {
            string path = dir + files[i][0];
            string content = read_file(path, 1, 150);
            if (!content) continue;
            if (strsrch(content, "\"objects\"") < 0) continue;

            string *npc_paths = extract_npc_refs(content, dir);
            if (!sizeof(npc_paths)) continue;

            for (int j = 0; j < sizeof(npc_paths); j++)
            {
                string npc = npc_paths[j];
                if (!refs[npc])
                    refs[npc] = ({ path });
                else if (member_array(path, refs[npc]) < 0)
                    refs[npc] += ({ path });
            }
        }
    }

    return refs;
}

// ─── 主入口 ───

int main(object me, string arg)
{
    if (!arg || arg == "")
        return notify_fail("指令格式: npcid <关键词>\n");

    string key = lower_case(trim(arg));

    // ====== PASS 1：已加载 NPC 索引 ======
    mapping loaded = ([]);
    object *all = objects();

    for (int i = 0; i < sizeof(all); i++)
    {
        object ob = all[i];
        if (!ob->is_character()) continue;
        if (userp(ob)) continue;

        string fn = file_name(ob);
        sscanf(fn, "%s#%*s", fn);

        if (!loaded[fn])
            loaded[fn] = ({ ob });
        else
            loaded[fn] += ({ ob });
    }

    // ====== PASS 2：NPC 目录收集 ======
    string npc_dirs = "";

    for (int i = 0; i < sizeof((mixed *)get_dir("/d/", -1)); i++)
    {
        mixed *info = get_dir("/d/", -1);
        if (info[i][1] != -2) continue;
        string d = "/d/" + info[i][0] + "/npc/";
        if (file_size(d) == -2)
            npc_dirs += d + ":";
    }

    for (int i = 0; i < sizeof((mixed *)get_dir("/kungfu/class/", -1)); i++)
    {
        mixed *info = get_dir("/kungfu/class/", -1);
        if (info[i][1] != -2) continue;
        string d = "/kungfu/class/" + info[i][0] + "/";
        if (file_size(d) == -2)
            npc_dirs += d + ":";
    }

    // ====== PASS 3：遍历 NPC 文件，匹配关键词 ======
    string *sub_dirs = explode(npc_dirs, ":");
    int count = 0;
    mapping room_refs = 0;

    for (int i = 0; i < sizeof(sub_dirs); i++)
    {
        if (sub_dirs[i] == "") continue;

        mixed *files = get_dir(sub_dirs[i] + "*.c", -1);
        if (!sizeof(files)) continue;

        for (int j = 0; j < sizeof(files); j++)
        {
            string file_path = sub_dirs[i] + files[j][0];
            string content = read_file(file_path, 1, 20);
            if (!content) continue;

            string name = extract_npc_name(content);
            if (!name) continue;

            string id = extract_npc_id(content);
            if (!id) id = "";

            int match = (strsrch(lower_case(name), key) >= 0);
            if (!match && id != "")
                match = (strsrch(id, key) >= 0);
            if (!match) continue;

            count++;

            // 优先用内存中克隆体的位置显示
            string location_file = 0;
            string location_name = "-";
            int has_clone = 0;

            string obj_key = file_path[0..sizeof(file_path) - 3];
            object *obs = loaded[obj_key];
            if (obs && sizeof(obs))
            {
                for (int k = 0; k < sizeof(obs); k++)
                {
                    if (!clonep(obs[k])) continue;
                    has_clone = 1;
                    object env = environment(obs[k]);
                    if (env)
                    {
                        location_file = file_name(env);
                        location_name = env->query("short");
                        break;
                    }
                }
            }

            // 内存中没有 → 从房间文本索引查
            if (!has_clone)
            {
                if (!room_refs)
                {
                    write("正在扫描房间 NPC 分布...\n");
                    room_refs = build_room_refs();
                }
                if (room_refs[file_path])
                {
                    string *rooms = room_refs[file_path];
                    location_file = rooms[0];
                    location_name = get_room_short(location_file);
                    if (!location_name) location_name = location_file;

                    // 有更多房间则额外显示
                    for (int k = 1; k < sizeof(rooms); k++)
                    {
                        string rn = get_room_short(rooms[k]);
                        write(sprintf("  ← %s  %s\n", rooms[k], rn ? "(" + rn + ")" : ""));
                    }
                }
            }

            write(sprintf("%-30s  %-10s %-10s  %s\n",
                file_path, name, location_name, location_file ? location_file : ""));
        }
    }

    if (!count)
        write("没有找到名字包含 \"" + arg + "\" 的 NPC。\n");
    else
        write("\n共找到 " + count + " 个 NPC。\n");

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式: npcid <关键词>

模糊搜索所有 NPC（中文名和英文 ID）。
纯文本扫描，不加载任何对象，修改 NPC/房间文件即时生效。

范例:
    npcid 流氓
    npcid 药
    npcid liumang

HELP
    );
    return 1;
}

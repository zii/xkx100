// npcid.c - 按名称模糊搜索 NPC（已加载/未加载均支持）
// 用法: npcid <关键词>
// 参考 roomid.c 的文件扫描方式实现
//
// 优化说明：一次 objects() 建立已加载 NPC 索引，避免对每个文件调 children()

inherit F_CLEAN_UP;

// 从 NPC 源文件中提取 set_name("中文名", ...) 的名称
string extract_npc_name(string content)
{
    string *lines = explode(content, "\n");
    int i;

    for (i = 0; i < sizeof(lines); i++)
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

// 从 NPC 源文件中提取英文 ID 列表中的第一个
string extract_npc_id(string content)
{
    string *lines = explode(content, "\n");
    int i;

    for (i = 0; i < sizeof(lines); i++)
    {
        string line = lines[i];
        int pos, q2;

        // 从 set_name 行的 ID 数组 ({ "id1", ... 中提取第一个
        pos = strsrch(line, "({ \"");
        if (pos < 0) continue;

        q2 = strsrch(line[pos + 4..], "\"");
        if (q2 < 1) continue;

        return lower_case(line[pos + 4..pos + 4 + q2 - 1]);
    }
    return 0;
}

int main(object me, string arg)
{
    string key;
    int i, j, count;

    if (!arg || (key = lower_case(trim(arg))) == "")
        return notify_fail("指令格式: npcid <关键词>\n");

    // ====== PASS 1：一次扫描所有已加载物件，建立 NPC 索引 ======
    // 索引键：去掉 #number 的文件路径（如 /d/city/npc/liumang）
    // 索引值：该文件的克隆实例数组（含 blueprint）
    mapping loaded = ([]);
    object *all = objects();

    for (i = 0; i < sizeof(all); i++)
    {
        object ob = all[i];
        string fn;

        if (!ob->is_character()) continue;
        if (userp(ob)) continue;

        fn = file_name(ob);
        // 去掉 #number 得到 blueprint 路径
        sscanf(fn, "%s#%*s", fn);

        if (!loaded[fn])
            loaded[fn] = ({ ob });
        else
            loaded[fn] += ({ ob });
    }

    // ====== PASS 2：收集 NPC 目录 ======
    string npc_dirs = "", *files;

    // 扫描 /d/*/npc/ 目录
    files = get_dir("/d/", -1);
    for (i = 0; i < sizeof(files); i++)
    {
        if (files[i][1] != -2) continue;
        string d = "/d/" + files[i][0] + "/npc/";
        if (file_size(d) == -2)
            npc_dirs += d + ":";
    }

    // 扫描 /kungfu/class/*/ 目录
    files = get_dir("/kungfu/class/", -1);
    for (i = 0; i < sizeof(files); i++)
    {
        if (files[i][1] != -2) continue;
        string d = "/kungfu/class/" + files[i][0] + "/";
        if (file_size(d) == -2)
            npc_dirs += d + ":";
    }

    // ====== PASS 3：遍历 NPC 文件，匹配关键词 ======
    string *sub_dirs = explode(npc_dirs, ":");
    count = 0;

    for (i = 0; i < sizeof(sub_dirs); i++)
    {
        if (sub_dirs[i] == "") continue;

        files = get_dir(sub_dirs[i] + "*.c", -1);
        if (!sizeof(files)) continue;

        for (j = 0; j < sizeof(files); j++)
        {
            string file_path, content, name, id, lookup;
            object *obs;
            int k, match;

            file_path = sub_dirs[i] + files[j][0];

            // 一次读文件，提取名称
            content = read_file(file_path, 1, 20);
            if (!content) continue;

            name = extract_npc_name(content);
            if (!name) continue; // 不是 NPC 文件

            id = extract_npc_id(content);
            if (!id) id = "";

            // 关键词模糊匹配（先过滤，避免不必要的 I/O）
            match = (strsrch(lower_case(name), key) >= 0);
            if (!match && id != "")
                match = (strsrch(id, key) >= 0);
            if (!match) continue;

            count++;

            // 从索引查已加载的实例
            // file_path 带 .c，索引键不带，截掉
            lookup = file_path[0..sizeof(file_path) - 4];
            obs = loaded[lookup];

            if (obs && sizeof(obs) > 0)
            {
                // 有已加载的实例：有 clone 则显示 clone，否则显示 blueprint
                object *show = ({});
                object *bp = ({});
                for (k = 0; k < sizeof(obs); k++)
                {
                    if (clonep(obs[k]))
                        show += ({ obs[k] });
                    else
                        bp += ({ obs[k] });
                }
                if (!sizeof(show))
                    show = bp;

                for (k = 0; k < sizeof(show); k++)
                {
                    object env = environment(show[k]);

                    write(sprintf("%-45s  %-20s  %-16s  %s\n",
                        file_name(show[k]),
                        (string)show[k]->query("name") + "(" + (string)show[k]->query("id") + ")",
                        env ? (string)env->query("short") : "无房间",
                        env ? file_name(env) : ""
                    ));
                }
            }
            else
            {
                // 未加载
                write(sprintf("%-45s  %-20s  %s\n",
                    file_path,
                    name + "(" + id + ")",
                    "未加载"
                ));
            }
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

模糊搜索所有 NPC（中文名和英文 ID），无论是否已加载。
已加载的 NPC 会显示当前所在房间，未加载的显示"未加载"。

扫描目录:
    /d/*/npc/   - 各区域 NPC
    /kungfu/class/*/ - 各门派师父

范例:
    npcid 流氓
    npcid 药
    npcid liumang

HELP
    );
    return 1;
}

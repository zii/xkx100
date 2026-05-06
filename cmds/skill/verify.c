// verify.c — 查询武功详细信息和分类列表
inherit F_CLEAN_UP;
#include <ansi.h>

mapping valid_type = ([
    "array":"阵法","axe":"斧法","blade":"刀法","claw":"爪法","club":"棍法",
    "cuff":"拳法","dagger":"短兵","dodge":"轻功","finger":"指法","force":"内功",
    "hammer":"锤法","hand":"手法","hook":"钩法","leg":"腿法","magic":"法术",
    "parry":"招架","spear":"枪法","staff":"杖法","stick":"棒法","strike":"掌法",
    "sword":"剑法","throwing":"暗器","unarmed":"拳脚","whip":"鞭法",
]);

string *special_cats = ({ "force","dodge","knowledge","all" });

// ─── 分项评估函数 ───

// 战斗技能评分（有 query_action 的进攻技能）
int combat_rating(object sk) {
    int r = 0;
    if (function_exists("hit_ob", sk)) r += 100;
    string pd = sk->perform_action_file("");
    if (pd) { mixed *f = get_dir(pd);
        if (sizeof(f)) for (int i = 0; i < sizeof(f); i++)
            if (strsrch(f[i], ".c") >= 0) r += 30; }
    return r;
}

// 招架评分（基于 action["parry"]）
int parry_rating(object sk) {
    object tp = this_player();
    if (!tp) return 0;
    mapping a = sk->query_action(tp, 0);
    if (mapp(a) && intp(a["parry"])) {
        int r = -a["parry"] * 10;
        if (function_exists("be_hit_ob", sk)) r += 100;
        return r;
    }
    return 0;
}

// 轻功评分：power_point³ × (100+action["dodge"])/100
int dodge_rating(object sk) {
    object tp = this_player();
    float pp = tp ? sk->power_point(tp) : 1.0;
    int r = (int)(pp * pp * pp * 100);
    // action dodge 修正（反映攻击时的闪避效果）
    if (tp) {
        mapping a = sk->query_action(tp, 0);
        if (mapp(a) && intp(a["dodge"]))
            r = r * (100 + a["dodge"]) / 100;
    }
    return r;
}

// 内功评分（基于特效数量）
int force_rating(object sk) {
    int r = 0;
    // 有 action force 的内功（太玄功等全能型）也算进来
    object _tp = this_player();
    if (_tp) {
        mapping _a = sk->query_action(_tp, 0);
        if (mapp(_a) && intp(_a["force"])) r += _a["force"];
    }
    if (function_exists("hit_ob", sk)) r += 60;
    if (function_exists("be_hit_ob", sk)) r += 40;
    string *std = ({ "recover","regenerate","revive","transfer","bipin" });
    string ed = sk->exert_function_file("");
    if (ed) { mixed *f = get_dir(ed);
        if (sizeof(f)) for (int i = 0; i < sizeof(f); i++) {
            string fn = f[i];
            if (strsrch(fn, ".c") < 0) continue;
            if (member_array(fn[0..<3], std) >= 0) continue;
            r += 20;
        }
    }
    return r;
}

// 按分类路由到对应的评估函数
int skill_rating(object sk, string cat) {
    if (cat == "force") return force_rating(sk);
    if (cat == "dodge") return dodge_rating(sk);
    if (cat == "parry") return parry_rating(sk);
    return combat_rating(sk);
}

int is_category(string arg) {
    return member_array(arg, special_cats) >= 0 || !undefinedp(valid_type[arg]);
}

int match_category(object sk, string cat) {
    string mt = sk->martialtype(), t = sk->type();
    if (cat == "all") return 1;
    if (cat == "force" && mt == "force") return 1;
    if (cat == "dodge" && mt == "dodge") return 1;
    if (cat == "knowledge" && t == "knowledge") return 1;
    return !undefinedp(valid_type[cat]) && sk->valid_enable(cat);
}

mapping school_names = ([
    "baituo":"白驼山","dali":"大理段家","emei":"峨嵋派","gaibang":"丐帮",
    "gumu":"古墓派","heimuya":"黑木崖","hengshan":"恒山派","henshan":"衡山派",
    "honghua":"红花会","huashan":"华山派","kunlun":"昆仑派","lingjiu":"灵鹫宫",
    "lingxiao":"凌霄城","mingjiao":"明教","murong":"姑苏慕容","nanshaolin":"南少林",
    "qingcheng":"青城派","quanzhen":"全真派","shaolin":"少林派","shenlong":"神龙教",
    "songshan":"嵩山派","taishan":"泰山派","taohua":"桃花岛","tiezhang":"铁掌帮",
    "wudang":"武当派","wudujiao":"五毒教","xiakedao":"侠客岛","xiaoyao":"逍遥派",
    "xingxiu":"星宿派","xueshan":"雪山派","yunlong":"天地会",
]);

mapping build_school_map(string *skills) {
    mapping r = ([]);
    mixed *dirs = get_dir("/kungfu/class/", -1);
    for (int d = 0; d < sizeof(dirs); d++) {
        string s = dirs[d][0];
        if (dirs[d][1] != -2 || !school_names[s]) continue;
        mixed *fs = get_dir("/kungfu/class/" + s + "/", -1);
        if (!sizeof(fs)) continue;
        for (int f = 0; f < sizeof(fs); f++) {
            string fn = fs[f][0];
            if (strsrch(fn, ".c") < 0) continue;
            string c = read_file("/kungfu/class/" + s + "/" + fn, 1, 50);
            if (!c) continue;
            for (int k = 0; k < sizeof(skills); k++)
                if (strsrch(c, "\"" + skills[k] + "\"") >= 0) r[skills[k]] = s;
        }
    }
    return r;
}

// 评分缓存，避免排序时反复调用 skill_rating（含 get_dir）
mapping _rating_cache;
int sort_skill(string a, string b) {
    int ra = _rating_cache[a], rb = _rating_cache[b];
    if (ra != rb) return ra > rb ? -1 : 1;
    return a > b ? 1 : a < b ? -1 : 0;
}

string pv(string s, int w) { while (strwidth(s) < w) s += " "; return s; }

// ─── 模式1: 无参数 ───
int show_categories(object me) {
    string *cats = sort_array(keys(valid_type), 1);
    string msg = "\n" HIC "┌──────────────────────────────┐\n" NOR;
    msg += HIC "│" NOR + HIW "              武功分类查询              " NOR HIC "│\n" NOR;
    msg += HIC "├──────────────────────────────┤\n" NOR;
    msg += HIC "│" NOR "  特殊分类:" HIC "                          │\n" NOR;
    msg += HIC "│" NOR "  " HIY "force" NOR "(内功)  " HIG "dodge" NOR "(轻功)  " HIM "knowledge" NOR "(知识)  " HIW "all" NOR "(全部)" HIC "  │\n" NOR;
    msg += HIC "│" NOR "                                          " HIC "│\n" NOR;
    msg += HIC "│" NOR "  兵器/拳脚分类:" HIC "                     │\n" NOR;
    for (int i = 0; i < sizeof(cats); i++) {
        msg += HIC "│" NOR "  " HIY + cats[i] + NOR + "(" + valid_type[cats[i]] + ")";
        msg += pv("", 60 - strwidth("  " + cats[i] + "(" + valid_type[cats[i]] + ")")) + HIC "│\n" NOR;
    }
    msg += HIC "└──────────────────────────────┘\n" NOR;
    write(msg + "用法: " HIY "verify <分类>" NOR " 或 " HIY "verify <武功英文ID>" NOR "\n");
    return 1;
}

// ─── 模式2: 按分类列出 ───
int list_skills(object me, string cat) {
    mixed *files = get_dir("/kungfu/skill/", -1);
    string *ids = ({});
    for (int i = 0; i < sizeof(files); i++) {
        string f = files[i][0];
        if (strsrch(f, ".c") < 0 || strsrch(f, ".u") >= 0) continue;
        ids += ({ f[0..<3] });
    }
    mapping data = ([]);
    int total = 0;
    for (int i = 0; i < sizeof(ids); i++) {
        object sk = load_object(SKILL_D(ids[i]));
        if (!sk || !match_category(sk, cat)) continue;
        total++;
        data[ids[i]] = ([ "rating": skill_rating(sk, cat), "name": to_chinese(ids[i]) ]);
    }
    string *sorted = keys(data);
    _rating_cache = ([]);
    for (int i = 0; i < sizeof(sorted); i++) _rating_cache[sorted[i]] = data[sorted[i]]["rating"];
    sorted = sort_array(sorted, (: sort_skill :));
    _rating_cache = ([]);
    mapping skill_school = build_school_map(sorted);
    string cat_cn = cat == "all" ? "全部" : cat == "knowledge" ? "知识" : cat == "force" ? "内功" : cat == "dodge" ? "轻功" : valid_type[cat];

    string msg = "\n" HIC "┌──────────────────────────────┐\n" NOR;
    msg += HIC "│" NOR " " HIW + cat_cn + "(" + cat + ")  共 " + total + " 项";
    msg += pv("", 58 - strwidth(cat_cn + "(" + cat + ")  共 " + total + " 项")) + HIC " │\n" NOR;
    msg += HIC "├──────────────────────────────┤\n" NOR;

    for (int i = 0; i < sizeof(sorted); i++) {
        string id = sorted[i];
        int ra = data[id]["rating"];
        string nm = data[id]["name"];
        string rc = ra >= 500 ? HIR : ra >= 350 ? HIM : ra >= 200 ? HIY : ra >= 80 ? HIG : "";
        string school = skill_school[id] ? school_names[skill_school[id]] : "-";

        // 纯文字列（不含 ANSI）用于 padding
        string row = pv(sprintf("%d", ra), 3) + "  ";
        row += pv(nm, 18) + "  ";
        row += pv(id, 18) + "  ";
        row += pv(school, 12);
        row += pv("", 58 - strwidth(row));

        // 评分上色（文字宽度不变）
        msg += HIC "│" NOR " " + rc + row[0..2] + NOR + row[3..] + HIC " │\n" NOR;
    }
    msg += HIC "└──────────────────────────────┘\n" NOR;
    me->start_more(msg);
    return 1;
}

// ─── 模式3: 具体武功详情 ───
int show_detail(object me, string skill) {
    object sk = load_object(SKILL_D(skill));
    string t = sk ? sk->type() : 0;
    if (!t) return notify_fail("没有「" + skill + "」这项武功。\n");
    string name = to_chinese(skill), mt = sk->martialtype();
    int lb = sk->learn_bonus(), pb = sk->practice_bonus(), sc = sk->success(), ra = skill_rating(sk, "");
    string tn = t == "knowledge" ? "知识" : mt == "force" ? "内功" : mt == "dodge" ? "轻功" : "武技";
    string rs = sprintf("%d", ra);
    string rc = ra >= 500 ? HIR : ra >= 350 ? HIM : ra >= 200 ? HIY : ra >= 80 ? HIG : "";
    string msg = "\n" HIC "┌──────────────────────────────┐\n" NOR;
    msg += HIC "│" NOR "  " HIW + name + NOR + "(" + skill + ")  " + (t == "knowledge" ? HIM : mt == "force" ? HIY : mt == "dodge" ? HIG : HIC) + tn + NOR;
    msg += pv("", 60 - strwidth("  " + name + "(" + skill + ")  " + tn)) + HIC "│\n" NOR;
    msg += HIC "├──────────────────────────────┤\n" NOR;
    msg += HIC "│" NOR + "  学习难度: " + lb + "    练习难度: " + pb + "    成功率: " + sc;
    msg += pv("", 48 - strwidth("  学习难度: " + lb + "    练习难度: " + pb + "    成功率: " + sc));
    msg += "  评分: " + rc + rs + NOR + HIC "  │\n" NOR;
    msg += HIC "├──────────────────────────────┤\n" NOR;
    string *uk = keys(valid_type);
    string *ea = ({});
    for (int i = 0; i < sizeof(uk); i++)
        if (sk->valid_enable(uk[i])) ea += ({ valid_type[uk[i]] + "(" + uk[i] + ")" });
    if (sizeof(ea) > 0) {
        string en = "  可启用为：";
        msg += HIC "│" NOR "  可启用为：";
        for (int i = 0; i < sizeof(ea); i++) {
            if (i > 0) { msg += "、"; en += "、"; }
            msg += HIY + ea[i] + NOR; en += ea[i];
        }
        msg += pv("", 60 - strwidth(en)) + HIC "│\n" NOR;
    }
    string exerts = sk->exert_function_file("");
    if (exerts) {
        string *ex_files = get_dir(exerts);
        if (sizeof(ex_files)) {
            ex_files = filter_array(ex_files, (: strsrch($1, ".c") >= 0 :));
            string en = "  内功特效(exert)：";
            msg += HIC "│" NOR "  内功特效(exert)：";
            for (int i = 0; i < sizeof(ex_files); i++) {
                string f = replace_string(ex_files[i], ".c", "");
                msg += HIY + " " + f + NOR; en += " " + f;
            }
            msg += pv("", 60 - strwidth(en)) + HIC "│\n" NOR;
        }
    }
    exerts = sk->perform_action_file("");
    if (exerts) {
        string *ex_files = get_dir(exerts);
        if (sizeof(ex_files)) {
            ex_files = filter_array(ex_files, (: strsrch($1, ".c") >= 0 :));
            string en = "  外功招式(perform)：";
            msg += HIC "│" NOR "  外功招式(perform)：";
            for (int i = 0; i < sizeof(ex_files); i++) {
                string f = replace_string(ex_files[i], ".c", "");
                msg += HIG + " " + f + NOR; en += " " + f;
            }
            msg += pv("", 60 - strwidth(en)) + HIC "│\n" NOR;
        }
    }
    msg += HIC "└──────────────────────────────┘\n" NOR;
    write(msg);
    if (!sk->help(me)) write("（该武功没有详细说明）\n");
    return 1;
}

// ─── 模式4: verify mine — 个人武功快捷命令 ───

// 纯构建别名（无输出），供 show_mine 和自动登录使用
int build_skill_aliases(object me) {
    mapping skill_map = me->query_skill_map();
    if (!mapp(skill_map) || !sizeof(skill_map)) return 0;

    // 清理旧别名
    for (int i = 1; i <= 99; i++) {
        me->set_skill_alias("e" + i, 0);
        me->set_skill_alias("p" + i, 0);
    }

    int e_cnt = 0, p_cnt = 0;
    string *excludes = ({ "array", "magic", "parry" });
    mapping shown = ([]);

    // ── 内功 exert ──
    string fs = me->query_skill_mapped("force");
    if (fs) {
        string dir = SKILL_D(fs)->exert_function_file("");
        if (dir) {
            mixed *files = get_dir(dir);
            if (sizeof(files)) {
                for (int i = 0; i < sizeof(files); i++) {
                    string fn = files[i];
                    if (strsrch(fn, ".c") < 0) continue;
                    string f = replace_string(fn, ".c", "");
                    e_cnt++;
                    me->set_skill_alias("e" + e_cnt, "yun " + f);
                }
            }
        }
    }

    // ── 外功 perform ──
    string *uks = keys(skill_map);
    for (int i = 0; i < sizeof(uks); i++) {
        string usage = uks[i];
        if (member_array(usage, excludes) >= 0 || undefinedp(valid_type[usage])) continue;
        string sid = skill_map[usage];
        if (!sid || shown[sid]) continue;
        shown[sid] = 1;

        string dir = SKILL_D(sid)->perform_action_file("");
        if (!dir) continue;
        mixed *files = get_dir(dir);
        if (!sizeof(files)) continue;

        for (int j = 0; j < sizeof(files); j++) {
            string fn = files[j];
            if (strsrch(fn, ".c") < 0) continue;
            string f = replace_string(fn, ".c", "");
            p_cnt++;
            me->set_skill_alias("p" + p_cnt, "perform " + usage + "." + f);
        }
    }

    return (e_cnt + p_cnt) > 0 ? 1 : 0;
}

int show_mine(object me) {
    build_skill_aliases(me);

    mapping skill_map = me->query_skill_map();
    string msg = "\n";
    int e_cnt = 0, p_cnt = 0;
    string *excludes = ({ "array", "magic", "parry" });
    mapping shown = ([]);

    // ── 内功 exert ──
    string fs = me->query_skill_mapped("force");
    if (fs) {
        msg += HIW "内功部分" NOR "\n";
        msg += "    " + to_chinese(fs) + "\n";
        string dir = SKILL_D(fs)->exert_function_file("");
        if (dir) {
            mixed *files = get_dir(dir);
            if (sizeof(files)) {
                for (int i = 0; i < sizeof(files); i++) {
                    string fn = files[i];
                    if (strsrch(fn, ".c") < 0) continue;
                    string f = replace_string(fn, ".c", "");
                    e_cnt++;
                    msg += sprintf("        " HIC "exert %-16s" NOR " => " HIY "e%d" NOR "\n", f, e_cnt);
                }
            }
        }
        msg += "\n";
    }

    // ── 外功 perform ──
    if (!mapp(skill_map)) {
        msg += "你还没有启用任何武功。\n";
        write(msg);
        return 1;
    }
    string *uks = keys(skill_map);
    int has_perform = 0;
    for (int i = 0; i < sizeof(uks); i++) {
        string usage = uks[i];
        if (member_array(usage, excludes) >= 0 || undefinedp(valid_type[usage])) continue;
        string sid = skill_map[usage];
        if (!sid || shown[sid]) continue;
        shown[sid] = 1;

        string dir = SKILL_D(sid)->perform_action_file("");
        if (!dir) continue;
        mixed *files = get_dir(dir);
        if (!sizeof(files)) continue;

        if (!has_perform) { msg += HIW "外功部分" NOR "\n"; has_perform = 1; }
        msg += "    " + to_chinese(sid) + "  " + HIY + valid_type[usage] + NOR "\n";

        for (int j = 0; j < sizeof(files); j++) {
            string fn = files[j];
            if (strsrch(fn, ".c") < 0) continue;
            string f = replace_string(fn, ".c", "");
            p_cnt++;
            msg += sprintf("        " HIC "perform %s" NOR " => " HIY "p%d" NOR "\n", usage + "." + f, p_cnt);
        }
    }

    if (e_cnt == 0 && !has_perform)
        return notify_fail("你的武功没有任何特殊功能。\n");

    write(msg);
    return 1;
}

int main(object me, string arg) {
    if (!arg || arg == "") return show_categories(me);
    arg = lower_case(trim(arg));
    if (arg == "mine") return show_mine(me);
    if (is_category(arg)) return list_skills(me, arg);
    return show_detail(me, arg);
}

int help(object me) {
    write(@HELP
指令格式: verify [<分类>|<武功英文ID>]
       verify mine

不带参数列出所有可查询的分类。
按分类查询 (如 force/dodge/sword/all) 列出该分类下所有武功及评分。
verify mine 显示你当前已 enable 武功的 exert/perform 快捷命令。
评分构成：action force (70%) + hit_ob (15%) + 招式特效数量 (15%)
范例: verify force / verify mine / verify bibo-shengong
HELP);
    return 1;
}

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

int skill_rating(object sk) {
    int r = (30 - sk->learn_bonus()) + (30 - sk->practice_bonus());
    string mt = sk->martialtype();
    if (mt == "force") {
        string ed = sk->exert_function_file("");
        if (ed) { mixed *f = get_dir(ed);
            if (sizeof(f)) for (int i = 0; i < sizeof(f); i++)
                if (strsrch(f[i], ".c") >= 0) r += 2; }
    } else {
        string pd = sk->perform_action_file("");
        if (pd) { mixed *f = get_dir(pd);
            if (sizeof(f)) for (int i = 0; i < sizeof(f); i++)
                if (strsrch(f[i], ".c") >= 0) r += 3; }
    }
    float pp = sk->power_point();
    if (pp > 1.0) r += (int)((pp - 1.0) * 50);
    return r;
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

int sort_skill(string a, string b) {
    object sa = load_object(SKILL_D(a)), sb = load_object(SKILL_D(b));
    int ra = sa ? skill_rating(sa) : 0, rb = sb ? skill_rating(sb) : 0;
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
        data[ids[i]] = ([ "rating": skill_rating(sk), "name": to_chinese(ids[i]) ]);
    }
    string *sorted = keys(data);
    sorted = sort_array(sorted, (: sort_skill :));
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
        string rc = ra >= 80 ? HIR : ra >= 55 ? HIM : ra >= 30 ? HIY : ra >= 15 ? HIG : "";
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
    int lb = sk->learn_bonus(), pb = sk->practice_bonus(), sc = sk->success(), ra = skill_rating(sk);
    string tn = t == "knowledge" ? "知识" : mt == "force" ? "内功" : mt == "dodge" ? "轻功" : "武技";
    string rs = sprintf("%d", ra);
    string rc = ra >= 80 ? HIR : ra >= 55 ? HIM : ra >= 30 ? HIY : ra >= 15 ? HIG : "";
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

int main(object me, string arg) {
    if (!arg || arg == "") return show_categories(me);
    arg = lower_case(trim(arg));
    if (is_category(arg)) return list_skills(me, arg);
    return show_detail(me, arg);
}

int help(object me) {
    write(@HELP
指令格式: verify [<分类>|<武功英文ID>]
不带参数列出所有可查询的分类。
按分类查询 (如 force/dodge/sword/all) 列出该分类下所有武功及评分。
综合评分：基础难度 + 招式数量 + 威力系数。数值越高实战越强。
范例: verify force / verify dodge / verify sword / verify bibo-shengong
HELP);
    return 1;
}

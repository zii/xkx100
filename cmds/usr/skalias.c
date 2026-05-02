// skalias.c — 查看 skill_aliases（verify mine 设定的快捷命令）
inherit F_CLEAN_UP;

int main(object me, string arg)
{
	mapping sa = me->query_skill_aliases();
	if( !mapp(sa) || !sizeof(sa) ) {
		write("目前没有 skill_alias，请先用 verify mine 生成。\n");
		return 1;
	}
	write("当前 skill_aliases：\n");
	string *vrbs = keys(sa);
	for(int i=0; i<sizeof(vrbs); i++)
		printf("%-15s = %s\n", vrbs[i], sa[vrbs[i]]);
	return 1;
}

int help(object me)
{
	write(@HELP
指令格式: skalias

查看 verify mine 所设定的 e1/e2/p1/p2 等快捷别名。
HELP);
	return 1;
}

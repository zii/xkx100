// hpmin.c - 精简状态信息
inherit F_CLEAN_UP;

int main(object me, string arg)
{
	object ob;
	mapping my;

	seteuid(getuid(me));

	if (!arg || arg == me->query("id"))
		ob = me;
	else
	{
		ob = present(arg, environment(me));
		if (!ob) ob = find_player(arg);
		if (!ob) ob = find_living(arg);
		if (wizardp(me))
		{
			if (!ob)
			{
				ob = new(USER_OB);
				seteuid(arg);
				export_uid(ob);
				if (!ob->restore())
				{
					destruct(ob);
					return notify_fail("没有这个玩家。\n");
				}
			}
		}
		else
		{
			if (!ob || !me->visible(ob))
				return notify_fail("只能察看自己的状态。\n");
		}
	}

	if (!ob->is_character())
		return notify_fail("那不是活物。\n");

	my = ob->query_entire_dbase();
	printf("{jing:%d,max_jing:%d,jingli:%d,max_jingli:%d,"
		"qi:%d,max_qi:%d,neili:%d,max_neili:%d}\n",
		my["jing"], my["max_jing"],
		my["jingli"], my["max_jingli"],
		my["qi"], my["max_qi"],
		my["neili"], my["max_neili"]);

	return 1;
}

int help(object me)
{
	write(@HELP
指令格式 : hpmin
           hpmin <对象名称>                   (巫师专用)

	输出最精简的状态信息，格式为一行 JSON 风格文本。

see also : hp
HELP
	);
	return 1;
}

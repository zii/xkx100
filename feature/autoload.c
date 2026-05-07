// autoload.c
// From ES2
// Modified by Xiang for XKX (12/15/95)

string *autoload;

void clean_up_autoload() { autoload = 0; }

void save_autoload()
{
	object *inv;
	int i, j;
	mixed param;
	string *wielded = ({});

	inv = all_inventory();
	autoload = allocate(sizeof(inv));
	for(i=0, j=0; i<sizeof(inv); i++) {
		param = inv[i]->query_autoload();
		autoload[j] = base_name(inv[i]);
		if( stringp(param) ) autoload[j] += ":" + param;
		if( inv[i]->query("equipped") == "wielded" )
			wielded += ({ base_name(inv[i]) });
		j++;
	}
	autoload = autoload[0..j-1];
	this_object()->set("autoload_wielded", wielded);
}

void restore_autoload()
{
	int i;
	object ob;
	string file, param, err;
	string *wielded = this_object()->query("autoload_wielded");

	if( !pointerp(autoload) ) return;

	for(i=0; i<sizeof(autoload); i++) {
	if (autoload[i] && autoload[i] != "") {
		if( sscanf(autoload[i], "%s:%s", file, param)!=2 ) {
			file = autoload[i];
			param = 0;
		}
		err = catch(ob = new(file));
		if( err || !ob ) {
			write("你觉得似乎失落了什么重要的东西，最好通知一下巫师。\n");
			log_file("AUTOLOAD", sprintf("Fail to autoload %s of %s, error %s\n",
				file, this_object()->query("name"), err));
			continue;
		}
		export_uid(ob);
		ob->move(this_object());
		err = catch(ob->autoload(param));
		if( err )
			log_file("AUTOLOAD", sprintf("Fail to call autoload on %s of %s, error %s\n",
				file, this_object()->query("name"), err));
		// 自动穿上护甲类物品
		catch(ob->wear());
		// 仅当退出时拿着此武器才自动 wield
		if( pointerp(wielded) && member_array(file, wielded) != -1 )
			ob->wield();
	}
	else {
		write("你觉得似乎失落了什么重要的东西，最好通知一下巫师。\n");
		log_file("AUTOLOAD", sprintf("Fail to autoload %s of %s, error %s\n",
			autoload[i], this_object()->query("name"), err));
	}
	}
	clean_up_autoload();		// To save memory.
	this_object()->delete("autoload_wielded");
}

// aliasd.c

mapping global_alias = ([
	"l":		"look",
	"看":		"look",
	"n":		"go north",
	"e":		"go east",
	"w":		"go west",
	"s":		"go south",
	"nu":		"go northup",
	"eu":		"go eastup",
	"wu":		"go westup",
	"su":		"go southup",
	"nd":		"go northdown",
	"ed":		"go eastdown",
	"wd":		"go westdown",
	"sd":		"go southdown",
	"ne":		"go northeast",
	"se":		"go southeast",
	"nw":		"go northwest",
	"sw":		"go southwest",
	"u":		"go up",
	"d":		"go down",
	"i":		"inventory",
	"tt":		"team talk",
	"lm":		"map",
	"sk":		"skills",
	"map":		"map all",
	"sc": 		"score",
//	"bai":		"apprentice",
/////add some/////////
//	"dating":	"check",
//	"dazuo":	"exercise",
//	"du":		"study",
//	"fangqi":	"abandon",
//	"jiali":	"enforce",
//	"kaichu":	"expell",
//	"lian":		"practice",
//	"lian":	"practice",
//	"yanjiu":	"research",
//	"cha":	"skills",
//	"bei":	"prepare",
//	"jifa":		"enable",
//	"yun":	"exert",
//	"jiao":	"teach",
//	"xue":		"learn",
//	"quanjia":	"persuade",
//	"shou":		"recruit",
//	"tuna": "respirate",
//	"yong": "perform",
//	"he": "drink",
//	"chi": "eat",
//"quit": "exit",
//////////////////
/*
//	"beifen":	"backup",
//	"biecheng":	"nick",
//	"bieming":	"alias",
	"chuan":	"wear",
	"cun":		"save",
	"diu":		"drop",
//	"dou":		"fight",
//	"eryu":		"whisper",
//	"fang":		"put",
	"fangxia":	"unwield",
//	"fei":		"fly",
	"gaosu":	"tell",
	"gei":		"give",
//	"gen":		"follow",
	"guan":		"close",
//	"guancha":	"observe",
	"huida":	"reply",
//	"jia":		"marry",
//	"jianju": 	"accuse",
//	"jieshou":	"accept",
	"kai":		"open",
//	"kan":		"look",
//	"mai":		"buy",
	"miaoshu":	"describe",
	"tuna":		"meditate",
//	"na":		"get",
//	"qiuhun":	"propose",
	"sha":		"kill",
//	"shui":		"sleep",
	"shuo":		"say",
	"tou":		"steal",
	"touxian":	"title",
	"touxiang":	"surrender",
	"tuo":		"remove",
//	"wen":		"ask",
//	"yaoqing":	"invite",
	"zhao":		"find",
	"zhuangbei":	"wield",
//	"zisha":	"suicide"
*/
]);

string process_global_alias(string arg)
{
	string *word;

	if (arg[0]=='\'') return "say " + arg[1..strwidth(arg)-1];

	word = explode(arg, " ");
	if( sizeof(word) && !undefinedp(global_alias[word[0]]) ) {
		word[0] = global_alias[word[0]];
		return implode(word, " ");
	}

	// skill_aliases（由 verify mine 设定）
	if( this_player() ) {
		mapping sa = this_player()->query_skill_aliases();
		if( mapp(sa) ) {
			if( !undefinedp(sa[arg]) )
				return sa[arg];
			if( sizeof(word) > 1 && !undefinedp(sa[word[0]]) ) {
					string _r = sa[word[0]] + " " + implode(word[1..], " ");
					return _r;
			}
		}
	}

	return arg;
}

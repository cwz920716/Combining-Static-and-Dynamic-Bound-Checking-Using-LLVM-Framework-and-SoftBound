#ifndef _CLONE_HELPER_H
#define _CLONE_HELPER_H

static const char* noopFuncs[] = {
	"_assert",
	"allocate",
	"deallocate",
	"newarray",
	"printdag",
	"getchr",
	"gettok",
	"fprint",
	"stringf",
	"check",
	"print",
	"append",
	"length",
	"ltov",
	"init",
	"checklab",
	"enumdcl",
	"main_init",
	"vfprint",
	"string",
	"stringn",
	"stringd",
	"relocate",
	"printtree",
	"printed",
	"hasproto",
	"outtype",
	"printdecl",
	"printproto",
	"typestring",
	"error",
	"warning",
	"malloc",
	"domalloc",
	"newstring",
	"fillbuf",
	"flushout",
};



static bool lookupName(const char* table[], const char* str)
{
	for (unsigned i = 0; table[i] != nullptr; ++i)
	{
		if (strcmp(table[i], str) == 0)
			return true;
	}
	return false;
}



#endif

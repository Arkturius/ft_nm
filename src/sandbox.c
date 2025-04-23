#include <stdint.h>
#include <stdio.h>
#define _CELF_LOGGING 
#define _CELF_IMPLEMENTATION
#include <celf.h>

int main(UNUSED int argc, char **argv)
{
	ELF_open(argv[0]);

	LOG("size = %u", ELF_SIZE);

	ELF_64_CALL(getSectionHeaders)();
	
	ELF64_Shdr	*symtab = ELF_64_CALL(getSectionByName)(".symtab");
	uint32_t	link = READ_FIELD(symtab->sh_link);

	ELF64_Shdr	*sym_names = ELF_64_CALL(getSectionByIndex)(link);
	char		*names = (char *) ELF_64_CALL(getSectionContent)(sym_names);

	ELF64_Sym	*symbols = ELF_64_CALL(getSymbols)();
	uint32_t	scount = ELF_64_CALL(getSymbolCount)();

	for (uint32_t i = 0; i < scount; ++i)
	{
		uint8_t		info = READ_FIELD(symbols[i].st_info);
		uint32_t	idx = READ_FIELD(symbols[i].st_name);

		printf("[%3u] %16s / %s\n", i, ELF_SymbolTypeToString(ELF64_ST_TYPE(info)), names + idx);
	}

//	ELF_close();
}

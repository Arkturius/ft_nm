#include <stdint.h>
#include <stdio.h>
#define _CELF_LOGGING 
#define _CELF_IMPLEMENTATION
#include <celf/celf.h>

int main(UNUSED int argc, char **argv)
{
	ELF_open(argv[0]);

	LOG("size = %u", ELF_SIZE);

	getSections();
	
	ELF64_Shdr	*symtab = getSectionByName(".symtab");
	uint32_t	link = READ_FIELD(symtab->sh_link);

//	char		*names = getSectionNames();
	char		*names = (char *) getSectionContent(getSectionByIndex(link));

	ELF64_Sym	*symbols = getSymbols();
	uint32_t	scount = getSymbolCount();

	for (uint32_t i = 0; i < scount; ++i)
	{
		uint8_t		info = READ_FIELD(symbols[i].st_info);
		uint32_t	idx = READ_FIELD(symbols[i].st_name);

		printf("[%3u] %16s / %s\n", i, ELF_SymbolTypeToString(ELF64_ST_TYPE(info)), names + idx);
	}

//	ELF_close();
}

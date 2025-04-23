#include <stdint.h>
#include <stdio.h>
#define _CELF_LOGGING 
#define _CELF_IMPLEMENTATION
#include <celf.h>

int main(UNUSED int argc, char **argv)
{
	ELF_open(argv[0]);

	LOG("size = %u", ELF_SIZE);

	celf64_getSectionHeaders();
	
	ELF64_Shdr	*symtab_hdr = celf64_getSectionByType(SHT_SYMTAB);
	uint32_t	link = READ_FIELD(symtab_hdr->sh_link);

	ELF64_Shdr	*symbol_names = celf64_getSectionByIndex(link);
	uint8_t		*names = celf64_getSectionContent(symbol_names);

	ELF64_Sym	*symbols = celf64_getSymbols();
	uint32_t	scount = celf64_getSymbolCount();

	for (uint32_t i = 0; i < scount; ++i)
	{
		uint8_t	info = READ_FIELD(symbols[i].st_info);
		uint32_t	idx = READ_FIELD(symbols[i].st_name);

		printf("[%3u] %16s / %s\n", i, ELF_SymbolTypeToString(ELF64_ST_TYPE(info)), (char *)names + idx);
	}

//	ELF_close();
}

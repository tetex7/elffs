//
// Created by tete on 8/23/26.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <elf.h>
#include "EfsObjectFileSubProgram.hpp"

#include "EfsFile.hpp"

void EfsObjectFileSubProgram::write_elf_object(const std::string& filename, const std::vector<uint8_t>& payload) {
    std::ofstream file(filename, std::ios::binary);

    // 1. Build String Tables
    // Section Header String Table (.shstrtab)
    std::vector<char> shstrtab = { '\0' };
    auto add_shstr = [&](const char* str) {
        uint32_t offset = shstrtab.size();
        shstrtab.insert(shstrtab.end(), str, str + strlen(str) + 1);
        return offset;
    };
    uint32_t name_data     = add_shstr(".efs");
    uint32_t name_symtab   = add_shstr(".symtab");
    uint32_t name_strtab   = add_shstr(".strtab");
    uint32_t name_shstrtab = add_shstr(".shstrtab");

    // Symbol String Table (.strtab)
    std::vector<char> strtab = { '\0' };
    auto add_str = [&](const char* str) {
        uint32_t offset = strtab.size();
        strtab.insert(strtab.end(), str, str + strlen(str) + 1);
        return offset;
    };
    uint32_t sym_start_name = add_str("_efs_data_bin_start");
    uint32_t sym_end_name   = add_str("_efs_data_bin_end");
    uint32_t sym_size_name   = add_str("_efs_data_bin_size");

    // 2. Build Symbol Table (.symtab)
    // Index 0 is always null
    std::vector<Elf64_Sym> symtab(1);
    memset(&symtab[0], 0, sizeof(Elf64_Sym));

    // Section indices in our layout: 0=SHN_UNDEF, 1=.efs, 2=.symtab, 3=.strtab, 4=.shstrtab
    const uint16_t UNDEF_SHNDX = SHN_UNDEF;
    const uint16_t EFS_SHNDX = 1;
    const uint16_t SYMTAB_SHNDX = 2;
    const uint16_t STRTAB_SHNDX = 3;
    const uint16_t SHSTRTAB_SHNDX = 4;

    // Symbol: _start
    Elf64_Sym sym_start{};
    sym_start.st_name  = sym_start_name;
    sym_start.st_info  = ELF64_ST_INFO(STB_GLOBAL, STT_OBJECT);
    sym_start.st_other = ELF64_ST_VISIBILITY(STV_HIDDEN);
    sym_start.st_shndx = EFS_SHNDX;
    sym_start.st_value = 0; // Starts at offset 0 in .data
    sym_start.st_size  = payload.size();
    symtab.push_back(sym_start);

    // Symbol: _end
    Elf64_Sym sym_end{};
    sym_end.st_name  = sym_end_name;
    sym_end.st_info  = ELF64_ST_INFO(STB_GLOBAL, STT_OBJECT);
    sym_end.st_other = ELF64_ST_VISIBILITY(STV_HIDDEN);
    sym_end.st_shndx = EFS_SHNDX;
    sym_end.st_value = payload.size(); // Points past the last byte
    sym_end.st_size  = 0;
    symtab.push_back(sym_end);

    Elf64_Sym sym_size{};
    sym_size.st_name = sym_size_name;
    sym_size.st_info = ELF64_ST_INFO(STB_GLOBAL, STT_OBJECT);
    sym_size.st_other = ELF64_ST_VISIBILITY(STV_HIDDEN);
    sym_size.st_shndx = EFS_SHNDX;
    sym_size.st_value = payload.size();
    sym_size.st_size  = sizeof(efs_size_t);
    symtab.push_back(sym_size);


    // 3. Calculate Layout Offsets
    uint64_t offset = sizeof(Elf64_Ehdr);

    uint64_t efs_offset = offset;
    offset += payload.size();
    offset += sizeof(efs_size_t);

    uint64_t symtab_offset = offset;
    offset += symtab.size() * sizeof(Elf64_Sym);

    uint64_t strtab_offset = offset;
    offset += strtab.size();

    uint64_t shstrtab_offset = offset;
    offset += shstrtab.size();

    uint64_t shdr_offset = offset;

    // 4. Build ELF Header
    Elf64_Ehdr ehdr{};
    ehdr.e_ident[EI_MAG0]    = ELFMAG0;
    ehdr.e_ident[EI_MAG1]    = ELFMAG1;
    ehdr.e_ident[EI_MAG2]    = ELFMAG2;
    ehdr.e_ident[EI_MAG3]    = ELFMAG3;
    ehdr.e_ident[EI_CLASS]   = ELFCLASS64;
    ehdr.e_ident[EI_DATA]    = ELFDATA2LSB; // Little-endian
    ehdr.e_ident[EI_VERSION] = EV_CURRENT;
    ehdr.e_ident[EI_OSABI]   = ELFOSABI_SYSV;

    ehdr.e_type      = ET_REL;       // Relocatable Object File
    ehdr.e_machine   = EM_X86_64;    // x86_64 Architecture
    ehdr.e_version   = EV_CURRENT;
    ehdr.e_ehsize    = sizeof(Elf64_Ehdr);
    ehdr.e_shentsize = sizeof(Elf64_Shdr);
    ehdr.e_shnum     = 5;            // Null, .data, .symtab, .strtab, .shstrtab
    ehdr.e_shoff     = shdr_offset;
    ehdr.e_shstrndx  = 4;            // Index of .shstrtab header

    // 5. Build Section Headers
    std::vector<Elf64_Shdr> shdrs(5);

    // Section 0: Null
    memset(&shdrs[UNDEF_SHNDX], 0, sizeof(Elf64_Shdr));

    // Section 1: .efs
    shdrs[EFS_SHNDX].sh_name      = name_data;
    shdrs[EFS_SHNDX].sh_type      = SHT_PROGBITS;
    shdrs[EFS_SHNDX].sh_flags     = SHF_ALLOC;
    shdrs[EFS_SHNDX].sh_offset    = efs_offset;
    shdrs[EFS_SHNDX].sh_size      = payload.size()+sizeof(efs_size_t);
    shdrs[EFS_SHNDX].sh_addralign = 8;

    // Section 2: .symtab
    shdrs[SYMTAB_SHNDX].sh_name      = name_symtab;
    shdrs[SYMTAB_SHNDX].sh_type      = SHT_SYMTAB;
    shdrs[SYMTAB_SHNDX].sh_offset    = symtab_offset;
    shdrs[SYMTAB_SHNDX].sh_size      = symtab.size() * sizeof(Elf64_Sym);
    shdrs[SYMTAB_SHNDX].sh_link      = 3; // Index of associated string table (.strtab)
    shdrs[SYMTAB_SHNDX].sh_info      = 1; // Index of first global symbol (1 local symbol: index 0)
    shdrs[SYMTAB_SHNDX].sh_addralign = 8;
    shdrs[SYMTAB_SHNDX].sh_entsize   = sizeof(Elf64_Sym);

    // Section 3: .strtab
    shdrs[STRTAB_SHNDX].sh_name      = name_strtab;
    shdrs[STRTAB_SHNDX].sh_type      = SHT_STRTAB;
    shdrs[STRTAB_SHNDX].sh_offset    = strtab_offset;
    shdrs[STRTAB_SHNDX].sh_size      = strtab.size();
    shdrs[STRTAB_SHNDX].sh_addralign = 1;

    // Section 4: .shstrtab
    shdrs[SHSTRTAB_SHNDX].sh_name      = name_shstrtab;
    shdrs[SHSTRTAB_SHNDX].sh_type      = SHT_STRTAB;
    shdrs[SHSTRTAB_SHNDX].sh_offset    = shstrtab_offset;
    shdrs[SHSTRTAB_SHNDX].sh_size      = shstrtab.size();
    shdrs[SHSTRTAB_SHNDX].sh_addralign = 1;

    efs_size_t z = payload.size();

    // 6. Write File Structure to Disk
    file.write(reinterpret_cast<const char*>(&ehdr), sizeof(ehdr));
    file.write(reinterpret_cast<const char*>(payload.data()), payload.size());
    file.write(reinterpret_cast<const char*>(&z), sizeof(efs_size_t));
    file.write(reinterpret_cast<const char*>(symtab.data()), symtab.size() * sizeof(Elf64_Sym));
    file.write(strtab.data(), strtab.size());
    file.write(shstrtab.data(), shstrtab.size());
    file.write(reinterpret_cast<const char*>(shdrs.data()), shdrs.size() * sizeof(Elf64_Shdr));
}

void EfsObjectFileSubProgram::pre_start()
{
    auto& vm = getInfoPack().vm.value().get();
    if (!vm.contains("input") || !vm.contains("output"))
    {
        throw std::runtime_error("No IO");
    }
}

int EfsObjectFileSubProgram::sub_program_main()
{
    EfsFile file = EfsFile(getInfoPack().input_file);

    std::vector<uint8_t> payload;
    payload.resize(file.getFileSize());
    file.getFileStream().read(reinterpret_cast<char*>(payload.data()), payload.size());
    file.getFileStream().seekg(0, std::ios::beg);
    write_elf_object(getInfoPack().output_file, payload);
    return 0;
}

/*
 * Copyright (C) 2019 Assured Information Security, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef BFELF_LOADER_PRIVATE_H
#define BFELF_LOADER_PRIVATE_H

#include "bftypes.h"

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------------------------------------------------------------- */
/* C Debugging                                                                                    */
/* ---------------------------------------------------------------------------------------------- */

#if !defined(KERNEL) && !defined(EFI)
#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#if LOG_LEVEL>=3
#define ELF_INFO(...) printf_("[ELF LOADER]: " __VA_ARGS__)
#define ELF_ALERT(...) printf_("[ELF LOADER ALERT]: " __VA_ARGS__)
#else
#define ELF_INFO(...)
#define ELF_ALERT(...)
#endif

#endif

/* ---------------------------------------------------------------------------------------------- */
/* Linux Debugging                                                                                */
/* ---------------------------------------------------------------------------------------------- */

#if defined(KERNEL) && !defined(EFI)
#ifdef __linux__
#include <linux/printk.h>
#define ELF_INFO(...) printk(KERN_INFO "[ELF LOADER]: " __VA_ARGS__)
#define ELF_ALERT(...) printk(KERN_INFO "[ELF LOADER ALERT]: " __VA_ARGS__)
#endif
#endif

/* ---------------------------------------------------------------------------------------------- */
/* Windows Debugging                                                                              */
/* ---------------------------------------------------------------------------------------------- */

#if defined(KERNEL) && !defined(EFI)
#ifdef _WIN32
#include <wdm.h>
#define ELF_INFO(...) DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "[ELF LOADER]: " __VA_ARGS__)
#define ELF_ALERT(...) DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "[ELF LOADER ALERT]: " __VA_ARGS__)
#endif
#endif

/* ---------------------------------------------------------------------------------------------- */
/* EFI Debugging                                                                                  */
/* ---------------------------------------------------------------------------------------------- */

#if defined(KERNEL) && defined(EFI)
#include "efi.h"
#include "efilib.h"
#define ELF_INFO(...) Print(L"[ELF LOADER]: " __VA_ARGS__)
#define ELF_ALERT(...) Print(L"[ELF LOADER ALERT]: " __VA_ARGS__)
#endif

/* ---------------------------------------------------------------------------------------------- */
/* ELF Data Types                                                                                 */
/* ---------------------------------------------------------------------------------------------- */

/*
 * Data Representation
 *
 * The following is defined in the ELF 64bit file format specification:
 * http://www.uclibc.org/docs/elf-64-gen.pdf, page 2
 */

#ifndef __cplusplus
typedef uint64_t bfelf64_addr;
typedef uint64_t bfelf64_off;
typedef uint16_t bfelf64_half;
typedef uint32_t bfelf64_word;
typedef int32_t bfelf64_sword;
typedef uint64_t bfelf64_xword;
typedef int64_t bfelf64_sxword;
#else
using bfelf64_addr = uint64_t;
using bfelf64_off = uint64_t;
using bfelf64_half = uint16_t;
using bfelf64_word = uint32_t;
using bfelf64_sword = int32_t;
using bfelf64_xword = uint64_t;
using bfelf64_sxword = int64_t;
#endif

/* ---------------------------------------------------------------------------------------------- */
/* ELF File Definition                                                                            */
/* ---------------------------------------------------------------------------------------------- */

struct bfelf_shdr;
struct bfelf_phdr;
struct bfelf_ehdr;

struct bfelf_file_t {

    bfelf64_addr entry;

    bfelf64_addr init_array_addr;
    bfelf64_xword init_array_size;

    bfelf64_addr fini_array_addr;
    bfelf64_xword fini_array_size;

    bfelf64_addr eh_frame_addr;
    bfelf64_xword eh_frame_size;

    /**
     * Private APIs
     *
     * Do not directly use the remaining variables defined in this structure
     * as these are for internal use only and are subject to change.
     *
     * Notes:
     * - The "file" variable store the ELF file, while the "exec" variable
     *   store the memory that the ELF file will be executed from. The "virt"
     *   variable store the virtual address that the ELF file will be executed
     *   from. Normally, there is no difference between "exec" and "virt". The
     *   only time there would be a difference is if you are writing an OS or
     *   hypervisor, and the ELF file that you are loading will be executed in
     *   a different memory space.
     *
     * - The "size" variable is the size of the memory that must be allocated
     *   by the user for "exec", not the size of the ELF "file" itself.
     */

    const uint8_t *file;

    bfelf64_xword size;
    uint8_t *exec;
    bfelf64_addr virt;

    uint8_t *rx_addr;
    bfelf64_xword rx_size;

    uint8_t *rw_addr;
    bfelf64_xword rw_size;

    const struct bfelf_ehdr *ehdr;
    const struct bfelf_phdr *phdrtab;
    const struct bfelf_shdr *shdrtab;
    const char *shstrtab;

    const struct bfelf_phdr *pt_load_rx;
    const struct bfelf_phdr *pt_load_rw;

    const struct bfelf_shdr *shdr_rela;
    const struct bfelf_shdr *shdr_init_array;
    const struct bfelf_shdr *shdr_fini_array;
    const struct bfelf_shdr *shdr_eh_frame;
};

static inline void dump_bfelf_file_t(struct bfelf_file_t * ef)
{
    ELF_ALERT("ELF Internal Data Structure Info:\n");
    ELF_ALERT("    entry: 0x%x\n", ef->entry);
    ELF_ALERT("    init_array_addr: 0x%x\n", ef->init_array_addr);
    ELF_ALERT("    init_array_size: 0x%x\n", ef->init_array_size);
    ELF_ALERT("    fini_array_addr: 0x%x\n", ef->fini_array_addr);
    ELF_ALERT("    fini_array_size: 0x%x\n", ef->fini_array_size);
    ELF_ALERT("    eh_frame_addr: 0x%x\n", ef->eh_frame_addr);
    ELF_ALERT("    eh_frame_size: 0x%x\n", ef->eh_frame_size);
    ELF_ALERT("    file: 0x%x\n", ef->file);
    ELF_ALERT("    size: 0x%x\n", ef->size);
    ELF_ALERT("    exec: 0x%x\n", ef->exec);
    ELF_ALERT("    virt: 0x%x\n", ef->virt);
    ELF_ALERT("    rx_addr: 0x%x\n", ef->rx_addr);
    ELF_ALERT("    rx_size: 0x%x\n", ef->rx_size);
    ELF_ALERT("    rw_addr: 0x%x\n", ef->rw_addr);
    ELF_ALERT("    rw_size: 0x%x\n", ef->rw_size);
    ELF_ALERT("    ehdr: 0x%x\n", ef->ehdr);
    ELF_ALERT("    phdrtab: 0x%x\n", ef->phdrtab);
    ELF_ALERT("    shdrtab: 0x%x\n", ef->shdrtab);
    ELF_ALERT("    shstrtab: 0x%x\n", ef->shstrtab);
    ELF_ALERT("    pt_load_rx: 0x%x\n", ef->pt_load_rx);
    ELF_ALERT("    pt_load_rw: 0x%x\n", ef->pt_load_rw);
    ELF_ALERT("    shdr_rela: 0x%x\n", ef->shdr_rela);
    ELF_ALERT("    shdr_init_array: 0x%x\n", ef->shdr_init_array);
    ELF_ALERT("    shdr_fini_array: 0x%x\n", ef->shdr_fini_array);
    ELF_ALERT("    shdr_eh_frame: 0x%x\n", ef->shdr_eh_frame);
}

/* ---------------------------------------------------------------------------------------------- */
/* ELF File Header                                                                                */
/* ---------------------------------------------------------------------------------------------- */

/*
 * e_ident indexes
 *
 * The following is defined in the ELF 64bit file format specification:
 * http://www.uclibc.org/docs/elf-64-gen.pdf, page 3
 */

#define bfei_mag0 BFSCAST(bfelf64_sword, 0)
#define bfei_mag1 BFSCAST(bfelf64_sword, 1)
#define bfei_mag2 BFSCAST(bfelf64_sword, 2)
#define bfei_mag3 BFSCAST(bfelf64_sword, 3)
#define bfei_class BFSCAST(bfelf64_sword, 4)
#define bfei_data BFSCAST(bfelf64_sword, 5)
#define bfei_version BFSCAST(bfelf64_sword, 6)
#define bfei_osabi BFSCAST(bfelf64_sword, 7)
#define bfei_abiversion BFSCAST(bfelf64_sword, 8)
#define bfei_pad BFSCAST(bfelf64_sword, 9)
#define bfei_nident BFSCAST(bfelf64_sword, 16)

/*
 * ELF Class Types
 *
 * The following is defined in the ELF 64bit file format specification:
 * http://www.uclibc.org/docs/elf-64-gen.pdf, page 5
 */

#define bfelfclass32 BFSCAST(unsigned char, 1)
#define bfelfclass64 BFSCAST(unsigned char, 2)

/*
 * ELF Data Types
 *
 * The following is defined in the ELF 64bit file format specification:
 * http://www.uclibc.org/docs/elf-64-gen.pdf, page 5
 */

#define bfelfdata2lsb BFSCAST(unsigned char, 1)
#define bfelfdata2msb BFSCAST(unsigned char, 2)

/*
 * ELF Version
 *
 * The following is defined in the ELF 64bit file format specification:
 * http://www.uclibc.org/docs/elf-64-gen.pdf, page 4
 */

#define bfev_current BFSCAST(unsigned char, 1)

/*
 * ELF OS / ABI Types
 *
 * The following is defined in the ELF 64bit file format specification:
 * http://www.uclibc.org/docs/elf-64-gen.pdf, page 5
 */

#define bfelfosabi_sysv BFSCAST(unsigned char, 0)
#define bfelfosabi_hpux BFSCAST(unsigned char, 1)
#define bfelfosabi_standalone BFSCAST(unsigned char, 255)

/*
 * ELF Types
 *
 * The following is defined in the ELF 64bit file format specification:
 * http://www.uclibc.org/docs/elf-64-gen.pdf, page 5
 */

#define bfet_none BFSCAST(bfelf64_half, 0)
#define bfet_rel BFSCAST(bfelf64_half, 1)
#define bfet_exec BFSCAST(bfelf64_half, 2)
#define bfet_dyn BFSCAST(bfelf64_half, 3)
#define bfet_core BFSCAST(bfelf64_half, 4)
#define bfet_loos BFSCAST(bfelf64_half, 0xFE00)
#define bfet_hios BFSCAST(bfelf64_half, 0xFEFF)
#define bfet_loproc BFSCAST(bfelf64_half, 0xFF00)
#define bfet_hiproc BFSCAST(bfelf64_half, 0xFFFF)

/*
 * ELF Machine Codes
 *
 * The following is defined in the Linux kernel sources:
 * linux/include/uapi/linux/elf-em.h
 */

#define bfem_none BFSCAST(bfelf64_half, 0)
#define bfem_m32 BFSCAST(bfelf64_half, 1)
#define bfem_sparc BFSCAST(bfelf64_half, 2)
#define bfem_386 BFSCAST(bfelf64_half, 3)
#define bfem_68k BFSCAST(bfelf64_half, 4)
#define bfem_88k BFSCAST(bfelf64_half, 5)
#define bfem_486 BFSCAST(bfelf64_half, 6)
#define bfem_860 BFSCAST(bfelf64_half, 7)
#define bfem_mips BFSCAST(bfelf64_half, 8)
#define bfem_mips_rs3_le BFSCAST(bfelf64_half, 10)
#define bfem_mips_rs4_be BFSCAST(bfelf64_half, 11)
#define bfem_parisc BFSCAST(bfelf64_half, 15)
#define bfem_sparc32plus BFSCAST(bfelf64_half, 18)
#define bfem_ppc BFSCAST(bfelf64_half, 20)
#define bfem_ppc64 BFSCAST(bfelf64_half, 21)
#define bfem_spu BFSCAST(bfelf64_half, 23)
#define bfem_arm BFSCAST(bfelf64_half, 40)
#define bfem_sh BFSCAST(bfelf64_half, 42)
#define bfem_sparcv9 BFSCAST(bfelf64_half, 43)
#define bfem_h8_300 BFSCAST(bfelf64_half, 46)
#define bfem_ia_64 BFSCAST(bfelf64_half, 50)
#define bfem_x86_64 BFSCAST(bfelf64_half, 62)
#define bfem_s390 BFSCAST(bfelf64_half, 22)
#define bfem_cris BFSCAST(bfelf64_half, 76)
#define bfem_v850 BFSCAST(bfelf64_half, 87)
#define bfem_m32r BFSCAST(bfelf64_half, 88)
#define bfem_mn10300 BFSCAST(bfelf64_half, 89)
#define bfem_openrisc BFSCAST(bfelf64_half, 92)
#define bfem_blackfin BFSCAST(bfelf64_half, 106)
#define bfem_altera_nios2 BFSCAST(bfelf64_half, 113)
#define bfem_ti_c6000 BFSCAST(bfelf64_half, 140)
#define bfem_aarch64 BFSCAST(bfelf64_half, 183)
#define bfem_frv BFSCAST(bfelf64_half, 0x5441)
#define bfem_avr32 BFSCAST(bfelf64_half, 0x18AD)
#define bfem_alpha BFSCAST(bfelf64_half, 0x9026)
#define bfem_cygnus_v850 BFSCAST(bfelf64_half, 0x9080)
#define bfem_cygnus_m32r BFSCAST(bfelf64_half, 0x9041)
#define bfem_s390_old BFSCAST(bfelf64_half, 0xA390)
#define bfem_cygnus_mn10300 BFSCAST(bfelf64_half, 0xBEEF)

/*
 * ELF File Header
 *
 * The following is defined in the ELF 64bit file format specification:
 * http://www.uclibc.org/docs/elf-64-gen.pdf, page 3
 *
 * The file header is located at the beginning of the file, and is used to
 * locate the other parts of the file.
 */

struct bfelf_ehdr {
    unsigned char e_ident[bfei_nident];
    bfelf64_half e_type;
    bfelf64_half e_machine;
    bfelf64_word e_version;
    bfelf64_addr e_entry;
    bfelf64_off e_phoff;
    bfelf64_off e_shoff;
    bfelf64_word e_flags;
    bfelf64_half e_ehsize;
    bfelf64_half e_phentsize;
    bfelf64_half e_phnum;
    bfelf64_half e_shentsize;
    bfelf64_half e_shnum;
    bfelf64_half e_shstrndx;
};

static inline void dump_bfelf_ehdr(struct bfelf_ehdr * ehdr)
{
    ELF_ALERT("ELF Header:\n");
    ELF_ALERT("    e_type: 0x%x\n", ehdr->e_type);
    ELF_ALERT("    e_machine: 0x%x\n", ehdr->e_machine);
    ELF_ALERT("    e_version: 0x%x\n", ehdr->e_version);
    ELF_ALERT("    e_entry: 0x%x\n", ehdr->e_entry);
    ELF_ALERT("    e_phoff: 0x%x\n", ehdr->e_phoff);
    ELF_ALERT("    e_shoff: 0x%x\n", ehdr->e_shoff);
    ELF_ALERT("    e_flags: 0x%x\n", ehdr->e_flags);
    ELF_ALERT("    e_ehsize: 0x%x\n", ehdr->e_ehsize);
    ELF_ALERT("    e_phentsize: 0x%x\n", ehdr->e_phentsize);
    ELF_ALERT("    e_phnum: 0x%x\n", ehdr->e_phnum);
    ELF_ALERT("    e_shentsize: 0x%x\n", ehdr->e_shentsize);
    ELF_ALERT("    e_shnum: 0x%x\n", ehdr->e_shnum);
    ELF_ALERT("    e_shstrndx: 0x%x\n", ehdr->e_shstrndx);
}

/* ---------------------------------------------------------------------------------------------- */
/* ELF Section Header Table                                                                       */
/* ---------------------------------------------------------------------------------------------- */

/*
 * ELF Section Type
 *
 * The following is defined in the ELF 64bit file format specification:
 * http://www.uclibc.org/docs/elf-64-gen.pdf, page 7
 */

#define bfsht_null BFSCAST(bfelf64_word, 0)
#define bfsht_progbits BFSCAST(bfelf64_word, 1)
#define bfsht_symtab BFSCAST(bfelf64_word, 2)
#define bfsht_strtab BFSCAST(bfelf64_word, 3)
#define bfsht_rela BFSCAST(bfelf64_word, 4)
#define bfsht_hash BFSCAST(bfelf64_word, 5)
#define bfsht_dynamic BFSCAST(bfelf64_word, 6)
#define bfsht_note BFSCAST(bfelf64_word, 7)
#define bfsht_nobits BFSCAST(bfelf64_word, 8)
#define bfsht_rel BFSCAST(bfelf64_word, 9)
#define bfsht_shlib BFSCAST(bfelf64_word, 10)
#define bfsht_dynsym BFSCAST(bfelf64_word, 11)
#define bfsht_init_array BFSCAST(bfelf64_word, 14)
#define bfsht_fini_array BFSCAST(bfelf64_word, 15)
#define bfsht_loos BFSCAST(bfelf64_word, 0x60000000)
#define bfsht_aarch64_dynstr BFSCAST(bfelf64_word, 0x6FFFFFF6)
#define bfsht_hios BFSCAST(bfelf64_word, 0x6FFFFFFF)
#define bfsht_loproc BFSCAST(bfelf64_word, 0x70000000)
#define bfsht_x86_64_unwind BFSCAST(bfelf64_word, 0x70000001)
#define bfsht_hiproc BFSCAST(bfelf64_word, 0x7FFFFFFF)

/*
 * ELF Section Attributes
 *
 * The following is defined in the ELF 64bit file format specification:
 * http://www.uclibc.org/docs/elf-64-gen.pdf, page 8
 */

#define bfshf_write BFSCAST(bfelf64_xword, 0x1)
#define bfshf_alloc BFSCAST(bfelf64_xword, 0x2)
#define bfshf_execinstr BFSCAST(bfelf64_xword, 0x4)
#define bfshf_maskos BFSCAST(bfelf64_xword, 0x0F000000)
#define bfshf_maskproc BFSCAST(bfelf64_xword, 0xF0000000)
#define bfshf_undocumneted BFSCAST(bfelf64_xword, 0x00000060)

#define bfshf_a (bfshf_alloc)
#define bfshf_wa (bfshf_write | bfshf_alloc)
#define bfshf_ai (bfshf_alloc | bfshf_write | bfshf_undocumneted)

/*
 * ELF Section Header Entry
 *
 * The following is defined in the ELF 64bit file format specification:
 * http://www.uclibc.org/docs/elf-64-gen.pdf, page 6
 *
 * Sections contain all the information in an ELF file, except for the ELF
 * header, program header table, and section header table. Sections are
 * identified by an index into the section header table.
 */

struct bfelf_shdr {
    bfelf64_word sh_name;
    bfelf64_word sh_type;
    bfelf64_xword sh_flags;
    bfelf64_addr sh_addr;
    bfelf64_off sh_offset;
    bfelf64_xword sh_size;
    bfelf64_word sh_link;
    bfelf64_word sh_info;
    bfelf64_xword sh_addralign;
    bfelf64_xword sh_entsize;
};

static inline void dump_bfelf_shdr(struct bfelf_shdr * shdr)
{
    ELF_ALERT("ELF Section Header:\n");
    ELF_ALERT("    sh_name: 0x%x\n", shdr->sh_name);
    ELF_ALERT("    sh_type: 0x%x\n", shdr->sh_type);
    ELF_ALERT("    sh_flags: 0x%x\n", shdr->sh_flags);
    ELF_ALERT("    sh_addr: 0x%x\n", shdr->sh_addr);
    ELF_ALERT("    sh_offset: 0x%x\n", shdr->sh_offset);
    ELF_ALERT("    sh_size: 0x%x\n", shdr->sh_size);
    ELF_ALERT("    sh_link: 0x%x\n", shdr->sh_link);
    ELF_ALERT("    sh_info: 0x%x\n", shdr->sh_info);
    ELF_ALERT("    sh_addralign: 0x%x\n", shdr->sh_addralign);
    ELF_ALERT("    sh_entsize: 0x%x\n", shdr->sh_entsize);
}

/* ---------------------------------------------------------------------------------------------- */
/* ELF Program Header                                                                             */
/* ---------------------------------------------------------------------------------------------- */

/*
 * ELF Segment Types
 *
 * The following is defined in the ELF 64bit file format specification:
 * http://www.uclibc.org/docs/elf-64-gen.pdf, page 12
 */

#define bfpt_null BFSCAST(bfelf64_word, 0)
#define bfpt_load BFSCAST(bfelf64_word, 1)
#define bfpt_dynamic BFSCAST(bfelf64_word, 2)
#define bfpt_interp BFSCAST(bfelf64_word, 3)
#define bfpt_note BFSCAST(bfelf64_word, 4)
#define bfpt_shlib BFSCAST(bfelf64_word, 5)
#define bfpt_phdr BFSCAST(bfelf64_word, 6)
#define bfpt_loos BFSCAST(bfelf64_word, 0x60000000)
#define bfpt_gnu_eh_frame BFSCAST(bfelf64_word, 0x6474e550)
#define bfpt_gnu_stack BFSCAST(bfelf64_word, 0x6474e551)
#define bfpt_gnu_relro BFSCAST(bfelf64_word, 0x6474e552)
#define bfpt_hios BFSCAST(bfelf64_word, 0x6FFFFFFF)
#define bfpt_loproc BFSCAST(bfelf64_word, 0x70000000)
#define bfpt_hiproc BFSCAST(bfelf64_word, 0x7FFFFFFF)

/*
 * ELF Segment Flags
 *
 * The following is defined in the ELF 64bit file format specification:
 * http://www.uclibc.org/docs/elf-64-gen.pdf, page 13
 */

#define bfpf_x BFSCAST(bfelf64_xword, 0x1)
#define bfpf_w BFSCAST(bfelf64_xword, 0x2)
#define bfpf_r BFSCAST(bfelf64_xword, 0x4)
#define bfpf_rx (bfpf_r | bfpf_x)
#define bfpf_rw (bfpf_r | bfpf_w)
#define bfpf_maskos BFSCAST(bfelf64_xword, 0x00FF0000)
#define bfpf_maskproc BFSCAST(bfelf64_xword, 0xFF000000)

/*
 * ELF Program Header Entry
 *
 * The following is defined in the ELF 64bit file format specification:
 * http://www.uclibc.org/docs/elf-64-gen.pdf, page 12
 *
 * In executable and shared object files, sections are grouped into segments for
 * loading. The program header table contains a list of entries describing
 * each segment. This information is needed when using the ELF loader to
 * load each segment into memory allocated by the user. For more information
 * on how to do this, please see the unit tests.
 */

struct bfelf_phdr {
    bfelf64_word p_type;
    bfelf64_word p_flags;
    bfelf64_off p_offset;
    bfelf64_addr p_vaddr;
    bfelf64_addr p_paddr;
    bfelf64_xword p_filesz;
    bfelf64_xword p_memsz;
    bfelf64_xword p_align;
};

static inline void dump_bfelf_phdr(struct bfelf_phdr * phdr)
{
    ELF_ALERT("ELF Program Header @ 0x%x:\n", phdr);
    ELF_ALERT("    p_type: 0x%x\n", phdr->p_type);
    ELF_ALERT("    p_flags: 0x%x\n", phdr->p_flags);
    ELF_ALERT("    p_offset: 0x%x\n", phdr->p_offset);
    ELF_ALERT("    p_vaddr: 0x%x\n", phdr->p_vaddr);
    ELF_ALERT("    p_paddr: 0x%x\n", phdr->p_paddr);
    ELF_ALERT("    p_filesz: 0x%x\n", phdr->p_filesz);
    ELF_ALERT("    p_memsz: 0x%x\n", phdr->p_memsz);
    ELF_ALERT("    p_align: 0x%x\n", phdr->p_align);
}

/* ---------------------------------------------------------------------------------------------- */
/* ELF Relocations                                                                                */
/* ---------------------------------------------------------------------------------------------- */

/*
 * ELF Relocation Addend
 *
 * The following is defined in the ELF 64bit file format specification:
 * http://www.uclibc.org/docs/elf-64-gen.pdf, page 11
 */

struct bfelf_rela {
    bfelf64_addr r_offset;
    bfelf64_xword r_info;
    bfelf64_sxword r_addend;
};

/*
 * ELF Relocation Info Algorithms
 *
 * The following is defined in the ELF 64bit file format specification:
 * http://www.uclibc.org/docs/elf-64-gen.pdf, page 11
 */

#define BFELF_REL_SYM(i) ((i) >> 32)
#define BFELF_REL_TYPE(i) ((i)&0xFFFFFFFFL)

/*
 * System V ABI 64bit Relocations
 *
 * The following is defined in the ELF 64bit file format specification:
 * http://www.x86-64.org/documentation/abi.pdf, page 71
 *
 * @cond
 */

#define BFR_X86_64_64 BFSCAST(bfelf64_xword, 1)
#define BFR_X86_64_GLOB_DAT BFSCAST(bfelf64_xword, 6)
#define BFR_X86_64_JUMP_SLOT BFSCAST(bfelf64_xword, 7)
#define BFR_X86_64_RELATIVE BFSCAST(bfelf64_xword, 8)

/*
 * AArch64 Relocations
 *
 * The following is defined in the ELF for the ARM 64-bit architecture
 * specification:
 * http://infocenter.arm.com/help/topic/com.arm.doc.ihi0056b/IHI0056B_aaelf64.pdf
 *
 * @cond
 */
#define BFR_AARCH64_RELATIVE BFSCAST(bfelf64_xword, 1027)

static inline void dump_bfelf_rela(struct bfelf_rela * rela)
{
    ELF_ALERT("ELF RELA Relocation:\n");
    switch (BFELF_REL_TYPE(rela->r_info)) {
        case BFR_X86_64_RELATIVE: {
            ELF_ALERT("    r_info: R_X86_64_RELATIVE\n");
            break;
        }

        case BFR_AARCH64_RELATIVE: {
            ELF_ALERT("    r_info: R_AARCH64_RELATIVE\n");
            break;
        }

        default:
            ELF_ALERT("    r_info: unknown type (0x%x)\n", rela->r_info);
    }

    ELF_ALERT("    r_offset: 0x%x\n", rela->r_offset);
    ELF_ALERT("    r_addend: 0x%x\n", rela->r_addend);
}

/* ---------------------------------------------------------------------------------------------- */
/* ELF Helpers                                                                                    */
/* ---------------------------------------------------------------------------------------------- */

#ifndef BFELF_LOADER_STRCMP
static inline status_t
private_strcmp(const char *s1, const char *s2)
{
    while ((*s1 != 0) && (*s1 == *s2)) {
        s1++, s2++;
    }

    return *s1 == *s2 ? BFSUCCESS : BFFAILURE;
}
#else
#define private_strcmp BFELF_LOADER_STRCMP
#endif

#ifndef BFELF_LOADER_MEMCPY
static inline void
private_memcpy(uint8_t *dst, const uint8_t *src, bfelf64_xword size)
{
    bfelf64_xword i;

    for (i = 0; i < size; i++) {
        dst[i] = src[i];
    }
}
#else
#define private_memcpy BFELF_LOADER_MEMCPY
#endif

#ifndef BFELF_LOADER_MEMSET
static inline void
private_memset(uint8_t *dst, uint8_t val, bfelf64_xword size)
{
    bfelf64_xword i;

    for (i = 0; i < size; i++) {
        dst[i] = val;
    }
}
#else
#define private_memset BFELF_LOADER_MEMSET
#endif

/* ---------------------------------------------------------------------------------------------- */
/* ELF Implementation                                                                        */
/* ---------------------------------------------------------------------------------------------- */

static inline status_t
private_check_support(struct bfelf_file_t *ef)
{
    if (ef->ehdr->e_ident[bfei_mag0] != 0x7F) {
        ELF_ALERT("magic #0 has unexpected value\n");
        return BFFAILURE;
    }

    if (ef->ehdr->e_ident[bfei_mag1] != 'E') {
        ELF_ALERT("magic #1 has unexpected value\n");
        return BFFAILURE;
    }

    if (ef->ehdr->e_ident[bfei_mag2] != 'L') {
        ELF_ALERT("magic #2 has unexpected value\n");
        return BFFAILURE;
    }

    if (ef->ehdr->e_ident[bfei_mag3] != 'F') {
        ELF_ALERT("magic #3 has unexpected value\n");
        return BFFAILURE;
    }

    if (ef->ehdr->e_ident[bfei_class] != bfelfclass64) {
        ELF_ALERT("file is not 64bit\n");
        return BFFAILURE;
    }

    if (ef->ehdr->e_ident[bfei_data] != bfelfdata2lsb) {
        ELF_ALERT("file is not little endian\n");
        return BFFAILURE;
    }

    if (ef->ehdr->e_ident[bfei_version] != bfev_current) {
        ELF_ALERT("unsupported version\n");
        return BFFAILURE;
    }

    if (ef->ehdr->e_ident[bfei_osabi] != bfelfosabi_sysv) {
        ELF_ALERT("file does not use the system v abi\n");
        return BFFAILURE;
    }

    if (ef->ehdr->e_ident[bfei_abiversion] != 0) {
        ELF_ALERT("unsupported abi version\n");
        return BFFAILURE;
    }

    if (ef->ehdr->e_type != bfet_dyn) {
        ELF_ALERT("file must be marked as ET_DYN\n");
        return BFFAILURE;
    }

    if (ef->ehdr->e_machine != bfem_x86_64 && ef->ehdr->e_machine != bfem_aarch64) {
        ELF_ALERT("file must be compiled for x86_64 or aarch64\n");
        return BFFAILURE;
    }

    if (ef->ehdr->e_version != bfev_current) {
        ELF_ALERT("unsupported version\n");
        return BFFAILURE;
    }

    if (ef->ehdr->e_flags != 0) {
        ELF_ALERT("unsupported flags\n");
        return BFFAILURE;
    }

    return BFSUCCESS;
}

static inline status_t
private_process_segments(struct bfelf_file_t *ef)
{
    bfelf64_half i = 0;

    ef->pt_load_rx = nullptr;
    ef->pt_load_rw = nullptr;

    for (i = 0; i < ef->ehdr->e_phnum; i++) {
        const struct bfelf_phdr *phdr = &(ef->phdrtab[i]);

        if (phdr->p_type == bfpt_gnu_stack && phdr->p_flags != bfpf_rw) {
            ELF_ALERT("executable stacks are not supported\n");
            return BFFAILURE;
        }

        if (phdr->p_type != bfpt_load) {
            continue;
        }

        switch(phdr->p_flags) {
            case bfpf_rx:
                if (ef->pt_load_rx != nullptr) {
                    ELF_ALERT("ELF file has too many bfpf_rx segments\n");
                    return BFFAILURE;
                }

                ef->pt_load_rx = phdr;
                break;

            case bfpf_rw:
                if (ef->pt_load_rw != nullptr) {
                    ELF_ALERT("ELF file has too many bfpf_rw segments\n");
                    return BFFAILURE;
                }

                ef->pt_load_rw = phdr;
                break;

            default:
                ELF_ALERT("ELF segments other than RW or RE are not supported\n");
                return BFFAILURE;
        };
    }

    if (ef->pt_load_rx == nullptr) {
        ELF_ALERT("ELF file is missing an RE segment\n");
        return BFFAILURE;
    }

    if (ef->pt_load_rw == nullptr) {
        ELF_ALERT("ELF file is missing an RW segment\n");
        return BFFAILURE;
    }

    return BFSUCCESS;
}

static inline status_t
private_process_sections(struct bfelf_file_t *ef)
{
    bfelf64_half i = 0;

    ef->shdr_rela = nullptr;
    ef->shdr_init_array = nullptr;
    ef->shdr_fini_array = nullptr;
    ef->shdr_eh_frame = nullptr;

    for (i = 0; i < ef->ehdr->e_shnum; i++) {
        const struct bfelf_shdr *shdr = &(ef->shdrtab[i]);

        switch(shdr->sh_type) {
            case bfsht_null:
            case bfsht_progbits:
            case bfsht_symtab:
            case bfsht_strtab:
            case bfsht_hash:
            case bfsht_dynamic:
            case bfsht_note:
            case bfsht_nobits:
            case bfsht_dynsym:
            case bfsht_x86_64_unwind:
            case bfsht_aarch64_dynstr:
                continue;

            case bfsht_rela:
                if (ef->shdr_rela != nullptr) {
                    ELF_ALERT("ELF file has too many bfsht_rela sections\n");
                    return BFFAILURE;
                }

                ef->shdr_rela = shdr;
                break;

            case bfsht_init_array:
                if (ef->shdr_init_array != nullptr) {
                    ELF_ALERT("ELF file has too many bfsht_init_array sections\n");
                    return BFFAILURE;
                }

                ef->shdr_init_array = shdr;
                break;

            case bfsht_fini_array:
                if (ef->shdr_fini_array != nullptr) {
                    ELF_ALERT("ELF file has too many bfsht_fini_array sections\n");
                    return BFFAILURE;
                }

                ef->shdr_fini_array = shdr;
                break;

            default:
                ELF_ALERT("Section %d in ELF file has unknown type: 0x%x\n", i, shdr->sh_type);
                return BFFAILURE;
        };
    }

    for (i = 0; i < ef->ehdr->e_shnum; i++) {
        const struct bfelf_shdr *shdr = &(ef->shdrtab[i]);
        const char *name = &ef->shstrtab[shdr->sh_name];

        if (private_strcmp(name, ".eh_frame") == BFSUCCESS) {
            if (ef->shdr_eh_frame != nullptr) {
                ELF_ALERT("ELF file has too many eh_frame sections\n");
                return BFFAILURE;
            }

            ef->shdr_eh_frame = shdr;
            break;
        }

        if (private_strcmp(name, ".init") == BFSUCCESS) {
            ELF_ALERT("ELF file has unsupported section: init\n");
            return BFFAILURE;
        }

        if (private_strcmp(name, ".fini") == BFSUCCESS) {
            ELF_ALERT("ELF file has unsupported section: fini\n");
            return BFFAILURE;
        }

        if (private_strcmp(name, ".ctors") == BFSUCCESS) {
            ELF_ALERT("ELF file has unsupported section: ctors\n");
            return BFFAILURE;
        }

        if (private_strcmp(name, ".dtors") == BFSUCCESS) {
            ELF_ALERT("ELF file has unsupported section: dtors\n");
            return BFFAILURE;
        }
    }

    return BFSUCCESS;
}

static inline status_t
private_get_mem_size(struct bfelf_file_t *ef)
{
    const struct bfelf_phdr *phdr = nullptr;
    ef->size = 0;

    phdr = ef->pt_load_rx;
    if (phdr->p_paddr + phdr->p_memsz > ef->size) {
        ef->size = BFALIGN(phdr->p_paddr + phdr->p_memsz, phdr->p_align);
    }

    phdr = ef->pt_load_rw;
    if (phdr->p_paddr + phdr->p_memsz > ef->size) {
        ef->size = BFALIGN(phdr->p_paddr + phdr->p_memsz, phdr->p_align);
    }

    return BFSUCCESS;
}

static inline status_t
private_get_section_info(struct bfelf_file_t *ef)
{
    if (ef->shdr_init_array != nullptr) {
        ef->init_array_addr = ef->virt + ef->shdr_init_array->sh_addr;
        ef->init_array_size = ef->shdr_init_array->sh_size;
    }

    if (ef->shdr_fini_array != nullptr) {
        ef->fini_array_addr = ef->virt + ef->shdr_fini_array->sh_addr;
        ef->fini_array_size = ef->shdr_fini_array->sh_size;
    }

    if (ef->shdr_eh_frame != nullptr) {
        ef->eh_frame_addr = ef->virt + ef->shdr_eh_frame->sh_addr;
        ef->eh_frame_size = ef->shdr_eh_frame->sh_size;
    }

    return BFSUCCESS;
}

static inline status_t
private_relocate(struct bfelf_file_t *ef)
{
    bfelf64_off i = 0;

    const struct bfelf_rela *rela_table =
        BFRCAST(const struct bfelf_rela *, ef->file + ef->shdr_rela->sh_offset);

    for (i = 0; i < ef->shdr_rela->sh_size / sizeof(struct bfelf_rela); i++) {
        const struct bfelf_rela *rela = &(rela_table[i]);

        switch (BFELF_REL_TYPE(rela->r_info)) {
            case BFR_X86_64_RELATIVE:
            case BFR_AARCH64_RELATIVE: {
                bfelf64_addr * addr = BFRCAST(bfelf64_addr *, ef->exec + rela->r_offset);
                *addr = BFRCAST(bfelf64_addr, ef->virt + rela->r_addend);
                break;
            }

            default:
                ELF_ALERT("unsupported relocation type: %u\n", BFELF_REL_TYPE(rela->r_info));
                return BFFAILURE;
        }
    }

    return BFSUCCESS;
}

static inline status_t
bfelf_file_init(const void *file, uint64_t filesz, struct bfelf_file_t *ef)
{
    status_t ret = 0;

    if (file == nullptr) {
        ELF_ALERT("file == nullptr\n");
        return BFFAILURE;
    }

    if (filesz < sizeof(struct bfelf_ehdr)) {
        ELF_ALERT("filesz invalid\n");
        return BFFAILURE;
    }

    if (ef == nullptr) {
        ELF_ALERT("ef == nullptr\n");
        return BFFAILURE;
    }

    ef->file = BFSCAST(const uint8_t *, file);
    ef->ehdr = BFRCAST(const struct bfelf_ehdr *, file);

    ret = private_check_support(ef);
    if (ret != BFSUCCESS) {
        return ret;
    }

    if (ef->ehdr->e_phoff + (ef->ehdr->e_phnum * sizeof(struct bfelf_phdr)) > filesz) {
        ELF_ALERT("filesz invalid\n");
        return BFFAILURE;
    }

    if (ef->ehdr->e_shoff + (ef->ehdr->e_shnum * sizeof(struct bfelf_shdr)) > filesz) {
        ELF_ALERT("filesz invalid\n");
        return BFFAILURE;
    }

    ef->phdrtab =
        BFRCAST(const struct bfelf_phdr *, ef->file + ef->ehdr->e_phoff);
    ef->shdrtab =
        BFRCAST(const struct bfelf_shdr *, ef->file + ef->ehdr->e_shoff);
    ef->shstrtab =
        BFRCAST(const char *, ef->file + ef->shdrtab[ef->ehdr->e_shstrndx].sh_offset);

    ret = private_process_segments(ef);
    if (ret != BFSUCCESS) {
        return ret;
    }

    ret = private_process_sections(ef);
    if (ret != BFSUCCESS) {
        return ret;
    }

    ret = private_get_mem_size(ef);
    if (ret != BFSUCCESS) {
        return ret;
    }

    return BFSUCCESS;
}

static inline void *
bfelf_file_alloc(
    struct bfelf_file_t *ef, void *(*alloc_func)(size_t))
{
    if (ef == nullptr) {
        ELF_ALERT("ef == nullptr\n");
        return nullptr;
    }

    if (alloc_func == nullptr) {
        ELF_ALERT("alloc_func == nullptr\n");
        return nullptr;
    }

    return alloc_func(ef->size);
}

static inline status_t
bfelf_file_load(
    void *exec, bfelf64_addr virt, struct bfelf_file_t *ef, status_t (*mark_rx_func)(void *, size_t))
{
    const struct bfelf_phdr *phdr = nullptr;
    status_t ret = 0;

    if (exec == nullptr) {
        ELF_ALERT("exec == nullptr\n");
        return BFFAILURE;
    }

    if (virt == 0) {
        virt = BFRCAST(bfelf64_addr, exec);
    }

    if (ef == nullptr) {
        ELF_ALERT("ef == nullptr\n");
        return BFFAILURE;
    }

    ELF_INFO("Loading ELF file\n");

    ef->exec = BFSCAST(uint8_t *, exec);
    ef->virt = virt;

    uintptr_t entry_addr = ef->virt + ef->ehdr->e_entry;
    ef->entry = entry_addr;

    ELF_INFO("    zeroing execution buffer...");
    private_memset(exec, 0, ef->size);
    ELF_INFO("done\n");

    phdr = ef->pt_load_rx;
    ef->rx_size = BFALIGN(phdr->p_memsz, phdr->p_align);
    ef->rx_addr = ef->exec + phdr->p_paddr;
    const void * rx_segment_file_addr = ef->file + phdr->p_offset;
    ELF_INFO("    loading read/execute segment: 0x%x -> 0x%x...", rx_segment_file_addr, ef->rx_addr);
    private_memcpy(ef->rx_addr, rx_segment_file_addr, phdr->p_filesz);
    ELF_INFO("done\n");

    phdr = ef->pt_load_rw;
    ef->rw_size = BFALIGN(phdr->p_memsz, phdr->p_align);
    ef->rw_addr = ef->exec + phdr->p_paddr;
    const void * rw_segment_file_addr = ef->file + phdr->p_offset;
    ELF_INFO("    loading read/write segment: 0x%x -> 0x%x...", rw_segment_file_addr, ef->rw_addr);
    private_memcpy(ef->rw_addr, rw_segment_file_addr, phdr->p_filesz);
    ELF_INFO("done\n");

    if (mark_rx_func != nullptr) {
        ret = mark_rx_func((void *)ef->rx_addr, ef->rx_size);
        if (ret != BFSUCCESS) {
            return ret;
        }
    }

    ret = private_get_section_info(ef);
    if (ret != BFSUCCESS) {
        return ret;
    }

    ELF_INFO("    performing symbol relocations...");
    ret = private_relocate(ef);
    if (ret != BFSUCCESS) {
        return ret;
    }
    ELF_INFO("done\n");

    ELF_INFO("    ELF file loaded, entry point: 0x%x\n", ef->entry);
    return BFSUCCESS;
}

#ifdef __cplusplus
}
#endif

#pragma pack(pop)

#endif

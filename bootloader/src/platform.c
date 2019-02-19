/*
 * Bareflank Hypervisor
 * Copyright (C) 2018 Assured Information Security, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "microlib.h"
#include <bfplatform.h>

char * g_next_addr = (char *)0x8C000000; // TODO: Calculate from linker script

void *platform_alloc(uint64_t len)
{
    char * next_addr = g_next_addr;
    g_next_addr += len;
    return (void *)next_addr;
}

void *platform_alloc_rw(uint64_t len)
{
    void * mem = platform_alloc(len);
    return mem;
}

void *platform_alloc_rwe(uint64_t len)
{
    return platform_alloc(len);
}

void platform_free_rw(const void *addr, uint64_t len)
{
    return;
}

void platform_free_rwe(const void *addr, uint64_t len)
{
    return;
}

void *platform_virt_to_phys(void *virt)
{
    return virt;
}

void *platform_memset(void *ptr, char value, uint64_t num)
{
    BOOTLOADER_INFO("platform_memset");
    BOOTLOADER_INFO("ptr: 0x%08x", ptr);
    BOOTLOADER_INFO("val: 0x%08x", value);
    BOOTLOADER_INFO("num: 0x%08x", num);
    void * set_buffer = memset(ptr, value, num);
    BOOTLOADER_INFO("ptr after memset: 0x%08x", ptr);
    return set_buffer;
}

void *platform_memcpy(void *dst, const void *src, uint64_t num)
{
    return memcpy(dst, src, num);
}

void platform_start(void)
{ }

void platform_stop(void)
{ }

/**
 * Get Number of CPUs
 *
 * @return returns the total number of CPUs available to the driver.
 */
int64_t
platform_num_cpus(void)
{
    return 1;
}

/**
 * Set CPU affinity
 *
 * Changes the current core that the driver is running on.
 *
 * @param affinity the cpu number to change to
 * @return The affinity mask of the CPU before the change
 */
int64_t
platform_set_affinity(int64_t affinity)
{
    return 0;
}

void
platform_restore_affinity(int64_t affinity)
{
    platform_set_affinity(affinity);
}

int64_t platform_get_current_cpu_num(void)
{
    return 0;
}

void platform_restore_preemption(void)
{ }

int64_t
platform_populate_info(struct platform_info_t *info)
{
    if (info) {
        platform_memset(info, 0, sizeof(struct platform_info_t));
    }

    return BF_SUCCESS;
}

void
platform_unload_info(struct platform_info_t *info)
{
    if (info) {
        platform_memset(info, 0, sizeof(struct platform_info_t));
    }
}

int printf(const char *format, ...)
{
    (void)format;
    return 0;
}

int64_t
private_call_vmm(uintptr_t request, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3);

int64_t
platform_start_core(void)
{
    int64_t ret = -1;
    int64_t cpuid = -1;

    cpuid = platform_get_current_cpu_num();
    if (cpuid < 0) {
        return ret;
    }

    ret = private_call_vmm(BF_REQUEST_VMM_INIT, (uint64_t)cpuid, 0, 0);
    if (ret != BF_SUCCESS) {
        return ret;
    }

    platform_start();

    return BF_SUCCESS;
}


#include "launch_vmm.h"
#include "cache.h"
#include "microlib.h"
#include <libfdt.h>

/**
 * Ensures that a valid FDT/image is accessible for the system, performing any
 * steps necessary to make the image accessible, and validating the device tree.
 *
 * @return SUCCESS, or an FDT error code.
 */
int ensure_image_is_accessible(const void *image)
{
    int rc;

    // Depthcharge loads images into memory with the cache on, and doesn't
    // flush the relevant cache lines when it switches the cache off. As a
    // result, we'll need to flush the cache lines for it before we'll be able
    // to see the FDT.

    // We start by flushing our first cache line, which we assume is large
    // enough to provide the first two fields of the FDT: an 8-byte magic number,
    // and 8-byte size.
    __invalidate_cache_line(image);

    // Validate that we have a valid-appearing device tree. All images should
    // conform to the device tree standard, as they should be either Linux
    // device trees, or FIT images.
    rc = fdt_check_header(image);
    if(rc) {
        BOOTLOADER_ERROR("fdt_check_header failed: %d", rc);
        return rc;
    }

    // If we do, invalidate the remainder of its cache lines.
    __invalidate_cache_region(image, fdt_totalsize(image));

    return SUCCESS;
}

void load_device_tree(void * fdt)
{
    int rc;
    char * fdt_raw = fdt;

    BOOTLOADER_PRINT("\nFinding device tree...");
    rc = ensure_image_is_accessible(fdt);

    BOOTLOADER_PRINT("  flattened device tree resident at:     0x%08x", fdt);
    BOOTLOADER_PRINT("  flattened device tree is:              %s (%d)", rc == SUCCESS ? "valid" : "INVALID", rc);

    if(rc != SUCCESS) {
        BOOTLOADER_ERROR("Cannot continue without a valid device tree.");
        panic();
    }

    BOOTLOADER_PRINT("  flattened device tree size:            %d bytes", fdt_totalsize(fdt));
}

/**
 * Finds the chosen node in the Discharged FDT, which contains
 * e.g. the location of our final payload.
 */
int find_node(const void * image, const char * path)
{
    int node = fdt_path_offset(image, path);

    // If we weren't able to get the chosen node, return NULL.
    if (node < 0)
        BOOTLOADER_ERROR("Could not find path %s in subimage! (%d)", path, node);
    else
        BOOTLOADER_PRINT("  image node found at offset:            %d", node);

    return node;
}

/**
 * Gets a CPU-friendly representation of a memory table entry.
 *
 * @param memory_table_entry A four-byte FDT addray in <addr_h, addr_l, size_h, size_l> format.
 * @param out_addr Out argument. Recieves the bank's start address.
 * @param outsize Out argument. Recieves the bank's size
 */
static void _from_mem_table_entry(const uint32_t *memory_table_entry, uint64_t *out_addr, uint64_t *out_size)
{
    // Get the CPU-endian representations of each memory attribute...
    uint64_t cpu_addr_high = fdt32_to_cpu(memory_table_entry[0]);
    uint64_t cpu_addr_low  = fdt32_to_cpu(memory_table_entry[1]);
    uint64_t cpu_size_high = fdt32_to_cpu(memory_table_entry[2]);
    uint64_t cpu_size_low  = fdt32_to_cpu(memory_table_entry[3]);

    // ... and combine them into the requested format.
    *out_addr = (cpu_addr_high << 32ULL) | cpu_addr_low;
    *out_size = (cpu_size_high << 32ULL) | cpu_size_low;
}

/**
 * Gets a CPU-friendly representation of a memory table entry.
 *
 * @param memory_table_entry A four-byte FDT addray in <addr_h, addr_l, size_h, size_l> format.
 * @param out_addr Out argument. Recieves the bank's start address.
 * @param outsize Out argument. Recieves the bank's size
 */
static void _to_mem_table_entry(uint32_t *memory_table_entry, uint64_t addr, uint64_t size)
{
    // Get the CPU-endian representations of each memory attribute...
    uint64_t cpu_addr_high = addr >> 32ULL;
    uint64_t cpu_addr_low  = addr & 0xFFFFFFFFULL;
    uint64_t cpu_size_high = size >> 32ULL;
    uint64_t cpu_size_low  = size & 0xFFFFFFFFULL;

    // ... and combine them into the requested format.
    memory_table_entry[0] = cpu_to_fdt32(cpu_addr_high);
    memory_table_entry[1] = cpu_to_fdt32(cpu_addr_low);
    memory_table_entry[2] = cpu_to_fdt32(cpu_size_high);
    memory_table_entry[3] = cpu_to_fdt32(cpu_size_low);
}

/**
 * Copies a given memory table entry from a source table to the target table, excluding
 * the relevant region. May produce two entries, depending on the 
 */
static size_t copy_or_split_memory_table_entries(const uint32_t *source_entry, uint32_t *target_entry,
    uintptr_t exclude_start, uintptr_t exclude_end)
{
    size_t entries_generated = 0;
    uint64_t source_entry_addr, source_entry_size, source_entry_end;


    // Interpret the source entry.
    _from_mem_table_entry(source_entry, &source_entry_addr, &source_entry_size);
    source_entry_end = source_entry_addr + source_entry_size;

    // Case 1: Do we have a region before the exlcusion region? If so, include it.
    if (source_entry_addr < exclude_start) {

        // Determine where the new entry ends.
        uint64_t new_entry_end = min(source_entry_end, exclude_start);
        uint64_t new_entry_size = new_entry_end - source_entry_addr;

        // Generate the new entry, and add it to the entry table.
        _to_mem_table_entry(&target_entry[entries_generated * 4], source_entry_addr, new_entry_size);
        ++entries_generated;
    }

    // Case 2: Do we have a region after the exlcusion region? If so, include it.
    if (source_entry_end > exclude_end) {

        // Determine where the new entry ends.
        uint64_t new_entry_start = max(source_entry_addr, exclude_end);
        uint64_t new_entry_size = source_entry_end - new_entry_start;

        // Generate the new entry, and add it to the entry table.
        _to_mem_table_entry(&target_entry[entries_generated * 4], new_entry_start, new_entry_size);
        ++entries_generated;
    }

    // Return the number of entries we've generated.
    return entries_generated;
}

/**
 * Helper function that prints out a memory table entry.
 *
 * @param memory_table A memory table, as extracted from an FDT.
 * @param entries The number of entries in the memory table.
 */
static void print_memory_table(uint32_t *memory_table, size_t entries)
{
    uint32_t *current_entry = memory_table;

    // Iterate over each entry in the table.
    while(entries--) {
        uint64_t addr, size;

        // Print the memory table entry.
        _from_mem_table_entry(current_entry, &addr, &size);

        if ((addr == 0) && (size == 0)) {
          BOOTLOADER_PRINT("  end of table");
        } else {
          BOOTLOADER_PRINT("  memory bank at 0x%08x, size 0x%08x\n", addr, size);
        }

        // Move to the next memory table entry.
        current_entry += 4;
    }
}

void * location_from_devicetree(uint32_t metalocation)
{
    return (void *)(uintptr_t)fdt32_to_cpu(metalocation);
}

int get_subcomponent_information(const void *image, const char *path,
    void **out_load_location, void const**out_data_location, int *out_size,
    int * node_offset)
{
    const uint32_t const *load_information_location;
    const void *data_location;
    void *load_location;

    int node, load_information_size, size;

    // Before running, check all of our pointers for validity.
    if(!out_data_location || !out_load_location || !out_size)
        return -FDT_ERR_BADVALUE;

    // Find the FIT node that describes the image.
    node = find_node(image, path);
    if(node < 0)
        return node;

    // Locate the node that specifies where we should load this image from.
    data_location = fdt_getprop(image, node, "data", &size);
    if(size <= 0) {
        BOOTLOADER_ERROR("ERROR: Couldn't find the data to load! (%d)", size);
        return size;
    }

    // Print out statistics regarding the loaded image...
    BOOTLOADER_PRINT("  loading image from:                    0x%08x", data_location);
    BOOTLOADER_PRINT("  loading a total of:                    %d bytes", size);

    // Locate the FIT node that specifies where we should load this image component to.
    load_information_location = fdt_getprop(image, node, "load", &load_information_size);
    if(load_information_size <= 0) {
        BOOTLOADER_ERROR("Couldn't determine where to load to! (%d)", load_information_size);
        return load_information_size;
    }

    // Retrieve the load location.
    load_location = location_from_devicetree(*load_information_location);
    BOOTLOADER_PRINT("  loading image to location:             0x%08x", load_location);
    BOOTLOADER_PRINT("  image will end at address:             0x%08x", load_location + size);

    // Set our out arguments, and return success.
    *out_load_location = load_location;
    *out_data_location = data_location;
    *out_size = size;

    // If a node argument was provided, set the active node for further
    // processing.
    if(node_offset)
      *node_offset = node;

    return SUCCESS;
}

void * load_image_component(const void *image, const char *path, int *out_size)
{
    const void *data_location;
    void *load_location;
    int size, rc;

    // Get the information that describe where our information is located...
    rc = get_subcomponent_information(image, path, &load_location,
        &data_location, &size, NULL);

    if(rc != SUCCESS)
        return NULL;

    // We're not using the cache, but Depthcharge was before us.
    // To ensure that our next stage sees the proper memory, we'll have to
    // make sure that there are no data cache entries for the regions we're
    // about to touch. As there's no way to invalidate without cleaning via
    // virtual address (i.e. all of the evicted cache lines will be written
    // back), it's important that this runs before memmove.
    __invalidate_cache_region(load_location, size);

    // Trivial load: copy the gathered information to its final location.
    memmove(load_location, data_location, size);

    // ... and update our size out argument, if provided.
    if(out_size)
      *out_size = size;

    return load_location;
}

void * load_image_component_verbosely(const void * image,
    const char * path, const char * description, int * size)
{
    void * component;

    BOOTLOADER_PRINT("\nLoading %s image...", description);
    component = load_image_component(image, path, size);
    if(!component) {
        BOOTLOADER_ERROR("Failed to load a required image!");
        panic();
    }

    return component;
}


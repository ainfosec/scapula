#ifndef BOOTLOADER_LAUNCH_VMM_H
#define BOOTLOADER_LAUNCH_VMM_H

int ensure_image_is_accessible(const void *image);
void load_device_tree(void *fdt);
void * load_image_component_verbosely(const void * image,
    const char * path, const char * description, int * size);

#endif

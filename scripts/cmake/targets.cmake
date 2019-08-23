add_custom_target(info)

add_custom_target_category("Scapula Bootloader")

add_custom_target(flash
    COMMAND mount ${FLASH_DEV} ${FLASH_MOUNT}
    COMMAND cp ${SCAPULA_AARCH64_INSTALL_PREFIX}/boot/bootloader.bin ${FLASH_MOUNT}/${FLASH_PATH}
    COMMAND sync
    COMMAND umount ${FLASH_MOUNT}
    USES_TERMINAL
)

add_custom_target_info(
    TARGET flash
    COMMENT "Mount ${FLASH_DEV} to ${FLASH_MOUNT}, install bootloader.bin to ${FLASH_MOUNT}/${FLASH_PATH}"
)

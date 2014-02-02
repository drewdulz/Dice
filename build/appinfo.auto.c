#include "pebble_app_info.h"
#include "src/resource_ids.auto.h"

const PebbleAppInfo __pbl_app_info __attribute__ ((section (".pbl_header"))) = {
  .header = "PBLAPP",
  .struct_version = { APP_INFO_CURRENT_STRUCT_VERSION_MAJOR, APP_INFO_CURRENT_STRUCT_VERSION_MINOR },
  .sdk_version = { APP_INFO_CURRENT_SDK_VERSION_MAJOR, APP_INFO_CURRENT_SDK_VERSION_MINOR },
  .app_version = { 0, 1 },
  .load_size = 0xb6b6,
  .offset = 0xb6b6b6b6,
  .crc = 0xb6b6b6b6,
  .name = "Dice",
  .company = "Drewlusions",
  .icon_resource_id = RESOURCE_ID_IMAGE_diceIcon,
  .sym_table_addr = 0xA7A7A7A7,
  .flags = 0,
  .num_reloc_entries = 0xdeadcafe,
  .uuid = { 0x70, 0x03, 0x7E, 0xDD, 0x60, 0x64, 0x4E, 0x5C, 0x87, 0xEB, 0x5C, 0x6C, 0xF3, 0x90, 0xA1, 0x0F },
  .virtual_size = 0xb6b6
};


VULKAN_LOADER_ERROR_BIT          equ 8       ; The numerical value of the enum value 'VULKAN_LOADER_ERROR_BIT'
PTR_SIZE                         equ 8       ; The size of a pointer
CHAR_PTR_SIZE                    equ 8       ; The size of a 'const char *' struct
FUNCTION_OFFSET_INSTANCE         equ 2864    ; The offset of 'phys_dev_ext_disp_functions' within a 'loader_instance' struct
PHYS_DEV_OFFSET_INST_DISPATCH    equ 728     ; The offset of 'phys_dev_ext' within in 'loader_instance_dispatch_table' struct
PHYS_DEV_OFFSET_PHYS_DEV_TRAMP   equ 24      ; The offset of 'phys_dev' within a 'loader_physical_device_tramp' struct
ICD_TERM_OFFSET_PHYS_DEV_TERM    equ 8       ; The offset of 'this_icd_term' within a 'loader_physical_device_term' struct
PHYS_DEV_OFFSET_PHYS_DEV_TERM    equ 24      ; The offset of 'phys_dev' within a 'loader_physical_device_term' struct
INSTANCE_OFFSET_ICD_TERM         equ 8       ; The offset of 'this_instance' within a 'loader_icd_term' struct
DISPATCH_OFFSET_ICD_TERM         equ 752     ; The offset of 'phys_dev_ext' within a 'loader_icd_term' struct
EXT_OFFSET_DEVICE_DISPATCH       equ 4448    ; The offset of 'ext_dispatch' within a 'loader_dev_dispatch_table' struct

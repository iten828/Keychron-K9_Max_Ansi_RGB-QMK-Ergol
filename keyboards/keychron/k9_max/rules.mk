include keyboards/keychron/common/wireless/wireless.mk
include keyboards/keychron/common/keychron_common.mk

VPATH += $(TOP_DIR)/keyboards/keychron
OS_DETECTION_ENABLE = yes
DEFERRED_EXEC_ENABLE = yes
UNICODE_ENABLE = yes
SRC += lib/emul_azerty/emul_azerty.c
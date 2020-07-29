#
# Copyright (C) 2006-2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

ARCH:=arm
BOARD:=imx28
BOARDNAME:=M2M box
FEATURES:=squashfs

LINUX_VERSION:=2.6.35.3

KEYIDX:=$(if $(findstring devel-priv.pem, $(CONFIG_KERNEL_SIGN_KEYPATH)), 999, 998)

# Image attributes
ATTR_HARD_PERS:=$(if $(CONFIG_ATTR_HARD_PERSISTENCE), 1, 0)
ATTR_SOFT_PERS:=$(if $(CONFIG_ATTR_SOFT_PERSISTENCE), 1, 0)
ATTR:=0 # Attribute combines all attribute options.

# Combine the attributes. Don't change this.
ATTR:=$(shell echo $$(($(ATTR) | ($(ATTR_HARD_PERS) << 0))))
ATTR:=$(shell echo $$(($(ATTR) | ($(ATTR_SOFT_PERS) << 1))))

define Image/Prepare
endef

define Image/BuildKernel
endef

define Image/Build
endef

define Image/mkfs/squashfs
	mkdir -p $(BIN_DIR)
	$(MKSQUASHFS_CMD) $(TARGET_DIR) $(BIN_DIR)/root.squashfs -nopad -noappend -root-owned -comp gzip -key $(KEYIDX) ../../../$(CONFIG_KERNEL_SIGN_KEYPATH) -desc '$(shell (echo -n 'sdk-'; date +'%Y%m%d%H%M%S'))' -attr $(shell printf "0x%x" $(ATTR)) $(shell [ "$(CONFIG_ATTR_MIN_FW_VERSION)" != "" ] && echo "-min-version $(CONFIG_ATTR_MIN_FW_VERSION)") $(shell [ "$(CONFIG_ATTR_MAX_FW_VERSION)" != "" ] && echo "-max-version $(CONFIG_ATTR_MAX_FW_VERSION)")
	$(MKSQUASHFS_CMD) $(TOPDIR)/config $(BIN_DIR)/config.squashfs -nopad -noappend -root-owned -comp gzip -key $(KEYIDX) ../../../$(CONFIG_KERNEL_SIGN_KEYPATH) -desc '$(shell (echo -n 'sdk-'; date +'%Y%m%d%H%M%S'))' -attr  $(shell printf "0x%x" $(ATTR)) $(shell [ "$(CONFIG_ATTR_MIN_FW_VERSION)" != "" ] && echo "-min-version $(CONFIG_ATTR_MIN_FW_VERSION)") $(shell [ "$(CONFIG_ATTR_MAX_FW_VERSION)" != "" ] && echo "-max-version $(CONFIG_ATTR_MAX_FW_VERSION)")
	mkdir -p $(BIN_DIR)/tmp
	cp $(BIN_DIR)/root.squashfs $(BIN_DIR)/config.squashfs $(BIN_DIR)/tmp

	if [[ $(KEYIDX) -eq 998 ]]; then \
		cp ../../../keys/customer_key $(BIN_DIR)/tmp; \
	else \
		touch $(BIN_DIR)/tmp/customer_key; \
		echo "empty" > $(BIN_DIR)/tmp/customer_key; \
	fi
	sync
	echo "colrmcheck (part of bsdmainutils)" | colrm 1>/dev/null
	cp $(SCRIPT_DIR)/bundle_all.cfg $(BIN_DIR)/tmp/bundle.cfg
	sed -i "s/(sha_prov)/`sha256sum $(BIN_DIR)/root.squashfs 2> /dev/null | colrm 65`/" $(BIN_DIR)/tmp/bundle.cfg
	sed -i "s/(sha_sconf)/`sha256sum $(BIN_DIR)/config.squashfs 2> /dev/null | colrm 65`/" $(BIN_DIR)/tmp/bundle.cfg
	sed -i "s/(sha_customerkey)/`sha256sum $(BIN_DIR)/tmp/customer_key 2> /dev/null | colrm 65`/" $(BIN_DIR)/tmp/bundle.cfg
	sed -i "s/(release_name)/$(shell (echo -n 'sdk-'; date +'%Y%m%d%H%M%S'))/" $(BIN_DIR)/tmp/bundle.cfg
	zip -j $(BIN_DIR)/bundle-$(shell (echo -n 'sdk-'; date +'%Y%m%d%H%M%S')).zip $(BIN_DIR)/tmp/bundle.cfg $(BIN_DIR)/tmp/root.squashfs $(BIN_DIR)/tmp/config.squashfs $(BIN_DIR)/tmp/customer_key

	rm -rf $(BIN_DIR)/tmp
endef

$(eval $(call BuildImage))

dumpinfo:



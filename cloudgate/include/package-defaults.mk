# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Package/Default
  CONFIGFILE:=
  SECTION:=opt
  CATEGORY:=Extra packages
  DEPENDS:=
  PROVIDES:=
  EXTRA_DEPENDS:=
  MAINTAINER:=OpenWrt Developers Team <openwrt-devel@openwrt.org>
  SOURCE:=$(patsubst $(TOPDIR)/%,%,$(CURDIR))
  ifneq ($(PKG_VERSION),)
    ifneq ($(PKG_RELEASE),)
      VERSION:=$(PKG_VERSION)-$(PKG_RELEASE)
    else
      VERSION:=$(PKG_VERSION)
    endif
  else
    VERSION:=$(PKG_RELEASE)
  endif
  ifneq ($(CONFIG_TARGET_adm5120),y)
    PKGARCH:=$(BOARD)
  else
    PKGARCH:=$(BOARD)_$(ARCH)
  endif
  PRIORITY:=optional
  DEFAULT:=
  MENU:=
  SUBMENU:=
  SUBMENUDEP:=
  TITLE:=
  KCONFIG:=
  BUILDONLY:=
  URL:=
  VARIANT:=
endef

Build/Patch:=$(Build/Patch/Default)
ifneq ($(strip $(PKG_UNPACK)),)
  define Build/Prepare/Default
	$(SH_FUNC) $(PKG_UNPACK)
	$(Build/Patch)
  endef
endif

Build/UnitPatch:=$(Build/UnitPatch/Default)
define Build/UnitPrepare/Default
	$(SH_FUNC) $(UNIT_UNPACK)
	$(Build/UnitPatch)
	-$(CP) src/* $(UNIT_BUILD_DIR)/
endef

CONFIGURE_PREFIX:=/usr
CONFIGURE_ARGS = \
		--target=$(GNU_TARGET_NAME) \
		--host=$(GNU_TARGET_NAME) \
		--build=$(GNU_HOST_NAME) \
		--program-prefix="" \
		--program-suffix="" \
		--prefix=$(CONFIGURE_PREFIX) \
		--exec-prefix=$(CONFIGURE_PREFIX) \
		--bindir=$(CONFIGURE_PREFIX)/bin \
		--sbindir=$(CONFIGURE_PREFIX)/sbin \
		--libexecdir=$(CONFIGURE_PREFIX)/lib \
		--sysconfdir=/etc \
		--datadir=$(CONFIGURE_PREFIX)/share \
		--localstatedir=/var \
		--mandir=$(CONFIGURE_PREFIX)/man \
		--infodir=$(CONFIGURE_PREFIX)/info \
		$(DISABLE_NLS) \
		$(DISABLE_LARGEFILE) \
		$(DISABLE_IPV6)

CONFIGURE_UNIT_ARGS = \
		--build=$(GNU_HOST_NAME) \
		--program-prefix="" \
		--program-suffix="" \
		--prefix=$(CONFIGURE_PREFIX) \
		--exec-prefix=$(CONFIGURE_PREFIX) \
		--bindir=$(CONFIGURE_PREFIX)/bin \
		--sbindir=$(CONFIGURE_PREFIX)/sbin \
		--libexecdir=$(CONFIGURE_PREFIX)/lib \
		--sysconfdir=/etc \
		--datadir=$(CONFIGURE_PREFIX)/share \
		--localstatedir=/var \
		--mandir=$(CONFIGURE_PREFIX)/man \
		--infodir=$(CONFIGURE_PREFIX)/info \
		$(DISABLE_NLS) \
		$(DISABLE_LARGEFILE) \
		$(DISABLE_IPV6)

CONFIGURE_VARS = \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS)" \
		CXXFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS)" \
		CPPFLAGS="$(TARGET_CPPFLAGS) $(EXTRA_CPPFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(EXTRA_LDFLAGS)" \

CONFIGURE_PATH = .
CONFIGURE_CMD = ./configure

replace_script=$(FIND) $(1) -name $(2) | $(XARGS) chmod u+w; $(FIND) $(1) -name $(2) | $(XARGS) -n1 cp $(SCRIPT_DIR)/$(2);

define Build/Configure/Default
	(cd $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)/$(strip $(3)); \
	if [ -x $(CONFIGURE_CMD) ]; then \
		$(call replace_script,$(PKG_BUILD_DIR)/$(3),config.guess) \
		$(call replace_script,$(PKG_BUILD_DIR)/$(3),config.sub) \
		$(CONFIGURE_VARS) \
		$(2) \
		$(CONFIGURE_CMD) \
		$(CONFIGURE_ARGS) \
		$(1); \
	fi; \
	)
endef

# 2 hacks, use the host's M4 binary and don't use configure's common settings file that is there for the target
define Build/UnitConfigure/Default
	(cd $(UNIT_BUILD_DIR)/$(CONFIGURE_PATH)/$(strip $(3)); \
	if [ -x $(CONFIGURE_CMD) ]; then \
		unset CONFIG_SITE; \
		$(2) \
		$(CONFIGURE_CMD) \
		$(CONFIGURE_UNIT_ARGS) \
		$(1); \
	fi; \
	)
endef

MAKE_VARS = \
	CFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS) $(TARGET_CPPFLAGS) $(EXTRA_CPPFLAGS)" \
	CXXFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS) $(TARGET_CPPFLAGS) $(EXTRA_CPPFLAGS)" \
	LDFLAGS="$(TARGET_LDFLAGS) $(EXTRA_LDFLAGS)"

MAKE_FLAGS = \
	$(TARGET_CONFIGURE_OPTS) \
	CROSS="$(TARGET_CROSS)" \
	ARCH="$(ARCH)"

MAKE_INSTALL_FLAGS = \
	$(MAKE_FLAGS) \
	DESTDIR="$(PKG_INSTALL_DIR)"

MAKE_PATH = .

define Build/Compile/Default
	$(MAKE_VARS) \
	$(MAKE) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) \
		$(MAKE_FLAGS) \
		$(1);
endef

# use unit's staging dir before target's staging dir
define Build/Unit/Default
	$(MAKE) -C $(UNIT_BUILD_DIR)/$(MAKE_PATH) CFLAGS="-fpic -idirafter $(STAGING_DIR_UNIT)/usr/include -idirafter $(STAGING_DIR)/usr/include -idirafter $(STAGING_DIR)/include" LDFLAGS="-L$(STAGING_DIR_UNIT)" \
		$(1);
endef

# use unit's staging dir before target's staging dir
define Build/UnitInstall/Default
	$(MAKE) -C $(UNIT_BUILD_DIR)/$(MAKE_PATH) CFLAGS="-fpic -idirafter $(STAGING_DIR_UNIT)/usr/include -idirafter $(STAGING_DIR)/usr/include -idirafter $(STAGING_DIR)/include" LDFLAGS="-L$(STAGING_DIR_UNIT)" \
	DESTDIR="$(UNIT_BUILD_DIR)/ipkg-install" \
	install
endef

define Build/UnitRun/Default
	$(if $(RUN_UNIT),(cd $(STAGING_DIR_UNIT); PATH=.:$(PATH) CK_DEFAULT_TIMEOUT=90 LD_LIBRARY_PATH=. CONFIG_DIR=$(STAGING_DIR_UNIT)/config ./$(1)),)
endef

define Build/Install/Default
	$(MAKE_VARS) \
	$(MAKE) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) \
		$(MAKE_INSTALL_FLAGS) \
		$(if $(1), $(1), install);
endef

define Build/Dist/Default
	$(call Build/Compile/Default, DESTDIR="$(PKG_BUILD_DIR)/tmp" CC="$(TARGET_CC)" dist)
endef

define Build/DistCheck/Default
	$(call Build/Compile/Default, DESTDIR="$(PKG_BUILD_DIR)/tmp" CC="$(TARGET_CC)" distcheck)
endef

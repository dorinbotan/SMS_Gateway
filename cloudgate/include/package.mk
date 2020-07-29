#
# Copyright (C) 2006-2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

all: $(if $(DUMP),dumpinfo,compile)

PKG_BUILD_DIR ?= $(BUILD_DIR)/$(PKG_NAME)$(if $(PKG_VERSION),-$(PKG_VERSION))
UNIT_BUILD_DIR ?= $(BUILD_DIR_UNIT)/$(PKG_NAME)$(if $(PKG_VERSION),-$(PKG_VERSION))
PKG_INSTALL_DIR ?= $(PKG_BUILD_DIR)/ipkg-install
PKG_MD5SUM ?= unknown

include $(INCLUDE_DIR)/prereq.mk
include $(INCLUDE_DIR)/host.mk
include $(INCLUDE_DIR)/unpack.mk
include $(INCLUDE_DIR)/depends.mk

STAMP_PREPARED=$(PKG_BUILD_DIR)/.prepared$(if $(QUILT)$(DUMP),,_$(shell $(call find_md5,${CURDIR} $(PKG_FILE_DEPENDS),)))
STAMP_CONFIGURED:=$(PKG_BUILD_DIR)/.configured$(if $(DUMP),,_$(call confvar,$(PKG_CONFIG_DEPENDS)))
STAMP_BUILT:=$(PKG_BUILD_DIR)/.built
STAMP_UNIT=$(UNIT_BUILD_DIR)/.unit
STAMP_UNIT_PREPARED=$(UNIT_BUILD_DIR)/.unitprepared$(if $(QUILT)$(DUMP),,_$(shell $(call find_md5,${CURDIR} $(PKG_FILE_DEPENDS),)))
STAMP_UNIT_CONFIGURED=$(UNIT_BUILD_DIR)/.configured$(if $(DUMP),,_$(call confvar,$(PKG_CONFIG_DEPENDS)))
STAMP_INSTALLED:=$(STAGING_DIR)/stamp/.$(PKG_NAME)_installed
STAMP_INSTALL_DEV:=$(STAGING_DIR)/stamp/.$(PKG_NAME)_install_dev
STARTMV=$(STAGING_DIR_UNIT)/startmv.sh

include $(INCLUDE_DIR)/download.mk
include $(INCLUDE_DIR)/quilt.mk
include $(INCLUDE_DIR)/package-defaults.mk
include $(INCLUDE_DIR)/package-dumpinfo.mk
include $(INCLUDE_DIR)/package-ipkg.mk
include $(INCLUDE_DIR)/package-bin.mk
include $(INCLUDE_DIR)/autotools.mk

override MAKEFLAGS=
CONFIG_SITE:=$(INCLUDE_DIR)/site/$(REAL_GNU_TARGET_NAME)
ifneq ($(CONFIG_LINUX_2_4),)
  CONFIG_SITE:=$(subst linux-,linux2.4-,$(CONFIG_SITE))
endif
CUR_MAKEFILE:=$(filter-out Makefile,$(firstword $(MAKEFILE_LIST)))
SUBMAKE:=$(NO_TRACE_MAKE) $(if $(CUR_MAKEFILE),-f $(CUR_MAKEFILE))

ifeq ($(DUMP)$(filter prereq clean refresh update,$(MAKECMDGOALS)),)
  ifneq ($(if $(QUILT),,$(CONFIG_AUTOREBUILD)),)
    define Build/Autoclean
      $(PKG_BUILD_DIR)/.dep_files: $(STAMP_PREPARED)
      $(call rdep,${CURDIR} $(PKG_FILE_DEPENDS),$(STAMP_PREPARED),$(PKG_BUILD_DIR)/.dep_files,-x "*/.dep_*")
      $(if $(filter prepare,$(MAKECMDGOALS)),,$(call rdep,$(PKG_BUILD_DIR),$(STAMP_BUILT),,-x "*/.dep_*" -x "*/ipkg*"))
    endef
  endif
endif

#ifeq ($(DUMP)$(filter prereq clean refresh update,$(MAKECMDGOALS)),)
#  ifneq ($(if $(QUILT),,$(CONFIG_AUTOREBUILD)),)
    define Build/UnitAutoclean
      $(UNIT_BUILD_DIR)/.dep_files: $(STAMP_UNIT_PREPARED)
      $(call rdep,${CURDIR} $(PKG_FILE_DEPENDS),$(STAMP_UNIT_PREPARED),$(UNIT_BUILD_DIR)/.dep_files,-x "*/.dep_*")
      $(if $(filter prepare,$(MAKECMDGOALS)),,$(call rdep,$(UNIT_BUILD_DIR),$(STAMP_UNIT),,-x "*/.dep_*" -x "*/ipkg*"))
    endef
#  endif
#endif

ifeq ($(CONFIG_$(PKG_NAME)_USE_CUSTOM_SOURCE_DIR),y)
# disable load stage
PKG_SOURCE_URL:=
# add hook to install a link to customer source path of dedicated package
Hooks/Prepare/Pre += prepare_custom_source_directory
# define empty default action
define Build/Prepare/Default
	@: 
endef
endif

define Download/default
  FILE:=$(PKG_SOURCE)
  URL:=$(PKG_SOURCE_URL)
  SUBDIR:=$(PKG_SOURCE_SUBDIR)
  PROTO:=$(PKG_SOURCE_PROTO)
  $(if $(PKG_SOURCE_MIRROR),MIRROR:=$(filter 1,$(PKG_MIRROR)))
  VERSION:=$(PKG_SOURCE_VERSION)
  MD5SUM:=$(PKG_MD5SUM)
endef

define sep

endef

define Build/Exports/Default
  $(1) : export ACLOCAL_INCLUDE=$$(foreach p,$$(wildcard $$(STAGING_DIR)/usr/share/aclocal $$(STAGING_DIR)/usr/share/aclocal-* $$(STAGING_DIR)/host/share/aclocal $$(STAGING_DIR)/host/share/aclocal-*),-I $$(p))
  $(1) : export STAGING_PREFIX=$$(STAGING_DIR)/usr
  $(1) : export PATH=$$(TARGET_PATH_PKG)
  $(1) : export CONFIG_SITE:=$$(CONFIG_SITE)
  $(1) : export PKG_CONFIG_PATH=$$(STAGING_DIR)/usr/lib/pkgconfig
  $(1) : export PKG_CONFIG_LIBDIR=$$(STAGING_DIR)/usr/lib/pkgconfig
  $(1) : export CCACHE_DIR:=$(STAGING_DIR)/ccache
endef
Build/Exports=$(Build/Exports/Default)

define Build/DefaultTargets
  $(if $(QUILT),$(Build/Quilt))
  $(if $(strip $(PKG_SOURCE_URL)),$(call Download,default))
  $(call Build/Autoclean)

  $(STAMP_PREPARED) : export PATH=$$(TARGET_PATH_PKG)
  $(STAMP_PREPARED):
	@-rm -rf $(PKG_BUILD_DIR)
	@mkdir -p $(PKG_BUILD_DIR)
	$(foreach hook,$(Hooks/Prepare/Pre),$(call $(hook))$(sep))
	$(Build/Prepare)
	$(foreach hook,$(Hooks/Prepare/Post),$(call $(hook))$(sep))
	touch $$@

  $(call Build/Exports,$(STAMP_CONFIGURED))
  $(STAMP_CONFIGURED): $(STAMP_PREPARED)
	$(foreach hook,$(Hooks/Configure/Pre),$(call $(hook))$(sep))
	$(Build/Configure)
	$(foreach hook,$(Hooks/Configure/Post),$(call $(hook))$(sep))
	touch $$@

  $(call Build/Exports,$(STAMP_BUILT))
  $(STAMP_BUILT): $(STAMP_CONFIGURED)
	$(foreach hook,$(Hooks/Compile/Pre),$(call $(hook))$(sep))
	$(Build/Compile)
	$(foreach hook,$(Hooks/Compile/Post),$(call $(hook))$(sep))
	$(Build/Install)
	$(foreach hook,$(Hooks/Install/Post),$(call $(hook))$(sep))
	touch $$@

  $(STAMP_INSTALLED) : export PATH=$$(TARGET_PATH_PKG)
  $(STAMP_INSTALLED): $(STAMP_BUILT)
	$(SUBMAKE) -j1 clean-staging
	rm -rf $(TMP_DIR)/stage-$(PKG_NAME)
	mkdir -p $(TMP_DIR)/stage-$(PKG_NAME)/host $(STAGING_DIR)/packages $(STAGING_DIR_HOST)/packages
	$(foreach hook,$(Hooks/InstallDev/Pre),\
		$(call $(hook),$(TMP_DIR)/stage-$(PKG_NAME),$(TMP_DIR)/stage-$(PKG_NAME)/host)$(sep)\
	)
	$(call Build/InstallDev,$(TMP_DIR)/stage-$(PKG_NAME),$(TMP_DIR)/stage-$(PKG_NAME)/host)
	$(foreach hook,$(Hooks/InstallDev/Post),\
		$(call $(hook),$(TMP_DIR)/stage-$(PKG_NAME),$(TMP_DIR)/stage-$(PKG_NAME)/host)$(sep)\
	)
	if [ -d $(TMP_DIR)/stage-$(PKG_NAME) ]; then \
		(cd $(TMP_DIR)/stage-$(PKG_NAME); find ./ > $(STAGING_DIR)/packages/$(PKG_NAME).list); \
		mkdir -p $(STAGING_DIR)/host; \
		mkdir -p $(STAGING_DIR)/usr/include; \
		$(CP) $(TMP_DIR)/stage-$(PKG_NAME)/* $(STAGING_DIR)/; \
	fi
	rm -rf $(TMP_DIR)/stage-$(PKG_NAME)
	touch $$@

  ifdef Build/InstallDev
    compile: $(STAMP_INSTALLED)
  endif

  define Build/DefaultTargets
  endef

  prepare: $(STAMP_PREPARED)
  configure: $(STAMP_CONFIGURED)
  dist: $(STAMP_CONFIGURED)
  distcheck: $(STAMP_CONFIGURED)
endef

define Build/DefaultUnitTargets
  ifdef Build/Unit
  $(if $(QUILT),$(Build/Quilt))
  $(if $(strip $(PKG_SOURCE_URL)),$(call Download,default))
  $(call Build/UnitAutoclean)

  $(STARTMV):
	@mkdir -p $(STAGING_DIR_UNIT)
	cp $(TOPDIR)/tools/unittests/startmv.sh $(STAGING_DIR_UNIT)

  $(call Build/Exports,$(STAMP_UNIT_PREPARED))
  $(STAMP_UNIT_PREPARED): $(STARTMV)
	@-rm -rf $(UNIT_BUILD_DIR)
	@mkdir -p $(UNIT_BUILD_DIR)
	$(Build/UnitPrepare)
	touch $$@

  $(call Build/Exports,$(STAMP_UNIT_CONFIGURED))
  $(STAMP_UNIT_CONFIGURED): $(STAMP_UNIT_PREPARED)
	$(Build/UnitConfigure)
	touch $$@

  $(call Build/Exports,$(STAMP_UNIT))
  $(STAMP_UNIT): $(STAMP_UNIT_CONFIGURED)
	$(Build/Unit)
	touch $$@

  $(STAMP_INSTALL_DEV) : export PATH=$$(TARGET_PATH_PKG)
  $(STAMP_INSTALL_DEV): $(STAMP_UNIT)
	rm -rf $(TMP_DIR)/stage-$(PKG_NAME)
	mkdir -p $(TMP_DIR)/stage-$(PKG_NAME)/host $(STAGING_DIR)/packages $(STAGING_DIR_HOST)/packages
	$(foreach hook,$(Hooks/InstallDev/Pre),\
		$(call $(hook),$(TMP_DIR)/stage-$(PKG_NAME),$(TMP_DIR)/stage-$(PKG_NAME)/host)$(sep)\
	)
	$(eval PKG_BUILD_DIR := $(UNIT_BUILD_DIR))
	$(eval PKG_INSTALL_DIR := $(UNIT_BUILD_DIR)/ipkg-install)
	$(call Build/InstallDev,$(TMP_DIR)/stage-$(PKG_NAME),$(TMP_DIR)/stage-$(PKG_NAME)/host)
	$(foreach hook,$(Hooks/InstallDev/Post),\
		$(call $(hook),$(TMP_DIR)/stage-$(PKG_NAME),$(TMP_DIR)/stage-$(PKG_NAME)/host)$(sep)\
	)
	$(eval PKG_BUILD_DIR := $(PKG_BUILD_DIR_ORIG))
	$(eval PKG_INSTALL_DIR := $(PKG_INSTALL_DIR_ORIG))
	if [ -d $(TMP_DIR)/stage-$(PKG_NAME)/usr/include ]; then \
		cd $(TMP_DIR)/stage-$(PKG_NAME); \
		mkdir -p $(STAGING_DIR_UNIT)/usr/include; \
		$(CP) $(TMP_DIR)/stage-$(PKG_NAME)/usr/include/* $(STAGING_DIR_UNIT)/usr/include; \
	fi
	rm -rf $(TMP_DIR)/stage-$(PKG_NAME)
	touch $$@

  ifdef Build/InstallDev
    unit: $(STAMP_INSTALL_DEV)
  else
    unit: $(STAMP_UNIT_PREPARED) $(STAMP_UNIT)
  endif

  define Build/DefaultUnitTargets
  endef

  unitprepare: $(STAMP_UNIT_PREPARED)
  unitconfigure: $(STAMP_UNIT_CONFIGURED)
  endif
endef

define Build/IncludeOverlay
  $(eval -include $(wildcard $(TOPDIR)/overlay/*/$(PKG_NAME).mk))
  define Build/IncludeOverlay
  endef
endef

define BuildPackage
  $(Build/IncludeOverlay)
  $(eval $(Package/Default))
  $(eval $(Package/$(1)))

ifdef DESCRIPTION
$$(error DESCRIPTION:= is obsolete, use Package/PKG_NAME/description)
endif

ifndef Package/$(1)/description
define Package/$(1)/description
	$(TITLE)
endef
endif

  $(foreach FIELD, TITLE CATEGORY PRIORITY SECTION VERSION,
    ifeq ($($(FIELD)),)
      $$(error Package/$(1) is missing the $(FIELD) field)
    endif
  )

  $(call shexport,Package/$(1)/description)
  $(call shexport,Package/$(1)/config)

  $(if $(DUMP), \
    $(Dumpinfo/Package), \
    $(foreach target, \
      $(if $(Package/$(1)/targets),$(Package/$(1)/targets), \
        $(if $(PKG_TARGETS),$(PKG_TARGETS), ipkg) \
      ), $(BuildTarget/$(target)) \
    ) \
  )
  $(eval PKG_BUILD_DIR_ORIG ?= $(PKG_BUILD_DIR))
  $(eval PKG_INSTALL_DIR_ORIG ?= $(PKG_INSTALL_DIR))
  $(if $(PKG_HOST_ONLY)$(DUMP),,$(call Build/DefaultTargets,$(1)))
  $(if $(PKG_HOST_ONLY)$(DUMP),,$(call Build/DefaultUnitTargets,$(1)))
endef

define pkg_install_files
	$(foreach install_file,$(1),$(INSTALL_DIR) $(3)/`dirname $(install_file)`; $(INSTALL_DATA) $(2)/$(install_file) $(3)/`dirname $(install_file)`;)
endef

define pkg_install_bin
	$(foreach install_apps,$(1),$(INSTALL_DIR) $(3)/`dirname $(install_apps)`; $(INSTALL_BIN) $(2)/$(install_apps) $(3)/`dirname $(install_apps)`;)
endef

Build/Prepare=$(call Build/Prepare/Default,)
# don't define a default unit rule, so we only do a build unit when it is wanted
#Build/Unit=$(call Build/Unit/Default,)
Build/UnitPrepare=$(call Build/UnitPrepare/Default,)
Build/Configure=$(call Build/Configure/Default,)
Build/UnitConfigure=$(call Build/UnitConfigure/Default,)
Build/Compile=$(call Build/Compile/Default,)
Build/Install=$(if $(PKG_INSTALL),$(call Build/Install/Default,))
Build/Dist=$(call Build/Dist/Default,)
Build/DistCheck=$(call Build/DistCheck/Default,)
Build/UnitRun=$(call Build/UnitRun/Default,$(1))

$(PACKAGE_DIR):
	mkdir -p $@
	

unit:
dumpinfo:
download:
prepare:
configure:
compile:
install:
clean-staging: FORCE
	rm -f $(STAMP_INSTALLED)
	@-(\
		cd "$(STAGING_DIR)"; \
		if [ -f packages/$(PKG_NAME).list ]; then \
			cat packages/$(PKG_NAME).list | xargs -r rm -f 2>/dev/null; \
		fi; \
	)

clean: clean-staging FORCE
	$(call Build/UninstallDev,$(STAGING_DIR),$(STAGING_DIR_HOST))
	$(Build/Clean)
	rm -f $(STAGING_DIR)/packages/$(PKG_NAME).list $(STAGING_DIR_HOST)/packages/$(PKG_NAME).list
	rm -rf $(PKG_BUILD_DIR)
	rm -rf $(UNIT_BUILD_DIR)

dist:
	$(Build/Dist)
   
distcheck:
	$(Build/DistCheck) 
include $(TOPDIR)/rules.mk

PKG_NAME:=libcg-examples
PKG_VERSION:=1.0

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/libcg-examples
  SECTION:=base
  CATEGORY:=Base system
  TITLE:=SDK Examples
  DEPENDS:=+libcg +libjson-c
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) -l src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) CC="$(TARGET_CC)" LD="$(TARGET_LD)" CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) $(FPIC)" LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/bin
	$(CP) $(PKG_BUILD_DIR)/example $(1)/bin/
	$(CP) $(PKG_BUILD_DIR)/example_*[!.][!c] $(1)/bin/
endef

define Package/libcg-examples/install
	$(INSTALL_DIR) $(1)/bin
	$(CP) $(PKG_BUILD_DIR)/example $(1)/bin/
	$(CP) $(PKG_BUILD_DIR)/example_*[!.][!c] $(1)/bin/
	$(INSTALL_DIR) $(1)/examples
	$(CP) ./files/* $(1)/examples
endef

$(eval $(call BuildPackage,libcg-examples))

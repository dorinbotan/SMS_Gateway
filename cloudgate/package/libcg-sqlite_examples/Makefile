include $(TOPDIR)/rules.mk

PKG_NAME:=libcg-sqlite_examples
PKG_VERSION:=1.0

include $(INCLUDE_DIR)/package.mk

define Package/libcg-sqlite_examples
  SECTION:=base
  CATEGORY:=Base system
  TITLE:=SDK sqlite examples
  DEPENDS:=+libcg +libjson-c +libsqlite3
endef

define Package/libcg-sqlite_examples/description
This separate package contains the libcg examples that require sqlite.

If you want to try this package you will first need to initialise the openWRT
feeds and install the libsqlite3 package:

	./scripts/feeds update
	./scripts/feeds install libsqlite3

After performing these commands in your SDK root directory you can enable
this package and it will automatically select libsqlite3 too.
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) -l src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) CC="$(TARGET_CC)" LD="$(TARGET_LD)" CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) $(FPIC)" LDFLAGS="$(TARGET_LDFLAGS)"
endef


define Package/libcg-sqlite_examples/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/example_*[!.][!c] $(1)/bin/
	$(INSTALL_DIR) $(1)/examples
	$(CP) ./files/* $(1)/examples
endef

$(eval $(call BuildPackage,libcg-sqlite_examples))

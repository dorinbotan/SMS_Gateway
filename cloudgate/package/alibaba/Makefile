include $(TOPDIR)/rules.mk

PKG_NAME    := alibaba
PKG_VERSION := 1.0

TARGET_CFLAGS  += -std=c99
TARGET_CFLAGS  += -fPIC
TARGET_CFLAGS  += -Wall -Werror
TARGET_CFLAGS  += -Iinclude
TARGET_CFLAGS  += -DDEBUG 									# Turn on debug messages

TARGET_LDFLAGS += -Bstatic
TARGET_LDFLAGS += -Llib
TARGET_LDFLAGS += -liot_sdk -liot_platform -liot_tls -lgcov # Alibaba IoT-Kit related
TARGET_LDFLAGS += -lsms -lcg -lpthread -ljson-c				# CG related

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/alibaba
	SECTION	 := base
	CATEGORY := Base system
	TITLE	 := SMS Proxy
	DEPENDS	 := +libcg
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP)    isv        $(PKG_BUILD_DIR)/
	$(CP)    run_app.sh $(PKG_BUILD_DIR)/
	$(CP) -l files/*    $(PKG_BUILD_DIR)/
	$(CP) -l src/*      $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) CC="$(TARGET_CC)" CXX="$(TARGET_CXX)" LD="$(TARGET_LD)" CFLAGS="$(TARGET_CFLAGS) $(FPIC)" CPPFLAGS="$(TARGET_CPPFLAGS) $(FPIC)" LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Package/alibaba/install
#	$(MKDIR) $(1)/files
	mkdir $(1)/files
	$(CP) -r $(PKG_BUILD_DIR)/etc $(1)/files/etc
	$(CP) -r $(PKG_BUILD_DIR)/ssh $(1)/files/ssh
	$(CP) $(PKG_BUILD_DIR)/alibaba $(1)/alibaba
	$(CP) $(PKG_BUILD_DIR)/run_app.sh $(1)/run_app.sh
	$(CP) $(PKG_BUILD_DIR)/isv $(1)/isv
endef

$(eval $(call BuildPackage,alibaba))

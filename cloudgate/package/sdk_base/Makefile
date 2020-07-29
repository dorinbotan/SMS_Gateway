# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=zlib
PKG_VERSION:=1.2.3
PKG_RELEASE:=5

include $(INCLUDE_DIR)/package.mk

define Package/libreadline
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Command lines edition library
  URL:=http://cnswww.cns.cwru.edu/php/chet/readline/rltop.html
endef

define Package/zlib
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Library implementing the deflate compression method
  URL:=http://www.zlib.net/
endef

define Package/librt
$(call Package/libc/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=POSIX.1b RealTime extension library
endef

define Package/libpthread
$(call Package/libc/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=POSIX thread library
  DEPENDS:= +librt
endef

define Package/libopenssl
$(call Package/openssl/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+zlib
  TITLE+= (libraries)
endef

define Package/libtool
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=A generic dynamic object loading library
  URL:=http://www.gnu.org/software/libtool/
endef

define Package/curl/Default
  SECTION:=net
  CATEGORY:=Network
  URL:=http://curl.haxx.se/
endef

define Package/curl
  $(call Package/curl/Default)
  DEPENDS:=+libcurl
  TITLE:=A client-side URL transfer utility
endef

define Package/libcurl
  $(call Package/curl/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+libopenssl +zlib
  TITLE:=A client-side URL transfer library
endef

define Package/sdk_base
  SECTION:=base
  CATEGORY:=Base system
  TITLE:=Option Firmware Dependencies
  DEPENDS:=+libreadline +zlib +librt +libpthread +libopenssl +libtool +libcurl +curl
endef

$(eval $(call BuildPackage,libreadline))
$(eval $(call BuildPackage,librt))
$(eval $(call BuildPackage,zlib))
$(eval $(call BuildPackage,libpthread))
$(eval $(call BuildPackage,libopenssl))
$(eval $(call BuildPackage,libtool))
$(eval $(call BuildPackage,libcurl))
$(eval $(call BuildPackage,curl))
$(eval $(call BuildPackage,sdk_base))

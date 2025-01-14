# Include the global rules file
include $(TOPDIR)/rules.mk

# Define the package name and version
PKG_NAME:=my_content_filter
PKG_VERSION:=1.0
PKG_RELEASE:=1

# Include the package definition template
include $(INCLUDE_DIR)/package.mk

# Define the package metadata
define Package/my_content_filter
  SECTION:=net
  CATEGORY:=Network
  TITLE:=My Content Filter
  DEPENDS:=+libpcap  # Specify dependencies, here it's libpcap
endef

# Provide a description for the package
define Package/my_content_filter/description
  A simple custom "Filtering!" application that utilizes libpcap for network packet filtering.
endef

# Specify how to prepare the package for building
define Build/Prepare
  mkdir -p $(PKG_BUILD_DIR)
  cp -r ./src/* $(PKG_BUILD_DIR)/
  cp -r ./include/* $(PKG_BUILD_DIR)/
  cp -r ./conf/* $(PKG_BUILD_DIR)/
endef

# Define the compilation instructions
define Build/Compile
  $(TARGET_CC) $(TARGET_CFLAGS) -I$(PKG_BUILD_DIR)/include $(PKG_BUILD_DIR)/src/my_filter.cpp -o $(PKG_BUILD_DIR)/$(PKG_NAME) -lpcap
endef

# Define how to install the package
define Package/my_content_filter/install
  $(INSTALL_DIR) $(1)/usr/bin
  $(INSTALL_DIR) $(1)/usr/share/my_content_filter/conf 
  $(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/bin/
  $(INSTALL_DATA) $(PKG_BUILD_DIR)/conf/* $(1)/usr/share/my_content_filter/conf/
endef

# Use the BuildPackage macro to build the package
$(eval $(call BuildPackage,my_content_filter))

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
	# This is typically used to extract source code or copy source files to the build directory
	mkdir -p $(PKG_BUILD_DIR)
	cp -r ./src/* $(PKG_BUILD_DIR)/
endef
 
# Define the compilation instructions
define Build/Compile
	# Compilation instructions
	$(TARGET_CC) $(TARGET_CFLAGS) -I$(PKG_BUILD_DIR)/../include $(PKG_BUILD_DIR)/*.cpp -o $(PKG_BUILD_DIR)/$(PKG_NAME) -lpcap
endef
 
# Define how to install the package
define Package/my_content_filter/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/bin/
endef
 
# Use the BuildPackage macro to build the package
$(eval $(call BuildPackage,my_content_filter))


##############################################################
#
# AESD-CLIENT
#
##############################################################

#TODO: Fill up the contents below in order to reference my camera app
AESD_CLIENT_VERSION = '0687c4592fe56073418a12f766cc9a60482c8887'
AESD_CLIENT_SITE = 'git@github.com:cu-ecen-aeld/final-project-charan1811.git'
AESD_CLIENT_SITE_METHOD = git


define AESD_CLIENT_BUILD_CMDS
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)/CLIENT/ all
endef

#TODO: Add required executables or scripts below
define AESD_CLIENT_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 $(@D)/CLIENT/client $(TARGET_DIR)/home
endef


$(eval $(generic-package))

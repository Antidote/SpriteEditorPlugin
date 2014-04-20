TEMPLATE = subdirs
CONFIG += ordered

DEFINES += ATHENA_USE_QT

Plugin.depends += \
        PluginFramework \
        Updater
		
SUBDIRS += \
    Updater \
    PluginFramework \
    Plugin

OTHER_FILES += \
    .travis.yml

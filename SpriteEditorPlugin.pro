TEMPLATE = subdirs
CONFIG += ordered

DEFINES += ATHENA_USE_QT

Plugin.depends += \
        Athena \
        PluginFramework \
        Updater
		
SUBDIRS += \
    Athena \
    Updater \
    PluginFramework \
    Plugin

OTHER_FILES += \
    .travis.yml

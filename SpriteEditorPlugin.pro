TEMPLATE = subdirs
CONFIG += ordered

DEFINES += LIBZELDA_USE_QT

Plugin.depends += libzelda \
        PluginFramework \
        Updater
		
SUBDIRS += \
    libzelda \
    Updater \
    PluginFramework \
    Plugin

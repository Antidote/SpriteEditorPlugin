// This file is part of Sakura Suite.
//
// Sakura Suite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Sakura Suite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Sakura Suite.  If not, see <http://www.gnu.org/licenses/>

#include "SpriteEditorPlugin.hpp"
#include <Athena/SpriteFile.hpp>
#include "SpriteDocument.hpp"
#include "MainWindowBase.hpp"
#include <Athena/BinaryReader.hpp>
#include <QAction>
#include <QMenu>

SpriteEditorPlugin* SpriteEditorPlugin::m_instance = NULL;

SpriteEditorPlugin::SpriteEditorPlugin()
    : m_actionNewDocument(new QAction("Sprite Container Document", this)),
    m_enabled(true),
    m_mainWindow(NULL)
{
    m_instance = this;
}

SpriteEditorPlugin::~SpriteEditorPlugin()
{
    if (m_mainWindow)
        m_mainWindow->newDocumentMenu()->removeAction(m_actionNewDocument);

    delete m_actionNewDocument;
    m_actionNewDocument = NULL;
}

void SpriteEditorPlugin::initialize(MainWindowBase *mainWindow)
{
    m_mainWindow = mainWindow;
    m_mainWindow->newDocumentMenu()->addAction(m_actionNewDocument);
    connect(m_actionNewDocument, SIGNAL(triggered()), this, SLOT(onNewDocument()));

}

QString SpriteEditorPlugin::filter() const
{
    return "Sakura Engine Sprites *.sprs (*.sprs)";
}

QString SpriteEditorPlugin::extension() const
{
    return "sprs";
}

QString SpriteEditorPlugin::name() const
{
    return "Sprite Editor Plugin";
}

QString SpriteEditorPlugin::author() const
{
    return "Phillip \"Antidote\" Stephens";
}

QString SpriteEditorPlugin::version() const
{
    return "0.0.1";
}

QString SpriteEditorPlugin::website() const
{
    return "http://wiiking2.com";
}

void SpriteEditorPlugin::setPath(const QString& path)
{
    if (!m_path.isEmpty())
        return;

    m_path = path;
}

QString SpriteEditorPlugin::path() const
{
    if (m_path.isEmpty())
        return "Unknown";
    return m_path;
}

QString SpriteEditorPlugin::license() const
{
    return "GPLv3";
}

QString SpriteEditorPlugin::description() const
{
    return "Plugin for editing and creating Sprites for the Sakura game engine.";
}

bool SpriteEditorPlugin::enabled() const
{
    return m_enabled;
}

void SpriteEditorPlugin::setEnabled(const bool enable)
{
    if (m_enabled == enable)
        return;

    m_enabled = enable;

    emit enabledChanged();
}

DocumentBase* SpriteEditorPlugin::loadFile(const QString& file) const
{
    SpriteDocument* ret = new SpriteDocument(this, file);
    if (ret->loadFile())
        return ret;

    delete ret;
    ret = NULL;
    return NULL;
}

bool SpriteEditorPlugin::canLoad(const QString& filename)
{
    if (!QString::compare(QFileInfo(filename).suffix(), extension(), Qt::CaseInsensitive))
    {
        try
        {
            Athena::io::BinaryReader reader(filename.toStdString());
            if (reader.readUint32() == Athena::Sakura::SpriteFile::Magic)
                return true;
        }
        catch(...)
        {
            // Fail silently
        }
    }

    return false;
}

bool SpriteEditorPlugin::hasUpdater() const
{
    return false;
}

void SpriteEditorPlugin::doUpdate()
{

}

MainWindowBase* SpriteEditorPlugin::mainWindow() const
{
    return m_mainWindow;
}

SpriteEditorPlugin* SpriteEditorPlugin::instance()
{
    return m_instance;
}

Updater* SpriteEditorPlugin::updater()
{
    return NULL;
}

PluginSettingsDialog* SpriteEditorPlugin::settingsDialog()
{
    return NULL;
}

QObject* SpriteEditorPlugin::object()
{
    return this;
}

QIcon SpriteEditorPlugin::icon() const
{
    return m_icon;
}

void SpriteEditorPlugin::onNewDocument()
{
    emit newDocument(new SpriteDocument(this));
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN(SpriteEditorPlugin)
#endif

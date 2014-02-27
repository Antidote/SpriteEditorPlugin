#include "SpriteDocument.hpp"
#include "SpriteEditorFrame.hpp"
#include "SpriteEditorPlugin.hpp"
#include "SpriteFileReader.hpp"
#include "SpriteFileWriter.hpp"
#include "SpriteFile.hpp"
#include <MainWindowBase.hpp>
#include <QMainWindow>
#include <Exception.hpp>
#include <QDebug>
#include <SpriteFile.hpp>
#include <SpriteFrame.hpp>

SpriteDocument::SpriteDocument(const PluginInterface* loader, const QString& file)
    : DocumentBase(loader, file),
      m_spriteContainer(NULL)
{
    m_widget = new SpriteEditorFrame();
}

SpriteDocument::~SpriteDocument()
{
    delete m_spriteContainer;
    m_spriteContainer = NULL;
}

bool SpriteDocument::loadFile()
{
    SpriteEditorFrame* sef = qobject_cast<SpriteEditorFrame*>(m_widget);

    if (!filePath().isEmpty())
    {
        try
        {
            zelda::io::SpriteFileReader reader(filePath().toStdString());
            m_spriteContainer = reader.readFile();
            connect(sef, SIGNAL(modified()), this, SLOT(onModified()));
            sef->setSpriteContainer(m_spriteContainer);
        }
        catch(...)
        {
            return false;
        }
    }
    else
    {
        m_spriteContainer = new zelda::Sakura::SpriteFile;
        if (!m_spriteContainer)
            return false;
    }

    return true;
}

bool SpriteDocument::save(const QString& filepath)
{
    if (!filePath().compare(filepath, Qt::CaseInsensitive))
        setFilePath(filepath);

    try
    {
        zelda::io::SpriteFileWriter writer(filePath().toStdString());
        writer.writeFile(m_spriteContainer);
        setDirty(false);
        return true;
    }
    catch(...)
    {
        // Fail silently
    }

    return false;
}

bool SpriteDocument::reload()
{
    return loadFile();
}

void SpriteDocument::onModified()
{
    setDirty(true);
    emit modified();
}

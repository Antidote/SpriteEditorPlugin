#include "SpriteDocument.hpp"
#include "SpriteEditorFrame.hpp"
#include "SpriteEditorPlugin.hpp"
#include "SpriteTree.hpp"
#include "SSpriteFileReader.hpp"
#include "SSpriteFileWriter.hpp"
#include "SSpriteFile.hpp"
#include <MainWindowBase.hpp>
#include <QMainWindow>
#include <Exception.hpp>
#include <QDebug>

SpriteDocument::SpriteDocument(const PluginInterface* loader, const QString& file)
    : DocumentBase(loader, file)
{
    m_widget = new SpriteEditorFrame();
    SpriteEditorFrame* sef = qobject_cast<SpriteEditorFrame*>(m_widget);

    if (!file.isEmpty())
    {
        try
        {
            SSpriteFileReader reader(file.toStdString());
            m_spriteContainer = reader.readFile();
        }
        catch(zelda::error::Exception e)
        {
            qDebug() << e.message().c_str();
        }
    }
    else
    {
        m_spriteContainer = new SSpriteFile;

    }
    sef->setSpriteContainer(m_spriteContainer);
}

SpriteDocument::~SpriteDocument()
{
    delete m_spriteContainer;
}

bool SpriteDocument::save(const QString& filepath)
{
    if (!filePath().compare(filepath, Qt::CaseInsensitive))
        setFilePath(filepath);

    try
    {
        SSpriteFileWriter writer(filePath().toStdString());
        writer.writeFile(m_spriteContainer);

        return true;
    }
    catch(...)
    {
        // Fail silently
    }

    return false;
}

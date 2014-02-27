#ifndef SPRITEDOCUMENT_HPP
#define SPRITEDOCUMENT_HPP

#include <DocumentBase.hpp>

namespace zelda
{
namespace Sakura
{
class SpriteFile;
}
}

class SpriteTree;
class SpriteDocument : public DocumentBase
{
    Q_OBJECT
public:
    explicit SpriteDocument(const PluginInterface* loader, const QString& file = QString());
    ~SpriteDocument();

    bool save(const QString &filename);
    bool loadFile();
    bool reload();
private slots:
    void onModified();
private:
    zelda::Sakura::SpriteFile* m_spriteContainer;
};

#endif // SPRITEDOCUMENT_HPP

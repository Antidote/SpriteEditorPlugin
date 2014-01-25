#ifndef SPRITEDOCUMENT_HPP
#define SPRITEDOCUMENT_HPP

#include <DocumentBase.hpp>

class SpriteTree;
class SSpriteFile;
class SpriteDocument : public DocumentBase
{
    Q_OBJECT
public:
    explicit SpriteDocument(const PluginInterface* loader, const QString& file = QString());
    ~SpriteDocument();

    bool save(const QString &filename);

private:
    SSpriteFile* m_spriteContainer;
};

#endif // SPRITEDOCUMENT_HPP

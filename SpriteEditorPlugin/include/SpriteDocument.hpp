#ifndef SPRITEDOCUMENT_HPP
#define SPRITEDOCUMENT_HPP

#include <DocumentBase.hpp>

class SpriteTree;
class SpriteDocument : public DocumentBase
{
    Q_OBJECT
public:
    SpriteDocument(const PluginInterface* loader, const QString& file = QString());

private:
};

#endif // SPRITEDOCUMENT_HPP

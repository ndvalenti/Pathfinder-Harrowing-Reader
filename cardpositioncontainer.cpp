#include "cardpositioncontainer.h"

CardDisplayDataContainer::CardDisplayDataContainer(CardDisplayLabel* Label)
    : label(Label)
{
    label->dataContainer = this;
}

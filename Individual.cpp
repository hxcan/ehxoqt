#include "Individual.h" // Individual

qint32 Individual::amount=0; // Total amount of this class object.

Individual *Individual::getParentIndividual() const
{
  return parentIndividual;
}

/*!
  * \brief Individual::~Individual 纯虚析构函数。
  */
Individual::~Individual()
{
  disconnect(); // Disconnect signals.

  amount--;

//  cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", amount: " << amount <<  endl; // Debug
} // Individual::~Individual()

/*!
 * \brief Individual::Individual Default constructor.
 */
Individual::Individual()
{
  connect (this, &Individual::idCalculated, this, &Individual::rememberUsedId); // Remember used id.

  amount++;
//  cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", amount: " << amount <<   endl; // Debug
} // Individual::Individual()

void Individual::setGeneration(const qint32 &value)
{
  generation = value;
}

const QSet<QString> &Individual::getUsedIdList() const
{
//  cout << __FILE__ << ", " << __LINE__ << ", " << __func__ <<  endl; // Debug
  return usedIdList;
}

qint32 Individual::getAmount()
{
  return amount;
} // Individual::~Individual()

/*!
 * \brief Individual::rememberUsedId Remember used id.
 * \param newId The new id.
 */
void Individual::rememberUsedId(const QString & newId)
{
//  cout << __FILE__ << ", " << __LINE__ << ", " << __func__ <<  endl; // Debug
  usedIdList << newId;
} // void Individual::rememberUsedId(const QString & newId)

/*!
 * \brief Individual::removeChildIndividual Remove child individual
 * \param thisChild The child to remove.
 */
void Individual::removeChildIndividual(Individual * thisChild) //!< Remove child individual
{
  childrenIndividualList.removeAll (thisChild); // REmove.
} // void Individual::removeChildIndividual(Individual * thisChild)

/*!
 * \brief Individual::rewireChildren Hand children to parent.
 */
void Individual::rewireChildren()
{
  for(auto currenChild: qAsConst(childrenIndividualList)) // rewire one by one
  {
    currenChild->setParentIndividual(parentIndividual);
  } // for(auto currenChild: childrenIndividualList) // rewire one by one

  unlinkParent(); // Unlink from parent. Forget each other with parent.
} // void Individual::rewireChildren()

/*!
 * \brief Individual::unlinkParent Unlink from parent. Forget each other with parent.
 */
void Individual::unlinkParent()
{
  if (parentIndividual) // Exists
  {
    parentIndividual->removeChildIndividual(this); // Remove child individual

    parentIndividual=nullptr;
  } // if (parentIndividual) // Exists
} // void Individual::unlinkParent()

void Individual::setParentIndividual( Individual *value)
{
  parentIndividual = value;

  if (parentIndividual) // If the parent individual existas
  {
    parentIndividual->addChildIndividual(this); // Add child.
  } // if (parentIndividual) // If the parent individual existas
  generation++; // Increase generation number.
}

/*!
 * \brief Individual::addChildIndividual Add child.
 * \param child The child to add.
 */
void Individual::addChildIndividual( Individual * child)
{
  childrenIndividualList << child;
} // void Individual::addChildIndividual(const Individual * child)

/*!
 * \brief Individual::incrementChildrenAmount Increment children amount.
 */
void Individual::incrementChildrenAmount()
{
  childrenAmount++;
} // void Individual::incrementChildrenAmount()

void Individual::setSpeciesLock(bool value)
{
  speciesLock = value;
}

qint32 Individual::getGeneration() const
{
  return generation;
}

qint32 Individual::getChildrenAmount() const
{
  return childrenAmount;
}

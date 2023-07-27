#include "Species.h" // Species

qint32 Species::amount=0; // Total amount of this class object.

/*!
 * \brief Species::Species DEFAULT constructor.
 */
Species::Species()
{
  amount++;
//  cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", amount: " << amount <<  endl; // Debug

} // Species::Species()

/*!
 * \brief Species::~Species Destructor.
 */
Species::~Species()
{
  amount--;
//  cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", amount: " << amount <<  endl; // Debug

} // Species::~Species()

/*!
 * \brief EhxoQt::generate1Iteration Generate 1 iteration.
 * \return Generated populations.
 */
Individual * Species::generate1Iteration()
{
  auto success = true; // Generated successfully?
  Individual * result=nullptr; // Create the new individual
  result=seedIndividual->copy(); // Create the new individual
  result->copyInternalParameters (seedIndividual); // Copy internal parameters

//  reportAddressEventChart(seedIndividual, "writeInstance", __LINE__);

  seedIndividual->incrementChildrenAmount(); // Increment children amount.

  result->setParentIndividual(seedIndividual); // SEt parent individual.

  result->setSpeciesLock(true); // Lock species.

  QString indiviaulId=result->getIndividualId()    ; // New individual id.

  auto trialCount=0; // The trial count.
  auto MaxTrialCount=20; // The max trial count.

  while (individualIdSet.contains( indiviaulId)) // Already contain it
  {
    result->mutate(); // Mutate the individual. Chen xin.

    trialCount++;

    indiviaulId=result->getIndividualId()    ;

    if (trialCount>=MaxTrialCount) // Don't try forever
    {
      success=false; // Fail
//      result=nullptr; // Fail.
      break;
    } //if (trialCount>=MaxTrialCount) // Don't try forever
  } // while (individualIdSet.contains( indiviaulId)) // Already contain it

  if (success) // Generated sucessfully
  {
    connect(result, &Individual::scoreCalculated, this, &Species::syncOptInd); // Refresh optimistic individual.
//    cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", success , amount: " << amount << ", id: " << result->getIndividualId().toStdString() <<  endl; // Debug

  } // if (result) // Generated sucessfully
  else // Failed
  {
    cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", failed , amount: " << amount <<  endl; // Debug

  } // else // Failed

  return result;
}

void Species::setId(const QString &value)
{
  id = value;
}

void Species::setSeedIndividual(Individual *value)
{
//  cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", setting new seed individual: " << value << ", id: " << value->getIndividualId ().toStdString () << ", species id: " << id.toStdString() << ", original seed individual: " << seedIndividual << ", time: " << QTime::currentTime().toString().toStdString() << ", species object: " << this  <<  endl; // Debug
  auto lastSdIndivl= seedIndividual;  // Rememger last seed invidividual

  seedIndividual = value;

  if (lastSdIndivl) // Last seed individual exists.
  {
    seedIndividual->copyInternalParameters(lastSdIndivl); // Copy internal parameters.

    QCborMap request; //The request object.

    request.insert (QStringLiteral("address"), (qint64)(lastSdIndivl)); // 设置 address

    auto scoreObject=request;

    request.insert (QStringLiteral("file"), __FILE__); // 设置 address
    request.insert (QStringLiteral("line"), __LINE__); // 设置 address

    lastSdIndivl->rewireChildren(); // Hand children to parent.
    emit individualRemoved (lastSdIndivl);
    delete lastSdIndivl;
  } //if (lastSdIndivl) // Last seed individual exists.
  else // Newly added seed individiaul
  {
  } //else // Newly added seed individiaul

  individualIdSet << QString::fromUtf16 (seedIndividual->getIndividualId().utf16 ());
}

/*!
 * \brief Species::removeSeedIndividual Remove seed individual.
 */
void Species::removeSeedIndividual()
{

//  cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", removing seed individual: " << seedIndividual << ", id: " << seedIndividual->getIndividualId ().toStdString () << ", species id: " << id.toStdString()  <<  endl; // Debug
  seedIndividual->rewireChildren(); // Hand children to parent.
  emit individualRemoved (seedIndividual);
  delete seedIndividual;
  seedIndividual= nullptr;

} // void Species::removeSeedIndividual()

/*!
 * \brief Species::removeIndividual  Remove the newly generated individual.
 * \param stockParameterToTry The individual to remove.
 */
void Species::removeIndividual( Individual * stockParameterToTry)
{
  if (stockParameterToTry!=seedIndividual) // It is not the seed individual
  {
//    cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", removing individual: " << stockParameterToTry << ", id: " << stockParameterToTry->getIndividualId ().toStdString () << ", species id: " << id.toStdString() << ", seed individual: " << seedIndividual << ", time: " << QTime::currentTime().toString().toStdString() << ", species object: " << this  <<  endl; // Debug
    stockParameterToTry->rewireChildren(); // Hand children to parent.
    emit individualRemoved (stockParameterToTry);
    delete stockParameterToTry;
  } //if (lastSdIndivl) // Last seed individual exists.
} // void Species::removeIndividual(const Individual * stockParameterToTry)

/*!
 * \brief EhxoQt::refreshOptimisticIndividual Refresh optimistic individual.
 * \param newIndividualScore The new individual score.
 */
void Species::syncOptInd(const qreal & newIndividualScore)
{
  (void)(newIndividualScore); // 不使用这个参数。

  auto senderObject=sender(); // 获取发送者。

  auto reportIngIndividual=qobject_cast<Individual*>(senderObject);

  auto indiviaulId=reportIngIndividual->getIndividualId()    ;

  individualIdSet << QString::fromUtf16 (indiviaulId.utf16 ());

  auto newIndividualScoreSpecies=reportIngIndividual->scoreSpecies(); // Get the score according to species rule.
//  cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", score: " << newIndividualScoreSpecies << ", id: " << reportIngIndividual->getIndividualId ().toStdString () << ", original optimistic score: " << optimisticScore  <<  endl; // Debug

  if (newIndividualScoreSpecies > optimisticScore) // Score bigger.
  {
    optimisticScore=newIndividualScoreSpecies; // Remember best score.

//    cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", assigning new seed individual: " << reportIngIndividual << ", id: " << reportIngIndividual->getIndividualId ().toStdString () << ", species id: " << id.toStdString() << ", original seed individual: " << seedIndividual << ", time: " << QTime::currentTime().toString().toStdString() << ", species object: " << this  <<  endl; // Debug
    seedIndividual = reportIngIndividual; // Remember best individual.

    seedIndividual->increaseFocus(); // Increase focus of mutaiton.

    emit optimisticIndividualChanged(reportIngIndividual); // Report optimistic individual changed.
  } //if (newIndividualScore> optimisticScore) // Score bigger.
  else // Not score biggner
  {
    auto parentIndividual=reportIngIndividual->getParentIndividual(); // Get the parent individual

    parentIndividual->decreaseFocus(); // Decrease focus of mutation.

    if (reportIngIndividual!=seedIndividual)
    {
      reportIngIndividual->rewireChildren(); // Hand children to parent.

      QCborMap request; //The request object.

      request.insert (QStringLiteral("address"), (qint64)(reportIngIndividual)); // 设置 address
      request.insert (QStringLiteral("file"), __FILE__); // 设置 address
      request.insert (QStringLiteral("line"), __LINE__); // 设置 address
      auto scoreObject=request;

      emit individualRemoved(reportIngIndividual); // Report, rmoveed individual
      delete reportIngIndividual;
    } // if (reportIngIndividual!=seedIndividual)
    } //else // Not score biggner
}

qint32 Species::getAmount()
{
    return amount;
}

QString Species::getId() const
{
    return id;
}

Individual *Species::getSeedIndividual() const
{
  return seedIndividual;
} // void EhxoQt::refreshOptimisticIndividual(const qreal & newIndividualScore)

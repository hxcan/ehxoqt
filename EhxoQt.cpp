#include "EhxoQt.h" // EhxoQt

#include "Individual.h" // load(messageObject); // Create a new individual.

/*!
 * \brief EhxoQt::addLoadedIndividual Add the loaded individual.
 * \param individualObject The loaded invididual.
 */
bool EhxoQt::addLoadedIndividual(Individual * individualObject)
{
  auto resut=false;
  auto individualId=individualObject->getIndividualId (); // Get the indivudial id.

  if (individualIdMap.contains (individualId)) // Already contains it
  {
  } // if (individualIdMap.contains (individualId)) // Already contains it
  else // Not contains it
  {
    auto         shouldLoadIndividual=true; // Should load this individual?
    auto score=individualObject->scoreEcoSystem (); // Check the score.

    if (maxScoreToLoad>0) // max score to load setted
    {
      if (score>maxScoreToLoad) // score too high
      {
        shouldLoadIndividual=false; // Should not load.
      } // if (score>maxScoreToLoad) // score too high
      else
      {
        shouldLoadIndividual=true; // Should load.
      }
    } // if (maxScoreToLoad>0) // max score to load setted
    else // No max score
    {
      shouldLoadIndividual=true; // Should load.
    } // else // No max score

    if (shouldLoadIndividual) // Should still load
    {
//      cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", score: " << score << ", min score: " << minScoreToLoad <<  endl; // Debug
      if (score<minScoreToLoad) // score too low
      {
        shouldLoadIndividual=false; // Should not load.
      } // if (score<minScoreToLoad) // score too low
    } // if (shouldLoadIndividual) // Should still load

    if (shouldLoadIndividual) // Should load
    {
//      cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", add individual: " << individualId.toStdString () <<  endl; // Debug
//      cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", mapping individual: " << individualObject << ", id: " << individualId.toStdString () <<  endl; // Debug
      individualIdMap[individualId]=individualObject; // Add hte map.
//      cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", map size: " << individualIdMap.size() <<  endl; // Debug
      auto speciesId=individualObject->speciesId (); // Get species id.

      auto species=idSpeciesMap.value(speciesId); // Try to get the species.

      if (!species) // The species does not exist.
      {
        species=createSpecies(speciesId); // Create species.
      } // else // Species not exist

      species->setSeedIndividual (individualObject); // Set seed individual.

      resut=true;
    } // if (shouldLoadIndividual) // Should load
  } //   else // Not contains it

  return resut;
} // void EhxoQt::addLoadedIndividual(Individual * individualObject)

/*!
 * \brief EhxoQt::getSpeciesById Get the species. By id.
 * \param speciesId The species id.
 * \return The speies object.
 */
Species * EhxoQt::getSpeciesById(const QString & speciesId)
{
  auto result= idSpeciesMap.value(speciesId); // Get from the map.

  return result;

} // Species * EhxoQt::getSpeciesById(const QString & speciesId)

/*!
 * \brief EhxoQt::getIndividualById Get the invidiaul of this response.
 * \param individualId The individual id
 * \return The individual
 */
Individual * EhxoQt::getIndividualById(const QString & individualId)
{
  Individual * result=nullptr;

  result= individualIdMap.value(individualId); // Get from the map.

  return result;
} // Individual * EhxoQt::getIndividualById(const QString & individualId)

/*!
 * \brief EhxoQt::loadAsync Load eco system.
 * \param filePath The ehxo file
 * \return
 */
void EhxoQt::loadAsync(const QString &filePath)
{
//  Chen xin
  auto future=QtConcurrent::run(this, &EhxoQt::load, filePath); // Load in a seperate thread.

  subDirsFutureWatcher.setFuture(future); // SEt the future.
} // qint32 EhxoQt::loadAsync(const QString &filePath="ehxo.cx")

/*!
 * \brief EhxoQt::getIndividualAmount Get the individual amount in the eco system.
 * \return the individual amount in the eco system.
 */
qint32 EhxoQt::getIndividualAmount() const
{
  return individualIdMap.size ();
} // qint32 EhxoQt::getIndividualAmount() const

/*!
 * \brief EhxoQt::load Load eco system.
 */
qint32 EhxoQt::load(const QString &filePath)
{
  QFile ehxoFile(filePath); // Ehxo file.

  ehxoFile.open (QIODevice::ReadOnly); // Open file

  auto datagram=ehxoFile.readAll (); // Rad all content.

  ehxoFile.close ();

  auto decodedMessage=QCborValue::fromCbor (datagram); // 解码

  auto individualDataList=decodedMessage.toArray(); // Get the data list.

  cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", individual amount in data file: " << individualDataList.size () <<  endl; // Debug


  QList<Individual*> individualList;

  if (individualLoader) // There is an individual loader
  {
    for(auto didividual:individualDataList) // recover the individusal on e by one.
    {
      auto messageObject=didividual.toMap (); //转换成映射

      auto individualObject=individualLoader->load(messageObject); // Create a new individual.
//      cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", species id: " << individualObject->speciesId().toStdString() <<  endl; // Debug

//      individualObject->setEventChart (eventChart); // SEt event chart.

      auto loadSuccess=addLoadedIndividual(individualObject); // Add the loaded individual.

      if (loadSuccess)
      {
        individualList << individualObject;

      }
      else // load fail
      {
        delete individualObject; // Release memory.
      } // else // load fail
    } // for(atuo didividual:individualDataList) // recover the individusal on e by one.

  } // if (individualLoader) // There is an individual loader

//  cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", actual loaded individual amount: " << individualList.size () << ", individual amount: " << Individual::getAmount() << ", species amount: " << Species::getAmount() <<  endl; // Debug
  cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", map size: " << individualIdMap.size() << ", individual amount: " << Individual::getAmount() << ", species amount: " << Species::getAmount()  <<  endl; // Debug

  for(auto idIterator = individualIdMap.cbegin (), endIterator = individualIdMap.cend (); idIterator!= endIterator; ++idIterator) // Iterate one by one
  {
//    if (speciesIndexCounter==speciesIdIndex) // This is the index
    {
//      cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", species id loaded: " << idIterator.value()->speciesId().toStdString() <<  endl; // Debug

//      auto species = idIterator.value (); // Get the species.

//      seedIndividual = species->getSeedIndividual ();
//      break; // Got it.
    } // if (speciesIndexCounter==speciesIdIndex) // This is the index
  } // for(auto idIterator = idSpeciesMap.cbegin (), endIterator = idSpeciesMap.cend (); idIterator!= endIterator; ++idIterator) // Iterate one by one

  if (individualList.size ())
  {
//    auto seedIndex=QRandomGenerator::global ()->generate () % (individualList.size ());
//    cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", seed individual index: " << seedIndex <<  endl; // Debug

//    seedIndividual=individualList[seedIndex];
//    cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", seed individual : " << seedIndividual <<  endl; // Debug

    chooseSeedFromRandomSpecies(); // Choose seed from random species

  }

  return individualList.size ();
} // void EhxoQt::load()

/*!
 * \brief EhxoQt::~EhxoQt Desctructor.
 */
EhxoQt::~EhxoQt()
{
  for(auto idIterator = individualIdMap.cbegin (), endIterator = individualIdMap.cend (); idIterator!= endIterator; ++idIterator) // Iterate one by one
  {
//    if (speciesIndexCounter==speciesIdIndex) // This is the index
    {
      auto species = idIterator.value (); // Get the species.

//      seedIndividual = species->getSeedIndividual ();
//      break; // Got it.
      delete species; // Release individual.
    } // if (speciesIndexCounter==speciesIdIndex) // This is the index
  } // for(auto idIterator = idSpeciesMap.cbegin (), endIterator = idSpeciesMap.cend (); idIterator!= endIterator; ++idIterator) // Iterate one by one

  for(auto idIterator = idSpeciesMap.cbegin (), endIterator = idSpeciesMap.cend (); idIterator!= endIterator; ++idIterator) // Iterate one by one
  {
//    if (speciesIndexCounter==speciesIdIndex) // This is the index
    {
      auto species = idIterator.value (); // Get the species.

//      seedIndividual = species->getSeedIndividual ();
//      break; // Got it.
      delete species; // Release species.
    } // if (speciesIndexCounter==speciesIdIndex) // This is the index
  } // for(auto idIterator = idSpeciesMap.cbegin (), endIterator = idSpeciesMap.cend (); idIterator!= endIterator; ++idIterator) // Iterate one by one

} // EhxoQt::~EhxoQt()

/*!
 * \brief EhxoQt::chooseSeedFromRandomSpecies Choose seed from random species
 */
void EhxoQt::chooseSeedFromRandomSpecies()
{
  auto speciesAmount = idSpeciesMap.size (); // Get the species amount.
  if (speciesAmount) // There is at least 1 species
  {
    auto speciesIdIndex= QRandomGenerator::global ()->generate () % (speciesAmount);
    quint32 speciesIndexCounter = 0; // The species index counter.

    for(auto idIterator = idSpeciesMap.cbegin (), endIterator = idSpeciesMap.cend (); idIterator!= endIterator; ++idIterator) // Iterate one by one
    {
      if (speciesIndexCounter==speciesIdIndex) // This is the index
      {
//        cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", species id chosen: " << idIterator.key().toStdString() <<  endl; // Debug

        auto species = idIterator.value (); // Get the species.

//        cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", seed object: " << seedIndividual << ", species object: " << species <<  endl; // Debug
        seedIndividual = species->getSeedIndividual ();
//        cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", seed object: " << seedIndividual << ", time: " << QTime::currentTime().toString().toStdString() <<  endl; // Debug
        break; // Got it.
      } // if (speciesIndexCounter==speciesIdIndex) // This is the index

      speciesIndexCounter++; // Count to try the next one.
    } // for(auto idIterator = idSpeciesMap.cbegin (), endIterator = idSpeciesMap.cend (); idIterator!= endIterator; ++idIterator) // Iterate one by one
  } // if (idSpeciesMap.size()) // There is something in the map
} // void EhxoQt::chooseSeedFromRandomSpecies()

/*!
 * \brief EhxoQt::save Save the eco system.
 */
void EhxoQt::save(const QString & filePath)
{
  for(auto idIterator = individualIdMap.cbegin (), endIterator = individualIdMap.cend (); idIterator!= endIterator; ++idIterator) // Iterate one by one
  {
    //    if (speciesIndexCounter==speciesIdIndex) // This is the index
    {
//      cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", species id to save: " << idIterator.value()->speciesId().toStdString() <<  endl; // Debug

      //      auto species = idIterator.value (); // Get the species.

      //      seedIndividual = species->getSeedIndividual ();
      //      break; // Got it.
    } // if (speciesIndexCounter==speciesIdIndex) // This is the index
  } // for(auto idIterator = idSpeciesMap.cbegin (), endIterator = idSpeciesMap.cend (); idIterator!= endIterator; ++idIterator) // Iterate one by one


  auto keys=individualIdMap.keys (); // Ge t the keys

  cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", map size: " << individualIdMap.size() << ", individual amount: " << Individual::getAmount() <<  endl; // Debug


  QCborArray individualDataList; // Individual data list.

  qint32 individualCounter=0; // Individual counter for debug

  for(auto individualId: keys) // Save one by one
  {
    auto individua=individualIdMap.value(individualId); // Get hte current individual

    if (individua) // Individual exists
    {
      auto individualData=individua->save(); // Get dave data.

      individualDataList << individualData; // Add to data list.

      if ((individualCounter % 100 ) == 1) // Output every 100
      {
        cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", individual id: " << individualId.toStdString() << ", score: " << individua->scoreEcoSystem() << ", individual counter: " << individualCounter <<  endl; // Debug

      } // if ((individualCounter % 100 ) == 1) // Output every 100
    } // if (individua) // Individual exists

    individualCounter++;
  } // for(auto individualId: keys) // Save one by one

  auto cborValue = individualDataList.toCborValue(); // Convert to cbor value.

  auto datagram=cborValue.toCbor(); // 序列化.

  QFile ehxoFile(filePath); // Ehxo file.

  ehxoFile.open (QIODevice::WriteOnly); // Open file

  ehxoFile.write(datagram); // Write file.

  ehxoFile.close ();
} // void EhxoQt::save()

/*!
 * Remove optimistic indidivual .
 */
void EhxoQt::removeOptimisticIndividual(const Individual *individual)
{
  if (individual == optimisticIndividual) // It is the optimistic individual
  {
    optimisticIndividual = nullptr; // Reset.
  } // if (individual == optimisticIndividual) // It is the optimistic individual
} // void EhxoQt::removeOptimisticIndividual(const Individual *individual)

/*!
 * \brief EhxoQt::removeCandidateOptimisticIndividual Remove candidate optimistic indidivual .
 * \param individual The individual to remove.
 */
void EhxoQt::removeCandidateOptimisticIndividual( Individual * individual)
{
  candidateOptimisticIndividualList.removeAll(individual); // Remove.
} // void EhxoQt::removeCandidateOptimisticIndividual(const Individual * individual)

/*!
 * \brief EhxoQt::removeIndividualMapItem Remove indidivual map item.
 * \param individual The individual to remove from map.
 */
void EhxoQt::removeIndividualMapItem(const Individual * individual)
{
//  reportAddressEventChart (individual, "unmapTarget", __LINE__); // Report event chart address eventy.

  auto currentIndividualId = individual->getIndividualId(); // Get the curent individual id.

  individualIdMap.remove(currentIndividualId); // Remove.
//  cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", map size: " << individualIdMap.size() << ", id: " << currentIndividualId.toStdString() <<  endl; // Debug

  auto usedIdList = individual->getUsedIdList(); // Get the used id list.

  usedIdList.remove (currentIndividualId); // Remove the curent individual id.

  for (auto usedId: usedIdList) // Remove the ids one by one
  {
    auto key = usedId; // Get the actual key of this individual.
    auto value = individualIdMap.value(key); // Get the mapped individual

    if (value==individual) // It is the individual
    {
      individualIdMap.remove(key); // Remove.
      cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", map size: " << individualIdMap.size() << ", used id: " << key.toStdString() <<  endl; // Debug
    } // if (value==individual) // It is the individual

  } // for (auto usedId: usedIdList) // Remove the ids one by one

//  auto key=individualIdMap.key (individual); // Get the actual key of this individual.

//  if (!key.isEmpty ()) // Key exists
//  {
//    individualIdMap.remove(key); // Remove.
//  } // if (!key.isEmpty ()) // Key exists
} // void EhxoQt::removeIndividualMapItem(const Individual * individual)

/*!
 * \brief EhxoQt::generate1Iteration Generate 1 iteration.
 * \return Generated populations.
 */
Individual * EhxoQt::generate1Iteration()
{
//  cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", map size: " << individualIdMap.size() <<  endl; // Debug
//  cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", seed object: " << seedIndividual <<  endl; // Debug
  auto currentSeedUsed=seedIndividual; // The curent seed used.
//  cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", seed object: " << currentSeedUsed <<  endl; // Debug
  if (useSeedFromRandomSpecies) // Should choose seed from random species.
  {
//    cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", map size: " << individualIdMap.size() <<  endl; // Debug
    chooseSeedFromRandomSpecies ();
//    cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", seed object: " << currentSeedUsed <<  endl; // Debug
    currentSeedUsed=seedIndividual; // The curent seed used.
//    cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", seed object: " << currentSeedUsed <<  endl; // Debug
  } // if (useSeedFromRandomSpecies) // Should choose seed from random species.
  else // Choose from candidate optimistic individual list
  {
    if (candidateOptimisticIndividualList.length()) // Candidate exists
    {
      auto randomGenerator=QRandomGenerator::global(); //get the global random generator.

  //    cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", map size: " << individualIdMap.size() <<  endl; // Debug
      auto multiplier=randomGenerator->bounded(candidateOptimisticIndividualList.length()); //-4 to 4.

      auto seedIndex=multiplier; // Seed index.
      cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", seed object: " << currentSeedUsed <<  endl; // Debug
      currentSeedUsed=candidateOptimisticIndividualList[seedIndex]; // The curent seed used.
      cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", seed object: " << currentSeedUsed <<  endl; // Debug
    } // if (candidateOptimisticIndividualList.length()) // Candidate exists

  } // else // Choose from candidate optimistic individual list

//  auto currentSeedUsed=seedIndividual; // The curent seed used.
//  cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", map size: " << individualIdMap.size() <<  endl; // Debug

//  cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", map size: " << individualIdMap.size() <<  endl; // Debug

  Individual * result=nullptr; // Create the new individual

  if (currentSeedUsed) // The seed exists
  {
//    cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", seed object: " << currentSeedUsed <<  endl; // Debug
    result=currentSeedUsed->copy(); // Create the new individual

    result->copyInternalParameters (currentSeedUsed); // Copy internal parameters

    result->setParentIndividual(currentSeedUsed); // SEt parent individual.
//    cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", map size: " << individualIdMap.size() <<  endl; // Debug

    result->setSpeciesLock(false); // Not Lock species.

    result->mutate(); // Mutate the individual. Chen xin.

//    cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", result individual id: " << result->getIndividualId().toStdString() <<  endl; // Debug
    auto specIdResult=result->speciesId(); // Get the species id of the result object.
    auto specIdSeed=currentSeedUsed->speciesId(); // Get the species id of the seed object.

    if (specIdResult!=specIdSeed) // Changed species
    {
//      cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", map size: " << individualIdMap.size() <<  endl; // Debug
      auto species=idSpeciesMap.value(specIdResult); // Find the species of the new result object.

      if (!species) // That species not exist
      {
//        cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", map size: " << individualIdMap.size() <<  endl; // Debug
        species=createSpecies(specIdResult); // Create species.

        auto individualId = result->getIndividualId(); // Get the individual id.
        auto idExists=individualIdMap.contains(individualId); // Check if this individual exists.

        if (idExists) // Already exists
        {
          delete result;

          result=individualIdMap.value(individualId); // point to existing individual
        } // if (idExists) // Already exists
        else // new individual
        {

        } // else // new individual

        species->setSeedIndividual(result); // Set the seed individual.
//        cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", map size: " << individualIdMap.size() <<  endl; // Debug
        individualIdMap[individualId]=result; // Remember individual id map.
//        cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", map size: " << individualIdMap.size() <<  endl; // Debug
      } // if (!species) // That species not exist
      else
      {
        result->unlinkParent(); // Unlink from parent. Forget each other with parent.

        delete result;
      }

      result = species->generate1Iteration(); // Let the species generate a new object.
    } // if (specIdResult!=specIdSeed) // Changed species

//    cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", map size: " << individualIdMap.size() <<  endl; // Debug
    auto individualId = result->getIndividualId(); // Get the individual id.

    if (individualIdMap.contains(individualId)) // Already in the map
    {
      delete result;
//      cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", map size: " << individualIdMap.size() <<  endl; // Debug

      result=generate1Iteration(); // Try agina.
    } // if (individualIdMap.contains(individualIdMap)) // Already in the map
    else //  new individual
    {
      individualIdMap[individualId]=result; // Remember individual id map.

//      add function, checkMapSize. Chen xin.

      auto mapSize= individualIdMap.size(); // Debug.
      auto individualAmount=Individual::getAmount(); // Debug.
      if (mapSize!=individualAmount) // Deb ug
      {
        cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", map size: " << mapSize << ", individual amount: " << individualAmount <<  endl; // Debug
        exit(397);

      }

    } // else //  new individual

  } // if (currentSeedUsed) // The seed exists


  return result;
} // Individual * EhxoQt::generate1Iteration()

/*!
 * \brief EhxoQt::removeSpecies Remove the species.
 * \param species The speiceis to remove.
 */
void EhxoQt::removeSpecies(Species * species)
{
//  Chen xin.
//  cout << __FILE__ << ":" << __LINE__ << ", " << __func__ << ", removing species id : " << species->getId().toStdString() <<  endl; // Debug

  species->removeSeedIndividual(); // Remove seed individual.

  idSpeciesMap.remove(species->getId()); // REmove.

  delete species;
} // void EhxoQt::removeSpecies(Species * species)

/*!
 * \brief EhxoQt::createSpecies Create species.
 * \return The created species.
 */
Species * EhxoQt::createSpecies(const QString & specIdResult)
{
  Species * species=new Species; // Create new species.

//  species->setEventChart(eventChart); // SEt event chart.

  connect(species, &Species::individualRemoved, this, &EhxoQt::removeIndividualMapItem); // Remove indidivual map item.
  connect(species, &Species::individualRemoved, this, &EhxoQt::removeCandidateOptimisticIndividual); // Remove candidate optimistic indidivual .
  connect(species, &Species::individualRemoved, this, &EhxoQt::removeOptimisticIndividual); // Remove optimistic indidivual .
  connect(species, &Species::optimisticIndividualChanged, this,  QOverload< Individual*>::of (&EhxoQt::refreshOptimisticIndividual)); // Refresh optimistic individual

//  cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", individual : " << result <<  endl; // Debug

  species->setId(specIdResult); // SEt id.

  idSpeciesMap[specIdResult]=species; // Remember species id map.

  return species;
} // Species * EhxoQt::createSpecies()

/*!
 * \brief EhxoQt::increaseCandidateAmount 增加候选列表长度，并提供参考的上限值。
 * \param betterDayAmount 参考的上限值。
 */
bool EhxoQt::increaseCandidateAmount(const qint32 & betterDayAmount)
{
  auto result=false; // 结果。

  if (maxCandidateOptimisticIndividualAmount>=betterDayAmount) //  不可再增加了
  {
  } // if (maxCandidateOptimisticIndividualAmount>=betterDayAmount) //  不可再增加了
  else // 可再增加。
  {
    maxCandidateOptimisticIndividualAmount++; // 增加。

    result=true;
  } // else // 可再增加。

  return result;
} // void EhxoQt::increaseCandidateAmount(const qint32 & betterDayAmount)

/*!
 * \brief EhxoQt::tryInsertCandidateOPtimisticIndividualList Try to insert into candidate optimistic individual list.
 * \param reportIngIndividual The promising individual
 */
auto EhxoQt::tryInsertCandidateOPtimisticIndividualList(Individual * reportIngIndividual)
{
  auto result=false; // Result;
  auto foundPlace=false; // Found insert places?
  auto speciesScore=reportIngIndividual->scoreSpecies();

  auto speciesId=reportIngIndividual->speciesId (); // 获取物种编号。

//  cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", reporting individual: " << reportIngIndividual << ", species id: " << speciesId.toStdString () <<  endl; // Debug
  auto candidateAmount=candidateOptimisticIndividualList.length(); // Get the existing candidate amount.

  auto indexCounter=0; // The index counter.

  for(indexCounter=0; indexCounter< candidateAmount; indexCounter++) // Find a place to insert
  {
    auto currentCandidate = candidateOptimisticIndividualList[indexCounter]; // Get the current existing candidate.
//    cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", individual from candidate optimistic individual list: " << currentCandidate <<  endl; // Debug
    auto currentIndexSpeciesScore=currentCandidate->scoreSpecies(); // Get the species score of the current candidate in existing list

    if (speciesScore>currentIndexSpeciesScore) // Higher score
    {
//      cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", inserting individual: " << reportIngIndividual << ", index: " <<  indexCounter <<  endl; // Debug
      candidateOptimisticIndividualList.insert(indexCounter, reportIngIndividual); // Insert here.

      foundPlace=true;

      break; // No need to find
    } // if (speciesScore>currentIndexSpeciesScore) // Higher score
  } // for(indexCounter=0; indexCounter< candidateAmount; indexCounter++) // Find a place to insert

  if (foundPlace) // Found place
  {
    // 删除后面的同物种记录：
    auto deleteSearchStart=indexCounter+1; // 从这个开始寻找同物种的记录。
    candidateAmount=candidateOptimisticIndividualList.length(); // Get the existing candidate amount.

    for(indexCounter=deleteSearchStart; indexCounter< candidateAmount; indexCounter++) // Find a 候选者 to 删除
    {
      auto currentCandidate = candidateOptimisticIndividualList[indexCounter]; // Get the current existing candidate.
//      cout << __FILE__ << ", " << __LINE__ << ", " << __func__ << ", individual from candidate optimistic individual list: " << currentCandidate <<  endl; // Debug
      auto currentIndexSpeciesScore=currentCandidate->speciesId (); // 获取物种编号

      if (speciesId==currentIndexSpeciesScore) // 相同物种
      {
        candidateOptimisticIndividualList.removeAt (indexCounter); // 删除。

        break; // No need to find
      } // if (speciesScore>currentIndexSpeciesScore) // Higher score
    } // for(indexCounter=0; indexCounter< candidateAmount; indexCounter++) // Find a place to insert
  } // if (foundPlace) // Found place
  else // NOt found place
  {
    if (candidateAmount<maxCandidateOptimisticIndividualAmount) // Still have space
    {
      candidateOptimisticIndividualList << reportIngIndividual; // Insert here.

      foundPlace=true; // Found place.
    } // if (candidateAmount<MaxCandidateOptimisticIndividualAmount) // Still have space
  } // else // NOt found place

  if (candidateOptimisticIndividualList.length()>maxCandidateOptimisticIndividualAmount) // Exceeds space
  {
    candidateOptimisticIndividualList.removeLast();
  } // if (candidateOptimisticIndividualList.length()>MaxCandidateOptimisticIndividualAmount) // Exceeds space

  result=foundPlace;

  return result;
}

Individual *EhxoQt::getSeedIndividual() const
{
  return seedIndividual;
} // void EhxoQt::tryInsertCandidateOPtimisticIndividualList(Individual * reportIngIndividual)

/*!
 * \brief EhxoQt::findOptimisticIndividual Find the new optimistic individual
 */
auto EhxoQt::findOptimisticIndividual() const
{
  auto result=candidateOptimisticIndividualList.first(); // Result;
  auto bestScore=result->scoreEcoSystem(); // best score.

  auto indexCounter=0; // Index counter

  for(indexCounter=1; indexCounter<candidateOptimisticIndividualList.length(); indexCounter++) // check one by one
  {
    auto currentCandidate=candidateOptimisticIndividualList[indexCounter]; // Get the current candidate

    auto currentScore=currentCandidate->scoreEcoSystem(); // get score

    if (currentScore> bestScore) // higher score
    {
      result=currentCandidate; // Change best individual
      bestScore=currentScore; // Remember best score.
    } // if (currentScore> bestScore) // higher score
  } // for(indexCounter=1; indexCounter<candidateOptimisticIndividualList.length(); indexCounter++) // check one by one

  return result;
} // auto EhxoQt::findOptimisticIndividual() const

/*!
 * \brief EhxoQt::refreshOptimisticIndividual Refresh optimistic individual.
 * \param newIndividualScore The new individual with high score
 */
void EhxoQt::refreshOptimisticIndividual( Individual * newIndividual)
{
  auto reportIngIndividual=newIndividual;

  auto insertResult=tryInsertCandidateOPtimisticIndividualList(reportIngIndividual); // Try to insert into candidate optimistic individual list.

  if (insertResult) // A new candidate.
  {
    auto nowOptimisticIndividual=findOptimisticIndividual(); // Find the new optimistic individual

    auto newIndividualScore=nowOptimisticIndividual->scoreEcoSystem(); // Get score according to eco system rule.

    if (nowOptimisticIndividual!=optimisticIndividual) // Changed.
    {
      optimisticScore=newIndividualScore; // Remember best score.

      optimisticIndividual = nowOptimisticIndividual; // Remember best individual.

      optimisticIndividual->increaseFocus(); // Increase focus of mutaiton.

      seedIndividual->increaseFocus(); // Increase focus of mutaiton.

      emit optimisticIndividualChanged(optimisticIndividual); // Emit signal, optimistic individual changed.
    } //if (nowOptimisticIndividual!=optimisticIndividual) // Changed.
  } // if (insertResult) // A new candidate.
} // void EhxoQt::refreshOptimisticIndividual(const Individual * newIndividualScore)

/*!
 * \brief EhxoQt::EhxoQt Default constructor.
 */
EhxoQt::EhxoQt()
{
//  eventChart->setWriteImmediately(true); // Write event to storage immediately to avoid lost event when crash.
  connect (&subDirsFutureWatcher, &QFutureWatcher<void>::finished, this, &EhxoQt::loadFinished); // Load finished signal.
} // EhxoQt::EhxoQt()

void EhxoQt::setIndividualLoader(IndividualLoader *newIndividualLoader)
{
  individualLoader = newIndividualLoader;
}

void EhxoQt::setUseSeedFromRandomSpecies(bool newUseSeedFromRandomSpecies)
{
  useSeedFromRandomSpecies = newUseSeedFromRandomSpecies;
}

/*!
 * \brief EhxoQt::setMinScoreToLoad Set the lowest score to load.
 * \param bestScorer2 the lowest score to load.
 */
void EhxoQt::setMinScoreToLoad(const qreal  & bestScorer2)
{
  minScoreToLoad = bestScorer2;
} // void EhxoQt::setMinScoreToLoad(const qreal  & bestScorer2)

void EhxoQt::setMaxScoreToLoad(qint32 newMaxScoreToLoad)
{
  maxScoreToLoad = newMaxScoreToLoad;
} // EhxoQt::EhxoQt()

void EhxoQt::setSeedIndividual(Individual *value)
{
  auto lastSdIndivl= seedIndividual;  // Rememger last seed invidividual

  seedIndividual = value;

  if (lastSdIndivl) // Last seed individual exists.
  {
    seedIndividual->copyInternalParameters(lastSdIndivl); // Copy internal parameters.
  } //if (lastSdIndivl) // Last seed individual exists.
  else // Newly added seed individiaul
  {
    optimisticIndividual=seedIndividual;
  } //else // Newly added seed individiaul

  auto individualId = seedIndividual->getIndividualId(); // Get the individual id.

  individualIdMap[individualId]=seedIndividual; // Remember individual id map.
}

Individual *EhxoQt::getOptimisticIndividual() const
{
  return optimisticIndividual;
} // Individual EhxoQt::generate1Iteration()

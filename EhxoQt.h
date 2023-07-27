#ifndef EHXOQT_H
#define EHXOQT_H

#include "EhxoQtGlobal.h" // EHXO_QT

#include "EhxoQtPaPreCompile.h" // Q_OBJECT

#include "Individual.h" // Individual
#include "Species.h" // Species
#include "IndividualLoader.h" // IndividualLoader

class EHXO_QT_EXPORT EhxoQt : public QObject
{
  Q_OBJECT

public:
  bool increaseCandidateAmount(const qint32 & betterDayAmount); //!< 增加候选列表长度，并提供参考的上限值。
  Individual * generate1Iteration(); //!< Generate 1 iteration.
  void setSeedIndividual(Individual *value);
  Individual * getIndividualById(const QString & individualId); //!< Get the invidiaul of this response.
  Species * getSpeciesById(const QString & speciesId); //!< Get the species. By id.
  Individual *getOptimisticIndividual() const;
  EhxoQt(); //!< Default constructor.
  ~EhxoQt(); //!< Desctructor.
  void save(const QString &filePath="ehxo.cx"); //!< Save the eco system.
  Species * createSpecies(const QString &specIdResult); //!< Create species.
  void removeSpecies(Species * species); //!< Remove the species.
  qint32 load(const QString &filePath="ehxo.cx"); //!< Load eco system.
  qint32 getIndividualAmount() const; //!< Get the individual amount in the eco system.
  void loadAsync(const QString &filePath="ehxo.cx"); //!< Load eco system.
//  void setEventChart(EventChartQt *value);
  void setIndividualLoader(IndividualLoader *newIndividualLoader);
//  void setMaxScoreToLoad(const qint32 & highScore); //!< Set the max score to be allowed to load.
  void setUseSeedFromRandomSpecies(bool newUseSeedFromRandomSpecies);
  void setMinScoreToLoad(const qreal  & bestScorer2); //!< Set the lowest score to load.
  void setMaxScoreToLoad(qint32 newMaxScoreToLoad);

  Individual *getSeedIndividual() const;

private:
  QFutureWatcher<void> subDirsFutureWatcher; //!< The future watcher of async load.
  qint32 maxScoreToLoad=-1; //!< The max score to load.
  qreal minScoreToLoad=0; //!< The minimal score to load.
  bool useSeedFromRandomSpecies=false; //!< Wether we should use seed from random species.
  void chooseSeedFromRandomSpecies(); //!< Choose seed from random species
  bool addLoadedIndividual(Individual * individualObject); //!< Add the loaded individual.
  IndividualLoader * individualLoader=nullptr; //!< Individual loader.
//  EventChartQt * eventChart=nullptr; //!< Event chart client object.
  QMap<QString, Individual*> individualIdMap; //!< The individual id to individual map.
  void removeIndividualMapItem(const Individual *individual); //!<Remove indidivual map item.
  void removeCandidateOptimisticIndividual(Individual *individual); //!< Remove candidate optimistic indidivual .
  void removeOptimisticIndividual(const Individual *individual); //!< Remove optimistic indidivual .
  QMap<QString, Species*> idSpeciesMap; //!< The id to species map.
  Individual * optimisticIndividual = nullptr; //!< The best individual.
  qreal optimisticScore = std::numeric_limits<qreal>::lowest(); //!< optimistic score.
  void refreshOptimisticIndividual(Individual *newIndividual); //!< Refresh optimistic individual.
  auto tryInsertCandidateOPtimisticIndividualList(Individual * reportIngIndividual) ; //!< Try to insert into candidate optimistic individual list.
  Individual * seedIndividual=nullptr; //!< The seed individual. Chen xin.
  QList< Individual*> candidateOptimisticIndividualList; //!< The candidate optimistic individual list.
  auto findOptimisticIndividual() const; //!< Find the new optimistic individual
  qint32 maxCandidateOptimisticIndividualAmount=10; //!< Max candidate optimistic individual amount.

signals:
  void loadFinished(); //!< Load finished signal.
  void optimisticIndividualChanged(const Individual * optimisticIndividual) ; // Emit signal, optimistic individual changed.
};

#endif

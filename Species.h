#ifndef SPECIES_H
#define SPECIES_H

#include "EhxoQtPaPreCompile.h" // QObject

#include "Individual.h" // Individual

class Species : public QObject
{
  Q_OBJECT

public:
  void setSeedIndividual(Individual *value);
  Individual * generate1Iteration(); //!< Generate 1 iteration.
  void setId(const QString &value);
  Species(); //!< DEFAULT constructor.
  ~Species(); //!< Destructor.
  Individual *getSeedIndividual() const;
  void removeIndividual(Individual *stockParameterToTry); //!< Remove the newly generated individual.
  void removeSeedIndividual(); //!< Remove seed individual.
  static qint32 getAmount();

  QString getId() const;

  private:
  QSet<QString> individualIdSet; //!< Individual id set.
  QString id; //!< The id of this species.
  Individual * seedIndividual=nullptr; //!< The seed individual. Chen xin.
  void syncOptInd(const qreal & newIndividualScore); //!< Refresh optimistic individual.
  qreal optimisticScore = std::numeric_limits<qreal>::lowest(); //!< optimistic score.
  static qint32 amount; //!< Total amount of this class object.

signals:
  void individualRemoved( Individual * reportIngIndividual) const; //!< Report, rmoveed individual
  void optimisticIndividualChanged( Individual * reportIngIndividual) ; //!< Report optimistic individual changed.
};

#endif

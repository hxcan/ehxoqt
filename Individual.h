#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "EhxoQtGlobal.h"

#include "EhxoQtPaPreCompile.h" // QObject

class EHXO_QT_EXPORT Individual : public QObject
{
  Q_OBJECT

public:
  void removeChildIndividual(Individual * thisChild); //!< Remove child individual
  void addChildIndividual(Individual *child); //!< Add child.
  void rewireChildren(); //!< Hand children to parent.
  void incrementChildrenAmount(); //!< Increment children amount.
  virtual QCborMap save() = 0; //!< Get dave data.
//  static Individual * load(const QCborMap & messageObject) = 0 ; //!< Create a new individual from cbor map.
  virtual QString getIndividualId() const =0    ; //!< Get individual id.
//  QSet<QString> getUsedIdList() const; //!< Get the used id list.
  virtual QString speciesId() const = 0; //!< Get the species id of the object.
  virtual qreal scoreSpecies() const = 0; //!< Get the score according to species rule.
  virtual qreal scoreEcoSystem() const = 0; //!< Get score according to eco system rule.
  virtual void copyInternalParameters(Individual * lastSdIndivl) = 0; //!< Copy internal parameters.
  virtual void decreaseFocus() = 0; //!< Decrease focus of mutation.
  virtual void increaseFocus() = 0; //!< Increase focus of mutaiton.
  virtual void mutate() =0; //!< Mutate the individual. Chen xin.
  virtual Individual * copy() =0; //!< Make a new copy of this individual.
  Individual *getParentIndividual() const;
  void setParentIndividual(Individual *value);
  void unlinkParent(); //!< Unlink from parent. Forget each other with parent.
  void setSpeciesLock(bool value);
  qint32 getGeneration() const;
  qint32 getChildrenAmount() const;
  virtual ~Individual(); //!< 纯虚析构函数。
//  void setEventChart(EventChartQt *value);
  Individual(); //!< Default constructor.
  void setGeneration(const qint32 &value);

  const QSet<QString> &getUsedIdList() const;

  static qint32 getAmount();

protected:
  bool speciesLock=false; //!< Whether lock species when mutating.
  qint32 generation=0; //!< generation number.

private:
  static qint32 amount; //!< Total amount of this class object.
  void rememberUsedId(const QString & newId); //!< Remember used id.
  QSet<QString> usedIdList; //!< The list of used id.
  QList<Individual*> childrenIndividualList; //!< Children individual list.
  Individual * parentIndividual=nullptr; //!< The parent individual.
  qint32 childrenAmount=0; //!< Children amount.

signals:
  void scoreCalculated(const qreal & newScoreSpecies) const; //!< Signal, score calculated.
  void idCalculated(const QString & result) const; //!< signal, id calculated.
};
                                                                                                                                                      #endif

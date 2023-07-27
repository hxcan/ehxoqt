#ifndef INDIVIDUALLOADER_H
#define INDIVIDUALLOADER_H

#include "Individual.h" // Individual

class IndividualLoader
{
public:
  virtual ~IndividualLoader(); //!< Destructor.
  virtual Individual * load(const QCborMap & messageObject)=0; //!< Create a new individual.
};

#endif // IndividualLoader

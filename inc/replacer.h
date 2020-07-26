#ifndef _REPLACER_H_
#define _REPLACER_H_

#include "replacerBase.h"

class Replacer : public ReplacerBase {
private:

  void replaceWord(char *word);

public:
  Replacer();
  Replacer(unsigned int maxLen);
  ~Replacer();
};

#endif // !_REPLACER_H_

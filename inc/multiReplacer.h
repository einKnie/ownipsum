#ifndef _MULTI_REPLACER_H_
#define _MULTI_REPLACER_H_

/* idea:
 *  - multiple rep string
 *  - for each word, most suitable rep string is determined (for now, by length alone)
 *  - this should result in a 'lorem ipsum'-like output text
 */

#include "replacerBase.h"
#include "lilist.h"
#include <string.h>

typedef struct RepWord {
  char word[MAXLEN];
  unsigned int idx;

  friend bool operator== (const RepWord &lhs, const RepWord &rhs) {
    return (strlen(lhs.word) == strlen(rhs.word));
  }
  friend bool operator< (const RepWord &lhs, const RepWord &rhs) {
    return (strlen(lhs.word) < strlen(rhs.word));
  }
  friend bool operator>(const RepWord &lhs, const RepWord &rhs) {
    return (strlen(lhs.word) > strlen(rhs.word));
  }
  friend bool operator!= (const RepWord &lhs, const RepWord &rhs) {
    return (strlen(lhs.word) != strlen(rhs.word));
  }

} repWord_t;

class MultiReplacer : public ReplacerBase {
private:

  LiList<repWord_t> *m_words;

  void replaceWord(char* word);
  bool findRepStr(char* word, repWord_t **rep);

public:

  MultiReplacer(repWord_t *reps, uint8_t n);
  ~MultiReplacer();

  bool addWord(repWord_t &rep);
  bool remWord(repWord_t &rep);

};

#endif // _MULTI_REPLACER_H_

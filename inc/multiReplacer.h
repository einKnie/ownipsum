#ifndef _MULTI_REPLACER_H_
#define _MULTI_REPLACER_H_

#include "replacerBase.h"
#include "lilist.h"
#include <string.h>

typedef struct RepWord {
  char word[MAX_REPLEN];
  unsigned int idx;

  RepWord(const char *word, unsigned int idx) {
    strncpy(this->word, word, sizeof(this->word));
    this->idx = idx;
  }

  RepWord() : RepWord("\0", 0) { }

  bool operator== (const RepWord &other) {
    return (strlen(word) == strlen(other.word));
  }
  bool operator< (const RepWord &other) {
    return (strlen(word) < strlen(other.word));
  }
  bool operator> (const RepWord &other) {
    return (strlen(word) > strlen(other.word));
  }
  bool operator!= (const RepWord &other) {
    return (strlen(word) != strlen(other.word));
  }

} repWord_t;

class MultiReplacer : public ReplacerBase {
private:

  LiList<repWord_t> *m_words;

  /// @brief Replace a given word \a word with a suitable replacement
  /// @param [in/out] word
  bool replaceWord(char* word);

  /// @brief Find a suiting replacement for a given \a word
  /// @param [in] word
  /// @param [in/out] rep will hold a reference to the found
  /// replacement word after successful call
  /// @return true on success, false on failure
  /// @note \a rep should not be allocated beforehand and does not need to be freed after use.
  bool findRepStr(char* word, repWord_t **rep);

  bool isExactMatch(repWord_t *orig, repWord_t *rep);
  bool isApproximateShorterMatch(repWord_t *orig, repWord_t *rep);
  bool isApproximateLongerMatch(repWord_t *orig, repWord_t *rep);
  bool isShorter(repWord_t *orig, repWord_t *rep);
  bool isLonger(repWord_t *orig, repWord_t *rep);

public:

  MultiReplacer(repWord_t *reps, uint8_t n);
  ~MultiReplacer();

};

#endif // _MULTI_REPLACER_H_

#include "multiReplacer.h"
#include <math.h>

MultiReplacer::MultiReplacer(repWord_t *reps, uint8_t n) : ReplacerBase() {
  m_words = new LiList<repWord_t>(listMode_e::EModeCopy);

  for (uint8_t i = 0; i < n; i++) {
    m_words->addSorted(&reps[i]);
  }

  log_dbg("created multi-replacer\n");
}

MultiReplacer::~MultiReplacer() {
  delete m_words;
  log_dbg("destroyed multi-replacer\n");
}

void MultiReplacer::replaceWord(char *word) {
  repWord_t *rep;
  if (!findRepStr(word, &rep)) {
    printf("error: could not find a suitable rep string for word %s\n", word);
    return;
  }

  if (!setText(rep->word, rep->idx)) {
    printf("error: failed to set replacement word %s(%d)\n", rep->word, rep->idx);
    return;
  }

  replaceSingleWord(word);
}

bool MultiReplacer::findRepStr(char *word, repWord_t **rep) {
  LiList<repWord_t> *potentials = new LiList<repWord_t>();

  // find correct-length rep words even with in-word specials by
  // 1. get no of specials in word
  // 2. set fndrep word to this length
  // (content of fndrep.word is not of importance, only length is compared)
  size_t wlen = strlen(word) - getWordSpecialCnt(word);
  char w[wlen];
  memset(w, 'a', wlen);
  w[wlen] = '\0';

  repWord_t         *fndRep = new repWord_t(w, 0);
  repWord_t         *tmp = m_words->Head();

  // methodology:
  // 1) get all words that match length exactly
  // 2) if no words: check if there is a shorter word to use
  // 3) if no word:  check if there is a longer word to use
  // etc
  // -> this should always produce a usable word, except when there are no words

  bool (MultiReplacer::*p[5]) (repWord_t *, repWord_t *);
  bool (MultiReplacer::**func) (repWord_t *, repWord_t *);
  p[0] = &MultiReplacer::isExactMatch;
  p[1] = &MultiReplacer::isApproximateShorterMatch;
  p[2] = &MultiReplacer::isApproximateLongerMatch;
  p[3] = &MultiReplacer::isShorter;
  p[4] = &MultiReplacer::isLonger;
  func = p;

  while (potentials->Size() == 0) { // this is of course a bit risky, but logically after the third loop potentials MUST contain at least one element
    tmp = m_words->Head();

    while (tmp != NULL) {
      if ((this->**func)(fndRep, tmp)) {
        log_dbg("adding %s to potentials list for %s\n", tmp->word, word);
        potentials->add(tmp);
      } else {
        log_dbg("did not add %s as potential for %s\n", tmp->word, word);
      }
      tmp = m_words->Next();
    }
    func++;
  }

  *rep = potentials->get_random();
  delete potentials;
  delete fndRep;

  if (*rep == NULL) {
    printf("\033[0;31mError: found no rep string\033[0m\n");
    return false;
  }
  return true;
}

bool MultiReplacer::isExactMatch(repWord_t *orig, repWord_t *rep) {
  return (*orig == *rep);
}

bool MultiReplacer::isApproximateShorterMatch(repWord_t *orig, repWord_t *rep) {
  if (isExactMatch(orig, rep) || (isLonger(orig, rep))) {
    return false;
  }

  size_t diff = strlen(orig->word) - strlen(rep->word);
  return (diff < 3);
}

bool MultiReplacer::isApproximateLongerMatch(repWord_t *orig, repWord_t *rep) {
  if (isExactMatch(orig, rep) || (isShorter(orig, rep))) {
    return false;
  }

  size_t diff = strlen(rep->word) - strlen(orig->word);
  return (diff < 3);
}

bool MultiReplacer::isShorter(repWord_t *orig, repWord_t *rep) {
  return (*rep < *orig);
}

bool MultiReplacer::isLonger(repWord_t *orig, repWord_t *rep) {
  return (*rep > *orig);
}

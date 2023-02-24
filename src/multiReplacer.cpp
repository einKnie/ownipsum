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

bool MultiReplacer::replaceWord(char *word) {
  repWord_t *rep;
  if (!findRepStr(word, &rep)) {
    printf("Error: Could not find a suitable rep string for word %s\n", word);
    return false;
  }

  if (!setText(rep->word, rep->idx)) {
    printf("Error: Failed to set replacement word %s(%d)\n", rep->word, rep->idx);
    return false;
  }

  replaceSingleWord(word);
  return true;
}

bool MultiReplacer::findRepStr(char *word, repWord_t **rep) {
  LiList<repWord_t> *potentials = new LiList<repWord_t>();

  // find correct-length rep words even with in-word specials by
  // 1. get no of specials in word
  // 2. set fndrep word to this length (i.e. total length - no of spaecials - no of multibyte characters)
  // (content of fndrep.word is not of importance, only length is compared)
  size_t wlen = strlen(word) - getWordSpecialCnt(word) - getMultibyteChars(word);
  char w[wlen];
  memset(w, 'a', wlen);
  w[wlen] = '\0';

  repWord_t         *fndRep = new repWord_t(w, 0);
  repWord_t         *tmp = m_words->Head();

  if (tmp == NULL) {
    return false;
  }

  // methodology:
  // this is probably a bit overkill but I felt like it.
  // create an array of function pointers to keep everything in a single (though nested) loop;
  // result: first (outer) loop iteration, check if words match exactly,
  //         second iteration, check if words match partially with shorter reps preferred,
  //         third iteration, check if words match partially with longer reps 'preferred',
  //         lastly, check if any shorter rep words exist, if this also fails, take a longer rep word.
  // As soon as at least one valid replacement is found, exit the loop.
  // This approach will lead to a guaranteed result, except
  // when the rep list is empty (but this is checked above)

  bool (MultiReplacer::*p[5]) (repWord_t *, repWord_t *);
  bool (MultiReplacer::**func) (repWord_t *, repWord_t *);
  p[0] = &MultiReplacer::isExactMatch;
  p[1] = &MultiReplacer::isApproximateShorterMatch;
  p[2] = &MultiReplacer::isApproximateLongerMatch;
  p[3] = &MultiReplacer::isShorter;
  p[4] = &MultiReplacer::isLonger;
  func = p;

  while (potentials->Size() == 0) { // this is of course a bit risky, but logically after the last loop potentials MUST contain at least one element
    tmp = m_words->Head();

    while (tmp != NULL) {
      if ((this->**func)(fndRep, tmp)) {
        log_dbg("adding %s to potentials list for %s\n", tmp->word, word);
        potentials->addSorted(tmp);
      } else {
        log_dbg("did not add %s as potential for %s\n", tmp->word, word);
      }
      tmp = m_words->Next();
    }
    func++;
  }

  // choose one of the found matches (depending on last iteration)
  func--;
  if ((*func == p[3]) || (*func == p[1])) {
    // get the longest of the shorter matches
    *rep = potentials->Tail();
  } else if ((func == &p[2]) || (func == &p[4])) {
    // get the shortest of the longer matches
    *rep = potentials->Head();
  } else {
    // get any of the exact matches
    *rep = potentials->get_random();
  }
  delete potentials;
  delete fndRep;

  if (*rep == NULL) {
    log_dbg("found no rep string\n");
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

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

  repWord_t *fndRep = new repWord_t();
  strncpy(fndRep->word, word, sizeof(fndRep->word));
  fndRep->idx = 0;

  LiList<repWord_t> *potentials = new LiList<repWord_t>();
  repWord_t *tmp = m_words->Head();
  // printf("%d replacement words\n", m_words->Size());
  while (tmp != NULL) {
    if ((*tmp == *fndRep) || ((uint8_t)(strlen(word) - strlen(tmp->word)) < 3)) {
      // printf("adding %s to potentials list for %s\n", tmp->word, word);
      potentials->add(tmp);
    } else {
      // printf("did not add %s as potential for %s\n", tmp->word, word);
    }
    tmp = m_words->Next();
  }

  if (potentials->Size() == 0) {
    // printf("Got no replacement for %s\n", word);
    // printf("\n");

    *rep = m_words->findPrevSorted(fndRep);
  } else {
    *rep = potentials->get_random();
  }

  // *rep = m_words->findPrevSorted(fndRep);
  delete potentials;
  if (*rep == NULL) {
    printf("error: found no rep string\n");
    return false;
  }
  return true;
}

bool MultiReplacer::addWord(repWord_t &rep) {

  return false;
}

bool MultiReplacer::remWord(repWord_t &rep) {

  return false;
}

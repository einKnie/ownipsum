#include "replacer.h"
#include <stdlib.h>
#include <string.h>

Replacer::Replacer(unsigned int maxLen) : ReplacerBase(maxLen) {
  log_dbg("created single-word replacer\n");
}

Replacer::Replacer() : ReplacerBase() {
  log_dbg("Created default single-word replacer\n");
}

Replacer::~Replacer() {
  log_dbg("destroyed single-word replacer\n");
}

bool Replacer::replaceWord(char *word) {
  replaceSingleWord(word);
  return true;
}

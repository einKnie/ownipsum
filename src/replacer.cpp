#include "replacer.h"
#include <stdlib.h>
#include <string.h>

Replacer::Replacer(unsigned int maxLen) {
  m_maxLen = maxLen;
  m_output = (char*)malloc(m_maxLen * sizeof(char));

  // set defaults
  sprintf(m_start, "%s", START);
  memset(m_middle, MID[0], sizeof(m_middle) - 1); // use memset to fill entire array
  sprintf(m_end,   "%s", END);
  m_vowel = &m_middle[0];

  log_dbg("Created replacer w/ max length %d\n", m_maxLen);
}

Replacer::Replacer() : Replacer(MAXLEN) {
  log_dbg("Created default replacer\n");
}

Replacer::~Replacer() {
  if (m_output) free(m_output);
}

bool Replacer::setText(const char *str, unsigned int vIdx) {

  unsigned int wlen = strlen(str);
  char text[wlen + 1] = {'\0'};
  memcpy(text, str, wlen + 1);

  if (!isValidReplacement(text)) {
    printf("Error: Replacement word is invalid.\nFalling back to sane defaults\n");
    return false;
  }

  log_dbg("setting long text \"%s\"\n", text);
  memset(m_start, '\0', sizeof(m_start));
  memset(m_middle,'\0', sizeof(m_middle));
  memset(m_end,   '\0', sizeof(m_end));

  if (vIdx == 0) {
    // vowel at beginning; two chunks only
    memset(m_start, text[vIdx], sizeof(m_start) - 1);
    snprintf(m_middle, wlen, "%s", &text[vIdx + 1]);
    m_vowel = m_start;
  } else if (vIdx == (wlen - 1)) {
    // vowel at the end; two chunks
    snprintf(m_middle, wlen, "%s", &text[0]);
    memset(m_end, text[vIdx], sizeof(m_end) - 1);
    m_vowel = m_end;
  } else {
    // vowel somewhere in the middle; three chunks
    snprintf(m_start, vIdx + 1, "%s", &text[0]);
    memset(m_middle, text[vIdx], sizeof(m_middle) -1);
    snprintf(m_end, wlen - vIdx, "%s", &text[vIdx + 1]);
    m_vowel = m_middle;
  }

  log_dbg("set replacement word to %s %s %s, with %s being the vowel\n", m_start, m_middle, m_end, m_vowel);
  return true;
}

char* Replacer::change(const char *fmt) {

  char word[m_maxLen];  // world length is same as line length b/c we could have just one 'word'
  states_e state = EInit;
  int8_t ccnt = 0;
  int8_t wcnt = 0;

  while (state != EDone) {

    switch (state) {
      case EInit:
        log_dbg("in state EInit\n");
        ccnt = 0;
        memset(m_output, 0, m_maxLen);
        memset(word, 0, sizeof(word));
        state = ECheck;
        break;

      case ECheck:
        log_dbg("in state ECheck: %c\n", fmt[ccnt]);
        if (fmt[ccnt] == '\0') {
          m_output[ccnt] = '\0';
          state = EDone;
        } else if (fmt[ccnt] == '%') {
          state = EFmt;
        } else if (isSpecial(fmt[ccnt])) {
          state = ESpecial;
        } else if (fmt[ccnt] != ' ') {
          state = EWord;
        } else {
          // basically to catch spaces
          m_output[ccnt] = fmt[ccnt];
          ccnt++;
        }
        break;

      case EFmt:
        log_dbg("in state EFmt\n");
        do {
          m_output[ccnt] = fmt[ccnt];
          ccnt++;
        } while (fmt[ccnt] != ' ');

        state = ECheck;
        break;

      case EWord:
        log_dbg("in state EWord\n");
        wcnt = 0;
        do {
          word[wcnt] = fmt[ccnt];
          wcnt++;
          ccnt++;
        } while ((fmt[ccnt] != ' ') && !isSpecial(fmt[ccnt]));
        word[wcnt] = '\0';

        // replace word
        log_dbg("word found: %s\n", word);
        replaceWord(word);

        // then add to m_output
        log_dbg("replaced with: %s\n", word);
        log_dbg("ccnt: %d\nwcnt: %d\ninsert at: %d\n", ccnt, wcnt, (ccnt - wcnt));
        memcpy(&m_output[ccnt - wcnt], word, wcnt);

        state = ECheck;
        break;

      case ESpecial:
        log_dbg("in state ESpecial\n");
        do {
          m_output[ccnt] = fmt[ccnt];
          ccnt++;
        } while(isSpecial(fmt[ccnt]));
        state = ECheck;
        break;

      case EDone:
        log_dbg("in state EDone\n"); // never gets here
        break;

      default:
        log_dbg("Error: Invalid state!\n");
    } // switch

  } //while

#ifndef _NO_DEBUG_
  printf("%s\n", fmt);
#endif
  return m_output;
}

void Replacer::replaceWord(char *word) {
  uint8_t wlen = strlen(word);
  uint8_t special_offs = 0;

  if (wlen < 1) {
    log_dbg("word is no word\n");
    return;
  }

  if (getWordSpecialCnt(word) > 0) {
    // in case the word contains special characters (e.g. ")
    // we have to adapt the indices accordingly.
    // e.g. "mu(uuu)h" w/ repword mama(3) shall become "ma(maa)a"
    log_dbg("word contains special characters\n");
  }

  uint8_t vIdx = 0;
  uint8_t mid_idx = 0;
  uint8_t end_idx = 0;
  char c = '\0';

  // set letter indices depending on which one is the vowel
  if (m_vowel == &m_start[0]) {
    log_dbg("Vowel at start\n");

    // vowel index; check if word starts w/ special chars and inc accordingly
    vIdx = 0;
    while ( isWordSpecial(word[vIdx]) ) vIdx++;

    // rest of word; check if end contains special chars and adjust index accordingly
    mid_idx = wlen - strlen(m_middle);
    while (((wlen - mid_idx) - getWordSpecialFrom(word, mid_idx)) < (int)strlen(m_middle)) {
      log_dbg("wlen - mid_idx = %d, strlen(m_middle) = %lu, curr mid_idx = %d\n", (wlen-mid_idx), strlen(m_middle), mid_idx);
      mid_idx--;
      if ((mid_idx > 0) && isWordSpecial(word[mid_idx - 1])) mid_idx--;
      if (mid_idx == 0) break;
    }

    // finally, check if there is enough space for rest of word
    // e.g. "bla" w/ repword "aber" => "aaa" (not "abe" or "ber")
    if (mid_idx <= vIdx) mid_idx = wlen;
    end_idx = wlen;

  } else if (m_vowel == &m_middle[0]) {
    log_dbg("Vowel in middle\n");

    // determine when to start with vowel
    vIdx = strlen(m_start);
    mid_idx = vIdx;
    while ( (mid_idx - getWordSpecialUntil(word, mid_idx)) < vIdx) {
      mid_idx++;
    }

    // determine when to sart with end
    end_idx = wlen - strlen(m_end);
    while (( (wlen - end_idx) - getWordSpecialFrom(word, end_idx)) < (int)strlen(m_end)) {
      end_idx--;
      if (end_idx == 0) break;
    }

    if ( (end_idx <= mid_idx) || ((end_idx - mid_idx) < (getWordSpecialFromUntil(word, mid_idx, end_idx) + 1)) ) {
      end_idx = wlen;
    }

  } else if (m_vowel == &m_end[0]) {

    log_dbg("Vowel at end\n");
    vIdx = strlen(m_middle);
    mid_idx = 0;
    end_idx = vIdx;

    // determine end index depending on special chars
    while (( end_idx - getWordSpecialUntil(word, end_idx) ) < vIdx) {
      end_idx++;
    }

  } else {
    log_dbg("invalid vowel\n");
  }

  log_dbg("wlen: %d\nvowel Index: %d\nmid_idx: %d\nend_idx: %d\n",
          wlen, vIdx, mid_idx, end_idx);
  log_dbg("start len: %lu\nmid len: %lu\nend len: %lu\n",
          strlen(m_start), strlen(m_middle), strlen(m_end));

  for (uint8_t i = 0; i < wlen; i++) {

    if (isNumeric(word[i])) continue;
    if (isWordSpecial(word[i])) {
      continue;
    }
    if (i < mid_idx) {
      special_offs = getWordSpecialFromUntil(word, 0, i);
      c = m_start[i - special_offs];
      log_dbg("replaced start char %c\n", c);
    } else if ((i >= mid_idx) && (i < end_idx)) {
      special_offs = getWordSpecialFromUntil(word, mid_idx, i);
      c = m_middle[i - mid_idx - special_offs];
      log_dbg("replaced middle char %c\n", c);
    } else if (i >= end_idx) {
      special_offs = getWordSpecialFromUntil(word, end_idx, i);
      c = m_end[i - end_idx - special_offs];
      log_dbg("replaced end char %c\n", c);
    }

    special_offs = 0;
    word[i] = isUpper(word[i]) ? toUpper(c) : c;
    log_dbg("i: %d, wordlen: %lu\n", i, strlen(word));
  }
}

char Replacer::toUpper(char c) {
  if ((c < ASCII_LOWER_START) || (c > ASCII_LOWER_END)) {}
  else c -= 32;
  return c;
}

bool Replacer::isUpper(char c) {
  return ((c >= ASCII_UPPER_START) && (c <= ASCII_UPPER_END));
}

bool Replacer::isNumeric(char c) {
  return ((c >= ASCII_ZERO) && (c <= ASCII_NINE));
}

bool Replacer::isSpecial(char c) {

  if ( (c == '.') ||
       (c == ',') ||
       (c == '-') ||
       (c == '?') ||
       (c == '!') ||
       (c == '_') ||
       (c == ':') ||
       (c == ';') ||
       (c == '=') ||
       (c == '\n')||
       (c == '\t') ) return true;

  return false;
}

bool Replacer::isWordSpecial(char c) {
  if ( ( c == '\'') ||
       ( c == '\"') ||
       ( c == '(')  ||
       ( c == ')')  ) return true;
  return false;
}

uint8_t Replacer::getWordSpecialCnt(char *word) {
  uint8_t cnt = 0;
  for (size_t i = 0; i < strlen(word); i++) {
    if (isWordSpecial(word[i])) cnt++;
  }
  log_dbg("Found %d special chars in word\n", cnt);
  return cnt;
}

uint8_t Replacer::getWordSpecialUntil(char *word, uint8_t until) {
  uint8_t cnt = 0;
  cnt = getWordSpecialFromUntil(word, 0, until);
  log_dbg("Found %d special chars in word from start until idx %d\n", cnt, until);
  return cnt;
}

uint8_t Replacer::getWordSpecialFrom(char *word, uint8_t from) {
  uint8_t cnt = 0;
  cnt = getWordSpecialFromUntil(word, from, strlen(word));
  log_dbg("Found %d special chars in word from idx %d to end\n", cnt, from);
  return cnt;
}

uint8_t Replacer::getWordSpecialFromUntil(char *word, uint8_t from, uint8_t until) {
  uint8_t cnt = 0;
  for (size_t i = from; i < until; i++) {
    if ( word[i] == '\0') break;
    if (isWordSpecial(word[i])) cnt++;
  }
  log_dbg("Found %d special chars in word from idx %d to idx %d\n", cnt, from, until);
  return cnt;
}

bool Replacer::isValidReplacement(char *str) {
  log_dbg("Checking if \"%s\" is a valid replacement\n", str);
  char *c = &str[0];
  while (*c != '\0') {
    if (!isalpha(*c)) return false;
    c++;
  }
  return true;
}

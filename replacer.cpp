#include "replacer.h"
#include <stdlib.h>
#include <string.h>

Replacer::Replacer(unsigned int maxLen) {
  m_maxLen = maxLen;
  m_output = (char*)malloc(m_maxLen * sizeof(char));

  m_start = 'm';
  m_middle = 'u';
  m_end = 'h';
  m_vowel = &m_middle;

  log_dbg("Created replacer w/ max length %d\n", m_maxLen);
}

Replacer::Replacer() : Replacer(MAXLEN) {
  log_dbg("Created default replacer\n");
}

Replacer::~Replacer() {
  if (m_output) free(m_output);
}

bool Replacer::setText(const char start, const char middle, const char end, int vIdx) {
  m_start = start;
  m_middle = middle;
  m_end = end;

  switch(vIdx) {
    case 0: m_vowel = &m_start; break;
    case 1: m_vowel = &m_middle; break;
    case 2: m_vowel = &m_end; break;
    default: log_dbg("error: invalid vowel\n"); m_vowel = &m_start; return false;
  }
  log_dbg("set replacement word to: %c%c%c\n", m_start, m_middle, m_end);
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

  return m_output;
}

void Replacer::replaceWord(char *word) {
  uint8_t wlen = strlen(word);

  if (wlen < 1) {
    log_dbg("word is no word\n");
    return;
  }

  uint8_t mid_idx = 1;
  uint8_t end_idx = wlen - 1;
  char c = '\0';

  // set letter indices depending on which one is the vowel
  if (m_vowel == &m_start) {
    mid_idx = wlen - 2;
    end_idx = wlen - 1;
  } else if (m_vowel == &m_middle) {
    mid_idx = 1;
    end_idx = wlen - 1;
  } else if (m_vowel == &m_end) {
    mid_idx = 1;
    end_idx = 2;
  } else {
    log_dbg("invalid vowel\n");
  }

  // hack to make sure a two-letter word is replaces as 'mu' instead of 'mh'
  // TODO: improve
  if (wlen < 3) {
    end_idx = 3;
  }

  log_dbg("wlen: %d\nmid_idx: %d\nend_idx: %d\n",
          wlen, vowel_idx, mid_idx, end_idx);

  for (uint8_t i = 0; i < wlen; i++) {

    if (isNumeric(word[i])) continue;

    if (i < mid_idx) {
      c = m_start;
    } else if ((i >= mid_idx) && (i < end_idx)) {
      c = m_middle;
    } else {
      c = m_end;
    }

    word[i] = isUpper(word[i]) ? toUpper(c) : c;
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

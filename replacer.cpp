#include "replacer.h"
#include <stdlib.h>

Replacer::Replacer(unsigned int maxLen) {
  m_maxLen = maxLen;
  m_output = (char*)malloc(m_maxLen * sizeof(char));

  // m_startChar = 'm';
  // m_longVowel = 'u';
  // m_endChar = 'h';

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

// bool Replacer::setText(const char start, const char vowel, const char end) {
//   m_startChar = start;
//   m_longVowel = vowel;
//   m_endChar = end;
//   log_dbg("set replacement word to: %c%c%c\n", m_startChar, m_longVowel, m_endChar);
//   return true;
// }

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
    log_dbg("word shorter than startchar\n");
    return;
  }

  uint8_t vowel_idx = 1;
  uint8_t vowel_len = wlen - 2;
  uint8_t mid_idx = 2;
  uint8_t end_idx = wlen - 1;
  char c = '\0';

  log_dbg("wlen: %d\nvowel_idx: %d\nvowel_len: %d\nend_idx: %d\n",
          wlen, vowel_idx, vowel_len, end_idx);

  // for (uint8_t i = 0; i < wlen; i++) {
  //
  //   if (isNumeric(word[i])) continue;
  //
  //   if (i < 1) {
  //     c = m_startChar;
  //   } else if ((i >= end_idx) && (wlen > 2)) {
  //     c = m_endChar;
  //   } else {
  //     c = m_longVowel;
  //   }
  //
  //   word[i] = isUpper(word[i]) ? toUpper(c) : c;
  // }

  if (m_vowel == &m_start) {
    vowel_idx = 0;
    mid_idx = wlen - 2;
    end_idx = wlen - 1;
  } else if (m_vowel == &m_middle) {
    vowel_idx = 1;
    mid_idx = 1;
    end_idx = wlen - 1;
  } else if (m_vowel == &m_end) {
    vowel_idx = 2;
    mid_idx = 1;
    end_idx = 2;
  }

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

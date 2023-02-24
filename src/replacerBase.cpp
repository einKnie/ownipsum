#include "replacer.h"
#include <stdlib.h>
#include <string.h>

ReplacerBase::ReplacerBase(unsigned int maxLen) {
  m_maxLen  = maxLen;
  m_output  = NULL;

  m_start   = (char*)malloc(m_maxLen * sizeof(char));
  m_middle  = (char*)malloc(m_maxLen * sizeof(char));
  m_end     = (char*)malloc(m_maxLen * sizeof(char));
  setDefaultText();

  log_dbg("Created replacer base w/ max length %d\n", m_maxLen);
}

ReplacerBase::ReplacerBase() : ReplacerBase(MAXLEN) {
  log_dbg("Created default replacer base\n");
}

ReplacerBase::~ReplacerBase() {
  if (m_output) free(m_output);
  if (m_start)  free(m_start);
  if (m_middle) free(m_middle);
  if (m_end)    free(m_end);
  log_dbg("Destroyed replacer base\n");
}

void ReplacerBase::setDefaultText() {
  memset(m_start,  '\0', m_maxLen);
  memset(m_middle, '\0', m_maxLen);
  memset(m_end,    '\0', m_maxLen);

  sprintf(m_start, "%s", START);
  memset (m_middle, MID[0], m_maxLen - 1); // use memset to fill entire array

  sprintf(m_end,   "%s", END);
  m_vowel = m_middle;
}

bool ReplacerBase::setText(const char *str, unsigned int vIdx) {

  unsigned int wlen = strlen(str);
  char text[wlen + 1] = {'\0'};
  memcpy(text, str, wlen);

  if (!isValidReplacement(text)) {
    printf("Error: Replacement word \"%s\" is invalid.\nFalling back to sane defaults\n", str);
    setDefaultText();
    return false;
  }

  log_dbg("setting long text \"%s\"\n", text);
  memset(m_start, '\0', m_maxLen);
  memset(m_middle,'\0', m_maxLen);
  memset(m_end,   '\0', m_maxLen);

  if (vIdx == 0) {
    // vowel at beginning; two chunks only
    memset(m_start, text[vIdx], m_maxLen - 1);
    snprintf(m_middle, wlen, "%s", &text[vIdx + 1]);
    m_vowel = m_start;
  } else if (vIdx == (wlen - 1)) {
    // vowel at the end; two chunks
    snprintf(m_middle, wlen, "%s", &text[0]);
    memset(m_end, text[vIdx], m_maxLen - 1);
    m_vowel = m_end;
  } else {
    // vowel somewhere in the middle; three chunks
    snprintf(m_start, vIdx + 1, "%s", &text[0]);
    memset(m_middle, text[vIdx], m_maxLen -1);
    snprintf(m_end, wlen - vIdx, "%s", &text[vIdx + 1]);
    m_vowel = m_middle;
  }

  log_dbg("set replacement word to %s %s %s, with %.1s being the vowel\n", m_start, m_middle, m_end, m_vowel);
  return true;
}

char* ReplacerBase::change(const char *fmt) {

  char     *word = NULL;
  states_e state = EInit;
  int16_t  ccnt = 0;
  int16_t  wcnt = 0;
  size_t   len = ((strlen(fmt) + 1) * sizeof(char));
  size_t   lenred = 0;
  size_t   lenred_total = 0;

  while (state != EDone) {

    switch (state) {
      case EInit:
        log_dbg("in state EInit\n");
        m_output = (char*)realloc(m_output, len);
        word     = (char*)realloc(word, len);

        if ((m_output == NULL) || (word == NULL)) {
          printf("Error: Failed to reallocate memory\n");
          exit(1);
        }

        memset(m_output, 0, len);
        memset(word, 0, len);
        state = ECheck;
        ccnt  = 0;

        break;

      case ECheck:
        log_dbg("in state ECheck: %c\n", fmt[ccnt]);
        if (fmt[ccnt] == '\0') {
          m_output[ccnt - lenred_total] = '\0';
          state = EDone;
        } else if (isFormat(fmt[ccnt])) {
          state = EFmt;
        } else if (isSpecial(fmt[ccnt])) {
          state = ESpecial;
        } else if (isWord(fmt[ccnt])) {
          state = EWord;
        } else if (isSpace(fmt[ccnt])) {
          m_output[ccnt - lenred_total] = fmt[ccnt];
          ccnt++;
        } else {
          printf("Error: Don't know how to interpret %c in input\n", fmt[ccnt]);
          exit(1);
        }
        break;

      // TODO feeling cute, might deprecate later
      case EFmt:
        log_dbg("in state EFmt\n");
        do {
          m_output[ccnt - lenred_total] = fmt[ccnt];
          ccnt++;
        } while ((fmt[ccnt] != ' ') && (fmt[ccnt] != '\0'));

        state = ECheck;
        break;

      case EWord:
        log_dbg("in state EWord\n");
        wcnt = 0;
        do {
          word[wcnt++] = fmt[ccnt++];
        } while (isWord(fmt[ccnt]));
        word[wcnt] = '\0';

        // replace word
        lenred = getMultibyteChars(word);
        log_dbg("word found: %s\n", word);
        replaceWord(word);

        // then add to output and take into account the length reduction from multibyte chars in input
        log_dbg("replaced with: %s\n", word);
        log_dbg("ccnt: %d\nwcnt: %ld\ninsert at: %ld\n", ccnt, wcnt - lenred, (ccnt - wcnt - lenred_total));
        log_dbg("length reduction in this word: %ld\nlength reduction total: %ld\n", lenred, lenred_total);
        memcpy(&m_output[ccnt - wcnt - lenred_total], word, wcnt - lenred);
        lenred_total += lenred;

        state = ECheck;
        break;

      case ESpecial:
        log_dbg("in state ESpecial\n");
        do {
          m_output[ccnt - lenred_total] = fmt[ccnt];
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

  log_dbg("%s\n", fmt);
  free(word);
  return m_output;
}

void ReplacerBase::replaceSingleWord(char *word) {
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

  int n = 0;
  if ((n = getMultibyteChars(word)) > 0) {
    log_dbg("%s has %d multibyte chars\n", word, n);
    wlen -= n;
  }

  uint8_t vIdx = 0;
  uint8_t mid_idx = 0;
  uint8_t end_idx = 0;
  char c = '\0';

  // set letter indices depending on which one is the vowel
  if (m_vowel == m_start) {
    log_dbg("Vowel at start\n");

    // vowel index; check if word starts w/ special chars and inc accordingly
    vIdx = 0;
    while ( isWordSpecial(word[vIdx]) ) vIdx++;

    // rest of word; check if end contains special chars and adjust index accordingly
    mid_idx = wlen - strlen(m_middle);
    while ((mid_idx > 0) && (((wlen - mid_idx) - getWordSpecialFrom(word, mid_idx)) < (int)strlen(m_middle))) {
      log_dbg("wlen - mid_idx = %d, strlen(m_middle) = %lu, curr mid_idx = %d\n", (wlen - mid_idx), strlen(m_middle), mid_idx);
      mid_idx--;
    }

    // finally, check if there is enough space for rest of word
    // e.g. "bla" w/ repword "aber" => "aaa" (not "abe" or "ber")
    if (mid_idx <= vIdx) mid_idx = wlen;
    end_idx = wlen;

  } else if (m_vowel == m_middle) {
    log_dbg("Vowel in middle\n");

    // determine when to start with vowel
    vIdx = strlen(m_start);
    mid_idx = vIdx;
    while ((mid_idx - getWordSpecialUntil(word, mid_idx)) < vIdx) {
      mid_idx++;
    }

    // determine when to start with end
    end_idx = wlen - strlen(m_end);
    while (((wlen - end_idx) - getWordSpecialFrom(word, end_idx)) < (int)strlen(m_end)) {
      end_idx--;
      if (end_idx == 0) break;
    }

    // check if there's still enough room for end part
    if ((end_idx <= mid_idx) || ((end_idx - mid_idx) < (getWordSpecialFromUntil(word, mid_idx, end_idx) + 1))) {
      end_idx = wlen;
    }

  } else if (m_vowel == m_end) {

    log_dbg("Vowel at end\n");
    vIdx = strlen(m_middle);
    mid_idx = 0;
    end_idx = vIdx;

    // determine end index depending on special chars
    while ((end_idx - getWordSpecialUntil(word, end_idx)) < vIdx) {
      end_idx++;
    }

  } else {
    log_dbg("invalid vowel\n");
    return;
  }

  log_dbg("wlen: %d\nvowel Index: %d\nmid_idx: %d\nend_idx: %d\n",
          wlen, vIdx, mid_idx, end_idx);
  log_dbg("start len: %lu\nmid len: %lu\nend len: %lu\n",
          strlen(m_start), strlen(m_middle), strlen(m_end));

  // replace word char for char
  uint8_t i, j = 0;
  for (i = 0, j = 0; i < strlen(word) && j < wlen; i++, j++) {

    if (isWordSpecial(word[i])) continue;

    if (j < mid_idx) {
      special_offs = getWordSpecialFromUntil(word, 0, i);
      c = m_start[j - special_offs];
      log_dbg("replaced start char %c\n", c);
    } else if ((j >= mid_idx) && (j < end_idx)) {
      special_offs = getWordSpecialFromUntil(word, mid_idx, i);
      c = m_middle[j - mid_idx - special_offs];
      log_dbg("replaced middle char %c\n", c);
    } else if (j >= end_idx) {
      special_offs = getWordSpecialFromUntil(word, end_idx, i);
      c = m_end[j - end_idx - special_offs];
      log_dbg("replaced end char %c\n", c);
    }

    if (isMultibyteChar(word[i])) {
      log_dbg("multibyte char at idx %d\n", i);
      i++;
    }

    special_offs = 0;
    word[j] = isUpper(word[j]) ? toUpper(c) : c;
    log_dbg("i: %d, j: %d, wordlen: %lu\n", i, j, strlen(word));
  }

  log_dbg("END OF WORD: i = %d, j = %d\n", i, j);
  if (wlen < strlen(word)) {
    word[j] = '\0';
  }

}

char ReplacerBase::toUpper(char c) {
  if ((c < ASCII_LOWER_START) || (c > ASCII_LOWER_END)) {}
  else c -= 32;
  return c;
}

bool ReplacerBase::isUpper(char c) {
  return ((c >= ASCII_UPPER_START) && (c <= ASCII_UPPER_END));
}

bool ReplacerBase::isNumeric(char c) {
  return ((c >= ASCII_ZERO) && (c <= ASCII_NINE));
}

bool ReplacerBase::isWord(char c) {
  return (isSpecial(c) || isFormat(c) || isSpace(c) || (c == '\0')) ? false : true;
}

bool ReplacerBase::isFormat(char c) {
  return (c == '%') ? true : false;
}

bool ReplacerBase::isSpace(char c) {
  return (c == ' ') ? true : false;
}

// todo: find some meaningful way to decide && discern which chars are
// 'special' and which are 'word special' because none of this is based on any reasoning

bool ReplacerBase::isSpecial(char c) {

  if ( (c == '.') ||
       (c == ',') ||
       (c == '-') ||
       (c == '?') ||
       (c == '!') ||
       (c == '+') ||
       (c == '*') ||
       (c == '_') ||
       (c == ':') ||
       (c == ';') ||
       (c == '=') ||
       (c == '{') ||
       (c == '}') ||
       // (c == '(') ||
       // (c == ')') ||
       (c == '\n')||
       (c == '\t') ) return true;

  return false;
}

bool ReplacerBase::isWordSpecial(char &c) {
  if ( (isNumeric(c)) ||
       ( c == '\'') ||
       ( c == '\"') ||
       ( c == '#')  ||
       ( c == '/')  ||
       ( c == '\\') ||
       ( c == '[')  ||
       ( c == ']')  ||
       ( c == '&')  ||
       ( c == '<')  ||
       ( c == '>')  ||
       ( c == '(')  ||
       ( c == ')')  ) return true;
  return false;
}

uint8_t ReplacerBase::getWordSpecialCnt(char *word) {
  uint8_t cnt = 0;
  for (size_t i = 0; i < strlen(word); i++) {
    if (isWordSpecial(word[i])) cnt++;
  }
  log_dbg("Found %d special chars in word\n", cnt);
  return cnt;
}

uint8_t ReplacerBase::getWordSpecialUntil(char *word, uint8_t until) {
  uint8_t cnt = 0;
  cnt = getWordSpecialFromUntil(word, 0, until);
  log_dbg("Found %d special chars in word from start until idx %d\n", cnt, until);
  return cnt;
}

uint8_t ReplacerBase::getWordSpecialFrom(char *word, uint8_t from) {
  uint8_t cnt = 0;
  cnt = getWordSpecialFromUntil(word, from, strlen(word));
  log_dbg("Found %d special chars in word from idx %d to end\n", cnt, from);
  return cnt;
}

uint8_t ReplacerBase::getWordSpecialFromUntil(char *word, uint8_t from, uint8_t until) {
  uint8_t cnt = 0;
  for (size_t i = from; i < until; i++) {
    if ( word[i] == '\0') break;
    if (isWordSpecial(word[i])) cnt++;
  }
  log_dbg("Found %d special chars in word from idx %d to idx %d\n", cnt, from, until);
  return cnt;
}

bool ReplacerBase::isValidReplacement(char *str) {
  log_dbg("Checking if \"%s\" is a valid replacement\n", str);
  char *c = &str[0];
  while (*c != '\0') {
    if (!isalpha(*c)) return false;
    c++;
  }
  return true;
}

int ReplacerBase::getMultibyteChars(char *word) {
  size_t len = strlen(word);
  int n = 0;
  for (size_t i = 0; i < len; i++) {
    if (word[i] & 0x80) {
#ifndef _NO_DEBUG_
      char mb[3] = { word[i], word[i+1], '\0'};
      log_dbg("0x%X 0x%X -> %s\n", word[i], word[i+1], mb);
#endif
      n++;
      i++;
    }
  }
  return n;
}

bool ReplacerBase::isMultibyteChar(char c) {
  return (c & 0x80);
}

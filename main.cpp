#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define MAX_WORD_LEN 50
#define MAX_LINE_LEN 200

#define ASCII_LOWER_START 97
#define ASCII_LOWER_END   122
#define ASCII_UPPER_START 65
#define ASCII_UPPER_END   90
#define ASCII_ZERO        48
#define ASCII_NINE        57

#define STARTCHAR 'm'
#define LONGVOWEL 'u'
#define ENDCHAR   'h'

#define TESTSTR "Oh hallo Lülü, wie gehts?"

#define log_dbg printf

#define _NO_DEBUG_
#ifdef _NO_DEBUG_
  #undef log_dbg
  #define log_dbg(...)
#endif

typedef enum states {
  EInit,
  EFmt,
  EWord,
  ESpecial,
  ECheck,
  EDone
} states_e;

char toUpper(char c) {

  if ((c < ASCII_LOWER_START) || (c > ASCII_LOWER_END)) {}
  else c -= 32;

  return c;
}

bool isUpper(char c) {
  return ((c >= ASCII_UPPER_START) && (c <= ASCII_UPPER_END));
}

bool isNumeric(char c) {
  return ((c >= ASCII_ZERO) && (c <= ASCII_NINE));
}

uint8_t checkSpecial(char c) {

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
       (c == '\t') ) return 1;

  return 0;
}

 void replaceWord(char *word) {
  uint8_t wlen = strlen(word);

  if (wlen < 1) {
    log_dbg("word shorter than startchar\n");
    return;
  }

  uint8_t vowel_idx = 1;
  uint8_t vowel_len = wlen - 2;
  uint8_t end_idx = wlen -1;
  char c = '\0';

  log_dbg("wlen: %d\nvowel_idx: %d\nvowel_len: %d\nend_idx: %d\n",
          wlen, vowel_idx, vowel_len, end_idx);

  for (uint8_t i = 0; i < wlen; i++) {

    if (isNumeric(word[i])) continue;

    if (i < 1) {
      c = STARTCHAR;
    } else if ((i >= end_idx) && (wlen > 2)) {
      c = ENDCHAR;
    } else {
      c = LONGVOWEL;
    }

    word[i] = isUpper(word[i]) ? toUpper(c) : c;
  }

}

char* change(const char *fmt, char* output) {

  char word[MAX_WORD_LEN];
  states_e state = EInit;
  int8_t ccnt = 0;
  int8_t wcnt = 0;

  while (state != EDone) {

    switch (state) {
      case EInit:
        log_dbg("in state EInit\n");
        ccnt = 0;
        memset(output, 0, MAX_LINE_LEN);
        memset(word, 0, sizeof(word));
        state = ECheck;
        break;

      case ECheck:
        log_dbg("in state ECheck: %c\n", fmt[ccnt]);
        if (fmt[ccnt] == '\0') {
          output[ccnt] = '\0';
          state = EDone;
        } else if (fmt[ccnt] == '%') {
          state = EFmt;
        } else if (checkSpecial(fmt[ccnt])) {
          state = ESpecial;
        } else if (fmt[ccnt] != ' ') {
          state = EWord;
        } else {
          // basically to catch spaces
          output[ccnt] = fmt[ccnt];
          ccnt++;
        }
        break;

      case EFmt:
        log_dbg("in state EFmt\n");
        do {
          output[ccnt] = fmt[ccnt];
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
        } while ((fmt[ccnt] != ' ') && !checkSpecial(fmt[ccnt]));
        word[wcnt] = '\0';

        // replace word
        replaceWord(word);

        // then add to output
        log_dbg("word found: %s\n", word);
        log_dbg("ccnt: %d\nwcnt: %d\ninsert at: %d\n", ccnt, wcnt, (ccnt - wcnt));
        memcpy(&output[ccnt - wcnt], word, wcnt);

        state = ECheck;
        break;

      case ESpecial:
        log_dbg("in state ESpecial\n");
        do {
          output[ccnt] = fmt[ccnt];
          ccnt++;
        } while(checkSpecial(fmt[ccnt]));
        state = ECheck;
        break;

      case EDone:
        log_dbg("in state EDone\n"); // never gets here
        break;

      default:
        log_dbg("Error: Invalid state!\n");
    } // switch

  } //while

  return output;
}

int main(void) {

  char output[MAX_LINE_LEN];
  printf("%s\n", change(TESTSTR, output));

  printf("%s",   change("Ich weiss gar nicht, wieso %s sich so ziert!\n", output));
  printf("%s\n", change("Um 15:00 gegen Norden ziehen, das wärs...", output));
  printf("%s\n", change("Alles nur nicht 17:45 am Sonntag! Du hast deine %d Tassen verloren?", output));

  return 0;
}

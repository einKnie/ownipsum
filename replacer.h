// text replacer class

#ifndef _REPLACER_H_
#define _REPLACER_H_

#include <stdint.h>
#include <stdio.h>

#define MAXLEN 200

#define ASCII_LOWER_START 97
#define ASCII_LOWER_END   122
#define ASCII_UPPER_START 65
#define ASCII_UPPER_END   90
#define ASCII_ZERO        48
#define ASCII_NINE        57

#define STARTCHAR 'm'
#define LONGVOWEL 'u'
#define ENDCHAR   'h'

#define log_dbg printf

#define _NO_DEBUG_
#ifdef _NO_DEBUG_
  #undef log_dbg
  #define log_dbg(...)
#endif

class Replacer {
private:

  typedef enum states {
    EInit,
    EFmt,
    EWord,
    ESpecial,
    ECheck,
    EDone
  } states_e;

  char m_start;
  char m_middle;
  char m_end;
  char *m_vowel;

  unsigned int m_maxLen;
  char *m_output; // placeholder for current replaced text

  // return uppercase version of input char
  char toUpper(char c);
  // check if input char is uppercase
  bool isUpper(char c);
  // check if input char is numeric
  bool isNumeric(char c);
  // check if input char is special character
  bool isSpecial(char c);

  void replaceWord(char *word);


public:
  Replacer();
  Replacer(unsigned int maxLen);
  ~Replacer();

  // set the replacement string.
  // each char must be given individually, vIdx should be the index of the long vowel (start = 0)
  // TODO: improve
  bool setText(const char start, const char middle, const char end, int vIdx);

  // return a modified version of the input string
  char* change(const char *fmt);

};


#endif // !_REPLACER_H_

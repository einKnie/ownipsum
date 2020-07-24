#ifndef _REPLACER_H_
#define _REPLACER_H_

#include "replacerBase.h"

// #include <stdint.h>
// #include <stdio.h>
// #include <ctype.h>
//
// #define MAXLEN 200
// #define ASCII_LOWER_START 97
// #define ASCII_LOWER_END   122
// #define ASCII_UPPER_START 65
// #define ASCII_UPPER_END   90
// #define ASCII_ZERO        48
// #define ASCII_NINE        57
//
// #define START "m"
// #define MID   "u"
// #define END   "h"
//
// #define _NO_DEBUG_
// #ifdef _NO_DEBUG_
//   #define log_dbg(...)
// #else
//   #define log_dbg printf
// #endif

class Replacer : public ReplacerBase {
// private:
//
//   typedef enum states {
//     EInit,
//     EFmt,
//     EWord,
//     ESpecial,
//     ECheck,
//     EDone
//   } states_e;
//
//   char m_start[MAXLEN];
//   char m_middle[MAXLEN];
//   char m_end[MAXLEN];
//   char *m_vowel;
//
//   unsigned int m_maxLen;
//   char *m_output; // placeholder for current replaced text
//
//   // revert to default replacement text
//   void setDefaultText();
//
//   // return uppercase version of input char
//   char toUpper(char c);
//   // check if input char is uppercase
//   bool isUpper(char c);
//   // check if input char is numeric
//   bool isNumeric(char c);
//   // check if input char is special character
//   bool isSpecial(char c);
//   // check if input char is word-internal special character (e.g. ')
//   bool isWordSpecial(char c);
//
//   uint8_t getWordSpecialFrom(char *word, uint8_t from);
//   uint8_t getWordSpecialUntil(char *word, uint8_t until);
//   uint8_t getWordSpecialFromUntil(char *word, uint8_t from, uint8_t until);
//   // count number of special chars inside word
//   uint8_t getWordSpecialCnt(char *word);
//   // check if a string is a valid word to be used as replacement
//   bool isValidReplacement(char *str);

protected:
  // replace a single word with the set replacement string
  void replaceWord(char *word);

public:
  Replacer();
  Replacer(unsigned int maxLen);
  ~Replacer();

  // /// @brief set replacement text
  // /// @param [in] str replacemet word
  // /// @param [in] vIdx index of the long vowel
  // /// @return true on success, else false
  // bool setText(const char *str, unsigned int vIdx);
  //
  // // return a modified version of the input string
  // char* change(const char *fmt);

};


#endif // !_REPLACER_H_

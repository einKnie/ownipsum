#ifndef _REPLACER_BASE_H_
#define _REPLACER_BASE_H_

#include <stdint.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_REPLEN 30

#define MAXLEN 200
#define ASCII_LOWER_START 97
#define ASCII_LOWER_END   122
#define ASCII_UPPER_START 65
#define ASCII_UPPER_END   90
#define ASCII_ZERO        48
#define ASCII_NINE        57

#define START "m"
#define MID   "u"
#define END   "h"

#define _NO_DEBUG_
#ifdef _NO_DEBUG_
  #define log_dbg(...)
#else
  #define log_dbg printf
#endif

class ReplacerBase {

protected:
    typedef enum states {
      EInit,
      EFmt,
      EWord,
      ESpecial,
      ECheck,
      EDone
    } states_e;

    char *m_start;
    char *m_middle;
    char *m_end;

    char *m_vowel;

    unsigned int m_maxLen;
    char *m_output;         ///< storage for current replaced text

    /// @brief Set the default replacement text
    void setDefaultText();

    /// @brief Return uppercase version of char \a c
    /// @param [in] c
    /// @return uppercase representation of \a c
    char toUpper(char c);

    /// @brief Check if char \a c is uppercase
    /// @param [in] c
    /// @return true if \a c is uppercase
    bool isUpper(char c);

    /// @brief Check if char \a c is numeric
    /// @param [in] c
    /// @return true if \a c is numeric
    bool isNumeric(char c);

    /// @brief Check if char \a c is a special character
    /// @param [in] c
    /// @return true if \a c is special
    bool isSpecial(char c);
    bool isWord(char c);
    bool isFormat(char c);
    bool isSpace(char c);

    /// @brief Check if char \a c is a word-internal special character
    /// @param [in] c
    /// @return true if \a c is special
    bool isWordSpecial(char &c);

    /// @brief Return number of word-internal special chars in \a word
    ///        from index \a from until the end of the string
    /// @param [in] word the string to search
    /// @param [in] from start index
    /// @return number of special chars found
    uint8_t getWordSpecialFrom(char *word, uint8_t from);

    /// @brief Return number of word-internal special chars in \a word
    ///        from the beginning to index \a until
    /// @param [in] word the string to search
    /// @param [in] until end index
    /// @return number of special chars found
    uint8_t getWordSpecialUntil(char *word, uint8_t until);

    /// @brief Return number of word-internal special chars in \a word
    ///        from index \a from to index \a until
    /// @param [in] word the string to search
    /// @param [in] from start index
    /// @param [in] until end index
    /// @return number of special chars found
    uint8_t getWordSpecialFromUntil(char *word, uint8_t from, uint8_t until);

    /// @brief Return number of word-internal special chars in \a word
    /// @param [in] word the string to search
    /// @return number of special chars found
    uint8_t getWordSpecialCnt(char *word);

    /// @brief Check if a string \a str is a valid word to be used as replacement
    /// @param [in] str
    /// @return true if valid, false otherwise
    bool isValidReplacement(char *str);

    /// @brief Return number of multibyte chaacters in string \a word
    /// @param [in] word
    /// @return number of multibyte chars
    int getMultibyteChars(char *word);

    /// @brief Check if a given character \a c is part of a multibyte character
    /// @param [in] c a character
    /// @return true if multibyte, false otherwise
    bool isMultibyteChar(char c);

    /// @brief Replace a single word with the set replacement string
    /// @param [in, out] word the word to replace
    /// @note The string \a word is changed in-place
    void replaceSingleWord(char *word);

    /// @brief Replace a single word with the set replacement string
    /// @note  Inheriting classes are required to implement this
    ///        If nothing special needs to be done, just call replaceSingleWord here
    virtual bool replaceWord(char *word) = 0;

  public:
    ReplacerBase();
    ReplacerBase(unsigned int maxLen);
    virtual ~ReplacerBase();

    /// @brief set replacement text
    /// @param [in] str replacemet word
    /// @param [in] vIdx index of the long vowel
    /// @return true on success, else false
    bool setText(const char *str, unsigned int vIdx);

    /// @brief Return a modified version of the input string
    /// @param [in] fmt
    /// @return pointer to resulting string
    /// @note The resulting text is stored inside this class and thus always
    ///       holds the last changed string.
    char* change(const char *fmt);

};


#endif // _REPLACER_BASE_H_

#include "../inc/catch.hpp"

#include "replacer.h"   // DUT
#include "multiReplacer.h" // multi DUT
#include "strings.cpp"  // test strings

#include <string.h>     // memcmp

#define TESTTEXT        "This is a simple test string."

#define DEFAULTIDX      1
#define DEFAULTREP      "muh"
#define DEFAULT_OUTPUT  "Muuh mu m muuuuh muuh muuuuh."

#define TESTIDX         2
#define TESTREP         "bla"
#define TEST_OUTPUT     "Blaa bl b blaaaa blaa blaaaa."

/* TODO:
 *  [BUG#1]There is an issue: umlauts are treated as two characters.
 *    need to find an easy way to detect that.
 * IDEA: multiple replacement words!
 *  - set an array of replacement words, each input word is replaced by a fitting replacement
 *    i.e. either an exactly fitting one (e.g replace "bla" with "oje")
 *          or the next shorter repword streched (e.g. replace "Mund" with "Ojee")
 *    -> This could exxentially be used to 'ipsumify' random text.
 *    (not sure if i have already implemented this, but use the sorting feature of lilist for storing the repwords!(sort by length, obvs))
 */

 /*
 * TODO: Property based testing!! [https://www.youtube.com/watch?v=IYzDFHx6QPY]
 *  don't (only) use examples for testing, rather test if the function satisfies its most basic properties,
 *  e.g. * every word in the output text must be as long as the corresponding word in the input text
 *       * the output text must contain exactly as many words as the input text
 *       * every word in the output text must - when special chars are removed - be a word from the rep list
 */

// HELPER FUNCS
void logRepResult(const char *in, const char *out, const char *expect) {
  UNSCOPED_INFO("Input:\t" << in);
  UNSCOPED_INFO("Output:\t" << out);
  UNSCOPED_INFO("Expected:\t" << expect);
}

void testStrings(Replacer* rep, const stringCombo_t* strings, int len) {
  const stringCombo_t *obj = NULL;

  for (int i = 0; i < len; i++) {
   obj = &strings[i];
   bool r = rep->setText(obj->rep.word, obj->rep.idx);
   REQUIRE(r == true);

   char *result = rep->change(obj->in);

   logRepResult(obj->in, result, obj->out);
   REQUIRE(strlen(result) == strlen(obj->out));

   logRepResult(obj->in, result, obj->out);
   REQUIRE((memcmp(result, obj->out, strlen(result))) == 0);
  }
}

void testMultiStrings(MultiReplacer *rep, const multiStringCombo_t* strings, int len) {
  const multiStringCombo_t *obj = NULL;

  for (int i = 0; i < len; i++) {
   obj = &strings[i];
  //  MultiReplacer *rep = new MultiReplacer(obj->rep, MAX_REP);
  //  REQUIRE(rep != NULL);

   char *result = rep->change(obj->in);

   logRepResult(obj->in, result, obj->out);
   REQUIRE(strlen(result) == strlen(obj->out));

   logRepResult(obj->in, result, obj->out);
   REQUIRE((memcmp(result, obj->out, strlen(result))) == 0);
  }
}

// TESTCASES

SCENARIO("Replacer is created and different replacement strings are set") {

  // This scenario tests basic usage

  GIVEN("A new Replacer Object") {

    Replacer *rep = new Replacer();
    REQUIRE(rep != NULL);

    WHEN("No replacement string is set") {
      // nothing to do here

      THEN("The substitution must still work using the defaults") {
        char *result = rep->change(TESTTEXT);
        REQUIRE(strlen(result) == strlen(DEFAULT_OUTPUT));
        REQUIRE((memcmp(DEFAULT_OUTPUT, result, strlen(result))) == 0);
      }
    }

    WHEN("A valid replacement String is set") {
      char text[] = TESTREP;
      uint8_t idx = TESTIDX;

      bool r = rep->setText(text, idx);

      THEN("the assignment must succeed") {
        REQUIRE(r == true);
      }

      AND_THEN("A sample input string must be converted correctly") {
        char *result = rep->change(TESTTEXT);
        REQUIRE(strlen(result) == strlen(TEST_OUTPUT));
        REQUIRE((memcmp(TEST_OUTPUT, result, strlen(result))) == 0);
      }
    }

    WHEN("An invalid replacement string is set") {
      bool r = rep->setText("99Luftballons", 3);

      THEN("the assignment must not succeed") {
        REQUIRE_FALSE(r == true);
      }

      AND_THEN("The replacer is still usable w/ default rep string") {
        char *result = rep->change(TESTTEXT);
        REQUIRE(strlen(result) == strlen(DEFAULT_OUTPUT));
        REQUIRE((memcmp(DEFAULT_OUTPUT, result, strlen(result))) == 0);
      }
    }

    delete rep;
  }
}

SCENARIO("A wide array of test strings are converted") {

  // This scenario tests a variety of strings and edge cases
  // with a variety of replacemet strings.
  // (see the test strings in strings.cpp)

  GIVEN("A newly created Replacer object") {
    Replacer *rep = new Replacer();
    REQUIRE(rep != NULL);

    WHEN("A replacement string with vowel at first position is set") {

      int len = strSizeStart();
      const stringCombo_t *strings = stringsVStart;

      THEN("All test strings must be converted correctly") {
        testStrings(rep, strings, len);
      }
    }

    WHEN("A replacement string with vowel at middle position is set") {

      int len = strSizeMid();
      const stringCombo_t *strings = stringsVMid;

      THEN("All test strings must be converted correctly") {
        testStrings(rep, strings, len);
      }
    }

    WHEN("A replacement string with vowel at end position is set") {

      int len = strSizeEnd();
      const stringCombo_t *strings = stringsVEnd;

      THEN("All test strings must be converted correctly") {
        testStrings(rep, strings, len);
      }
    }

    delete rep;
  }
}

// TESTCASES MULTIREPLACER

SCENARIO("A MultiReplacer is created and different replacement words are set") {

  GIVEN("A new Mutireplacer with valid repwords of the same length") {

    repWord_t words[3] = { {"ach", 0}, {"muh", 1}, {"bla", 2}};

    MultiReplacer *rep = new MultiReplacer(words, 3);
    REQUIRE(rep != NULL);

    WHEN("a given teststring is converted") {
      char input[] = "das ist krz";
      char *result = rep->change(input);

      logRepResult(input, result, "");
      REQUIRE(strlen(result) == strlen(input));

      THEN("different replacement words from the list shall be used randomly") {
        char* tok = strtok(result, " ");
        REQUIRE(tok != NULL);

        // maybe tokenize and check something? but technically, same word every time would also be a valid option
        do {
          // at least check that the rep words were used
          if (!(strncmp(tok, words[0].word, strlen(tok)) == 0) &&
              !(strncmp(tok, words[1].word, strlen(tok)) == 0) &&
              !(strncmp(tok, words[2].word, strlen(tok)) == 0)) {
                FAIL("Invalid rep word used");
              }
        } while ((tok = strtok(NULL, " ")));
      }
    }
  }

  GIVEN("A new Multireplacer with different length words") {

    repWord_t words[3] = { {"oder", 2}, {"muh", 1}, {"oi", 0}};

    MultiReplacer *rep = new MultiReplacer(words, 3);
    REQUIRE(rep != NULL);

    WHEN("A string with same legth words is replaced") {
      char text[] = "ja, ich kann.";
      char expectedResult[] = "oi, muh oder.";
      char* result = rep->change(text);

      logRepResult(text, result, expectedResult);
      REQUIRE(strlen(result) == strlen(expectedResult));

      THEN("the lenght-matching words must be used for each input word") {
        REQUIRE(memcmp(result, expectedResult, strlen(expectedResult)) == 0);
      }
    }

    WHEN("A string with a longer word is replaced") {
      char text[] = "da ist mutti";
      char expectedResult[] = "oi muh odeer";
      char* result = rep->change(text);

      logRepResult(text, result, expectedResult);
      REQUIRE(strlen(result) == strlen(expectedResult));

      THEN("a shorter replacement word must be streched to length") {
        REQUIRE(memcmp(result, expectedResult, strlen(expectedResult)) == 0);
      }
    }
  }
}

SCENARIO("The MultiReplacer is tested with different input strings") {
  GIVEN("A new MultiReplacer with three replacement words") {

    // reps for multireplacer tests
    repWord_t multiReps[] = {{"ach", 0}, {"krach", 2}, {"schach", 3}};

    int len = strSize();
    const multiStringCombo_t *strings = multiStrings;
    MultiReplacer *rep = new MultiReplacer(&multiReps[0], MAX_REP);
    REQUIRE(rep != NULL);

    THEN("All test strings must be converted correctly") {
      testMultiStrings(rep, strings, len);
    }
  }
}

SCENARIO("Known bugs are reproduced", "[!shouldfail]") {

  // This scenario contains reproductions of all known bugs.
  // When a bug is detected, a section reproducing that bug should be added here.
  // The Scenario is supposed to fail and will not stop execution of later scenarios.
  // If a single section does not initially fail, it shall be failed with a message ("this bug might be fixed")

  // EXAMPLE

  // WHEN("Some action is performed") {
  //
  //   // reproduce the bug here
  //
  //   // Do not test for the existance of a bug, test for the absence of one!
  //   THEN("The correct thing happens") {
  //
  //     // log bug description for easier readability of results
  //     INFO("BUG#n: <bug description>");
  //
  //     // always check for the correct behavior, not the buggy behavior!
  //     // this way we can copy this section to the fixed-scenario without changes to the logic
  //     REQUIRE( result == correct_result );
  //
  //     // fail the section in case the above REQUIRE did not fail.
  //     // This is also added for readbility of the results.
  //     FAIL("--> !!This bug might be fixed!!");
  //   }
  // }

  GIVEN("A new Replacer object with default repstring") {
    Replacer *rep = new Replacer();
    REQUIRE(rep != NULL);

    INFO("This is a bug and should be fixed");

    // insert bug reproduction here


    delete rep;
  }

  FAIL("expected failure: no open bugs");
}

SCENARIO("Old (fixed) bugs are tested") {

  // This scenario contains checks of previously fixed bugs to avoid regression.
  // Once a bug is fixed, the section reproducing that bug should be
  // moved from the bugs-scenario to here.

  GIVEN("A new Replacer object with default repstring") {
    Replacer *rep = new Replacer();
    REQUIRE(rep != NULL);

    /// BUG#1: incorrect treatment of multibyte characters
    WHEN("A text containing a multibyte character (e.g. 'ö') is changed") {
      char text[]     = "Das ist aber blöd.";
      char expected[] = "Muh muh muuh muuh.";
      char *result    = rep->change(text);

      THEN("The multibyte char is treated as a single character") {
        INFO("[FIXED]Bug#1: Incorrect treatment of multibyte characters");

        logRepResult(text, result, expected);
        REQUIRE(strlen(result) == strlen(expected));
        REQUIRE((memcmp(result, expected, strlen(expected))) == 0);

      }
    }

    /// BUG#2: No fallback to defaults on error
    WHEN("A valid replacement string is set") {

      char in[]       = TESTTEXT;     // = "This is a simple test string."
      char out[]      = TEST_OUTPUT;  // = "Blaa bl b blaaaa blaa blaaaa."
      char *result    = NULL;

      bool r = rep->setText(TESTREP, TESTIDX);
      REQUIRE(r == true);

      THEN("A teststring is changed as expected") {
        result = rep->change(in);
        logRepResult(in, result, out);
        REQUIRE(strlen(result) == strlen(out));
        REQUIRE((memcmp(result, out, strlen(out))) == 0);
      }

      AND_WHEN("An invalid replacement string is set afterwards") {
        char invalid[]  = "ba/";
        int  idx        = 1;

        bool r = rep->setText(invalid, idx);
        REQUIRE(r == false);

        THEN("Then the Replacer reverts back to the default replacement string") {
          INFO("[FIXED]Bug#2: No fallback to defaults on error");

          result = rep->change(in);
          logRepResult(in, result, DEFAULT_OUTPUT);
          REQUIRE(strlen(result) == strlen(DEFAULT_OUTPUT));
          REQUIRE((memcmp(result, DEFAULT_OUTPUT, strlen(result))) == 0);
        }
      }
    }

    // BUG#3: Segfault on strings that end on a word
    WHEN("A text that ends directly after a word is changed") {
      char text[] = "sowas aber auch";
      char expected[] = "muuuh muuh muuh";
      char *result = rep->change(text);

      THEN("The Replacer must not segfault (lol)") {
        INFO("[FIXED]Bug#3: Segfault on strings that end on a word");

        logRepResult(text, result, expected);
        REQUIRE(strlen(result) == strlen(expected));
        REQUIRE((memcmp(result, expected, strlen(result))) == 0);
      }

      AND_WHEN("A text that ends directly after a format specifier is changed") {
        char text[] = "Format: %s";
        char expected[] = "Muuuuh: %s";
        char *result = rep->change(text);

        THEN("The Replacer must not segfault (lol)") {
          INFO("[FIXED]Bug#3.1: Segfault on strings that end on a format");

          logRepResult(text, result, expected);
          REQUIRE(strlen(result) == strlen(expected));
          REQUIRE((memcmp(result, expected, strlen(result))) == 0);
        }
      }
    }

    delete rep;
  }
}

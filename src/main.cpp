/*  ____                 _____
 * / __ \               |_   _|
 * | |  | |_      ___ __   | |  _ __  ___ _   _ _ __ ___
 * | |  | \ \ /\ / / '_ \  | | | '_ \/ __| | | | '_ ` _ \
 * | |__| |\ V  V /| | | |_| |_| |_) \__ \ |_| | | | | | |
 * \____/  \_/\_/ |_| |_|_____| .__/|___/\__,_|_| |_| |_|
 *                           | | <einKnie@gmx.at>
 *                           |_|
 */

#include "multiReplacer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

#define VERSION "0.4"

// helper stuff
#define ASCII_ZERO        48
#define ASCII_NINE        57

// enable debug messages
bool DEBUG = true;

void printHelp() {

  printf("\nOwnIpsum v%s\n", VERSION);
  printf("\n");
  printf("Turn any given text into your own version of Lorem Ipsum!\n");
  printf("In case no long enough replacement is found for a given word\n");
  printf("a shorter replacement is streched to length.\n");
  printf("Therefore, the index of the 'strechable' character (usually a vowel)\n");
  printf("must be provided along every replacement word.\n");
  printf("E.g. \"muh\"(1) => \"muuuh\"\n");
  printf("\n");
  printf("Usage:\n");
  printf("\t-r  ... single replacement word with index\n");
  printf("\t        : in format <word> <vowel index>, e.g. -r muh 1\n");
  printf("\t        : to set multiple words : -r muh 1 -r oh 0\n");
  printf("\t-f  ... path to file containing multiple replacement words\n");
  printf("\t        : one word per line in format <word> <vowel index>, e.g. muh 1\n");
  printf("\t-i  ... path to input text file\n");
  printf("\t        : input is read from stdin per default\n");
  printf("\t-o  ... path to output text file\n");
  printf("\t        : output is written to stdout per default\n");
  printf("\n");
  printf("Examples:\n");
  printf("\t> ownipsum -r muh 1 -s ./my_file.txt\n\t\treplace every word in my_file.txt with muh\n");
  printf("\t> ownipsum -r muh 1 < ./my_file.txt\n\t\treplace every word in my_file.txt with muh\n");
  printf("\t> ownipsum -r muh 1 -r bla 2 -i ./my_file.txt\n\t\treplace every word in my_file.txt with muh or bla\n");
  printf("\t> ownipsum -f ./my_words.txt -i ./my_file.txt\n\t\treplace every word in my_file.txt with a word from my_words.txt\n");
  printf("\t> ownipsum -f ./my_words.txt < ./my_file.txt\n\t\treplace every word in my_file.txt with a word from my_words.txt\n");
  printf("\t> ownipsum -r muh 1 -i ./my_file.txt -o my_output.txt\n\t\treplace every word in my_file.txt with muh and write to my_output.txt\n");
  printf("\n");

}

void dbg(const char *fmt, ...) {
  if (DEBUG) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
  }
}

void log_err(const char *fmt, ...) {
    char msg[MAXLEN] = {'\0'};

    va_list args;
    va_start(args, fmt);

    if (snprintf(msg, MAXLEN, "\033[0;31m%s\033[0m", fmt) > -1) {
      vfprintf(stdout, msg, args);
    } else {
      vfprintf(stdout, fmt, args);
    }
    va_end(args);
}

// Check if a given string contains only numeric characters
bool isNumeric(char *str) {
  size_t len = strlen(str);
  for (size_t i = 0; i < len; i++) {
    if (!isdigit(str[i])) {
      return false;
    }
  }
  return true;
}

int main(int argc, char *argv[]) {

  MultiReplacer *rep = NULL;
  FILE           *fd = NULL;
  FILE       *src_fd = NULL;
  FILE       *dst_fd = NULL;
  FILE      *readSrc = stdin;
  FILE     *writeDst = stdout;

  repWord_t   *words = NULL;
  int         nW = 0;

  // get command line parameters
  int  opt;
  int  err           = 0;
  bool f_singleWord  = false;
  bool f_multiWord   = false;
  char *idxstr       = NULL;
  char *repstr       = NULL;
  int  idx           = -1;
  int  argvidx       = 0;

  while ((opt = getopt(argc, argv, "r:f:i:o:")) != -1) {
    switch(opt) {

      case 'r':
        // single replacement word w/ index
        dbg("Got replacement word %s\n", optarg);

        argvidx = optind - 1;
        repstr = strdup(argv[argvidx++]);
        idxstr = strdup(argv[argvidx++]);

        if ((repstr[0] != '-') && (idxstr[0] != '-')) {
          dbg("valid -r\n");
        } else {
          log_err("Error: Got invalid replacement word: %s %s\n", repstr, idxstr);
          err++;
          break;
        }

        if (!repstr || (strlen(repstr) >= MAX_REPLEN)) {
          log_err("Error: Replacement string invalid: %s\n", repstr);
          err++;
          break;
        }

        if (!idxstr || (!isNumeric(idxstr))) {
          log_err("Error: invalid vowel index: %s\n", idxstr);
          err++;
          break;
        }

        idx = atoi(idxstr);
        if (!(idx < (int)strlen(repstr))) {
          log_err("Error: Invalid vowel index given.\nIndex must point to character in word\n");
          err++;
          break;
        }

        optind = argvidx;
        words = (repWord_t*)realloc(words, ++nW * sizeof(repWord_t));
        strncpy(words[nW - 1].word, repstr, sizeof(words[nW - 1].word));
        words[nW - 1].idx = idx;
        f_singleWord = true;
        break;

      case 'f':
        // read replacement words from file
        dbg("Reading replacement words from file %s\n", optarg);

        fd = fopen(optarg, "r");
        if (fd == NULL) {
          log_err("Error: could not open replacement word file %s: %s\n", optarg, strerror(errno));
          err++;
        } else {
          f_multiWord = true;
        }
        break;

      case 'i':
        // read input from file
        dbg("Reading input from file %s\n", optarg);

        src_fd = fopen(optarg, "r");
        if (src_fd == NULL) {
          log_err("Error: could not open input file %s: %s\n", optarg, strerror(errno));
          err++;
        } else {
          readSrc = src_fd;
        }
        break;

      case 'o':
        // write output to file
        dbg("Writing output to file %s\n", optarg);

        dst_fd = fopen(optarg, "w+");
        if (dst_fd == NULL) {
          log_err("Error: could not open output file %s: %s\n", optarg, strerror(errno));
          err++;
        } else {
          writeDst = dst_fd;
        }
        break;

      default:
        printf("Ignoring unrecognized command line option -%c\n", optopt);
        break;
    }
    if (repstr) { free(repstr); repstr = NULL; }
    if (idxstr) { free(idxstr); idxstr = NULL; }
  }

  // verify command line arguments
  if (err) {
    printHelp();
    exit(1);
  }

  if (!(f_singleWord ^ f_multiWord)) {
    log_err("Error: Need either single replacement word (with -r)\n");
    log_err("or a file with replacement words (with -f)\n");
    printHelp();
    exit(1);
  }

  if (f_multiWord) {
    // rep words from file
    char    *line = NULL;
    int         n = 0;
    size_t  nRead = 0;

    while (getline(&line, &nRead, fd) > 0) {
      n++;
    }

    if (n < 1) {
      log_err("Error: Failed to read anything from word file\n");
      fclose(fd);
      exit(1);
    }

    words = (repWord_t*)malloc(n * sizeof(repWord_t));
    rewind(fd);
    free(line);
    nRead = 0;
    for (int i = 0; i < n; i++) {
      if (getline(&line, &nRead, fd) < 0) {
        log_err("Error: Failed to read line %d from word file\n", i);
        dbg("%lines total, current i: %d, line: %s\n", n, i, line);
        fclose(fd);
        free(line);
        free(words);
        exit(1);
      }

      if ((line[0] == '#') || (strlen(line) < 2)) {
        // skip line
        dbg("skipping comment or blank line in word file\n");
        n--;
        i--;
        free(line);
        nRead = 0;
        continue;
      }

      char *word   = strtok(line, " ");
      char *idxstr = strtok(NULL, " \n");

      if (!word || !idxstr || !isNumeric(idxstr)) {
        log_err("Error: invalid format in word file\n");
        fclose(fd);
        free(line);
        free(words);
        exit(1);
      }

      idx = atoi(idxstr);
      dbg("got word: %s(%d)\n", word, idx);
      strncpy(words[i].word, word, sizeof(words[i].word));
      words[i].idx = idx;
      nW++;
    }

    free(line);
    fclose(fd);

  } else {
    dbg("got %d replacements words from command line\n", nW);
  }

  // finally, instantiate replacer
  rep = new MultiReplacer(words, nW);
  free(words);

  // read input line for line and replace
  size_t size = 0;
  char  *line = NULL;
  while ((getline(&line, &size, readSrc)) > -1) {
    fprintf(writeDst, "%s", rep->change(line));
  }

  // cleanup
  free(line);
  if (src_fd) fclose(src_fd);
  if (dst_fd) fclose(dst_fd);
  delete rep;

  return 0;
}

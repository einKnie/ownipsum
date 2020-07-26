
// #define TESTBUILD
#define MULTIREP

#if defined MULTIREP
# include "multiReplacer.h"
#else
# include "replacer.h"
#endif

#ifndef TESTBUILD

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

void printHelp() {
  printf("arguments needed:\n");
  printf(" -s  ... replacement string\n");
  printf(" -i  ... index of long vowel\n");
  printf("example: -s \"muh\" - i 1 => mu(uuu...)h\n");
}

int main(int argc, char *argv[]) {

#ifdef MULTIREP
  // additional command line parameter? for words to use (for user simplicity, maybe as file?)
  MultiReplacer *rep = NULL;
  char optstr[]      = "s:i:vf:";
  FILE *fd           = NULL;
#else
  Replacer *rep       = NULL;
  char optstr[]       = "s:i:v";
#endif

  char repstr[MAXLEN] = {'\0'};
  int  vIdx           = -1;

  // get parameters
  int opt;
  while ((opt = getopt(argc, argv, optstr)) != -1) {
    switch(opt) {
      case 's':
        // 'string'
        printf("Got replacement string %s\n", optarg);
        strncpy(repstr, optarg, MAXLEN - 1);
        break;
      case 'i':
        // 'vowel index'
        printf("Long vowel at position %d\n", atoi(optarg));
        vIdx = atoi(optarg);
        break;
      case 'v':
        printf("verbose mode [ not available yet ]\n");
        break;
#ifdef MULTIREP
      case 'f':
        printf("reading rep words from file %s\n", optarg);
        fd = fopen(optarg, "r");
        if (fd == NULL) {
          printf("error: could not open word file\n");
          exit(1);
        }
        break;
#endif
      default:
        printHelp();
        break;
    }
  }

#ifndef MULTIREP

  if ((repstr[0] == '\0') || (vIdx == -1)) {
    printHelp();
    exit (1);
  }

  rep = new Replacer();
  rep->setText(repstr, vIdx);

#else

if (!fd) {
  printHelp();
  exit(1);
}
  // read words from file
  repWord_t *words = NULL;
  int nWords = 0;
  char *wline = NULL;
  size_t n = 0;

  while (getline(&wline, &n, fd) > 0) {
    printf("%s\n", wline);
    nWords++;
  }

  words = (repWord_t*)malloc(nWords * sizeof(repWord_t));
  rewind(fd);
  free(wline);
  n= 0;
  for (int i = 0; i < nWords; i++) {
    if (getline(&wline, &n, fd) < 0) {
      printf("error reading line %d\n", i);
      fclose(fd);
      free(wline);
      free(words);
      exit(1);
    }

    char* word = strtok(wline, " ");
    vIdx = atoi(strtok(NULL, " \n"));
    printf("read word: %s(%d)\n", word, vIdx);
    strncpy(words[i].word, word, sizeof(words[i].word));
    words[i].idx = vIdx;
  }

  rep = new MultiReplacer(words, nWords);
  free(wline);
  free(words);
  fclose(fd);
#endif


  size_t size = 0;
  char  *line = NULL;
  while ((getline(&line, &size, stdin)) > -1) {
    printf("%s", rep->change(line));
  }
  free(line);

  printf("\nALL DONE\n");
  delete rep;
  return 0;
}

#else // TESTBUILD

typedef struct testword {
  char    rep[10];
  uint8_t vIdx;
} testword_t;

int main(int argc, char *argv[]) {

# ifdef MULTIREP
  MultiReplacer *rep = NULL;

  repWord_t words[] = {{"muh", 1}, {"achso", 0}, {"bla", 2}, {"oh", 0}, {"ah", 0}, {"jo", 1}, {"mama", 1}, {"foto", 3}, {"yippie", 4}, {"alternate", 0}};
  rep = new MultiReplacer(words, 10);


# else
  Replacer *rep = NULL;

  rep = new Replacer();
  testword_t words[] = {{"muh", 1}, {"ach", 0}, {"bla", 2}, {"mama", 1}, {"mama", 3}, {"aber", 0}};
  uint8_t    nWords  = 6;

  for (uint8_t i = 0; i < nWords; i++) {
    printf("Using replacement word %s(%d)\n", words[i].rep, words[i].vIdx);
    rep->setText(words[i].rep, words[i].vIdx);

#  endif // multirep

    printf("%s\n", rep->change("Um 15:00 gegen Norden ziehen, das wärs..."));
    printf("%s\n", rep->change("Alles nur nicht 17:45 am Sonntag! Du hast deine %d Tassen verloren?"));
    printf("%s\n", rep->change("Es war einmal ein Lattenzaun, mit Zwischenraum, hindurchzuschaun."));
    printf("%s\n", rep->change("Ein Architekt, der dieses sah, stand eines Abends plötzlich da -"));
    printf("%s\n", rep->change("und nahm den Zwischenraum heraus und baute draus ein großes Haus."));
    printf("%s\n", rep->change("Der Zaun indessen stand ganz dumm mit Latten ohne was herum,"));
    printf("%s\n", rep->change("ein Anblick gräßlich und gemein. Drum zog ihn der Senat auch ein."));
    printf("%s\n", rep->change("Der Architekt jedoch entfloh nach Afri - od - Ameriko."));
    printf("%s\n", rep->change("Let's try to replace everything with \"mu((uuu))h\"."));
    printf("%s\n", rep->change("Let's try to replace everything with (\"mu((uuu))h\")."));
    printf("%s\n", rep->change("Let's try to replace everything with (\"mu((\"uuu\"))h\")."));
    printf("%s\n", rep->change("Let's try to replace everything with (\"mu(\"(\"uuu\")u)h\"\")."));
    printf("%s\n", rep->change("\"(O)\"))h)\"\""));
    printf("%s\n", rep->change("Aber schau mal: \"\"(/<<bla>>\"\\a<&o>)"));

    printf("\n");

# ifndef MULTIREP
  }
# endif // multirep

  delete rep;
  return 0;
}
#endif

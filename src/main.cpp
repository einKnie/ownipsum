#include "replacer.h"

// #define TESTBUILD
#ifndef TESTBUILD

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void printHelp() {
  printf("arguments needed:\n");
  printf(" -s  ... replacement string\n");
  printf(" -i  ... index of long vowel\n");
  printf("example: -s \"muh\" - i 1 => mu(uuu...)h\n");
}

int main(int argc, char *argv[]) {

  Replacer *rep            = NULL;
  char      repstr[MAXLEN] = {'\0'};
  int       vIdx           = -1;

  // get parameters
  int opt;
  while ((opt = getopt(argc, argv, "s:i:v")) != -1) {
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
      default:
        printHelp();
        break;
    }
  }

  if ((repstr[0] == '\0') || (vIdx == -1)) {
    printHelp();
    exit (1);
  }

  rep = new Replacer();
  rep->setText(repstr, vIdx);

  size_t size = 0;
  char  *line = NULL;
  while ((getline(&line, &size, stdin)) > -1) {
    printf("%s\n", rep->change(line));
  }

  printf("\nALL DONE\n");
  delete rep;
  return 0;
}

#else

typedef struct testword {
  char    rep[10];
  uint8_t vIdx;
} testword_t;

int main(int argc, char *argv[]) {

  Replacer *rep = NULL;

  rep = new Replacer();
  testword_t words[] = {{"muh", 1}, {"ach", 0}, {"bla", 2}, {"mama", 1}, {"mama", 3}, {"aber", 0}};
  uint8_t    nWords  = 6;

  for (uint8_t i = 0; i < nWords; i++) {
    printf("Using replacement word %s(%d)\n", words[i].rep, words[i].vIdx);
    rep->setText(words[i].rep, words[i].vIdx);

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
  }

  delete rep;
  return 0;
}
#endif

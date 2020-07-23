#include "replacer.h"

typedef struct testword {
  char rep[10];
  uint8_t vIdx;
} testword_t;

int main(void) {

  Replacer *rep = new Replacer();
  // rep->setText('m', 'u', 'h', 1);
  // rep->setText("plan", 2);
  // rep->setText("mama", 3);

  rep->setText("muh", 1);
  printf("%s\n",   rep->change("Let's try to replace everything with \"mu(uuu)h\"."));

  rep->setText("ach", 0);
  printf("%s\n",   rep->change("Let's try to replace everything with \"mu(uuu)h\"."));

  rep->setText("bla", 2);
  printf("%s\n",   rep->change("Let's try to replace everything with \"mu(uuu)h\"."));

  rep->setText("mama", 1);
  printf("%s\n",   rep->change("Let's try to replace everything with \"mu((uuu))h\"."));

  rep->setText("mama", 3);
  printf("%s\n",   rep->change("Let's try to replace everything with \"mu((uuu))h\"."));

  rep->setText("aber", 0);
  printf("%s\n",   rep->change("Let's try to replace everything with \"mu((uuu))h\"."));
  printf("%s\n", rep->change("Let's try to replace everything with (\"mu((uuu))h\")."));


  // printf("%s",   rep->change("Ich weiss gar nicht, wieso %s sich so ziert!\n"));
// #ifdef _NO_DEBUG_

  testword_t words[] = {{"muh", 1}, {"ach", 0}, {"bla", 2}, {"mama", 1}, {"mama", 3}, {"aber", 0}};
  uint8_t nWords = 6;

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

    printf("\n");
  }


// #endif

  delete rep;
  return 0;
}

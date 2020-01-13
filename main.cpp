#include "replacer.h"

int main(void) {

  Replacer *rep = new Replacer();
  rep->setText('m', 'u', 'h', 1);
  printf("%s",   rep->change("Ich weiss gar nicht, wieso %s sich so ziert!\n"));
  printf("%s\n", rep->change("Um 15:00 gegen Norden ziehen, das wärs..."));
  printf("%s\n", rep->change("Alles nur nicht 17:45 am Sonntag! Du hast deine %d Tassen verloren?"));

  return 0;
}

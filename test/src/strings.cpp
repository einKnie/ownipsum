/* Strings for use in unit testing */

#define MAXTESTLEN 200

typedef struct RepWord {
  char word[MAXTESTLEN];
  unsigned int idx;
} repWord_t;

typedef struct StringCombo {
  repWord_t rep;
  char in[MAXTESTLEN];
  char out[MAXTESTLEN];
} stringCombo_t;

const stringCombo_t stringsVStart[] = {
  {{"ach", 0}, "This is a test.", "Aach aa a aach."},
  {{"ach", 0}, "Let's try to replace everything with (\"mu(\"(\"uuu\")u)h\"\").", "Aac'h ach aa aaaaach aaaaaaaach aach (\"aa(\"(\"aaa\")c)h\"\")."},
  {{"ach", 0}, "Um 15:00 gegen Norden ziehen, das ists...", "Aa 15:00 aaach Aaaach aaaach, ach aach..."},
  {{"ach", 0}, "Der Architekt jedoch entfloh nach Afri - od - Ameriko.", "Ach Aaaaaaach aaaach aaaaach aach Aach - aa - Aaaaach."},
  {{"ach", 0}, "When the S(((un))) sh[i]nes, noone \"(will)\" be the w<<is>>er.", "Aach ach A(((ch))) aa[a]ach, aaach \"(aach)\" aa ach a<<aa>>ch."},
  {{"ach", 0}, "Aber schau mal: \"\"(/<<bla>>\"\\a<&o>)", "Aach aaach ach: \"\"(/<<aaa>>\"\\c<&h>)"},
  {{"ach", 0}, ".some strange stuff . wow", ".aach aaaaach aaach . ach"},
  {{"ach", 0}, "Format %s", "Aaaach %s"}
};

const stringCombo_t stringsVMid[] = {
  {{"muh", 1}, "This is a test.", "Muuh mu m muuh."},
  {{"muh", 1}, "Let's try to replace everything with (\"mu(\"(\"uuu\")u)h\"\").", "Muu'h muh mu muuuuuh muuuuuuuuh muuh (\"mu(\"(\"uuu\")u)h\"\")."},
  {{"muh", 1}, "Um 15:00 gegen Norden ziehen, das ists...", "Mu 15:00 muuuh Muuuuh muuuuh, muh muuh..."},
  {{"muh", 1}, "Der Architekt jedoch entfloh nach Afri - od - Ameriko.", "Muh Muuuuuuuh muuuuh muuuuuh muuh Muuh - mu - Muuuuuh."},
  {{"muh", 1}, "When the S(((un))) sh[i]nes, noone \"(will)\" be the w<<is>>er.", "Muuh muh M(((uh))) mu[u]uuh, muuuh \"(muuh)\" mu muh m<<uu>>uh."},
  {{"muh", 1}, "Aber schau mal: \"\"(/<<bla>>\"\\a<&o>)", "Muuh muuuh muh: \"\"(/<<muu>>\"\\u<&h>)"},
  {{"muh", 1}, ".some strange stuff . wow", ".muuh muuuuuh muuuh . muh"},
  {{"muh", 1}, "Format %s", "Muuuuh %s"}
};

const stringCombo_t stringsVEnd[] = {
    {{"oje", 2}, "This is a test.", "Ojee oj o ojee."},
    {{"oje", 2}, "Let's try to replace everything with (\"mu(\"(\"uuu\")u)h\"\").", "Oje'e oje oj ojeeeee ojeeeeeeee ojee (\"oj(\"(\"eee\")e)e\"\")."},
    {{"oje", 2}, "Um 15:00 gegen Norden ziehen, das ists...", "Oj 15:00 ojeee Ojeeee ojeeee, oje ojee..."},
    {{"oje", 2}, "Der Architekt jedoch entfloh nach Afri - od - Ameriko.", "Oje Ojeeeeeee ojeeee ojeeeee ojee Ojee - oj - Ojeeeee."},
    {{"oje", 2}, "When the S(((un))) sh[i]nes, noone \"(will)\" be the w<<is>>er.", "Ojee oje O(((je))) oj[e]eee, ojeee \"(ojee)\" oj oje o<<je>>ee."},
    {{"oje", 2}, "Aber schau mal: \"\"(/<<bla>>\"\\a<&o>)", "Ojee ojeee oje: \"\"(/<<oje>>\"\\e<&e>)"},
    {{"oje", 2}, ".some strange stuff . wow", ".ojee ojeeeee ojeee . oje"},
    {{"oje", 2}, "Format %s", "Ojeeee %s"}

};


static int strSizeStart() {
  return (sizeof(stringsVStart) / sizeof(stringCombo_t));
}

static int strSizeMid() {
  return (sizeof(stringsVMid) / sizeof(stringCombo_t));
}

static int strSizeEnd() {
  return (sizeof(stringsVEnd) / sizeof(stringCombo_t));
}

/* Strings for use in unit testing */

#define MAXTESTLEN 200
#define MAX_REP      3

typedef struct MultiStringCombo {
  char in[MAXTESTLEN];
  char out[MAXTESTLEN];
} multiStringCombo_t;

typedef struct TestRepWord {
  char word[MAXTESTLEN];
  unsigned int idx;
} testrepWord_t;

typedef struct StringCombo {
  testrepWord_t rep;
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
  {{"ach", 0}, "Format %s", "Aaaach %s"},
  {{"ach", 0}, "Mü, meine liebe!", "Aa, aaach aaach!"},
  {{"ach", 0}, "Und die Mütter ächzen so", "Ach ach Aaaach aaaach aa"}
};

const stringCombo_t stringsVMid[] = {
  {{"muh", 1}, "This is a test.", "Muuh mu m muuh."},
  {{"muh", 1}, "Let's try to replace everything with (\"mu(\"(\"uuu\")u)h\"\").", "Muu'h muh mu muuuuuh muuuuuuuuh muuh (\"mu(\"(\"uuu\")u)h\"\")."},
  {{"muh", 1}, "Um 15:00 gegen Norden ziehen, das ists...", "Mu 15:00 muuuh Muuuuh muuuuh, muh muuh..."},
  {{"muh", 1}, "Der Architekt jedoch entfloh nach Afri - od - Ameriko.", "Muh Muuuuuuuh muuuuh muuuuuh muuh Muuh - mu - Muuuuuh."},
  {{"muh", 1}, "When the S(((un))) sh[i]nes, noone \"(will)\" be the w<<is>>er.", "Muuh muh M(((uh))) mu[u]uuh, muuuh \"(muuh)\" mu muh m<<uu>>uh."},
  {{"muh", 1}, "Aber schau mal: \"\"(/<<bla>>\"\\a<&o>)", "Muuh muuuh muh: \"\"(/<<muu>>\"\\u<&h>)"},
  {{"muh", 1}, ".some strange stuff . wow", ".muuh muuuuuh muuuh . muh"},
  {{"muh", 1}, "Format %s", "Muuuuh %s"},
  {{"muh", 1}, "Mü, meine liebe!", "Mu, muuuh muuuh!"},
  {{"muh", 1}, "Und die Mütter ächzen so", "Muh muh Muuuuh muuuuh mu"}
};

const stringCombo_t stringsVEnd[] = {
    {{"oje", 2}, "This is a test.", "Ojee oj o ojee."},
    {{"oje", 2}, "Let's try to replace everything with (\"mu(\"(\"uuu\")u)h\"\").", "Oje'e oje oj ojeeeee ojeeeeeeee ojee (\"oj(\"(\"eee\")e)e\"\")."},
    {{"oje", 2}, "Um 15:00 gegen Norden ziehen, das ists...", "Oj 15:00 ojeee Ojeeee ojeeee, oje ojee..."},
    {{"oje", 2}, "Der Architekt jedoch entfloh nach Afri - od - Ameriko.", "Oje Ojeeeeeee ojeeee ojeeeee ojee Ojee - oj - Ojeeeee."},
    {{"oje", 2}, "When the S(((un))) sh[i]nes, noone \"(will)\" be the w<<is>>er.", "Ojee oje O(((je))) oj[e]eee, ojeee \"(ojee)\" oj oje o<<je>>ee."},
    {{"oje", 2}, "Aber schau mal: \"\"(/<<bla>>\"\\a<&o>)", "Ojee ojeee oje: \"\"(/<<oje>>\"\\e<&e>)"},
    {{"oje", 2}, ".some strange stuff . wow", ".ojee ojeeeee ojeee . oje"},
    {{"oje", 2}, "Format %s", "Ojeeee %s"},
    {{"oje", 2}, "Mü, meine liebe!", "Oj, ojeee ojeee!"},
    {{"oje", 2}, "Und die Mütter ächzen so", "Oje oje Ojeeee ojeeee oj"}
};

// @todo: improve this so it does not depend on rep words defined in the testcase itself
const multiStringCombo_t multiStrings[] = {
  {"This is a test.", "Aach aa a aach."},
  {"Let's try to replace everything with (\"mu(\"(\"uuu\")u)h\"\").", "Aac'h ach aa schaach schaaaaach aach (\"sc(\"(\"haa\")c)h\"\")."},
  {"Um 15:00 gegen Norden ziehen, das ists...", "Aa 15:00 krach Schach schach, ach aach..."},
  {"Der Architekt jedoch entfloh nach Afri - od - Ameriko.", "Ach Schaaaach schach schaach aach Aach - aa - Schaach."},
  {"When the S(((un))) sh[i]nes, noone \"(will)\" be the w<<is>>er.", "Aach ach A(((ch))) sc[h]ach, krach \"(aach)\" aa ach k<<ra>>ch."},
  {"Aber schau mal: \"\"(/<<bla>>\"\\a<&o>)", "Aach krach ach: \"\"(/<<kra>>\"\\c<&h>)"},
  {".some strange stuff . wow", ".aach schaach krach . ach"},
  {"Format %s", "Schach %s"},
  {"Mü, meine liebe!", "Aa, krach krach!"},
  {"Und die Mütter ächzen so", "Ach ach Schach schach aa"},
  {"Die seltene Mühe, sich immer anzuziehen.", "Ach schaach Aach, aach krach schaaaaach."}
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

static int strSize() {
  return (sizeof(multiStrings) / sizeof(multiStringCombo_t));
}

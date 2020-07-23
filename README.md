# Text Replacer

Replace any given text with a predefined word.  
This is the epitome of all that is unnecessary but at least it was fun to write.

## Compilation

### Prerequisites
This program uses the logging functionality provided by [Logger](https://github.com/einKnie/Logger).  
Clone the [Logger](https://github.com/einKnie/Logger) repo to your machine and compile the library. Then change the variable LIB_DIR of the RoomExplorer's Makefile to point to the libcpplogging library.

### Compilation
To compile, cd into the repo and call <i>make -C ./proj</i>
The resulting binary can be found in /bin.

### Repo Structure
TextReplacer  
&nbsp;&nbsp;|_ bin/   &nbsp;&nbsp;&nbsp;&nbsp;...binary files<br>
&nbsp;&nbsp;|_ inc/   &nbsp;&nbsp;&nbsp;&nbsp;...header files<br>
&nbsp;&nbsp;|_ proj/  &nbsp;&nbsp;&nbsp;...build files<br>
&nbsp;&nbsp;|_ src/   &nbsp;&nbsp;&nbsp;&nbsp;...source files<br>

### Example

    Replacer *rep = new Replacer();
    
    // set the replacement text. The second parameter specifies the index of the vowel
    rep->setText("muh", 1);
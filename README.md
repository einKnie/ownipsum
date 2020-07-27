# OwnIpsum

Turn any given text into your own version of Lorem Ipsum!  
Aute sed lorem enim elit nisi sed aliquip in Ipsum Irure!

## Usage

Provide OwnIpsum with one or more (though one would be a bit boring) replacement words and some input text and watch your very own dummy text be generated.  

Replacement words can either be provided directly on the command line or more comfortably from a file. The program expects input from stdin per default, but an input text file may also be supplied.

### Command Line Parameters

|||
---|---
| **-r** | ... single replacement word with index __*__ <br>*in format \<word> \<vowel index>, e.g. -r muh 1<br>to set multiple words : -r muh 1 -r oh 0*|
| **-f** | ... path to file containing one or more replacement words<br>*one word per line in format \<word> \<vowel index>, e.g. muh 1*|
| **-i** | ... path to input text file<br>*input is read from stdin per default*|  
| **-o** | ... path to output text file<br>*output is written to stdout per default*

>__* Index Explanation__ In case no long enough replacement is found for a given word
a shorter replacement is stretched to length.  
>Therefore, the index of the *stretchable* character (this would usually be a vowel)
must be provided along with every replacement word.
>> #### Example
>> Replacement word "lorem" with index 1 and input word "advantageous" becomes "loooooooorem"  

Multiple replacement words can be provided on the command line by repeating the **-r** argument.  
` -r lorem 1 -r ipsum 3`

### Examples
Wait for input from stdin and replace every word with *lorem* in stdout  
`> ownipsum -r lorem 1`  

Read input from file *my_file.txt* and replace every word with either lorem or ipsum, then write to file *my_result.txt*  
`> ownipsum -r lorem 1 -r ipsum 3 -i my_file.txt -o my_result.txt`  

Read input from file *my_file.txt* and replace every word with a word from *words.txt* in stdout (but pipe that into *my_result.txt*)  
`> ownipsum -f words.txt -i my_file.txt > my_result.txt`  

Read input from stdin (and pipe in *my_file.txt*), replace every word with a word from *words.txt* and write the resulting text to file *my_result.txt*  
`> ownipsum -f words.txt -o my_result.txt < my_file.txt`

| *my_file.txt* | *my_result.txt* |
| ------------  | --------------  |
| Hello, this is a short test text to demonstrate the OwnIpsum text generator. The provided example 'words.txt' file contains all words from the original Lorem Ipsum text.| Ipsum, anim do d nulla elit nisi ut consectetur est OccAecat sint cupidatat. Sit deserunt laboris 'minim.sit' anim pariatur sed culpa sint non occaecat Culpa Culpa amet.|

## Compilation

To compile, cd into the repo and call <i>make -C ./proj</i>
The resulting binary can be found in /bin.

### Repo Structure
TextReplacer  
&nbsp;&nbsp;|_ bin/   &nbsp;&nbsp;&nbsp;&nbsp;...binary files<br>
&nbsp;&nbsp;|_ inc/   &nbsp;&nbsp;&nbsp;&nbsp;...header files<br>
&nbsp;&nbsp;|_ proj/  &nbsp;&nbsp;&nbsp;...build files<br>
&nbsp;&nbsp;|_ src/   &nbsp;&nbsp;&nbsp;&nbsp;...source files<br>

## Known Bugs

 * multibyte characters in input are not detected correctly and are replaced with two characters in output (*ojä* with replacement *muh* becomes *muuh*)

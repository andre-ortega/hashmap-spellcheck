# HashMap Spellchecker

A spellchecker will determine whether a word is spelled either correctly or incorrecty, a good spellchecker
is also able to offer suggestions. This program represents the application of the Hashmap data structure in C.

## Dictionary Hashing

Upon program start, a large text file containing words from a standard dictionary are each hashed into a 
table of values and for storage. The user is then prompted to enter a word and determine if the word is 
spelled correctly. If it is not, the incorrectly spelled word is compared to all other words in the dictionary
until 5 words are determined to be the likeliest candidates

## Levenshtein Distance Formula

Levenshtein distance is a method of comparing two string sequences. The determined distance between two
words is the minimum number of single-character insertions, deletions, or substitutions needed to edit
one word into another. Upon entry of an incorrectly spelled word in this program, the five words provided
to the user as suggestions are those with the lowest Levenshtein Distance compared to the incorrectly spelled
word that was entered.

parser grammar CoolParser;
options { tokenVocab = CoolLexer; }

// TODO: Implement Cool syntax; retain the program entry rule.
// Label expression alternatives to generate useful visitor methods.
program : EOF;

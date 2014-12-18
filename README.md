Connect Four
============

The game 'connect four' or 'four in a row' with strategies/ai implemented as
Lua script.  The goal is to compare various strategies for the game.

## Quickstart
Run `make` to compile.  Requires Lua installed, and a C compiler.  You may
have to edit the Makefile to suit your needs.
Run with `./c4 ai-random ai-random` or similar.  Adjust the macro `MATCHES` in
`main.c` to set the number of games to play.  Statistics (win/lose/draw) are
printed in the end.

#!/bin/bash

./minibasic < $1 > a.s
as -o a.o a.s
ld -o a.out a.o libminibasic.a

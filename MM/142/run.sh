#!/bin/bash

clang++ HelpTheElves.cpp && java -jar tester.jar -exec "./a.out" -seed $1

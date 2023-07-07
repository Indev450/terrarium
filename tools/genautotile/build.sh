#!/bin/bash

# Was too lazy to actually write Makefile/add this to cmake project.
# Will probably need too do that at some point but eeh, this will do for now.
g++ main.cpp -o genautotile --std=c++20 -lsfml-graphics -lsfml-system

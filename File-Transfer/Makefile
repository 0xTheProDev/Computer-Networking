# Copyright (c) 2018 Progyan Bhattacharya <progyanb@acm.org>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# Makefile: Project Build Configuration

# Compiler Information
CC=gcc
CFLAGS=-std=c99 -Werror -Wall -pedantic

# Directories
DIR=-I$(INC)
BIN=bin
SRC=src
INC=inc

# Source Files
SRCS=$(wildcard $(SRC)/*.c)

# Executable Files
EXES=$(addprefix $(BIN)/, $(notdir $(SRCS:.c=.out)))

# Default Build Target
all: $(EXES)

# Each Binary build
$(BIN)/%.out: $(SRC)/%.c
	@mkdir -p $(BIN)
	@echo "  CC    $@"
	@$(CC) $(CFLAGS) $< $(DIR) -o $@

# House-keeping
clean:
	@rm -rf core $(BIN) *.o *.out *.exe

# Help Option
help:
	@echo "Target Binary: $(EXES)"
	@echo "Source Files: $(SRCS)"

.PHONY: all clean help

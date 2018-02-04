#!/bin/bash

set -ex

# Signal bash to include filenames beginning with a `.'
# in the results of pathname expansion:
shopt -s dotglob

cp -rafv $PWD/home_settings/* $HOME/
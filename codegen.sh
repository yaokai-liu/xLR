#!/usr/bin/env bash

if [ -d "json/xLR" ]; then
  echo ''
else
  mkdir -p json/xLR
fi
if [ -d "grammar/generated/xLR" ]; then
  echo ''
else
  mkdir -p grammar/generated/xLR
fi
python liu-xLR/generate.py json
python xnf-json2C/codegen.py json template grammar/generated

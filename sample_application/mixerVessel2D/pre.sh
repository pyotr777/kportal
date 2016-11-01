#!/bin/bash

echo "Prescript $0 called with parameters: $@" > pre.log

pwd >> pre.log
find . >> pre.log

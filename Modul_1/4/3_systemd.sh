#!/bin/bash
ps axj | awk '$1=="1" { print $10 }'

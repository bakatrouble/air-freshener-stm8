#!/bin/bash

command() {
  stm8flash -c stlinkv3 -p stm8s003f3p6 -s flash -w cmake-build-stm8sdisco/stm8test.ihx
}

until command; do
  echo "retry"
done

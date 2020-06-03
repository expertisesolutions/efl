#!/bin/sh

set -e
. .ci/travis.sh

if [ "$1" = "codecov" ] || [ "$1" = "coverity" ] || [ "$1" = "mingw" ] || [ "$1" = "release-ready" ]; then
  exit 0
fi

travis_fold compile_test compile_test
travis_endfold compile_test

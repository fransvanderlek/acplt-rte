#!/bin/bash

echo "Creating docs"
mkdir html
makeinfo --html --force doc/libov.texi -o html/ --no-warn
cd ..

cd rte
TAG="$(git rev-parse --short=10 HEAD)"

if [ ${OV_ARCH_BITWIDTH} == "32" ]; then bash ./tools/travis_release_and_crossCompile.sh ; fi
if [ ${OV_ARCH_BITWIDTH} == "64" ]; then bash ./tools/travis_release_64.sh ; fi
cd ..

echo "Moving everything to rte-www"
git clone -q --depth=5 -b gh-pages https://$GH_TOKEN:x-oauth-basic@github.com/acplt/rte-www
if [ ${OV_ARCH_BITWIDTH} == "32" ]; then
  cp ./rte/acpltRTE-linux32.tar.gz ./rte-www/releases/`date +%F-%R`-${TAG}_acpltRTE-linux32.tar.gz
  cp ./rte/acpltRTE-win32.zip ./rte-www/releases/`date +%F-%R`-${TAG}_acpltRTE-win32.zip
  cp ./rte/acpltRTE-RPi.tar.gz ./rte-www/releases/`date +%F-%R`-${TAG}_acpltRTE-RPi.tar.gz
fi
if [ ${OV_ARCH_BITWIDTH} == "64" ]; then
  cp ./rte/acpltRTE-linux64.tar.gz ./rte-www/releases/`date +%F-%R`-${TAG}_acpltRTE-linux64.tar.gz
fi

if [ ${OV_ARCH_BITWIDTH} == "32" ]; then bash ./rte/tools/travis_cleanReleaseDir.sh ; fi
cd rte-www
cd ./releases
if [ ${OV_ARCH_BITWIDTH} == "32" ]; then bash ../../rte/tools/travis_createListPage.sh ; fi
cd ..

if [ ${OV_ARCH_BITWIDTH} == "32" ]; then
  git rm -r -f ./doc/current/*
  mkdir doc
  cd doc
  mkdir current
  cd ..
  cp -r ../rte/html/* ./doc/current/
  git add -A ./doc/current/*
fi
git add -A ./releases/*
git config --global user.email "rte_bot@outlook.de"
git config --global user.name "rte-bot"
git config --global push.default simple
git commit -am "updated generated documentation (texinfo) on webpage and created releases by travis-ci [ci skip]"
git push -f -q https://$GH_TOKEN:x-oauth-basic@github.com/acplt/rte-www
cd .. && rm -rf rte-www

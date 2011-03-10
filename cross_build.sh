#!/bin/sh

for file in ./build.properties.*
do
	ant clean
	cp -v $file ./build.properties
	ant compile
done

#!/bin/bash

if [ "$(id -u)" != "0" ]; then
	echo "You must be root to apply root settings" 1>&2
	exit 1
fi

set -ex

cp -rfv --preserve=mode $PWD/root_settings/* /